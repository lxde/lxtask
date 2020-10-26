/* $Id: interface.c 3940 2008-02-10 22:48:45Z nebulon $
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

/* 2008-04-26 modified by Hong Jen Yee to be used in LXDE. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>
#include "interface.h"

GtkTreeStore *list_store;
GtkTreeSelection *selection;
GtkWidget *treeview;
GtkWidget *mainmenu;
GtkWidget *taskpopup;
GtkWidget *cpu_usage_progress_bar;
GtkWidget *mem_usage_progress_bar;
GtkWidget *cpu_usage_progress_bar_box;
GtkWidget *mem_usage_progress_bar_box;

GtkTreeViewColumn *column;

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

void show_preferences(void);
extern gint refresh_interval;
extern guint rID;
GtkWidget *refresh_spin;

GtkWidget* create_main_window (void)
{
    GtkWidget *window;
    GtkWidget *menubar, *menu, *item;// *sub;
    GtkWidget *vbox1;
    GtkWidget *bbox1;
    GtkWidget *scrolledwindow1;
    GtkWidget *button1;
 //   GtkWidget *button2;
    GtkWidget *button3;

    GtkWidget *system_info_box;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), _("Task Manager"));
    gtk_window_set_default_size (GTK_WINDOW (window), win_width, win_height);
    gtk_window_set_icon_name (GTK_WINDOW (window),"utilities-system-monitor");

    bbox1 = gtk_vbox_new (FALSE, 10);
    gtk_widget_show (bbox1);
    gtk_container_add (GTK_CONTAINER (window), bbox1);

	menubar = gtk_menu_bar_new();
	gtk_widget_show( menubar );
	gtk_box_pack_start(GTK_BOX (bbox1), menubar, FALSE, TRUE, 0 );

	/* build menu */
	menu = gtk_menu_new();
	
	item = gtk_menu_item_new_with_mnemonic( _("_File") );
	gtk_menu_item_set_submenu(GTK_MENU_ITEM (item), menu );
	gtk_menu_shell_append( (GtkMenuShell*)menubar, item );

	item = gtk_image_menu_item_new_from_stock( GTK_STOCK_QUIT, NULL );
	GtkAccelGroup* accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
