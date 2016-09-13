/* $Id: interface.h 3940 2008-02-10 22:48:45Z nebulon $
 *
 * Copyright (c) 2006 Johannes Zellner, <webmaster@nebulon.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "functions.h"
#include "callbacks.h"
#include "types.h"

extern GtkTreeStore *list_store;
extern GtkTreeSelection *selection;
extern GtkWidget *treeview;
extern GtkWidget *mainmenu;
extern GtkWidget *taskpopup;
extern GtkWidget *cpu_usage_progress_bar;
extern GtkWidget *mem_usage_progress_bar;
extern GtkWidget *cpu_usage_progress_bar_box;
extern GtkWidget *mem_usage_progress_bar_box;

enum {
    COLUMN_NAME = 0,
    COLUMN_UNAME,
    COLUMN_TIME,
    COLUMN_RSS,
    COLUMN_MEM,
    COLUMN_PID,
    COLUMN_STATE,
    COLUMN_PRIO,
    COLUMN_PPID
};

extern GtkTreeViewColumn *column;

void change_list_store_view(void);
void create_list_store(void);

GtkWidget* create_main_window (void);
GtkWidget* create_taskpopup (void);
GtkWidget* create_mainmenu (void);
GtkWidget *create_prio_submenu(void);

void show_about_dialog(void);

void fill_list_item(guint i, GtkTreeIter *iter);
void add_new_list_item(guint i);
gint compare_list_item(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data);
gint compare_int_list_item(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data);
gint compare_size_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column);
gint compare_string_list_item(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data);
void remove_list_item(pid_t i);
void refresh_list_item(guint i);

void change_task_view(void);
void change_full_path(void);

#endif
