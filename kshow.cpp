/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de
          
	kshow.cpp
*/

#include <kshow.h>
#include <kshow.moc>

extern KApplication *app;

KShowWindow::KShowWindow( QWidget *, const char *name )
	: KTopLevelWidget( name ) {

	single = TRUE;
	copyDir = QDir::homeDirPath();
	setCaption( "kShow" );
	windowList.setAutoDelete( FALSE );
	setMinimumSize( MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
	newVersion();

	filemenu = new QPopupMenu();
	filemenu->insertItem(icon("newwin.xpm"), i18n( "New Window" ), this, SLOT( NewWindow() ),0,0,-1);
	filemenu->insertSeparator(-1);
	filemenu->insertItem(icon("fileopen.xpm") , i18n( "Open..." ), this, SLOT( Load() ),0,1,-1 );
	filemenu->insertItem(icon("filenew.xpm"), i18n( "Catalog Open..." ), this, SLOT( openCat() ),0,2,-1 );
	filemenu->insertSeparator(-1);
	filemenu->insertItem(icon("editcopy.xpm"), i18n( "Copy To..." ), this, SLOT( Copy() ),0,3,-1 );
	filemenu->insertSeparator(-1);
	filemenu->insertItem( i18n( "Exit" ), this, SLOT( Quit() ),0,5,-1 );
    
	optionmenu = new QPopupMenu();
	optionmenu->insertItem(icon("help.xpm"), i18n( "Info..." ), this, SLOT( kShowBildInfo() ),0,0,-1 );
	optionmenu->insertSeparator(-1);
	optionmenu->insertItem(icon("next.xpm"), i18n( "Listing..." ), this, SLOT( ViewFileListe() ),0,1,-1 );
	optionmenu->insertItem(icon("thumdialog.xpm"), i18n( "Thumbnails..." ), this, SLOT( ViewAlbum() ),0,2,-1 );

	QPopupMenu *helpmenu = kapp->getHelpMenu( TRUE,
	"kShow v" VERSION "\n\n	Copyright 1998-1999\n         Ralf Berger\n  rberger@fun.horx.de" );

	menubar = new KMenuBar( this );
	menubar->insertItem( i18n( "&File" ), filemenu );
	menubar->insertItem( i18n( "&Image" ), optionmenu );
	menubar->insertItem( i18n( "&Options" ), this, SLOT( kShowConfig() ) );
	menubar->insertSeparator();
	menubar->insertItem( i18n( "&Help" ), helpmenu );
	setMenu( menubar );

	toolbar = new KToolBar( this, "toolbar" );
	toolbar->insertButton(icon("fileopen.xpm"), 0, SIGNAL(clicked()), this, SLOT(Load()), TRUE, i18n("Open..."));
	toolbar->insertButton(icon("editcopy.xpm"), 5, SIGNAL(clicked()), this, SLOT(Copy()), FALSE, i18n("Copy Image To ..."));
	toolbar->insertButton(icon("thumdialog.xpm"), 6, SIGNAL(clicked()), this, SLOT(ViewAlbum()), FALSE, i18n("View Thumbnail"));
	toolbar->insertSeparator();
	toolbar->insertButton(icon("back.xpm"), 1, SIGNAL(clicked()), this, SLOT(prevImage()), FALSE, i18n("Prev Image"));
	toolbar->insertButton(icon("forward.xpm"), 2, SIGNAL(clicked()), this, SLOT(nextImage()), FALSE, i18n("Next Image"));
	toolbar->insertSeparator();
	toolbar->insertSeparator();
	toolbar->insertButton(icon("diashow.xpm"), 3, SIGNAL(clicked()), this, SLOT(diaShow()), FALSE, i18n("Picture Show"));
	toolbar->insertButton(icon("stop.xpm"), 4, SIGNAL(clicked()), this, SLOT(diaStop()), FALSE, i18n("Show Stop"));
//	toolbar->insertSeparator();
//	toolbar->insertButton(icon("help.xpm"), 5, SIGNAL(clicked()), this, SLOT(inputFormat()), TRUE, i18n("Help"));
	addToolBar( toolbar );
	toolbar->setBarPos( KToolBar::Top );
	enableToolBar( KToolBar::Show );

	statusbar = new KStatusBar( this );
	statusbar->insertItem( i18n( "Image 0 from 0     " ), 0 );
	statusbar->insertItem( i18n( "Welcome to kShow..." ), 1 );
	setStatusBar( statusbar );
	
	///////////// Bildfenster ///////////////
	Scroller = new QwViewport( this );
	Pixview	= new PixView( Scroller->portHole() );	
	Scroller->portHole()->setBackgroundColor( optstr.WinColor );
	Pixview->setBackgroundColor( optstr.WinColor );
	Pixview->setCursor(crossCursor);	

	/////////////////////////////////////////
	keyset =new QAccel(this);
	keyset->connectItem( keyset->insertItem( Key_PageDown ), this, SLOT( nextImage() ) );
	keyset->connectItem( keyset->insertItem( Key_PageUp ), this, SLOT( prevImage() ) );

	/////////////////////////////////////////
	connect( menubar, SIGNAL( moved( menuPosition ) ), this , SLOT( barsMoved() ) );
	connect( toolbar, SIGNAL( moved( BarPosition ) ), this , SLOT( barsMoved() ) );
	connect( Pixview, SIGNAL( sizeChanged() ), this , SLOT( setImageRectangle() ) );

	KDNDDropZone * dropZone = new KDNDDropZone( Scroller->portHole(), DndURL);
	connect( dropZone, SIGNAL( dropAction( KDNDDropZone *) ), this, SLOT( slotDropEvent( KDNDDropZone *) ) );
	
	kfmConnection = NULL;
	menubar->show();
	toolbar->show();
	Scroller->show();
	statusbar->show();
	
	if( optstr.WinFix == FALSE ) 
	    this->resize( MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT ); 
	else this->resize( optstr.WinWidth, optstr.WinHeight ); 
}

void KShowWindow::newVersion()
{
	config = kapp->getConfig();
	loadConfig();
	config->setGroup("kshow");
	if( config->readEntry("Version", VERSION) != VERSION ) {
	    QString text;
	    QPixmap icon;
	    icon.load( kapp->kde_datadir().copy()+"/kshow/pics/manni.xpm");
	    text = i18n(
	    "They installed a new version of kShow.!!!\n" \
	    "The configuration file changed. They should\n" \
	    "check kShow the adjustments and store again.\n" \
	    "Much fun still ...\n" \
	    "                          rberger@fun.horx.de" );
	    QMessageBox *msg = new QMessageBox(this, "Message" );  
	    msg->setCaption( "kShow v" VERSION ); 
	    msg->setIconPixmap(icon);
	    msg->setText(text);
	    KSimpleConfig *oldc = new KSimpleConfig( kapp->localconfigdir()
						     + "/kshowrc" );
	    oldc->deleteGroup("kshow", TRUE);	
	    oldc->sync();
	}	
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
	fileListCheck();
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
	fileListCheck();
	fileUrl = fileList.first();
	LoadFile();
	}
}