#if GTK_CHECK_VERSION(3,0,0)
	gtk_widget_add_accelerator(item, "activate", accel_group, 
		GDK_KEY_Escape, (GdkModifierType)0, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(item, "activate", accel_group, 
		GDK_KEY_W, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
#else
	gtk_widget_add_accelerator(item, "activate", accel_group, 
		GDK_Escape, (GdkModifierType)NULL, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(item, "activate", accel_group, 
		GDK_W, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
#endif
	gtk_menu_shell_append( (GtkMenuShell*)menu, item );
	g_signal_connect( item, "activate", G_CALLBACK(gtk_main_quit), NULL );

	item = gtk_menu_item_new_with_mnemonic( _("_View") );
	gtk_menu_shell_append( (GtkMenuShell*)menubar, item );

	menu = create_mainmenu();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM (item), menu );

	item = gtk_menu_item_new_with_mnemonic( _("_Help") );
	gtk_menu_shell_append( (GtkMenuShell*)menubar, item );
	menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM (item), menu );

    item= gtk_image_menu_item_new_from_stock ("gtk-about", NULL);
    gtk_widget_show (item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
    g_signal_connect ((gpointer) item, "activate", G_CALLBACK (show_about_dialog), NULL);

	gtk_widget_show_all( menubar );

	/* window content */
    vbox1 = gtk_vbox_new (FALSE, 10);
    gtk_widget_show (vbox1);
    gtk_box_pack_start( GTK_BOX(bbox1), vbox1, TRUE, TRUE, 0 );
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 6);

    system_info_box = gtk_hbox_new (TRUE, 10);
    gtk_widget_show (system_info_box);
    gtk_box_pack_start (GTK_BOX (vbox1), system_info_box, FALSE, TRUE, 0);

    cpu_usage_progress_bar_box = gtk_event_box_new ();
    cpu_usage_progress_bar = gtk_progress_bar_new ();
#if GTK_CHECK_VERSION(3,0,0)
    gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (cpu_usage_progress_bar), TRUE);
#endif
    gtk_progress_bar_set_text (GTK_PROGRESS_BAR (cpu_usage_progress_bar), _("cpu usage"));
    gtk_widget_show (cpu_usage_progress_bar);
    gtk_widget_show (cpu_usage_progress_bar_box);
    gtk_container_add (GTK_CONTAINER (cpu_usage_progress_bar_box), cpu_usage_progress_bar);
    gtk_box_pack_start (GTK_BOX (system_info_box), cpu_usage_progress_bar_box, TRUE, TRUE, 0);

    mem_usage_progress_bar_box = gtk_event_box_new ();
    mem_usage_progress_bar = gtk_progress_bar_new ();
#if GTK_CHECK_VERSION(3,0,0)
    gtk_progress_bar_set_show_text (GTK_PROGRESS_BAR (mem_usage_progress_bar), TRUE);
#endif
    gtk_progress_bar_set_text (GTK_PROGRESS_BAR (mem_usage_progress_bar), _("memory usage"));
    gtk_widget_show (mem_usage_progress_bar);
    gtk_widget_show (mem_usage_progress_bar_box);
    gtk_container_add (GTK_CONTAINER (mem_usage_progress_bar_box), mem_usage_progress_bar);
    gtk_box_pack_start (GTK_BOX (system_info_box), mem_usage_progress_bar_box, TRUE, TRUE, 0);

    scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow1);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

    treeview = gtk_tree_view_new ();
    gtk_widget_show (treeview);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview);

    create_list_store();

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));

    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(list_store), COLUMN_TIME, GTK_SORT_ASCENDING);

    bbox1 = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(vbox1), bbox1, FALSE, TRUE, 0);
    gtk_widget_show (bbox1);

    button3 = gtk_toggle_button_new_with_label (_("more details"));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(button3), full_view);
    gtk_widget_show (button3);
    gtk_box_pack_start (GTK_BOX (bbox1), button3, FALSE, FALSE, 0);

    button1 = gtk_button_new_from_stock ("gtk-quit");
    gtk_widget_show (button1);
    gtk_box_pack_start (GTK_BOX (bbox1), button1, FALSE, FALSE, 0);

    g_signal_connect ((gpointer) window, "destroy", G_CALLBACK (on_quit), NULL);
    g_signal_connect_swapped ((gpointer) treeview, "button-press-event", G_CALLBACK(on_treeview1_button_press_event), NULL);
    g_signal_connect ((gpointer) button1, "clicked",  G_CALLBACK (on_quit),  NULL);
    g_signal_connect ((gpointer) button3, "toggled",  G_CALLBACK (on_button3_toggled_event),  NULL);

    return window;
}

