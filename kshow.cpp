/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de
          
	kshow.cpp
*/

#include "main.h"
#include "formats.h"
#include "QwViewport.h"
#include "pixview.h"
#include "configdlg.h"
#include "infodlg.h"
#include "listdlg.h"

extern KApplication *app;

KShowWindow::KShowWindow( QWidget *, const char *name )
	: KTopLevelWidget( name ) {

	single = TRUE;
	setCaption( "kShow" );
	windowList.setAutoDelete( FALSE );
	setMinimumSize( MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);

	filemenu = new QPopupMenu();
	filemenu->insertItem( klocale->translate( "Open..." ), this, SLOT( Load() ) );
	filemenu->insertSeparator();
	filemenu->insertItem( klocale->translate( "New Window" ), this, SLOT( NewWindow() ) );
	filemenu->insertSeparator();
	filemenu->insertItem( klocale->translate( "Close" ), this, SLOT( Close() ) );
	filemenu->insertSeparator();
	filemenu->insertItem( klocale->translate( "Exit" ), this, SLOT( Quit() ) );
    
	optionmenu = new QPopupMenu();
	optionmenu->insertItem( klocale->translate( "Information..." ), this, SLOT( kShowBildInfo() ) );
	optionmenu->insertSeparator();
	optionmenu->insertItem( klocale->translate( "Imagelisting..." ), this, SLOT( ViewFileListe() ) );

	QPopupMenu *helpmenu = kapp->getHelpMenu( TRUE,
	"     kShow v" KSHOW_VERSION "\n\n        Copyright 1998\n            Ralf Berger\n     rberger@fun.horx.de     " );

	menubar = new KMenuBar( this );
	menubar->insertItem( klocale->translate( "&File" ), filemenu );
	menubar->insertItem( klocale->translate( "&Image" ), optionmenu );
	menubar->insertItem( klocale->translate( "&Options" ), this, SLOT( kShowConfig() ) );
	menubar->insertSeparator();
	menubar->insertItem( klocale->translate( "&Help" ), helpmenu );
	setMenu( menubar );
	
	QPixmap icon;
	QString iconpath;
	QString datapath;
	
	iconpath = kapp->kde_toolbardir().copy()+"/";
	datapath = kapp->kde_datadir().copy()+"/";
	toolbar = new KToolBar( this, "toolbar" );

	icon.load( iconpath+"fileopen.xpm" );
	toolbar->insertButton( icon, 0, SIGNAL(clicked()), this, SLOT(Load()), TRUE, klocale->translate("Open..."));

	icon.load( iconpath+"back.xpm" );
	toolbar->insertButton( icon, 1, SIGNAL(clicked()), this, SLOT(prevImage()), FALSE, klocale->translate("Prev Image"));

	icon.load( iconpath+"forward.xpm" );
	toolbar->insertButton( icon, 2, SIGNAL(clicked()), this, SLOT(nextImage()), FALSE, klocale->translate("Next Image"));

	toolbar->insertSeparator();
	toolbar->insertSeparator();
	icon.load( datapath+"kshow/diashow.xpm" );
	toolbar->insertButton( icon, 3, SIGNAL(clicked()), this, SLOT(diaShow()), FALSE, klocale->translate("Picture Show"));

	icon.load( iconpath+"stop.xpm" );
	toolbar->insertButton( icon, 4, SIGNAL(clicked()), this, SLOT(diaStop()), FALSE, klocale->translate("Show Stop"));

	toolbar->insertSeparator();
	icon.load( iconpath+"help.xpm" );
	toolbar->insertButton( icon, 5, SIGNAL(clicked()), this, SLOT(Help()), TRUE, klocale->translate("Help"));

	addToolBar( toolbar );
	toolbar->setBarPos( KToolBar::Top );
	enableToolBar( KToolBar::Show );

	statusbar = new KStatusBar( this );
	statusbar->insertItem( klocale->translate( "Image 0 from 0     " ), 0 );
	statusbar->insertItem( klocale->translate( "Welcome to kShow..." ), 1 );
	setStatusBar( statusbar );
	
	loadConfig();
	
	///////////// Bildfenster ///////////////

	Scroller = new QwViewport( this );
	Pixview	= new PixView( Scroller->portHole() );	

	Scroller->portHole()->setBackgroundColor( WinColor );
	Pixview->setBackgroundColor( WinColor );
	Pixview->setCursor(crossCursor);	

	/////////////////////////////////////////

	connect( menubar, SIGNAL( moved( menuPosition ) ), this , SLOT( barsMoved() ) );
	connect( toolbar, SIGNAL( moved( BarPosition ) ), this , SLOT( barsMoved() ) );
	connect( Pixview, SIGNAL( sizeChanged() ), this , SLOT( setImageRectangle() ) );

	KDNDDropZone * dropZone = new KDNDDropZone( Scroller->portHole(), DndURL);
	connect( dropZone, SIGNAL( dropAction( KDNDDropZone *) ), this, SLOT( slotDropEvent( KDNDDropZone *) ) );
	
	kfmConnection = NULL;

	if( WinFix == FALSE )  
	   this->resize( MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT ); 
	else { this->resize( WinWidth.toInt(), WinHeight.toInt() ); }
 
	menubar->show();
	toolbar->show();
	Scroller->show();
	statusbar->show();
	updateRects();
}