void KShowWindow::NewWindow() {

	KShowWindow *ksw = new KShowWindow();
	ksw->show();
	windowList.append( ksw );
}

void KShowWindow::Close()
{
	if( windowList.count() > 1 ) {
	    windowList.remove( this );
	    delete this;
	}
	else{ Quit(); }
}

void KShowWindow::Quit() {

	app->quit();
	return;
}

void KShowWindow::Load() {

	fileUrl = KFileDialog::getOpenFileURL();

	if ( !fileUrl.isNull() ) {
		fileList.clear();
		fileList.append(fileUrl);
		fileListCheck();
	}
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
	QString c = i18n( "Image " ) + a + i18n( " from " ) + b; 	
	statusbar->changeItem( c, 0 );
	statusbar->changeItem( fileList.current(), 1 );
	if(fileList.count() > 0) 
	    Pixview->load( filename );
	else{ Pixview->clear(); }
	setImageRectangle();
}

void KShowWindow::setNavigator()
{
	if( fileList.current() == fileList.getLast() )
	    toolbar->setItemEnabled( 2, FALSE );  
	else toolbar->setItemEnabled( 2, TRUE );
		
	if( fileList.current() == fileList.getFirst() )
		toolbar->setItemEnabled( 1, FALSE );
	else toolbar->setItemEnabled( 1, TRUE );
	
	if( fileList.count() < 1 ) 
	    toolbar->setItemEnabled( 5, FALSE );
	else toolbar->setItemEnabled( 5, TRUE );

	if( fileList.count() > 1 ) {
	    if(stop_dia)	
		toolbar->setItemEnabled( 3, TRUE );
	    toolbar->setItemEnabled( 6, TRUE );
	}
	else{
	    toolbar->setItemEnabled( 3, FALSE );
	    toolbar->setItemEnabled( 6, FALSE );
	}
	if(stop_dia)	
	    toolbar->setItemEnabled( 4, FALSE );
}	

