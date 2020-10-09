/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	infodlg.cpp
*/

#include <infodlg.h>
#include <infodlg.moc>

InfoDialog::InfoDialog( const char *filename, QWidget *parent )
	: QDialog( parent, "kShow Information", TRUE ) {

	int width = 300;
	int height = 220;
	int center = width / 2;
	
	resize( width, height );
	setFixedSize( size() );
	setCaption( i18n( "kShow Information" ) );

	fenster = new QLabel(this);

	fenster->setFrameStyle( QFrame::Panel | QFrame::Raised );
	fenster->setGeometry( 0, 0, width, height - 45 );

	QPixmap pix;
	pix.load( filename );
	image_width.sprintf("%d", pix.width());	
	image_height.sprintf("%d", pix.height());
	QString format = pix.imageFormat(filename);
	image_format = " " + format.lower();
	image_width = " " + image_width;
	image_height = " " + image_height;

	QFileInfo fileinfo; 
	fileinfo.setFile(filename);
	QString length;
	length.sprintf("%d", fileinfo.size() );
	image_length = " " + length + " byte";
	image_name = " " + fileinfo.fileName();
	image_path = " " + fileinfo.dirPath();

	lb_name = new QLabel( i18n("Name:"), fenster );
	lb_name->setAlignment(AlignRight | AlignVCenter);
	lb_name->setGeometry( 10, 10, 50, 25);
	tx_name = new QLabel( image_name, fenster );
	tx_name->setAlignment(AlignVCenter);
	tx_name->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	tx_name->setGeometry( 65, 12, width - 85, 21);

	lb_path = new QLabel( i18n("Path:"), fenster );
	lb_path->setAlignment(AlignRight | AlignVCenter);
	lb_path->setGeometry( 10, 35, 50, 25);
	tx_path = new QLabel( image_path, fenster );
	tx_path->setAlignment(AlignVCenter);
	tx_path->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	tx_path->setGeometry( 65, 37, width - 85, 21);

	lb_width = new QLabel( i18n("Width:"), fenster );
	lb_width->setAlignment(AlignRight | AlignVCenter);
	lb_width->setGeometry( 10, 60, 50, 25);
	tx_width = new QLabel( image_width, fenster );
	tx_width->setAlignment(AlignVCenter);
	tx_width->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	tx_width->setGeometry( 65, 62, 40, 21);

	lb_height = new QLabel( i18n("Height:"), fenster );
	lb_height->setAlignment(AlignRight | AlignVCenter);
	lb_height->setGeometry( 10, 85, 50, 25);
	tx_height = new QLabel( image_height, fenster );
	tx_height->setAlignment(AlignVCenter);
	tx_height->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	tx_height->setGeometry( 65, 87, 40, 21);

	lb_format = new QLabel( i18n("Format:"), fenster );
	lb_format->setAlignment(AlignRight | AlignVCenter);
	lb_format->setGeometry( 120, 60, 75, 25);
	tx_format = new QLabel( image_format, fenster );
	tx_format->setAlignment(AlignVCenter);
	tx_format->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	tx_format->setGeometry( 200, 62, width - 220, 21);

	lb_length = new QLabel( i18n("File length:"), fenster );
	lb_length->setAlignment(AlignRight | AlignVCenter);
	lb_length->setGeometry( 120, 85, 75, 25);
	tx_length = new QLabel( image_length, fenster );
	tx_length->setAlignment(AlignVCenter);
	tx_length->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	tx_length->setGeometry( 200, 87, width - 220, 21);

	lb_comment = new QLabel( i18n("Comment:"), fenster );
	lb_comment->setAlignment(AlignVCenter);
	lb_comment->setGeometry( 10, 115, 90, 25);
	tx_comment = new QLineEdit( fenster );
	tx_comment->setGeometry( 10, 135, width - 20, 25);

	ok_bt = new QPushButton(i18n("Ok"), this);
	ok_bt->setGeometry(center - 35, height - 35, 70, 25);
	connect(ok_bt, SIGNAL(clicked()), SLOT( reject() ));
}
