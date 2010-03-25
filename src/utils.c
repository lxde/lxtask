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
    va_start(ap, format);
    msg = g_strdup_vprintf( format, ap );
    va_end(ap);

    dlg = gtk_message_dialog_new_with_markup( NULL, GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_ERROR,
                                              GTK_BUTTONS_OK,
                                              "%s", msg );
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
                                              "%s", question );
    gtk_window_set_title( (GtkWindow*)dlg, _("Confirm") );
    ret = gtk_dialog_run( (GtkDialog*)dlg );
    gtk_widget_destroy( dlg );

    return ret == GTK_RESPONSE_YES;
}

char* size_to_string( char* buf, guint64 size )
{
    char * unit;
    /* guint point; */
    gfloat val;

    /*
       FIXME: Is floating point calculation slower than integer division?
              Some profiling is needed here.
    */
    if ( size > ( ( guint64 ) 1 ) << 30 )
    {
        if ( size > ( ( guint64 ) 1 ) << 40 )
        {
            /*
            size /= ( ( ( guint64 ) 1 << 40 ) / 10 );
            point = ( guint ) ( size % 10 );
            size /= 10;
            */
            val = ((gfloat)size) / ( ( guint64 ) 1 << 40 );
            unit = "TB";
        }
        else
        {
            /*
            size /= ( ( 1 << 30 ) / 10 );
            point = ( guint ) ( size % 10 );
            size /= 10;
            */
            val = ((gfloat)size) / ( ( guint64 ) 1 << 30 );
            unit = "GB";
        }
    }
    else if ( size > ( 1 << 20 ) )
    {
        /*
        size /= ( ( 1 << 20 ) / 10 );
        point = ( guint ) ( size % 10 );
        size /= 10;
        */
        val = ((gfloat)size) / ( ( guint64 ) 1 << 20 );
        unit = "MB";
    }
    else if ( size > ( 1 << 10 ) )
    {
        /*
        size /= ( ( 1 << 10 ) / 10 );
        point = size % 10;
        size /= 10;
        */
        val = ((gfloat)size) / ( ( guint64 ) 1 << 10 );
        unit = "KB";
    }
    else
    {
        unit = size > 1 ? "Bytes" : "Byte";
        sprintf( buf, "%u %s", ( guint ) size, unit );
        return buf;
    }
    /* sprintf( buf, "%llu.%u %s", size, point, unit ); */
    sprintf( buf, "%.1f %s", val, unit );

    return buf;
}

guint64 string_to_size(char *s)
{
    double ret=0;
    char c;
    int count;
    if(!s) return 0;

    count=sscanf(s,"%lf %c",&ret,&c);
    if(count==0) return 0;
    if(c=='K') ret*=1LL<<10;
    else if(c=='M') ret*=1LL<<20;
    else if(c=='G') ret*=1LL<<30;
    else if(c=='T') ret*=1LL<<40;
    return (guint64)ret;
}

