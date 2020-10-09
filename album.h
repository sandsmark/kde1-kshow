/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	album.h
*/

#ifndef ALBUM_H 
#define ALBUM_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include "configdlg.h"

// QT
#include <qlabel.h>
#include <qwidget.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qpushbt.h>
#include <qbutton.h>
#include <qwmatrix.h>
#include <qpainter.h>
#include <qbttngrp.h>
#include <qcolor.h>
#include <qtimer.h>
#include <qtableview.h>

// KDE
#include <kapp.h>
#include <ktopwidget.h>
#include <ktoolbar.h>
#include <kwm.h>

// X11
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define DndURL 128

////////// Table Widget ///////////
class AlbumTable : public QTableView {


Q_OBJECT
  
public:
	AlbumTable(QWidget *parent=0, const char *name=0);
	~AlbumTable() {};

	void setTable( int th, QStrList liste );
	void nextPageView();
	void lastPageView();

private slots:
	void process();
	void sendSig();

private:
	int		tm;
	int		num, numa, nume ;
	int		cols;
	int		rows;
	int		sel_id;
	int		press_id;
	int		n_indx;
	int		pnum;
	QStrList	namelist;
	QList<QPixmap>	pixlist;
	QStrList	sizelist;
	QTimer		*timer;
	KDNDIcon	*dragicon;
	QString		dragurl;
	bool		dragon;
	
signals:
	void loadEnd( int, int, int );
	void doubleClick( int );
	void overPicName( QString , QString );

protected:
	void paintCell( QPainter *p, int r, int c );
	void resizeEvent( QResizeEvent * e);	
	void mousePressEvent( QMouseEvent * e);
	void mouseDoubleClickEvent( QMouseEvent * e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
};

//////////  Album Widget ///////////
class AlbumShow : public KTopLevelWidget {

Q_OBJECT
  
public:
	AlbumShow( opt_struct opt, QStrList fileList, QWidget *parent );
	~AlbumShow() {};	

	void reloadList( int th, QStrList liste_neu );
	void setNaviIcon();

private slots:
	void Quit();
	void pressLeft();
	void pressRight();
	void setPageNavi( int top, int num, int bottom);
	void setTableRects();
	void setFileName( int id);
	void resizeFullHeight();
	void setInfoBar( QString name, QString size);

private:
	bool		changeList;
	QString		page_tx;
	QStrList	liste;
	QLabel		*fenster;
	KToolBar	*navibar, *infobar;
	QPushButton	*ok_bt;
	QLabel		*page_lb; 
	AlbumTable	*table;
	QLabel		*picinfo;

signals:
	void closed();
	void clicked( QString, int );

protected:
	opt_struct optstr;
	void closeEvent( QCloseEvent * );
	void resizeEvent( QResizeEvent * );
};

#endif // ALBUM_H 
