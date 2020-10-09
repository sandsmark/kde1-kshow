/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	pixview.h
*/

#ifndef PIXVIEW_H 
#define PIXVIEW_H 

// QT
#include <qwmatrix.h>
#include <qpainter.h>
#include <qpen.h>
#include <qaccel.h>

// KDE
#include <kapp.h>
#include <kmsgbox.h>

class PixView : public QLabel
{
	Q_OBJECT
private:
	QWMatrix	*matrix;
	QPopupMenu	*popmenu; 
	QPixmap		*copyimage;
	QPixmap	 	*image;
	QAccel		*keyset;
	int		startx, starty;
	int		oldx, oldy;
	bool		draw;

private slots:
	void pressKey( int );

public:
	PixView(QWidget *parent=0, const char *name=0, WFlags f = 0);
	bool load(const char *filename);
	bool clear();

public slots:	
	void restoreImage();
	void zoomIn( int );
	void zoomOut( int );
	void zoomPixmap( int );
signals:
	void sizeChanged();

protected:
	void mousePressEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
};

#endif // PIXVIEW_H 
