lxtask_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-DG_LOG_DOMAIN=\"xfce4-taskmanager\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\"

bin_PROGRAMS = lxtask

lxtask_SOURCES = \
	main.c \
	callbacks.c \
	callbacks.h \
	functions.c \
	functions.h \
	interface.c \
	interface.h \
	xfce-taskmanager-linux.c \
	xfce-taskmanager-linux.h \
	types.h \
	utils.c \
	utils.h

lxtask_CFLAGS =	$(GTK_CFLAGS)

lxtask_LDADD = $(GTK_LIBS)
