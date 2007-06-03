/*
 * client.c - HTTP client
 *
 * Copyright (C) 2007 Marco Barisione <marco@barisione.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <glib.h>
#include <glib-object.h>

#include <libsoup/soup.h>


static GMainLoop *loop;

static void
sighandler(int sig) {
  g_main_loop_quit(loop);
}

static void
received_chunk_cb(SoupMessage *msg,
                  gpointer user_data) {
  write(1, msg->response.body, msg->response.length);
}

static void
finished_chunks_cb(SoupMessage *msg,
                   gpointer user_data) {
  g_signal_handlers_disconnect_by_func (msg, received_chunk_cb, NULL);
  g_main_loop_quit(loop);
}

int main() {
  SoupSession *session;
  SoupMessage *msg;

  g_type_init();
  g_thread_init(NULL);

  loop = g_main_loop_new(NULL, FALSE);
  signal(SIGINT, sighandler);

  session = soup_session_async_new();
  msg = soup_message_new(SOUP_METHOD_GET, "http://localhost/");
  g_signal_connect(msg, "got_chunk", G_CALLBACK (received_chunk_cb), NULL);
  soup_message_set_flags(msg, SOUP_MESSAGE_OVERWRITE_CHUNKS);
  soup_session_queue_message(session, msg, finished_chunks_cb, NULL);

  g_main_loop_run(loop);

  g_object_unref(session);
  g_main_loop_unref(loop);

  return 0;
}

