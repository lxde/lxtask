/* $Id: main.c 2350 2007-01-13 10:12:31Z nick $
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glib.h>
#include <signal.h>
#include <unistd.h>

#include "types.h"
#include "interface.h"
#include "functions.h"

GtkWidget *main_window;

GArray *task_array;
gint tasks;
gint own_uid;

gchar *config_file;

gboolean show_user_tasks;
gboolean show_root_tasks;
gboolean show_other_tasks;

gboolean show_cached_as_free; /* Show memory used Cache as free memory */

gboolean full_view;

guint win_width;
guint win_height;
guint refresh_interval;
guint rID;

int PAGE_SIZE;

int main (int argc, char *argv[])
{

#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif

    gtk_set_locale ();
    gtk_init (&argc, &argv);

    PAGE_SIZE=sysconf(_SC_PAGESIZE)>>10;
    own_uid = getuid();

    config_file = g_build_filename(g_get_user_config_dir(), "lxtask.conf", NULL);
    load_config();

    task_array = g_array_new (FALSE, FALSE, sizeof (struct task));
    tasks = 0;

    main_window = create_main_window ();
    gtk_widget_show (main_window);

    if(!refresh_task_list())
        return 0;

    rID = g_timeout_add_seconds(refresh_interval, (gpointer) refresh_task_list, NULL);

    gtk_main ();

    return 0;
}