void create_list_store(void)
{
    GtkCellRenderer *cell_renderer;

    /* my change 8->9 */
    list_store = gtk_tree_store_new(9, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

    cell_renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new_with_attributes(_("Command"), cell_renderer, "text", COLUMN_NAME, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_NAME);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_NAME, compare_string_list_item, (void *)COLUMN_NAME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(_("User"), cell_renderer, "text", COLUMN_UNAME, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_UNAME);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_UNAME, compare_string_list_item, (void *)COLUMN_UNAME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    cell_renderer = gtk_cell_renderer_text_new();
    g_object_set(cell_renderer, "xalign", 1.0, NULL);

    column = gtk_tree_view_column_new_with_attributes(_("CPU%"), cell_renderer, "text", COLUMN_TIME, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_TIME);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_TIME, compare_int_list_item, (void *)COLUMN_TIME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(_("RSS"), cell_renderer, "text", COLUMN_RSS, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_RSS);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_RSS, compare_size_list_item, (void *)COLUMN_RSS, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(_("VM-Size"), cell_renderer, "text", COLUMN_MEM, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_MEM);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_MEM, compare_size_list_item, (void *)COLUMN_MEM, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(_("PID"), cell_renderer, "text", COLUMN_PID, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_PID);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_PID, compare_int_list_item, (void *)COLUMN_PID, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(_("State"), cell_renderer, "text", COLUMN_STATE, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_STATE);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_STATE, compare_string_list_item, (void *)COLUMN_STATE, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(_("Prio"), cell_renderer, "text", COLUMN_PRIO, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_PRIO);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_PRIO, compare_int_list_item, (void *)COLUMN_PRIO, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes(_("PPID"), cell_renderer, "text", COLUMN_PPID, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, COLUMN_PPID);
    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(list_store), COLUMN_PPID, compare_int_list_item, (void *)COLUMN_PPID, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    change_list_store_view();
}

GtkWidget* create_taskpopup (void)
{
    GtkWidget *taskpopup;
    GtkWidget *menu_item;

    taskpopup = gtk_menu_new ();

    menu_item = gtk_menu_item_new_with_mnemonic (_("Stop"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (taskpopup), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_task_menu), "STOP");

    menu_item = gtk_menu_item_new_with_mnemonic (_("Continue"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (taskpopup), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_task_menu), "CONT");

    menu_item = gtk_menu_item_new_with_mnemonic (_("Term"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (taskpopup), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_task_menu), "TERM");

    menu_item = gtk_menu_item_new_with_mnemonic (_("Kill"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (taskpopup), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_task_menu), "KILL");

    menu_item = gtk_menu_item_new_with_mnemonic ( _("Priority") );
    gtk_menu_item_set_submenu((gpointer) menu_item, create_prio_submenu());
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (taskpopup), menu_item);

    return taskpopup;
}

GtkWidget *create_prio_submenu(void)
{
    GtkWidget *prio_submenu = gtk_menu_new ();
    GtkWidget *menu_item;

    menu_item = gtk_menu_item_new_with_mnemonic (_("  -10"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (prio_submenu), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_prio_menu), "-10");

    menu_item = gtk_menu_item_new_with_mnemonic (_("  -5"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (prio_submenu), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_prio_menu), "-5");

    menu_item = gtk_menu_item_new_with_mnemonic (_("    0"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (prio_submenu), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_prio_menu), "0");

    menu_item = gtk_menu_item_new_with_mnemonic (_("   5"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (prio_submenu), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_prio_menu), "5");

    menu_item = gtk_menu_item_new_with_mnemonic (_("   10"));
    gtk_widget_show (menu_item);
    gtk_container_add (GTK_CONTAINER (prio_submenu), menu_item);
    g_signal_connect ((gpointer) menu_item, "activate", G_CALLBACK (handle_prio_menu), "10");

    return prio_submenu;
}

