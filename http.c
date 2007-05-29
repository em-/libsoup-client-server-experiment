/*
 * main.c - Main file for the HTTP server/client
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


static GMainLoop *loop;

static void
sighandler (int sig)
{
  g_main_loop_quit (loop);
}

int main ()
{
  g_type_init ();
  loop = g_main_loop_new (NULL, FALSE);
  signal (SIGINT, sighandler);

  g_print ("Starting HTTP server\n");

  g_main_loop_run (loop);

  g_print ("Stopping HTTP server\n");

  return 0;
}
