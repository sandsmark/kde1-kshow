/*
 	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	album.cpp
*/

#include <album.h>
#include <album.moc>

////////////////// AlbumTable Widget /////////////////
AlbumTable::AlbumTable(QWidget *parent, const char *name ) 
	:QTableView( parent, name ) {

//	setFrameStyle( QFrame::Panel | QFrame::Raised );	
	setMouseTracking(TRUE);
	timer = new QTimer( this );
	connect( timer, SIGNAL( timeout() ), SLOT( process() ) );
	dragurl = "";
	dragon = FALSE; 
}

void AlbumTable::process()
{
	setCursor(waitCursor);
	double w, h;
	n_indx = 0;
	QWMatrix matrix;
	QString pw, ph, new_size;
	QPixmap *new_pix = 0;
	QString filename = namelist.at(numa);
	for( int i=numa; i < nume && filename !=0; i++ ) {
	    if( pixlist.at(i) == 0) {
		if( filename.left(5) == "file:" ) 
		    filename = filename.remove( 0, 5 );
		    new_pix = new QPixmap;
		    new_pix->load( filename );
		    pw.sprintf("%d", new_pix->width());
		    ph.sprintf("%d", new_pix->height());
		    new_size = pw + "x" + ph;
		if( new_pix->width() > tm || new_pix->height() > tm ) {
		    w = (double)100;
		    h = (double)100;
		    if(new_pix->width() > new_pix->height()) {
			w = (double)100 / ( (double) new_pix->width() / tm );
			h = (double) w;
		    }
		    else {
			h = (double) 100 / ( (double) new_pix->height() / tm );
			w = (double) h;
		    }
		    matrix.reset();
		    matrix.scale( (float) w/100, (float) h/100 );
		    QPixmap tmp_pix = new_pix->xForm(matrix);
		    new_pix->resize( tmp_pix.width(), tmp_pix.height() );
		    bitBlt( new_pix, 0, 0, &tmp_pix );
		    if ( tmp_pix.mask() )
			new_pix->setMask( *tmp_pix.mask() );
		}
		pixlist.append(new_pix);
		sizelist.append(new_size);
	    }
	    filename = namelist.next();
	    repaint(TRUE);
	}
	sendSig();
	setCursor(arrowCursor);
}

void AlbumTable::setTable( int th, QStrList liste)
{
	tm = th;
	numa = 0;
	sizelist.clear();
	namelist.clear();
	pixlist.clear();
	namelist = liste;
	repaint(TRUE);
	if ( timer->isActive() )
	    timer->stop();
	num = namelist.count();
	sel_id = 0;
	setCellWidth( tm+6 );
	setCellHeight( tm+6 );
	lastPageView();
}

void AlbumTable::paintCell( QPainter *p, int r, int c )
{
	int id;
	id = numa + ( r * numCols() ) + c;
 	if( id < num ) {
	    QPixmap *thumbnail = pixlist.at(id);
	    if( !thumbnail )
		return;
	    int w = cellWidth();
	    int h = cellHeight();
	    int pw = thumbnail->width();
	    int ph = thumbnail->height();
	    int x = (w-pw)/2;
	    int y = (h-ph)/2;
	    p->setPen(QColor(0,0,0));
	    p->drawPixmap( x, y, *thumbnail );
	    p->setPen(backgroundColor());
	    p->drawRect( x-3, y-3, pw+6, ph+6 );	
	    if( id == sel_id )
		p->setPen(QColor(0,0,0));
    	        p->drawRect( x-3, y-3, pw+6, ph+6 );
	}
}

void AlbumTable::nextPageView()
{
	numa = nume;
	nume = numa + numCols()*numRows();
	timer->start( 0, true );
}

void AlbumTable::lastPageView()
{
	numa = numa - numCols()*numRows(); 
	if(numa < 0 )
	    numa = 0;
	nume = numa + numCols()*numRows();
	timer->start( 0, true );
}

void AlbumTable::sendSig()
{	
	emit loadEnd( numa, num, nume );
}

void AlbumTable::resizeEvent( QResizeEvent * e)
{
	cols = width() / ( tm+6);
	rows = height() / ( tm+6);
	setNumCols( cols );
	setNumRows( rows );
	QTableView::resizeEvent(e);
	int s = cols*rows;
        numa = (numa/(nume-numa-1)) * s;	
	while( numa > num )
		numa = numa - s;  
	nume = numa + s;
	timer->start( 0, true );
}

void AlbumTable::mousePressEvent( QMouseEvent *e)
{
	setMouseTracking(FALSE);
	press_id = numa + ( findRow(e->pos().y()) * numCols() + findCol(e->pos().x()) );
	dragurl = namelist.at(press_id);
}

void AlbumTable::mouseDoubleClickEvent( QMouseEvent *e )
{
	if( namelist.count () == 0 )
	    return;
	int i = sel_id - numa;
	sel_id = numa + ( findRow(e->pos().y()) * numCols() + findCol(e->pos().x()) );
	updateCell( i / numCols(), i % numCols() );
	updateCell( findRow(e->pos().y()), findCol(e->pos().x()) );
	emit doubleClick( sel_id );
}