void KShowWindow::resizeEvent( QResizeEvent * ) {

	updateRects();
	resizeScroller();
}

void KShowWindow::argLoad( QStrList argList ) {

	int e = argList.count();
	QString string;

	for ( int i=0; i < e - 1; i++ ) {
		string = argList.next();
		if( string.left(5) != "file:" )
			string = "file:" + string;	

		fileList.append( string );
	}
	checkFileList();
	fileUrl = fileList.first();
	LoadFile();
}

void KShowWindow::closeEvent( QCloseEvent * ) {

	Close();
}

void KShowWindow::barsMoved() {

	updateRects();
	setImageRectangle();
}

void KShowWindow::slotDropEvent( KDNDDropZone * drop_icon ) {

	if( single ) {
	fileList = drop_icon->getURLList();
	fileUrl = fileList.first();
	checkFileList();
	fileUrl = fileList.first();
	LoadFile();
	}
}

void KShowWindow::Load() {

	fileUrl = KFileDialog::getOpenFileURL();

	if ( !fileUrl.isNull() ) {
		fileList.clear();
		CheckDir();
	}
}

void KShowWindow::NewWindow() {

	KShowWindow *ksw = new KShowWindow();
	ksw->show();
	windowList.append( ksw );
}

void KShowWindow::Close() {

	if ( windowList.count() > 1 ) {
		windowList.remove( this );
		delete this;
	} else {
		Quit();
	}
}

void KShowWindow::Quit() {

	app->quit();
	return;
}

void KShowWindow::CheckDir() {

	fileList.append(fileUrl);

	if( fileUrl.left(5) == "file:" )
		fileUrl = fileUrl.remove( 0, 5 );

	QFileInfo fileinfo;
	fileinfo.setFile( fileUrl );

	if( fileinfo.isDir() == TRUE ) { 
		fileList.clear();
		fillFileList();
		checkFileList();
		fileUrl = fileList.first();
		LoadFile();
	}
	else { LoadFile(); }
}

void KShowWindow::LoadFile() {

	if( fileUrl.left(5) == "file:" ) {
		filename = fileUrl.remove( 0, 5 );
	}
	else { filename = fileUrl; }

	setNavigator();
	QString	a, b;
	a.sprintf("%d", fileList.at() + 1);
	b.sprintf("%d", fileList.count());
	QString c = klocale->translate( "Image " ) + a + klocale->translate( " from " ) + b; 	
	statusbar->changeItem( c, 0 );
	statusbar->changeItem( fileList.current(), 1 );
	Pixview->load( filename );
	setImageRectangle();
}

void KShowWindow::setNavigator() {
	if( fileList.current() == fileList.getLast() ) {
		toolbar->setItemEnabled( 2, FALSE );  
	}
	else { toolbar->setItemEnabled( 2, TRUE );
	}

	if( fileList.current() == fileList.getFirst() ) {
		toolbar->setItemEnabled( 1, FALSE );
	}
	else { toolbar->setItemEnabled( 1, TRUE );
	}
}	