void KShowWindow::nextImage() {

	if( fileList.current() != fileList.getLast() ) {
		fileUrl = fileList.next();
		LoadFile();
	}
}

void KShowWindow::prevImage() {

	if( fileList.current() != fileList.getFirst() ) {
		fileUrl = fileList.prev();
		LoadFile();
	}
}

void KShowWindow::Message( QString mtext ) {

	KMsgBox::message( this, "kShow - Message", ( mtext ) );
}

void KShowWindow::fileListCheck()
{
	progdlg = new ProgDialog(fileList,this );
	connect( progdlg, SIGNAL( closed() ), this , SLOT( fileListOk() ) );
}

void KShowWindow::fileListOk()
{
	fileList = progdlg->getListe();
	if(albexec == TRUE)
	    album->reloadList( optstr.ThumSize, fileList );
	setNavigator();
	fileUrl = fileList.first();
	LoadFile();
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
	confdlg->setOptions( optstr );
	confdlg->exec();
	if(confdlg->result() == 1 || confdlg->result() == 2 ) {
	    optstr = confdlg->getOptions();
	    PcdOpt = optstr.PcdOpt;
	    if( optstr.WinFix == TRUE )  
		this->resize( optstr.WinWidth, optstr.WinHeight );
	    setImageRectangle();
	    Scroller->portHole()->setBackgroundColor( optstr.WinColor );
	    Pixview->setBackgroundColor( optstr.WinColor );

	    toolbar->setButtonPixmap(0,icon("fileopen.xpm"));
	    toolbar->setButtonPixmap( 5, icon("editcopy.xpm") );
	    toolbar->setButtonPixmap( 6, icon("thumdialog.xpm") );
	    toolbar->setButtonPixmap( 1, icon("back.xpm") );
	    toolbar->setButtonPixmap( 2, icon("forward.xpm") );
	    toolbar->setButtonPixmap( 3, icon("diashow.xpm") );
	    toolbar->setButtonPixmap( 4, icon("stop.xpm") );

	    filemenu->changeItem( icon("newwin.xpm"), i18n( "New Window" ), 0);
	    filemenu->changeItem( icon("fileopen.xpm"), i18n( "Open..." ), 1);
	    filemenu->changeItem( icon("filenew.xpm"), i18n( "Catalog Open..." ), 2);
	    filemenu->changeItem( icon("editcopy.xpm"), i18n( "Copy To..." ), 3);

	    optionmenu->changeItem( icon("help.xpm"), i18n( "Info..." ), 0);
	    optionmenu->changeItem( icon("next.xpm"), i18n( "Listing..." ), 1);
	    optionmenu->changeItem( icon("thumdialog.xpm"), i18n( "Thumbnails..." ), 2);

	    setNavigator();

	    if(confdlg->result() == 2 )
		writeConfig();
	}
	delete confdlg;
	single = TRUE;
}

QPixmap KShowWindow::icon( QString iname)
{
	QPixmap icon;
	bool ok;
	if( optstr.ToolIcon == AppTbI )
	    ok = icon.load(kapp->kde_datadir().copy()+"/kshow/pics/"+iname);
	else
	    ok = icon.load(kapp->kde_toolbardir().copy()+"/"+iname);
	if(!ok) {
	    icon.resize(22,22);
	    icon.fill(kapp->backgroundColor); 
	}
	return(icon);
}

