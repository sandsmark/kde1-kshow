/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de.de

	main.h
*/

#ifdef KSHOWDEBUG
#include <iostream.h>
#endif

// QT
#include <qkeycode.h>
#include <qbttngrp.h>
#include <qmsgbox.h>
#include <qwidget.h>
#include <qimage.h>
#include <qpoint.h>
#include <qevent.h>
#include <qscrbar.h>
#include <qdir.h>

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

#define MIN_WINDOW_WIDTH	300
#define MIN_WINDOW_HEIGHT	200

#define KSHOW_VERSION "0.1.2"

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

public slots:	
	void Load();
	void NewWindow();
	void Close();
	void Quit();

	void Message();
	void LoadFile();
	void CheckDir();	
	void setNavigator();
	void nextImage();
	void prevImage();
	void loadConfig();
	void writeConfig();
	void fillFileList();
	void checkFileList();

 	void slotDropEvent( KDNDDropZone * drop_icon );	
	void resizeScroller();
	void barsMoved();
	void kShowConfig();
	void kShowBildInfo();
	void ViewFileListe();
	void setImageRectangle();
	void diaShow();
	void diaStop();

private:	
	bool		single;

	KDNDDropZone	*dropZone;
	KFM		*kfmConnection;

	QString		filename;
	QString		fileUrl;
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
	QPopupMenu	*filemenu, *optionmenu, *helpmenu;

	MainWindow	*Window;
	QwViewport	*Scroller;
	PixView		*Pixview;
	bool		stop_dia;
	int		timerID;

	////////// Configs //////////
	bool		WinFix;
	QString		WinWidth;
	QString		WinHeight;
	QColor		WinColor;
	int		ShowAction;
	int		ShowTime;
	bool		ShowLoop;

protected:
	void resizeEvent( QResizeEvent * );
	void closeEvent( QCloseEvent * );
	void timerEvent( QTimerEvent *);
};

