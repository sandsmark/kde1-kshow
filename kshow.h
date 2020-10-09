/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	kshow.h
*/

#ifndef KSHOW_H 
#define KSHOW_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#ifdef KSHOWDEBUG
#include <iostream.h>
#endif

#include <qdir.h>

#include "QwViewport.h"
#include "pixview.h"
#include "configdlg.h"
#include "infodlg.h"
#include "listdlg.h"
#include "progdlg.h"
#include "album.h"

// QT
#include <qmessagebox.h> 

// KDE
#include <kfm.h>
#include <kapp.h>
#include <ktopwidget.h>
#include <kurl.h>
#include <kmsgbox.h> 
#include <kstatusbar.h>
#include <kbutton.h>
#include <klocale.h>
#include <ktoolbar.h>
#include <kpopmenu.h>
#include <kfileinfo.h>
#include <kmenubar.h>
#include <kfiledialog.h>
#include <kprogress.h> 
#include <kwizard.h>
#include <ksimpleconfig.h>


#define MIN_WINDOW_WIDTH	300
#define MIN_WINDOW_HEIGHT	200

class MainWindow;
class QwViewport;
class PixView;

class KShowWindow : public KTopLevelWidget {

	Q_OBJECT
	
public:
	KShowWindow( QWidget *parent = 0, const char *name = 0 );
	~KShowWindow() {};

	void argLoad( QStrList argList );

	static QList<KShowWindow> windowList;
	static QString PcdOpt;

private slots:
	QPixmap icon( QString iname);
	void newVersion();
	void Load();
	void NewWindow();
	void Close();
	void Quit();

	void Message( QString mtext);
	void LoadFile();
	void setNavigator();
	void nextImage();
	void prevImage();
	void loadConfig();
	void writeConfig();
	void fileListCheck();
	void fileListOk();

 	void slotDropEvent( KDNDDropZone *drop_icon );	
	void resizeScroller();
	void barsMoved();
	void kShowConfig();
	void kShowBildInfo();
	void ViewFileListe();
	void setImageRectangle();
	void diaShow();
	void diaStop();
	void Help(); 
	void saveCat();
	void openCat();
	void Copy();
	void ViewAlbum();
	void viewSelPix( QString name, int pos);
	void closeAlbum();

	void inputFormat();

private:	
	bool		single;

	KDNDDropZone	*dropZone;
	KFM		*kfmConnection;

	QString		filename;
	QString		fileUrl;
	QString		copyDir;
	QStrList  	fileList;
	QString		image_format;
	QString		mtext;
	QString		image_width;
	QString		image_height;
	QString		pixmap;
		
	KMenuBar 	*menubar;
	KToolBar	*toolbar;
	KStatusBar	*statusbar;
	KMsgBox		*messagebox;
	KConfigBase 	*config;
	QPopupMenu	*filemenu, *optionmenu, *helpmenu;
	QAccel		*keyset;

	MainWindow	*Window;
	QwViewport	*Scroller;
	PixView		*Pixview;
	AlbumShow	*album;
	ProgDialog	*progdlg;
	bool		stop_dia;
	int		timerID;

protected:
	bool		albexec;	
	opt_struct	optstr;
	void resizeEvent( QResizeEvent * );
	void closeEvent( QCloseEvent * );
	void timerEvent( QTimerEvent *);

};

#endif // KSHOW_H 