GtkWidget* create_mainmenu (void)
{
    GtkWidget *mainmenu;
//    GtkWidget *info1;
//    GtkWidget *trennlinie1;
    GtkWidget *preferences1;
    GtkWidget *show_user_tasks1;
    GtkWidget *show_root_tasks1;
    GtkWidget *show_other_tasks1;
    GtkWidget *show_cached_as_free1;
    GtkWidget *show_full_path1;
    GtkWidget *separator1;
    GtkAccelGroup *accel_group;

    accel_group = gtk_accel_group_new ();

    mainmenu = gtk_menu_new ();

    show_user_tasks1 = gtk_check_menu_item_new_with_mnemonic (_("Show user tasks"));
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(show_user_tasks1), show_user_tasks);
    gtk_widget_show (show_user_tasks1);
    gtk_menu_shell_append(GTK_MENU_SHELL(mainmenu), show_user_tasks1);

    show_root_tasks1 = gtk_check_menu_item_new_with_mnemonic (_("Show root tasks"));
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(show_root_tasks1), show_root_tasks);
    gtk_widget_show (show_root_tasks1);
    gtk_menu_shell_append(GTK_MENU_SHELL(mainmenu), show_root_tasks1);

    show_other_tasks1 = gtk_check_menu_item_new_with_mnemonic (_("Show other tasks"));
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(show_other_tasks1), show_other_tasks);
    gtk_widget_show (show_other_tasks1);
    gtk_menu_shell_append(GTK_MENU_SHELL(mainmenu), show_other_tasks1);

    show_full_path1 = gtk_check_menu_item_new_with_mnemonic (_("Show full cmdline"));
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(show_full_path1), show_full_path);
    gtk_widget_show (show_full_path1);
    gtk_menu_shell_append(GTK_MENU_SHELL(mainmenu), show_full_path1);

    show_cached_as_free1 = gtk_check_menu_item_new_with_mnemonic (_("Show memory used by cache as free"));
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(show_cached_as_free1), show_cached_as_free);
    gtk_widget_show (show_cached_as_free1);
    gtk_menu_shell_append(GTK_MENU_SHELL(mainmenu), show_cached_as_free1);

    separator1 = gtk_separator_menu_item_new();
    gtk_widget_show(separator1);
    gtk_menu_shell_append(GTK_MENU_SHELL(mainmenu), separator1);

    preferences1 = gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES, NULL);
    g_signal_connect(G_OBJECT (preferences1), "activate", G_CALLBACK (show_preferences), NULL);
    gtk_widget_show(preferences1);
    gtk_menu_shell_append(GTK_MENU_SHELL(mainmenu), preferences1);

    g_signal_connect ((gpointer) show_user_tasks1, "toggled", G_CALLBACK (on_show_tasks_toggled), (void *)(gsize)own_uid);
    g_signal_connect ((gpointer) show_root_tasks1, "toggled", G_CALLBACK (on_show_tasks_toggled), (void *)0);
    g_signal_connect ((gpointer) show_other_tasks1, "toggled", G_CALLBACK (on_show_tasks_toggled), (void *)-1);
    g_signal_connect ((gpointer) show_full_path1, "toggled", G_CALLBACK (on_show_tasks_toggled), (void *)-2);
    g_signal_connect ((gpointer) show_cached_as_free1, "toggled", G_CALLBACK (on_show_cached_as_free_toggled), (void *)0);

    gtk_menu_set_accel_group (GTK_MENU (mainmenu), accel_group);

    return mainmenu;
}

void show_about_dialog(void)
{
    GtkWidget * about_dlg;
    const gchar *authors[] =
    {
        "Hong Jen Yee <pcman.tw@gmail.com>",
        "Jan Dlabal <dlabaljan@gmail.com>",
        _("LXTask is derived from Xfce4 Task Manager by:\n"
        "  * Johannes Zellner <webmaster@nebulon.de>"),
        NULL
    };
    /* TRANSLATORS: Replace this string with your names, one name per line. */
    gchar *translators = _( "translator-credits" );

    /* gtk_about_dialog_set_url_hook( open_url, this, NULL); */

    about_dlg = gtk_about_dialog_new ();

    gtk_container_set_border_width ( ( GtkContainer*)about_dlg , 2 );
    gtk_about_dialog_set_version ( (GtkAboutDialog*)about_dlg, VERSION );
#if !GTK_CHECK_VERSION(2,12,0)
    gtk_about_dialog_set_name ( (GtkAboutDialog*)about_dlg, _( "LXTask" ) );
#else
	gtk_about_dialog_set_program_name ( (GtkAboutDialog*)about_dlg, _( "LXTask" ) );
#endif
    /* gtk_about_dialog_set_logo( (GtkAboutDialog*)about_dlg, gdk_pixbuf_new_from_file(  PACKAGE_DATA_DIR"/pixmaps/lxtask.png", NULL ) ); */
    gtk_about_dialog_set_logo_icon_name( (GtkAboutDialog*)about_dlg, "utilities-system-monitor" );
    gtk_about_dialog_set_copyright ( (GtkAboutDialog*)about_dlg, _( "Copyright (C) 2008-2020 LXDE team" ) );
    gtk_about_dialog_set_comments ( (GtkAboutDialog*)about_dlg, _( "Lightweight task manager for LXDE project" ) );
    gtk_about_dialog_set_license ( (GtkAboutDialog*)about_dlg, "This program is free software; you can redistribute it and/or\nmodify it under the terms of the GNU General Public License\nas published by the Free Software Foundation; either version 2\nof the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA." );
    gtk_about_dialog_set_website ( (GtkAboutDialog*)about_dlg, "http://lxde.org/" );
    gtk_about_dialog_set_authors ( (GtkAboutDialog*)about_dlg, authors );
    gtk_about_dialog_set_translator_credits ( (GtkAboutDialog*)about_dlg, translators );
    /*gtk_window_set_transient_for( (GtkWindow*) about_dlg, GTK_WINDOW( this ) );*/

    gtk_dialog_run( ( GtkDialog*)about_dlg );
    gtk_widget_destroy( about_dlg );
}