void AlbumTable::mouseMoveEvent(QMouseEvent *e)
{
	if(!hasMouseTracking() && !dragon) {
	    QPixmap *dragpix = pixlist.at(press_id);
	    int px = - dragpix->width()/2;
	    int py = - dragpix->height()/2;
	    QPoint p = mapToGlobal( e->pos() );
	    dragicon = new KDNDIcon(*dragpix,p.x()+px,p.y()+py );
	    dragon = TRUE;
	}
	if(dragon) {
	    QPoint p = mapToGlobal( e->pos() );
	    int x = p.x() - dragicon->width()/2;
	    int y = p.y() - dragicon->height()/2;
	    dragicon->move(x,y);
	}
	else {
	    int rw = findRow( e->pos().y() );
	    int cl = findCol( e->pos().x() );
	    if( rowIsVisible(rw) && colIsVisible(cl) ) {
		int nc = numa + ( rw * numCols() + cl );
		emit overPicName( namelist.at(nc), sizelist.at(nc) );
	    }
	    else{ emit overPicName( "", "" );}	
	}
}

void AlbumTable::mouseReleaseEvent(QMouseEvent *e)
{
  if(dragon) {
    QPoint p = mapToGlobal( e->pos() );
    dragon = FALSE;
    delete dragicon;

    char *dndData;
    int dndSize = dragurl.length() + 1;
    dndData = new char[ dndSize ];
    memcpy( dndData, dragurl.data(), dndSize );
    dndData[ dragurl.length() ] = 0;
    Window dndLastWindow = (Window)0;
    Window root = DefaultRootWindow( kapp->getDisplay() );
    XChangeProperty( kapp->getDisplay(), root, kapp->getDndSelectionAtom(),
	XA_STRING, 8, PropModeReplace, (const unsigned char*)dndData, dndSize);
    int x,y;
    Window win;
    XTranslateCoordinates( kapp->getDisplay(), root, root, p.x(), p.y(), &x, &y, &win );
    if ( win != 0 ) {
      XWindowAttributes Wattr;
      XGetWindowAttributes(kapp->getDisplay(), win, &Wattr);
      if (Wattr.map_state != IsUnmapped) {
	XEvent Event;
	Event.xclient.type          = ClientMessage;
	Event.xclient.display       = kapp->getDisplay();
	Event.xclient.message_type  = kapp->getDndProtocolAtom();
	Event.xclient.format        = 32;
	Event.xclient.window        = dndLastWindow;
	Event.xclient.data.l[0]     = DndURL;
	Event.xclient.data.l[1]     = 0;
	Event.xclient.data.l[2]     = 0;
	Event.xclient.data.l[3]     = p.x();
	Event.xclient.data.l[4]     = p.y();
	XSendEvent( kapp->getDisplay(), dndLastWindow, True, NoEventMask, &Event );	
	XSync( kapp->getDisplay(), FALSE );	
      }
    }
    else{
      Window root;
      Window parent;
      Window *children;
      unsigned int cchildren;
      root = DefaultRootWindow( kapp->getDisplay() );
      XQueryTree( kapp->getDisplay(), root, &root, &parent, &children, &cchildren );
      for ( uint i = 0; i < cchildren; i++ ) {
	XWindowAttributes Wattr;
	XGetWindowAttributes(kapp->getDisplay(), children[i], &Wattr);
	if (Wattr.map_state == IsUnmapped)
	  continue;
	XEvent Event;
	Event.xclient.type          = ClientMessage;
	Event.xclient.display       = kapp->getDisplay();
	Event.xclient.message_type  = kapp->getDndRootProtocolAtom();
	Event.xclient.format        = 32;
	Event.xclient.window        = children[ i ];
	Event.xclient.data.l[0]     = DndURL;
	Event.xclient.data.l[1]     = (long) time( 0L );
	Event.xclient.data.l[2]     = 0;
	Event.xclient.data.l[3]     = p.x();
	Event.xclient.data.l[4]     = p.y();
	XSendEvent( kapp->getDisplay(), children[i], True, NoEventMask, &Event);
	XSync( kapp->getDisplay(), FALSE );	
      }
    }
    delete [] dndData;
  }
  setMouseTracking(TRUE);
}

////////////////// AlbumShow Widget //////////////////

