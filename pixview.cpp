/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de
          
	pixview.cpp
*/

#include <pixview.h>
#include <pixview.moc>

PixView::PixView(QWidget *parent, const char *name, WFlags f)
	: QLabel(parent, name, f) {

	draw = FALSE;		
	setAutoResize(TRUE);
	image = new QPixmap();
	copyimage = new QPixmap();
	matrix = new QWMatrix();
	setPixmap( *image );

	QPopupMenu *popin = new QPopupMenu;	
	popin->insertItem(" 25 % ");
	popin->insertItem(" 50 % ");
	popin->insertItem(" 75 % ");
	popin->insertItem("100 % ");

	QPopupMenu *popout = new QPopupMenu;	
	popout->insertItem(" 25 % ");
	popout->insertItem(" 50 % ");
	popout->insertItem(" 75 % ");

	popmenu = new QPopupMenu;
	popmenu->insertItem( i18n("Restore"), this, SLOT( restoreImage() ) );
	popmenu->insertItem( i18n("Zoom in..."), popin );
	popmenu->insertItem( i18n("Zoom out..."), popout );

	resize( image->size() );

	connect( popin, SIGNAL( activated( int ) ), this, SLOT( zoomIn( int ) ) );	
	connect( popout, SIGNAL( activated( int ) ), this, SLOT( zoomOut( int ) ) );	

	keyset =new QAccel(this);
	keyset->insertItem( Key_Plus, 200 );
	keyset->insertItem( Key_Minus, 201 );
	keyset->insertItem( SHIFT + Key_Plus, 202 );
	keyset->insertItem( SHIFT + Key_Minus, 203 );
	keyset->insertItem( Key_Asterisk, 204 );
	connect( keyset, SIGNAL( activated( int ) ), this, SLOT( pressKey( int ) ) );
}

bool PixView::load(const char *filename) {
	bool ret;
	image->load( filename );
	setPixmap( *image );
	resize( image->size() );
	ret = TRUE;
	return ret;
}

bool PixView::clear() {
	bool ret;
	QPixmap nullimage;
	setPixmap( nullimage );
	resize( image->size() );
	ret = TRUE;
	return ret;
}

void PixView::mousePressEvent(QMouseEvent *mouse) {

	if( mouse->button() == LeftButton ) {
	    if(draw == FALSE ) {
		startx = (mouse->pos()).x();
		starty = (mouse->pos()).y();
		oldx = startx;
		oldy = starty;
		draw = TRUE;
	    }
	}
	if( mouse->button() == RightButton ) {
 	    if(popmenu)
		popmenu->popup( QCursor::pos() );
	}
}

void PixView::mouseMoveEvent(QMouseEvent *mouse) {

	int x, y;
	QPainter paint;
		
	x = ( mouse->pos() ).x();
	y = ( mouse->pos() ).y();

	if( (oldx != x) || (oldy != y) ) {
	    if(draw) {
		paint.begin( this );
		paint.setPen( QPen( green, 0, DashLine ) );
		paint.setRasterOp(XorROP);

		paint.drawRect( startx, starty, oldx - startx, oldy - starty );
		paint.drawRect( startx, starty, x - startx, y - starty );

		oldx = x;
		oldy = y;

		paint.end();
	    }
	}
}


void PixView::mouseReleaseEvent(QMouseEvent *mouse) {

	int x, y;
	QPainter paint;
	
	if( mouse->button() == LeftButton ) { 
	    x = ( mouse->pos() ).x();
	    y = ( mouse->pos() ).y();

	    paint.begin( this );
	    paint.setPen( QPen( green, 0, DashLine ) );
	    paint.setRasterOp(XorROP);

	    paint.drawRect( startx, starty, oldx - startx, oldy - starty );

	    paint.end();
	    draw = FALSE;
	}
}

void PixView::restoreImage() {

	int z = 100;
	zoomPixmap( z );
}

void PixView::zoomIn( int id ) {

	++id;
	id = id * 25;
	id = id + 100;	
	zoomPixmap( id );
}

void PixView::zoomOut( int id ) {

	++id;
	id = id * 25;
	id = 100 - id;
	zoomPixmap( id );
}

void PixView::zoomPixmap( int z ) {

	QPainter paint;
	int w, h;

	if( copyimage == 0 )	
		*copyimage = *image;
	
	matrix->reset();
	matrix->scale((float) z/100, (float) z/100);

	w = (int) (image->width()* ((float) z/100));
	h = (int) (image->height()*((float) z/100));
	
 	paint.begin(image);
	paint.setWorldMatrix(*matrix);
	paint.drawPixmap(0,0,*copyimage);
	paint.end();

	resize( w, h );
	setPixmap(image->xForm(*matrix));

	emit sizeChanged();
}

void PixView::pressKey( int key ) {

	switch( key ) {
	case 200:
		zoomIn( 0 );
		break;
	case 201:
		zoomOut( 0 );
		break;
	case 202:
		zoomIn( 1 );
		break;
	case 203:
		zoomOut( 1 );
		break;
	case 204:
		restoreImage();
		break;
	}
}
