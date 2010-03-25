/*
 *      utils.h
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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <gtk/gtk.h>
#include <stdarg.h>

G_BEGIN_DECLS

void show_error( const char* format, ... );
gboolean confirm( const char* question );
char* size_to_string( char* buf, guint64 size );
guint64 string_to_size(char *s);

G_END_DECLS

#endif