void KShowWindow::loadConfig() {

	config->setGroup( "kshow" );
	optstr.ToolIcon = config->readNumEntry( "ToolIcon", KdeTbI );
	config->setGroup("window");	
	optstr.WinFix = config->readBoolEntry( "WinFix", FALSE );
	optstr.WinWidth = config->readNumEntry( "WinWidth" );
	optstr.WinHeight = config->readNumEntry( "WinHeight" );
	optstr.WinColor = config->readColorEntry( "WinColor" );	
	config->setGroup("slideshow");
	optstr.ShowAction = config->readNumEntry( "ShowAction", 1 );
	optstr.ShowTime = config->readNumEntry( "ShowTime", 3 );
	optstr.ShowLoop = config->readBoolEntry( "ShowLoop", FALSE );
	config->setGroup("image");
	optstr.PcdOpt = config->readEntry("PcdOption", "-pgm -1");
	optstr.ThumSize = config->readNumEntry( "ThumSize", 80 );
	PcdOpt = optstr.PcdOpt;
}

void KShowWindow::writeConfig() {

	if(optstr.WinWidth < 300 )
		optstr.WinWidth = 300;
	if(optstr.WinHeight < 200 )
		optstr.WinHeight = 200;
	config->setGroup("kshow");
	config->writeEntry( "Version", VERSION );
	config->writeEntry( "ToolIcon", optstr.ToolIcon );
	config->setGroup("window");
	config->writeEntry( "WinFix", optstr.WinFix );
	config->writeEntry( "WinWidth", optstr.WinWidth );
	config->writeEntry( "WinHeight", optstr.WinHeight );
	config->writeEntry( "WinColor", optstr.WinColor );
	config->setGroup("slideshow");
	config->writeEntry( "ShowAction", optstr.ShowAction );
	config->writeEntry( "ShowTime", optstr.ShowTime );
	config->writeEntry( "ShowLoop", optstr.ShowLoop );
	config->setGroup("image");
	config->writeEntry( "PcdOption", optstr.PcdOpt );
	config->writeEntry( "ThumSize", optstr.ThumSize);
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

	if(listdlg->result() > 0 ) {
	   if( listdlg->result() >= 2 ) {	// View selected Image
		fileList = listdlg->getNewList();
	   }
	   fileUrl = fileList.at( listdlg->getSelectNum() );
	   LoadFile();
	   if( listdlg->result() == 3 ) 	// Catalog save
		saveCat();
	}

	delete listdlg;
	single = TRUE;
}