void change_list_store_view(void)
{
    gtk_tree_view_column_set_visible (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), COLUMN_PPID), full_view);
    gtk_tree_view_column_set_visible (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), COLUMN_MEM), full_view);
    //gtk_tree_view_column_set_visible (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), COLUMN_RSS), full_view);
    gtk_tree_view_column_set_visible (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), COLUMN_STATE), full_view);
    gtk_tree_view_column_set_visible (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), COLUMN_PRIO), full_view);

    if(!show_root_tasks && !show_other_tasks)
        gtk_tree_view_column_set_visible (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), COLUMN_UNAME), full_view);
    else
        gtk_tree_view_column_set_visible (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), COLUMN_UNAME), TRUE);
}

void fill_list_item(guint i, GtkTreeIter *iter)
{
    if(iter != NULL)
    {
        char buf[32];
        struct task *task = &g_array_index(task_array, struct task, i);

        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_NAME, task->name, -1);
        sprintf(buf,"%u",(guint)task->pid);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_PID, buf, -1);
        sprintf(buf,"%u",(guint)task->ppid);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_PPID, buf, -1);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_STATE, task->state, -1);

        /* size */
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_MEM, size_to_string(buf, (task->size)*1024), -1);

        /* rss */
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_RSS, size_to_string(buf, (task->rss)*1024), -1);

        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_UNAME, task->uname, -1);
        sprintf(buf,"%0d%%", (guint)task->time_percentage);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_TIME, buf, -1);
        sprintf(buf,"%d",task->prio);
        gtk_tree_store_set(GTK_TREE_STORE(list_store), iter, COLUMN_PRIO, buf, -1);    /* my change */
    }
}

void add_new_list_item(guint i)
{
        GtkTreeIter iter;
        gtk_tree_store_append(GTK_TREE_STORE(list_store), &iter, NULL);

        fill_list_item(i, &iter);
}

void refresh_list_item(guint i)
{
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store), &iter);
    struct task *task = &g_array_index(task_array, struct task, i);
    while(valid)
    {
        gchar *str_data = NULL;
        gtk_tree_model_get(GTK_TREE_MODEL(list_store), &iter, COLUMN_PID, &str_data, -1);

        if(str_data && task->pid == atol(str_data))
        {
            g_free(str_data);
            fill_list_item(i, &iter);
            break;
        }

        g_free(str_data);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
    }
}

void remove_list_item(pid_t pid)
{
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store), &iter);

    while(valid)
    {
        gchar *str_data = NULL;
        gtk_tree_model_get(GTK_TREE_MODEL(list_store), &iter, COLUMN_PID, &str_data, -1);

        if(str_data && pid == atol(str_data))
        {
            g_free(str_data);
            gtk_tree_store_remove(GTK_TREE_STORE(list_store), &iter);
            break;
        }

        g_free(str_data);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), &iter);
    }
}

