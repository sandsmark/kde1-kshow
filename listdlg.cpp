/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	listdlg.cpp
*/

#include <listdlg.h>
#include <listdlg.moc>

ListDialog::ListDialog( QStrList fileList, QWidget *parent )
	: QDialog( parent, "kShow Imagelisting", TRUE ) {

	int width = 300;
	int height = 220;
	int center = width / 2;
	changeList = FALSE;
	
	resize( width, height );
	setFixedSize( size() );
	setCaption( i18n("kShow Imagelisting") );

	fenster = new QLabel( this );
	fenster->setFrameStyle( QFrame::Panel | QFrame::Raised );
	fenster->setGeometry( 0, 0, width, height - 45 );

	liste = fileList;

	ls_box = new QListBox( fenster );
	ls_box->setGeometry( 10, 10, fenster->width() - 95, fenster->height() - 50 );

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
	a_bt = new QPushButton(i18n("View"), fenster);
	a_bt->setGeometry(fenster->width() - 80, 10, 70, 25);
	connect(a_bt, SIGNAL(clicked()), SLOT( pressBtA() ));

	// Button b
	b_bt = new QPushButton(i18n("Add"), fenster);
	b_bt->setGeometry(fenster->width() - 80, 40, 70, 25);
	connect(b_bt, SIGNAL(clicked()), SLOT( pressBtB() ));

	// Button c
	c_bt = new QPushButton(i18n("Remove"), fenster);
	c_bt->setGeometry(fenster->width() - 80, 70, 70, 25);
	connect(c_bt, SIGNAL(clicked()), SLOT( pressBtC() ));

	// Button d
	d_bt = new QPushButton(i18n("Save"), fenster);
	d_bt->setGeometry(fenster->width() - 80, 100, 70, 25);
	connect(d_bt, SIGNAL(clicked()), SLOT( pressBtD() ));

	// ok button
	ok_bt = new QPushButton(i18n("Ok"), this);
	ok_bt->setGeometry(center - 35, height - 35, 70, 25);
	connect(ok_bt, SIGNAL(clicked()), SLOT( pressOk() ));
}

void ListDialog::setEntryNum( int inum ) {

	ph_box->setText( liste.at( inum ) );
	ls_box->setSelected( inum, TRUE );
	ls_box->centerCurrentItem();
}

void ListDialog::selectEntry( int num ) {

	ph_box->setText( liste.at( num ) );
}

QStrList ListDialog::getNewList() const { 

	return liste;
}

int ListDialog::getSelectNum() const {

	return ls_box->currentItem();
}

void ListDialog::pressOk() {

	if( changeList == TRUE )
		done(2);
	else{ reject(); }
}

void ListDialog::pressBtA() {

	if( changeList == TRUE )
		done(2);
	else{ done(1); }
}

void ListDialog::pressBtB() {

	fileUrl = KFileDialog::getOpenFileURL();
	if( !fileUrl.isNull() ) 
		addFileUrl();		
}

void ListDialog::pressBtC() {

	int remov;
	remov = ls_box->currentItem();
	ls_box->removeItem( remov );
	liste.remove( remov );
	setEntryNum( ls_box->currentItem() );
	changeList = TRUE;
}

void ListDialog::pressBtD() {

	done(3);
}

void ListDialog::addFileUrl() {

	QString mtext;
	QFileInfo fileinfo;
	if( fileUrl.left(5) == "file:" )
		fileUrl = fileUrl.remove( 0, 5 );

	fileinfo.setFile( fileUrl );
	if( fileinfo.isFile() != TRUE ) {  
		mtext = i18n("Directory cannot be adding");
		//"Verzeichnis kann nicht hinzugefügt werden"	

		KMsgBox::message( this, i18n("kShow Message"),( mtext ) );
	}
	else if( QImageIO::imageFormat( fileUrl ) == 0 ) {
		mtext = i18n("is not an Image File\n" \
					   "           or\n" \
					   "Image Format known");
		 //"ist keine Bilddatei oder Bildformat unbekannt"

		KMsgBox::message( this, i18n("kShow Message"),( mtext ) );
	}
	else {	
		ls_box->insertItem( fileinfo.fileName() , -1 );
		fileUrl = "file:" + fileUrl;
		liste.append( fileUrl );
		int neunum;
		neunum = liste.at();
		setEntryNum( neunum );
		changeList = TRUE;
	}
}
