/*
 *      utils.c
 *
 *      Copyright 2008 PCMan <pcman.tw@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>
#include "utils.h"

void show_error( const char* format, ... )
{
    GtkWidget* dlg;
    char* msg;
    va_list ap;
    va_start(ap, msg);
    msg = g_strdup_vprintf( format, ap );
    va_end(ap);

    dlg = gtk_message_dialog_new_with_markup( NULL, GTK_DIALOG_MODAL,
                                                                            GTK_MESSAGE_ERROR,
                                                                            GTK_BUTTONS_OK,
                                                                            msg );
    g_free( msg );
    gtk_window_set_title( (GtkWindow*)dlg, _("Error") );
    gtk_dialog_run( (GtkDialog*)dlg );
    gtk_widget_destroy( dlg );
}

gboolean confirm( const char* question )
{
    GtkWidget* dlg;
    int ret;
    dlg = gtk_message_dialog_new_with_markup( NULL, GTK_DIALOG_MODAL,
                                                                            GTK_MESSAGE_QUESTION,
                                                                            GTK_BUTTONS_YES_NO,
                                                                            question );
    gtk_window_set_title( (GtkWindow*)dlg, _("Confirm") );
    ret = gtk_dialog_run( (GtkDialog*)dlg );
    gtk_widget_destroy( dlg );

    return ret == GTK_RESPONSE_YES;
}