void KShowWindow::nextImage() {

	fileUrl = fileList.next();
	LoadFile();
}

void KShowWindow::prevImage() {

	fileUrl = fileList.prev();
	LoadFile();
}

void KShowWindow::Message() {

	KMsgBox::message( this, "kShow - Message", ( mtext ) );
}

void KShowWindow::fillFileList() {

	QDir filedir( fileUrl );
	const QFileInfoList *list = filedir.entryInfoList();
	QFileInfoListIterator it( *list );
	QFileInfo *fileinfo;
	
	while( ( fileinfo = it.current() ) )
	{
	    if ( strcmp( fileinfo->fileName().data(), "." ) != 0 && 
		 strcmp( fileinfo->fileName().data(), ".." ) != 0 )
	    {
		QString tmp = fileinfo->absFilePath().data();
		tmp = "file:" + tmp;
		fileList.append( tmp );
	    }
	    ++it;
	}
	return;
}

void KShowWindow::checkFileList() {

	QFileInfo fileinfo;
	int e = fileList.count();
	int i = 0;
	while( i < e ) {
		fileUrl = fileList.at( i );
		if( fileUrl.left(5) == "file:" )
			fileUrl = fileUrl.remove( 0, 5 );
		fileinfo.setFile( fileUrl );
		if( fileinfo.isDir() == TRUE ) {
			fileList.remove( i );
			fillFileList();
		}
		else if ( QImageIO::imageFormat( fileUrl ) == 0 ) {
			fileList.remove( i );
		}
		else { i++; }

		e = fileList.count();
	}
		
	if( fileList.count() > 1 )
		toolbar->setItemEnabled( 3, TRUE );
	else { toolbar->setItemEnabled( 3, FALSE );
	}
}

void KShowWindow::resizeScroller() {

	int sw = width();
	int sh = height();
	int sx=0; int sy=0;  

	if( menubar->menuBarPos() == KMenuBar::Top ) 
		sy = sy + menubar->height();

	if( menubar->menuBarPos() == KMenuBar::Bottom ) 
		sh = sh - menubar->height();				

	if( toolbar->barPos() == KToolBar::Top ) 
		sy = sy + toolbar->height();

	if( toolbar->barPos() == KToolBar::Bottom )
		sh = sh - toolbar->height();

	if( toolbar->barPos() == KToolBar::Right )
		sw = sw - toolbar->width();

	if( toolbar->barPos() == KToolBar::Left ) {
		sx = sx + toolbar->width();
		sw = sw - toolbar->width();
	}	

	Scroller->move( sx, sy );
	Scroller->resize( sw, sh - sy - statusbar->height() );
	Scroller->resizeScrollBars();
	Scroller->centralize( Pixview->width()/2, Pixview->height()/2 ,1.0, 1.0 );
}

void KShowWindow::kShowConfig() {

	single = FALSE;
	ConfigDialog *confdlg = new ConfigDialog( this );

	struct opt_struct optstr;
	optstr.WinFix = WinFix;
	optstr.WinWidth = WinWidth;
	optstr.WinHeight = WinHeight;
	optstr.WinColor = WinColor;
	optstr.ShowAction = ShowAction;
	optstr.ShowTime = ShowTime;
	optstr.ShowLoop = ShowLoop;
	
	confdlg->setOptions( optstr );

	confdlg->exec();

	if(confdlg->result() == 1 || confdlg->result() == 2 ) {

		struct opt_struct optstr = confdlg->getOptions();

		WinFix = optstr.WinFix;
		WinWidth = optstr.WinWidth;
		WinHeight = optstr.WinHeight;
		WinColor = optstr.WinColor;
		ShowAction = optstr.ShowAction;
		ShowTime = optstr.ShowTime;
		ShowLoop = optstr.ShowLoop;

		if( WinFix == TRUE )  
			this->resize( WinWidth.toInt(), WinHeight.toInt() );

		setImageRectangle();

		Scroller->portHole()->setBackgroundColor( WinColor );
		Pixview->setBackgroundColor( WinColor );

		if(confdlg->result() == 2 )
			writeConfig();
	}
	delete confdlg;
	single = TRUE;
}