void KShowWindow::setImageRectangle()
{
	if( optstr.WinFix == FALSE ) {
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
	timerID = startTimer( optstr.ShowTime * 1000 );	
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
	if( optstr.ShowAction == 1 ) {
	    if( fileList.current() == fileList.getLast() ) { 
		if( optstr.ShowLoop == TRUE ) {
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
	else if( optstr.ShowAction == 2 ) {
	    if( fileList.current() == fileList.getFirst() ) { 
		if( optstr.ShowLoop == TRUE ) {
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
	else if( optstr.ShowAction == 3 ) {
		int nr;
		do {
		   nr = rand() % fileList.count();
		} while( nr == fileList.at() );

		fileUrl = fileList.at( nr );
	}
	if( stop_dia != TRUE ) {
		LoadFile();
		timerID = startTimer( optstr.ShowTime * 1000 );
	}
}

void KShowWindow::Help() {

	app->invokeHTMLHelp( "" , "" );
}

void KShowWindow::saveCat() {

	int m = 0;
	QString catUrl;
	QString catDir;
	catDir = QDir::homeDirPath();
	catUrl = KFileDialog::getSaveFileName( catDir, "*.cat" );
	if(!catUrl.isNull() ) { 
	    if( catUrl.right(1) == "/" )
		catUrl = catUrl + "kshowcat01.cat";
	    if( catUrl.right(4) != ".cat" )
		catUrl = catUrl + ".cat";
	    QFileInfo fileinfo( catUrl );
	    if(fileinfo.exists()) {
		if(fileinfo.isWritable()) {
		    m = KMsgBox::yesNo( this,
		    i18n("kShow Warning"),
		    i18n("A Document with this Name exists already\n"\
			 "Do you want to overwrite it ?"),
		    2, i18n("Yes"), i18n("No") );
	        }	
	        else {	
		    KMsgBox::message( this,
		    i18n("kShow Message"),
		    i18n("You do not have write permission "\
		    	 "to this file.\n"),
		    4 );
		}
	    }
	    else{ m = 1; }
	    if( m == 1 ) {
		QFile cat( catUrl );			
		if(cat.open( IO_WriteOnly ) ) {		// catalog öffnen
		    int e = fileList.count();						
		    QTextStream t(&cat);
		    t <<"*kshow.cat*"<<'\n'; 
		    t <<"v"VERSION<<'\?';
		    t <<e<<'\n';	
		    t <<"rberger@fun.horx.de"<<'\n';
		    QString line;
		    for ( int i=0; i < e ; i++ ) {
			line = fileList.at(i);
			t <<line<<'\n';	
		    }	
		    cat.close();			// catalog schließen
		}
	    }
	}
}

void KShowWindow::openCat() {

	QString catUrl;
	QString catDir;
	QString line;
	catDir = QDir::homeDirPath();
	catUrl = KFileDialog::getOpenFileName( catDir, "*.cat" );
	if(!catUrl.isNull() && catUrl.right(4) == ".cat") { 
	    QFileInfo fileinfo( catUrl );
	    if(fileinfo.isReadable()) {
		fileList.clear();
		QFile cat( catUrl );			
		if(cat.open( IO_ReadOnly | IO_Raw ) ) {		// catalog öffnen
		    QTextStream t(&cat);
		    if( t.readLine() == "*kshow.cat*" ) {
			line = t.readLine();		
			line = t.readLine();
			while( !t.eof() ) {
			    line = t.readLine();		    				
			    fileList.append(line);
			}
		    }
 		cat.close();					// catalog schließen
		}
		fileListCheck();
	    }
	    else {
		KMsgBox::message( this,
		i18n("kShow Message"),
		i18n("You do not have read permission "\
		     "to this file.\n"),
		4 );	
	    }
	}
}

void KShowWindow::Copy() {

    if(fileList.count() > 0 ) { 
	copyDir = KDirDialog::getDirectory( copyDir );
	int m = 1;
	QString copyline = "cp -p ";
	QString zielUrl;
	QFileInfo fileinfo;
	if( !copyDir.isNull() ) {
	    fileinfo.setFile(copyDir);
	    if(fileinfo.isWritable()) {
		fileinfo.setFile(fileUrl);
		zielUrl = copyDir + fileinfo.fileName();	
		fileinfo.setFile( zielUrl );
		if( fileinfo.exists() ) {
		    m = KMsgBox::yesNo( this,
		    i18n("kShow Warning"),
		    i18n("A Document with this Name exists already\n"\
			 "Do you want to overwrite it ?"),
		    2, i18n("Yes"), i18n("No") );
		    copyline = "cp -bp ";	
		    if(!fileinfo.isWritable() && m == 1) {
			KMsgBox::message( this,
			i18n("kShow Message"),
			i18n("You do not have write permission "\
			     "to this file.\n"),
			4 );
		        m = 0;
		    }
		}
		if( m == 1) {
		   QString buf = copyline + fileUrl + " " + zielUrl;
		   char *line = qstrdup( buf );
		   system( line );
		}
	    }	
	    else{ KMsgBox::message( this,
		  i18n("kShow Message"),
		  i18n("You do not have write permission "\
		       "to this directory.\n"),
		  4 );
	    }
	}
    }
}

void KShowWindow::ViewAlbum()
{
	single = FALSE;
	album = new AlbumShow( optstr, fileList, 0 );
	albexec = TRUE;
	album->show();
	connect( album, SIGNAL( clicked( QString, int ) ), this , SLOT( viewSelPix( QString, int ) ) );
	connect( album, SIGNAL( closed() ), this , SLOT( closeAlbum() ) );
	single = TRUE;
}

void KShowWindow::viewSelPix( QString name, int pos )
{
	if(name == fileList.at(pos) ) {
	    fileUrl = fileList.at(pos);
	    LoadFile();
	}
}

void KShowWindow::closeAlbum()
{
	albexec = FALSE;
}

void KShowWindow::inputFormat()
{
	mtext ="";
	QStrList format = QImage::inputFormats();
	int e = format.count();
	for( int i=0; i <= e ;i++) {
		mtext = mtext + format.at(i) + "\n";
	}

	Message(mtext);
}

