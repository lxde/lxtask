/*
 *  xfce4-taskmanager - very simple taskmanger
 *
 *  Copyright (c) 2006 Johannes Zellner, <webmaster@nebulon.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __CALLBACKS_H_
#define __CALLBACKS_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "functions.h"
#include "interface.h"


#include "xfce-taskmanager-linux.h"

void on_button1_button_press_event(GtkButton *button, GdkEventButton *event);
void on_button3_toggled_event(GtkButton *button, GdkEventButton *event);
gboolean on_treeview1_button_press_event(GtkButton *button, GdkEventButton *event);
void handle_task_menu(GtkWidget *widget, gchar *signal);
void handle_prio_menu(GtkWidget *widget, gchar *prio);
void on_show_tasks_toggled (GtkMenuItem *menuitem, gpointer uid);
void on_show_cached_as_free_toggled (GtkMenuItem *menuitem, gpointer uid);

void on_quit(void);

#endif
