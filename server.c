/*
 * server.c - HTTP server
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

#include <glib.h>
#include <glib-object.h>

#include <libsoup/soup.h>
#include <libsoup/soup-server.h>
#include <libsoup/soup-server-message.h>


static GMainLoop *loop;

static void
wrote_chunk_cb (SoupMessage *msg,
                gpointer user_data) {
  static int i = 0;

  if (i < 4) {
    soup_message_add_chunk(msg, SOUP_BUFFER_STATIC, "Hello", 5);
    soup_message_io_unpause(msg);
  } else {
    soup_message_add_final_chunk(msg);
    soup_message_io_unpause(msg);
    g_main_loop_quit(loop);
  }

  i++;
}

static void
server_cb(SoupServerContext *context,
          SoupMessage *msg,
          gpointer user_data) {
  const SoupUri *uri = soup_message_get_uri(msg);

  if (context->method_id != SOUP_METHOD_ID_GET) {
    soup_message_set_status(msg, SOUP_STATUS_NOT_IMPLEMENTED);
    return;
  }

  g_print("Requested '%s'\n", uri->path);

  soup_message_set_status(msg, SOUP_STATUS_OK);
  soup_server_message_set_encoding(SOUP_SERVER_MESSAGE(msg),
                                   SOUP_TRANSFER_CHUNKED);

  g_signal_connect (msg, "wrote-chunk", G_CALLBACK (wrote_chunk_cb), NULL);

  wrote_chunk_cb(msg, NULL);
}

int main() {
  SoupServer *server;

  g_type_init();
  g_thread_init(NULL);

  loop = g_main_loop_new(NULL, FALSE);

  server = soup_server_new(SOUP_SERVER_PORT, 3333, NULL);
  soup_server_add_handler(server, "/test", NULL, server_cb, NULL, NULL);

  g_print("Starting HTTP server on port %d\n", soup_server_get_port(server));
  soup_server_run_async(server);
  g_main_loop_run(loop);
  g_print("Stopping HTTP server\n");

  soup_server_quit(server);
  g_object_unref(G_OBJECT(server));
  g_main_loop_unref(loop);

  return 0;
}