AlbumShow::AlbumShow( opt_struct opt, QStrList fileList, QWidget *parent )
	: KTopLevelWidget()
{
	optstr = opt;
	int width = (optstr.ThumSize + 7) * 3;
	int height = (optstr.ThumSize + 7) * 2 + 52;
	liste=fileList;
	changeList = FALSE;

	resize( width, height );
	setMinimumSize( width, height);
	setCaption( i18n("kShow Thumbnail") );

	navibar = new KToolBar(this);
	setNaviIcon();
	navibar->setBarPos( KToolBar::Bottom );
	addToolBar( navibar, 0 );
	enableToolBar( KToolBar::Show );

	infobar = new KToolBar(this);
	picinfo = new QLabel(infobar);
//	picinfo->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	infobar->insertWidget( 0, 20, picinfo );
     	infobar->setItemAutoSized ( 0, TRUE);

	infobar->setBarPos( KToolBar::Bottom );
	addToolBar(infobar, 1);
	enableToolBar( KToolBar::Show );

	table = new AlbumTable(this);
	connect( navibar, SIGNAL( moved( BarPosition ) ), this , SLOT( setTableRects() ) );
	connect( infobar, SIGNAL( moved( BarPosition ) ), this , SLOT( setTableRects() ) );
	connect( table, SIGNAL( loadEnd( int, int, int ) ), this , SLOT( setPageNavi( int, int, int) ) );
	connect( table, SIGNAL( doubleClick( int ) ), this , SLOT( setFileName( int ) ) );
	connect( table, SIGNAL( overPicName( QString, QString ) ),this ,SLOT( setInfoBar( QString, QString) ) );
	table->setTable( optstr.ThumSize, liste);
}

void AlbumShow::setNaviIcon()
{
	QPixmap icon;
	QString iconpath;
	if( optstr.ToolIcon == AppTbI )
	    iconpath = kapp->kde_datadir().copy()+"/kshow/pics/thum";
	else
	    iconpath = kapp->kde_toolbardir().copy() + "/";

	page_tx = i18n("Page: ");
	page_lb = new QLabel( page_tx + "0", navibar );
//	page_lb->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	navibar->insertWidget( 0, 55, page_lb );

	icon.load( iconpath+"back.xpm" );
	navibar->insertButton(icon, 1, SIGNAL(clicked()),this,SLOT(pressLeft()), FALSE, i18n("Prev Page"));

	icon.load( iconpath+"forward.xpm" );
	navibar->insertButton(icon, 2, SIGNAL(clicked()),this,SLOT(pressRight()), FALSE, i18n("Next Page"));

	icon.load( iconpath+"move.xpm" );
	navibar->insertButton(icon, 3, SIGNAL(clicked()),this,SLOT(resizeFullHeight()), TRUE, i18n("Resize full Vertical"));

	icon.load( iconpath+"exit.xpm" );
	navibar->insertButton(icon, 4, SIGNAL(clicked()),this,SLOT(Quit()), TRUE, i18n("Quit Thumbnail"));
	navibar->alignItemRight( 4, TRUE ); 
}

void AlbumShow::closeEvent( QCloseEvent * )
{
	Quit();
}

void AlbumShow::Quit()
{
	emit closed();
	delete this;
}

void AlbumShow::pressLeft()
{
	table->lastPageView();
}

void AlbumShow::pressRight()
{
	table->nextPageView();
}

void AlbumShow::resizeEvent( QResizeEvent * )
{
	updateRects();
	setTableRects(); 
}

void AlbumShow::setTableRects()
{
	int tw = width();
	int th = height();
	int tx=0; int ty=0;  
	int nw = navibar->width();
	int nh = navibar->height();
	int ih = infobar->height();

	if( infobar->barPos() != KToolBar::Top &&
	    infobar->barPos() != KToolBar::Bottom) {
	    if( infobar->barPos() != KToolBar::Floating )
		infobar->setBarPos( KToolBar::Bottom );
        }
	if( infobar->barPos() == KToolBar::Top )
	    ty += ih, th -= ih;
	else if( infobar->barPos() == KToolBar::Bottom )
	    th -= ih;
	if( navibar->barPos() == KToolBar::Top )
	    ty += nh, th -= nh;
	else if( navibar->barPos() == KToolBar::Bottom )  
	    th -= nh;
	else if( navibar->barPos() == KToolBar::Left )  
	    tx += nw, tw -= nw;
	else if( navibar->barPos() == KToolBar::Right )  
	    tw -= nw;
	table->setGeometry( tx, ty, tw, th );
}

void AlbumShow::setPageNavi( int top, int num, int bottom)
{
	QString pn;	
	pn.sprintf("%d", (top/(bottom-top))+ 1) ;
	page_lb->setText( page_tx + pn);

	if( bottom < num ) {
		navibar->setItemEnabled( 2, TRUE );	
	}
	else{
		navibar->setItemEnabled( 2, FALSE );	
	}
	if( top > 0 ) {
		navibar->setItemEnabled( 1, TRUE );	
	}
	else{
		navibar->setItemEnabled( 1, FALSE );	
	}
}

void AlbumShow::setFileName( int id )
{
	QString fname = liste.at(id);
	emit clicked( fname, id);
}

void AlbumShow::setInfoBar( QString name, QString size)
{
	name = name.replace( 0,name.findRev("/",name.length())+1,"");
	name += "   " + size;
	picinfo->setText(name);
}

void AlbumShow::resizeFullHeight()
{
	QRect rect = KWM::getWindowRegion(KWM::currentDesktop());
	move( rect.right()-width()-7, rect.top() );	// right  -7
	resize( width(), rect.bottom() - 27 );		// height -27
}

void AlbumShow::reloadList( int th, QStrList liste_neu )
{
	liste = liste_neu;
	table->setTable( th, liste);
}

