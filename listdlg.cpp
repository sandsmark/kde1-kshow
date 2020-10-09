/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	listdlg.cpp
*/

#include "listdlg.h"

ListDialog::ListDialog( QStrList fileList, QWidget *parent )
	: QDialog( parent, "kShow Imagelisting", TRUE ) {

	int width = 300;
	int height = 215;
	int center = width / 2;
	
	resize( width, height );
	setFixedSize( size() );
	setCaption( klocale->translate( "kShow Imagelisting" ) );

	fenster = new QLabel( this );
	fenster->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
	fenster->setGeometry( 5, 5, width -10, height - 50 );

///// List Fenster /////
	liste = fileList;

	ls_box = new QListBox( fenster );
	ls_box->setGeometry( 10, 10, fenster->width() - 95, fenster->height() - 50 );
//	ls_box->setScrollBar( TRUE );

	int e = liste.count();

	QFileInfo fileinfo;

	for( int i=0; i <= e; i++) {
		fileinfo.setFile( liste.at( i ) );
		ls_box->insertItem( fileinfo.fileName() , -1 );
	}

	ph_box = new QLineEdit( fenster );
	ph_box->setGeometry( 10, fenster->height()- 35, fenster->width() - 20, 25);

	connect( ls_box, SIGNAL( highlighted( int ) ), SLOT( selectEntry( int ) ) );

///// Button Leiste ////

	// Button a
	a_bt = new QPushButton(klocale->translate(" "), fenster);
	a_bt->setGeometry(fenster->width() - 80, 10, 70, 25);

	// Button b
	b_bt = new QPushButton(klocale->translate(" "), fenster);
	b_bt->setGeometry(fenster->width() - 80, 40, 70, 25);

	// Button c
	c_bt = new QPushButton(klocale->translate(" "), fenster);
	c_bt->setGeometry(fenster->width() - 80, 70, 70, 25);

	// Button d
	d_bt = new QPushButton(klocale->translate(" "), fenster);
	d_bt->setGeometry(fenster->width() - 80, 100, 70, 25);


	// ok button
	ok_bt = new QPushButton(klocale->translate("Ok"), this);
	ok_bt->setGeometry(center - 36, height - 37, 72, 27);
	connect(ok_bt, SIGNAL(clicked()), SLOT( reject() ));
}

void ListDialog::setEntryNum( int inum ) {

	ph_box->setText( liste.at( inum ) );
	ls_box->setSelected( inum, TRUE );
	ls_box->centerCurrentItem();
}

void ListDialog::selectEntry( int num ) {

	ph_box->setText( liste.at( num ) );
}
