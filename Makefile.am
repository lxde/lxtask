ACLOCAL_AMFLAGS = -I .

SUBDIRS =	po src

desktopdir = $(datadir)/applications
desktop_in_files = data/lxtask.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

man_MANS = data/lxtask.1

distclean-local:
	rm -rf *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

EXTRA_DIST =								\
	$(desktop_in_files)

DISTCLEANFILES =							\
	$(desktop_DATA)