void KShowWindow::loadConfig() {

	KConfig *config = kapp->getConfig();
	config->setGroup( "kshow" );
	WinFix = config->readBoolEntry( "WinFix", FALSE );
	WinWidth = config->readEntry( "WinWidth" );
	WinHeight = config->readEntry( "WinHeight" );
	WinColor = config->readColorEntry( "WinColor" );	
	ShowAction = config->readNumEntry( "ShowAction", 1 );
	ShowTime = config->readNumEntry( "ShowTime", 3 );
	ShowLoop = config->readBoolEntry( "ShowLoop", FALSE );
}

void KShowWindow::writeConfig() {

	KConfig *config = kapp->getConfig();

	if(WinWidth < 300 )
		WinWidth = "300";
	if(WinHeight < 200 )
		WinHeight = "200";
	
	config->writeEntry( "WinFix", WinFix );
	config->writeEntry( "WinWidth", WinWidth );
	config->writeEntry( "WinHeight", WinHeight );
	config->writeEntry( "WinColor", WinColor );
	config->writeEntry( "ShowAction", ShowAction );
	config->writeEntry( "ShowTime", ShowTime );
	config->writeEntry( "ShowLoop", ShowLoop );
	config->sync();
}

void KShowWindow::kShowBildInfo() {

	single = FALSE;
	InfoDialog *infodlg = new InfoDialog( filename, this );
	infodlg->exec();
	delete infodlg;
	single = TRUE;
}

void KShowWindow::ViewFileListe() {

	single = FALSE;
	ListDialog *listdlg = new ListDialog( fileList, this );

	listdlg->setEntryNum( fileList.at() );
	listdlg->exec();

	delete listdlg;
	single = TRUE;
}

void KShowWindow::setImageRectangle() {
	
	if( WinFix == FALSE ) {
		int window_width = 0;
		int window_height = 0;

			if( menubar->menuBarPos() != KMenuBar::Floating )
				window_height = window_height + menubar->height();

			if( toolbar->barPos() != KToolBar::Floating ) {

				if( toolbar->barPos() == KToolBar::Top )
					window_height = window_height + toolbar->height();

				else if( toolbar->barPos() == KToolBar::Bottom )
					window_height = window_height + toolbar->height();

				else { 
					window_width = window_width + toolbar->width();
				}
			}
		int w = Pixview->width() + window_width;
		int h = Pixview->height() + window_height + statusbar->height();
		this->resize( w, h );
	}
	resizeScroller();
}

void KShowWindow::diaShow() {

	stop_dia = FALSE;	
	toolbar->setItemEnabled( 3, FALSE );
	toolbar->setItemEnabled( 4, TRUE );
	timerID = startTimer( ShowTime * 1000 );	
	srand((unsigned) time((time_t *)0));
}

void KShowWindow::diaStop() {

	stop_dia = TRUE;
	killTimer( timerID );
	toolbar->setItemEnabled( 3, TRUE );
	toolbar->setItemEnabled( 4, FALSE );
}

void KShowWindow::timerEvent( QTimerEvent *) {

	killTimer( timerID );

///// show - forward /////
	if( ShowAction == 1 ) {
	    if( fileList.current() == fileList.getLast() ) { 
		if( ShowLoop == TRUE ) {
		    fileUrl = fileList.first();
		}
		else {
		    diaStop();	
		}
	    }
	    else { 
		fileUrl = fileList.next();
	    }
	}

///// show - backward /////
	else if( ShowAction == 2 ) {
	    if( fileList.current() == fileList.getFirst() ) { 
		if( ShowLoop == TRUE ) {
		    fileUrl = fileList.last();
		}
		else {
		    diaStop();	
		}
	    }
	    else { 
		fileUrl = fileList.prev();
	    }
	}

///// show - random /////
	else if( ShowAction == 3 ) {
		int nr;
		do {
		   nr = rand() % fileList.count();
		} while( nr == fileList.at() );

		fileUrl = fileList.at( nr );
	}
	if( stop_dia != TRUE ) {
		LoadFile();
		timerID = startTimer( ShowTime * 1000 );
	}
}