gint compare_int_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column)
{
    gchar *s1 = "";
    gchar *s2 = "";

    gint ret;

    gtk_tree_model_get(model, iter1, column, &s1, -1);
    gtk_tree_model_get(model, iter2, column, &s2, -1);

    gint i1 = 0;
    gint i2 = 0;

    if(s1 != NULL)
        i1 = atoi(s1);
    if(s2 != NULL)
        i2 = atoi(s2);

    ret = i2 - i1;

    g_free(s1);
    g_free(s2);

    return ret;
}

gint compare_size_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column)
{
    gchar *s1 = "";
    gchar *s2 = "";

    gint ret;

    gtk_tree_model_get(model, iter1, column, &s1, -1);
    gtk_tree_model_get(model, iter2, column, &s2, -1);

    guint64 i1 = string_to_size(s1);
    guint64 i2 = string_to_size(s2);

    ret = (i2 > i1) - (i2 < i1);

    g_free(s1);
    g_free(s2);

    return ret;
}

gint compare_string_list_item(GtkTreeModel *model, GtkTreeIter *iter1, GtkTreeIter *iter2, gpointer column)
{
    gchar *s1 = "";
    gchar *s2 = "";

    gint ret = 0;

    gtk_tree_model_get(model, iter1, GPOINTER_TO_INT(column), &s1, -1);
    gtk_tree_model_get(model, iter2, GPOINTER_TO_INT(column), &s2, -1);

    if(s1 != NULL && s2 != NULL)
        ret = strcmp(s2, s1);

    g_free(s1);
    g_free(s2);

    return ret;
}

/* change the task view (user, root, other) */
void change_task_view(void)
{
    gint i = 0;

    gtk_tree_store_clear(GTK_TREE_STORE(list_store));
    change_list_store_view();

    for(i = 0; i < tasks; i++)
    {
        struct task *task = &g_array_index(task_array, struct task, i);

        if((task->uid == own_uid && show_user_tasks) || (task->uid == 0 && show_root_tasks) || (task->uid != own_uid && task->uid != 0 && show_other_tasks))
            add_new_list_item(i);
    }

    refresh_task_list();
}

void change_full_path(void)
{
    guint i;
    GArray *new_task_list;

    /* gets the new task list */
    new_task_list = (GArray*) get_task_list();

    /* check if task is new and marks the task that its checked*/
    for(i = 0; i < task_array->len; i++)
    {
        struct task *tmp = &g_array_index(task_array, struct task, i);
        struct task *new_tmp = &g_array_index(new_task_list, struct task, i);
	g_strlcpy(tmp->name, new_tmp->name, 255);
        refresh_list_item(i);
    }
	
    g_array_free(new_task_list, TRUE);    
}

void apply_prefs()
{
    g_source_remove(rID);
    refresh_interval = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (refresh_spin));
    rID = g_timeout_add(1000 * refresh_interval, (gpointer) refresh_task_list, NULL);
}


void show_preferences(void)
{
    GtkWidget *dlg;
    GtkWidget *c_area;
    GtkWidget *notebook = gtk_notebook_new();
    GtkWidget *general_box = gtk_vbox_new(FALSE, 0);
    GtkWidget *refresh_box = gtk_hbox_new(FALSE, 0);
    refresh_spin = gtk_spin_button_new_with_range(1, 60, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(refresh_spin), refresh_interval);

    dlg = gtk_dialog_new_with_buttons(_("Preferences"), NULL, 0, GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, NULL);
    c_area = gtk_dialog_get_content_area(GTK_DIALOG (dlg));

    gtk_box_pack_start(GTK_BOX (c_area), notebook, TRUE, TRUE, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK (notebook), general_box, gtk_label_new(_("General")));
    gtk_box_pack_start(GTK_BOX (refresh_box), gtk_label_new(_("Refresh rate (seconds):")), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX (refresh_box), refresh_spin, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX (general_box), refresh_box, TRUE, TRUE, 0);

    gtk_widget_show_all(notebook);

    if (gtk_dialog_run(GTK_DIALOG (dlg)) == GTK_RESPONSE_ACCEPT)
      apply_prefs();
    gtk_widget_destroy(dlg);
}
