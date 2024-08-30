/*
 *  xfce4-taskmanager - very simple taskmanger
 *
 *  Copyright (c) 2006 Johannes Zellner, <webmaster@nebulon.de>
 *  Copyright (C) 2024 Ingo Brückl
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>

#include "callbacks.h"

extern guint rID;

void on_button3_toggled_event(GtkButton *button, GdkEventButton *event)
{
    full_view = !full_view;
    gtk_button_set_label(button, details());
    change_list_store_view();
}

gboolean on_treeview1_button_press_event(GtkButton *button, GdkEventButton *event)
{
    if (event->button == 3 && event->type == GDK_BUTTON_PRESS)
    {
        GdkEventButton *mouseevent = (GdkEventButton *)event;
        gtk_menu_popup(GTK_MENU(taskpopup), NULL, NULL, NULL, NULL, mouseevent->button, mouseevent->time);
    }
    return FALSE;
}

static void set_popup_position (GtkMenu *menu, gint *x, gint *y, gboolean *push_in, GtkTreeView *treeview)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreePath *path;
    GdkRectangle rect;
    gint bx, by;

    gtk_tree_selection_get_selected(selection, &model, &iter);

    /* scroll to selected line to make it visible */
    path = gtk_tree_model_get_path(model, &iter);
    gtk_tree_view_scroll_to_cell(treeview, path, NULL, FALSE, 0, 0);

    /* get bin_window coordinates of the name column of the selected row */
    gtk_tree_view_get_cell_area(treeview, path, gtk_tree_view_get_column(treeview, COLUMN_NAME), &rect);
    gtk_tree_path_free(path);

    /* GTK+ 3 returns coordinates outside the visible area
       after scrolling a row to the visible treeview area */
#if GTK_CHECK_VERSION(3,0,0)
    int bh = gdk_window_get_height(gtk_tree_view_get_bin_window(treeview));
    if (rect.y > bh) rect.y = bh - rect.height;
    if (rect.y < 0) rect.y = 0;
#endif

    /* get screen coordinates of bin_window */
    gdk_window_get_origin(gtk_tree_view_get_bin_window(treeview), &bx, &by);

    /* add relative coordinates of column and center on it */
    *x = bx + rect.x + rect.width / 2;
    *y = by + rect.y + rect.height / 2;

    /* place pop-up menu inside the screen if parts of it would be outside */
    *push_in = TRUE;
}

gboolean on_treeview_popup_menu (GtkWidget *widget, gpointer user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gtk_menu_popup(GTK_MENU(taskpopup), NULL, NULL, (GtkMenuPositionFunc) set_popup_position, widget, 0, gtk_get_current_event_time());
        return TRUE;
    }

    return FALSE;
}

void handle_task_menu(GtkWidget *widget, gchar *signal)
{
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (signal && gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gint task_action = SIGNAL_NO;

        switch(signal[0])
        {
            case 'K':
                if(confirm(_("Really kill the task?"), GTK_RESPONSE_NO))
                    task_action = SIGNAL_KILL;
                break;
            case 'T':
                if(confirm(_("Really terminate the task?"), GTK_RESPONSE_YES))
                    task_action = SIGNAL_TERM;
                break;
            case 'S':
                task_action = SIGNAL_STOP;
                break;
            case 'C':
                task_action = SIGNAL_CONT;
                break;
            default:
                return;
        }

        if(task_action != SIGNAL_NO)
        {
            gchar *task_id = "";

            gtk_tree_model_get(model, &iter, COLUMN_PID, &task_id, -1);
            if(atoi(task_id)==getpid() && task_action==SIGNAL_STOP)
            {
                show_error(_("Can't stop process self"));
            }
            else
            {
                send_signal_to_task(atoi(task_id), task_action);
                refresh_task_list();
            }
        }
    }
}

void handle_prio_menu(GtkWidget *widget, gchar *prio)
{
    gchar *task_id = "";
    GtkTreeModel *model;
    GtkTreeIter iter;

    if(gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gtk_tree_model_get(model, &iter, COLUMN_PID, &task_id, -1);

        set_priority_to_task(atoi(task_id), atoi(prio));
        refresh_task_list();
    }
}

void on_show_tasks_toggled (GtkMenuItem *menuitem, gpointer data)
{
    gssize uid = (gssize)data;

    if(uid == (gssize)own_uid)
        show_user_tasks = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
    else if(uid == 0)
        show_root_tasks = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
    else if(uid == -1)
        show_other_tasks = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
    else {

	show_full_path = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
        change_full_path();
        return;
    }

    change_task_view();
}

void on_show_cached_as_free_toggled (GtkMenuItem *menuitem, gpointer data)
{
    show_cached_as_free = !show_cached_as_free;
    change_task_view();
}

void on_quit(void)
{
    g_source_remove(rID);
    save_config();
    free(config_file);

    gtk_main_quit();
}
