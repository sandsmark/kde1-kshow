# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# just set the variable
APPSDIR = $(kde_appsdir)/Graphics
# set the include path for X, qt and KDE
INCLUDES= @all_includes@
# claim, which subdirectories you want to install
SUBDIRS = doc pics toolbar

#DEFS = @DEFS@ -I. -I$(srcdir) -I.. -DDOTRACE
####### This part is very test specific
# you can add here more. This one gets installed 
bin_PROGRAMS = kshow

# Which sources should be compiled for test.
kshow_SOURCES	= jpeg.cpp netpbm.cpp formats.cpp kshow.cpp \
                  QwViewport.cpp pixview.cpp configdlg.cpp infodlg.cpp \
		  listdlg.cpp progdlg.cpp album.cpp main.cpp

# the library search path
kshow_LDFLAGS = @all_libraries@ @KDE_RPATH@

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
kshow_LDADD  = -lkdeui -lkdecore -lkfm -lkfile -lgif -ljpeg -lqt -lX11 -lXext @LIBSOCKET@     

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = netpbm.h formats.h kshow.h \
		 QwViewport.h pixview.h configdlg.h infodlg.h \
		 listdlg.h progdlg.h album.h

# just to make sure, automake makes them 
BUILTSOURCES =	kshow.moc QwViewport.moc pixview.moc configdlg.moc \
		infodlg.moc listdlg.moc progdlg.moc album.moc

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
CLEANFILES = $(BUILTSOURCES) *~   

# make messages.po. Move this one to ../po/ and "make merge" in po
messages:
	$(XGETTEXT) -C -ki18n $(kshow_SOURCES)

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) kshow.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) kshow.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kshow.xpm $(kde_minidir)/kshow.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(APPSDIR)/kshow.kdelnk
	-rm -f $(kde_icondir)/kshow.xpm
	-rm -f $(kde_minidir)/kshow.xpm

# add a dependency for every moc file to be full portable
kshow.cpp:kshow.moc
kshow.moc: kshow.h
	$(MOC) kshow.h -o kshow.moc

QwViewport.cpp:QwViewport.moc
QwViewport.moc: QwViewport.h
	$(MOC) QwViewport.h -o QwViewport.moc

pixview.cpp:pixview.moc
pixview.moc: pixview.h
	$(MOC) pixview.h -o pixview.moc

configdlg.cpp:configdlg.moc
configdlg.moc: configdlg.h
	$(MOC) configdlg.h -o configdlg.moc

infodlg.cpp:infodlg.moc
infodlg.moc: infodlg.h
	$(MOC) infodlg.h -o infodlg.moc

listdlg.cpp:listdlg.moc
listdlg.moc: listdlg.h
	$(MOC) listdlg.h -o listdlg.moc

progdlg.cpp:progdlg.moc
progdlg.moc: progdlg.h
	$(MOC) progdlg.h -o progdlg.moc

album.cpp:album.moc
album.moc: album.h
	$(MOC) album.h -o album.moc

