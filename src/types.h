/* $Id: types.h 3940 2008-02-10 22:48:45Z nebulon $
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

#ifndef TYPES_H
#define TYPES_H

#include <gtk/gtk.h>

#define REFRESH_INTERVAL 1000

struct task
{
    pid_t pid;
    pid_t ppid;
    uid_t uid;
    gchar uname[64];
    gchar name[255];
    gchar state[16];
    guint64 size;
    guint64 rss;
    gboolean checked;
    gint time;
    gint old_time;
    gfloat time_percentage;
    gint prio;  /* my change */
};

typedef struct
{
    guint64 mem_total;
    guint64 mem_free;
    guint64 mem_cached;
    guint64 mem_buffered;
    guint64 cpu_count;
    guint64 cpu_idle;
    guint64 cpu_user;
    guint64 cpu_nice;
    guint64 cpu_system;
    guint64 cpu_old_jiffies;
    guint64 cpu_old_used;
    gboolean valid_proc_reading;
} system_status;

extern GtkWidget *main_window;

extern GArray *task_array;
extern gint tasks;
extern uid_t own_uid;

extern gchar *config_file;

extern gboolean show_user_tasks;
extern gboolean show_root_tasks;
extern gboolean show_other_tasks;
extern gboolean show_full_path;

extern gboolean show_cached_as_free; /* Show memory used Cache as free memory */

extern gboolean full_view;

extern gint win_width;
extern gint win_height;

extern int page_size;

#endif
