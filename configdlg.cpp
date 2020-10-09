/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	configdlg.cpp
*/

#include <configdlg.h>
#include <configdlg.moc>

ConfigDialog::ConfigDialog( QWidget *parent )
	:QDialog( parent, "kShow Options", TRUE ) {

	int width = 300;
	int height = 200;
	mode_list.append("-ppm");
	mode_list.append("-pgm");
	mode_list.append("-ycc");
	resize( width, height);
	setCaption( i18n( "kShow Options" ) );
	karten = new KTabCtl( this );
	karten->setGeometry( 0, 0, width, height );
	window = new QWidget( karten );
	karten->addTab( window, i18n( "Window" ) );
	diashow = new QWidget( karten );
	karten->addTab( diashow, i18n( "ImageShow" ) );
	pcdimage = new QWidget( karten );
	karten->addTab( pcdimage, i18n( "PCD Image" ) );
	album = new QWidget( karten );
	karten->addTab( album, i18n( "Thumbnail" ) );
	privat = new QWidget( karten );
	karten->addTab( privat, i18n( "Privat" ) );
	if(karten->width() < width )
		karten->resize( width, karten->height());
	resize( karten->width(), karten->height() + 45 );
	setFixedSize( size() );

///////////// WINDOW SETTINGS /////

	QLabel *wfeld = new QLabel(window);
	wfeld->setGeometry( karten->width()/2 - 105,0 ,210 ,165 );
	
	win_fix = new QCheckBox( i18n("fixed Rectangle"), wfeld );
	win_fix->setGeometry( 10, 10, wfeld->width() - 20, 30 );

	lb_width = new QLabel( i18n("Window Width:"), wfeld );
	lb_width->setAlignment(AlignRight | AlignVCenter);
	lb_width->setGeometry( 0, 50, 130, 25);

	win_width = new QSpinBox(300,1200,10,wfeld);
	win_width->setGeometry( 140, 50, 55, 25);

	lb_height = new QLabel( i18n("Window Height:"), wfeld );
	lb_height->setAlignment(AlignRight | AlignVCenter);
	lb_height->setGeometry( 0, 90, 130, 25);

	win_height = new QSpinBox(200,800,10,wfeld);
	win_height->setGeometry( 140, 90, 55, 25);
		
	lb_bgcolor = new QLabel( i18n("BackgroundColor:"), wfeld);
	lb_bgcolor->setAlignment(AlignRight | AlignVCenter);
	lb_bgcolor->setGeometry( 0, 130, 130, 25);

	win_bgcolor = new KColorButton( optstr.WinColor, wfeld );
	win_bgcolor->move( 140, 130 );
	win_bgcolor->resize( 50, 25 );

	setFix();
	connect( win_fix, SIGNAL( clicked() ), SLOT( click_win_fix() ) );
	connect( win_bgcolor, SIGNAL( changed( const QColor & ) ), SLOT ( colorchange( const QColor & ) ) ); 
	connect( win_width, SIGNAL( valueChanged( int ) ), SLOT ( widthchange( int ) ) );
	connect( win_height, SIGNAL( valueChanged( int ) ), SLOT ( heightchange( int ) ) );

///////////// DiaShow SETTINGS /////

	QLabel *dfeld = new QLabel(diashow);
	dfeld->setGeometry( karten->width()/4 - 70,0 ,140 ,150 );
	
	lb_tx = new QLabel( i18n("Delay Time"), dfeld );
	lb_tx->setAlignment(AlignCenter | AlignVCenter);
	lb_tx->setGeometry( 10, 25, 120, 25);

	lcd = new QLCDNumber( 2, dfeld );
	lcd->setGeometry( 10, 50 , 120, 30 );
	lcd->setFrameStyle(QFrame::NoFrame);
	lcd->setNumDigits( 2 );

	lb_sk = new QLabel( i18n("Seconds"), dfeld );
	lb_sk->setAlignment(AlignCenter | AlignVCenter);
	lb_sk->setGeometry( 10, 80, 120, 25);

	slider = new KSlider( 1, 30, 1, 1, KSlider::Horizontal, dfeld );
	slider->setRange( 0, 30 );
	slider->setSteps( 1, 10 );
	slider->setGeometry( 10, 110, 120, 20 );

	connect( slider, SIGNAL( valueChanged( int ) ), SLOT( timechange( int ) ));

	QLabel *ffeld = new QLabel(diashow);
	ffeld->setGeometry( (karten->width()/ 4)*3 - 75 ,0 ,150 ,150 );
//	ffeld->setFrameStyle( QFrame::Panel | QFrame::Raised );

	dia_for = new QCheckBox( i18n("show forward"), ffeld );
	dia_for->setGeometry( 10, 10, ffeld->width() - 10, 27 );

	dia_bac = new QCheckBox( i18n("show backward"), ffeld );
	dia_bac->setGeometry( 10, 40, ffeld->width() - 10, 27 );

	dia_ran = new QCheckBox( i18n("show random"), ffeld );
	dia_ran->setGeometry( 10, 70, ffeld->width() - 10, 27 );
	
	dia_loop = new QCheckBox( i18n("show loop"), ffeld );
	dia_loop->setGeometry( 10, 120, ffeld->width() - 10, 27 );

	connect( dia_for, SIGNAL( clicked() ), SLOT( showvalue_1() )); 
	connect( dia_bac, SIGNAL( clicked() ), SLOT( showvalue_2() ));
	connect( dia_ran, SIGNAL( clicked() ), SLOT( showvalue_3() ));
	connect( dia_loop, SIGNAL( clicked() ), SLOT( click_dia_loop() ));

///////////// PCD-Image SETTINGS /////

	pcd_size = new QButtonGroup( i18n("Extract Size"), pcdimage );
	pcd_size->setGeometry( karten->width()/4 - 55 , 20, 120, 125 );
	pcd_size->setExclusive( TRUE );

	pcd_a = new QPushButton( "128 x 192", pcd_size );
	pcd_a->setToggleButton( TRUE );
	pcd_a->setGeometry( 15, 25, 90, 25 );

	pcd_b = new QPushButton( "256 x 384", pcd_size );
	pcd_b->setToggleButton( TRUE );
	pcd_b->setGeometry( 15, 55, 90, 25 );

	pcd_c = new QPushButton( "512 x 768", pcd_size );
	pcd_c->setToggleButton( TRUE );
	pcd_c->setGeometry( 15, 85, 90, 25 );

	pcd_size->insert( pcd_a, 1 );
	pcd_size->insert( pcd_b, 2 );
	pcd_size->insert( pcd_c, 3 );

	pcd_mode = new QButtonGroup( i18n("Extract Mode"), pcdimage );
	pcd_mode->setGeometry( (karten->width()/4)*3 - 65, 20, 120, 125 );
	pcd_mode->setExclusive( TRUE );

	pcd_rgb = new QPushButton( i18n("Color"), pcd_mode );
	pcd_rgb->setToggleButton( TRUE );
	pcd_rgb->setGeometry( 15, 25, 90, 25 );

	pcd_gray = new QPushButton( i18n("Gray"), pcd_mode );
	pcd_gray->setToggleButton( TRUE );
	pcd_gray->setGeometry( 15, 55, 90, 25 );

	pcd_ycc = new QPushButton( i18n("Negative"), pcd_mode );
	pcd_ycc->setToggleButton( TRUE );
	pcd_ycc->setGeometry( 15, 85, 90, 25 );

	pcd_mode->insert( pcd_rgb, 0 );
	pcd_mode->insert( pcd_gray, 1 );
	pcd_mode->insert( pcd_ycc, 2 );

	connect(pcd_size, SIGNAL(clicked( int )), SLOT( pressPcdSize( int ) ));
	connect(pcd_mode, SIGNAL(clicked( int )), SLOT( pressPcdMode( int ) ));

///////////// Album SETTINGS /////

	thum_size = new QButtonGroup( i18n("Thumbnail Size"), album );
	thum_size->setGeometry( karten->width()/2 - 145, 20, 290, 125 );
	thum_size->setExclusive( TRUE );

	thum_0 = new QPushButton( i18n("60x60"), thum_size );
	thum_0->setToggleButton( TRUE );
	thum_0->setGeometry( 15, 55, 60, 60 );

	thum_1 = new QPushButton( i18n("80x80"), thum_size );
	thum_1->setToggleButton( TRUE );
	thum_1->setGeometry( 85, 35, 80, 80 );

	thum_2 = new QPushButton( i18n("100x100"), thum_size );
	thum_2->setToggleButton( TRUE );
	thum_2->setGeometry( 175, 15, 100, 100 );

	thum_size->insert( thum_0, 0 );
	thum_size->insert( thum_1, 1 );
	thum_size->insert( thum_2, 2 );
	connect(thum_size, SIGNAL(clicked( int )), SLOT( selectThumb( int ) ));

///////////// Private SETTINGS /////

	QPixmap icon;
	tb_style = new QButtonGroup( i18n("Toolbar Style"), privat );
	tb_style->setGeometry( karten->width()/4 - 70 , 20, 140, 95 );
	tb_style->setExclusive( TRUE );
	
	tb_kde = new QPushButton( tb_style );
	tb_kde->setToggleButton( TRUE );
	tb_kde->setGeometry( 15, 25, 25,25 );
	icon.load(kapp->kde_toolbardir().copy()+"/thumdialog.xpm");
	tb_kde->setPixmap(icon);

	lb_kde = new QLabel( i18n("KDE Standart"), tb_style );
	lb_kde->setAlignment(AlignVCenter);
	lb_kde->setGeometry( 50, 25, tb_style->width() - 55, 25 );

	tb_app = new QPushButton( tb_style );
	tb_app->setToggleButton( TRUE );
	tb_app->setGeometry( 15, 55, 25, 25 );
	icon.load(kapp->kde_datadir().copy()+"/kshow/pics/thumdialog.xpm");
	tb_app->setPixmap(icon);

	lb_app = new QLabel( i18n("kShow Magic"), tb_style );
	lb_app->setAlignment(AlignVCenter);
	lb_app->setGeometry( 50, 55, tb_style->width() - 55, 25 );

	tb_style->insert( tb_kde, 1 );
	tb_style->insert( tb_app, 2 );
	connect(tb_style, SIGNAL(clicked( int )), SLOT( selectToolIcon( int ) ));

///////////// Ende SETTINGS /////

	// save button
	save = new QPushButton(i18n("Save"), this);
	save->setGeometry( 10, this->height() - 35, 70, 25);
	connect(save, SIGNAL(clicked()), SLOT( press_save() ));
  
	// use button
	use = new QPushButton(i18n("Use"), this);
	use->setGeometry( 90, this->height() - 35, 70, 25);
	connect(use, SIGNAL(clicked()), SLOT( press_use() ));

	// cancel button
	cancel = new QPushButton(i18n("Cancel"), this);
	cancel->setGeometry( this->width() - 80, this->height() - 35, 70, 25);
	connect(cancel, SIGNAL(clicked()), SLOT( reject()));
}

void ConfigDialog::press_use() {

	done(1);
}

void ConfigDialog::press_save() {

	done(2);
}

void ConfigDialog::click_win_fix() {

	if( win_fix->isChecked() == TRUE) {
		optstr.WinFix = TRUE;
	} 
	else {	optstr.WinFix = FALSE;
	}
	setFix();
}

void ConfigDialog::setFix() {

	if( optstr.WinFix == TRUE ) {
		win_fix->setChecked( TRUE );
	}
	win_width->setEnabled( win_fix->isChecked() );
	win_height->setEnabled( win_fix->isChecked() );
}

void ConfigDialog::colorchange( const QColor &newColor ) {

	if (optstr.WinColor != newColor )
		optstr.WinColor = newColor;
}

void ConfigDialog::widthchange( int newWidth ) {

	if (optstr.WinWidth != newWidth )
		optstr.WinWidth = newWidth;
}

void ConfigDialog::heightchange( int newHeight ) {

	if (optstr.WinHeight != newHeight )
		optstr.WinHeight = newHeight;
}

void ConfigDialog::showvalue_1() {

	dia_for->setChecked( TRUE );
	dia_bac->setChecked( FALSE );
	dia_ran->setChecked( FALSE );	
	optstr.ShowAction = 1;
}

void ConfigDialog::showvalue_2() {

	dia_for->setChecked( FALSE );
	dia_bac->setChecked( TRUE );
	dia_ran->setChecked( FALSE );
	optstr.ShowAction = 2;
}

void ConfigDialog::showvalue_3() {

	dia_for->setChecked( FALSE );
	dia_bac->setChecked( FALSE );
	dia_ran->setChecked( TRUE );
	click_dia_loop();
	optstr.ShowAction = 3;
}

void ConfigDialog::click_dia_loop() {

	if( dia_ran->isChecked() == TRUE)
		dia_loop->setChecked( FALSE );
	if( dia_loop->isChecked() == TRUE) {
		optstr.ShowLoop = TRUE;
	} 
	else {	optstr.ShowLoop = FALSE;
	}
}

void ConfigDialog::timechange( int newtime ) {

	optstr.ShowTime = newtime;
	lcd->display(optstr.ShowTime);
}

void ConfigDialog::pressPcdSize( int s ) {
	
	PcdSize.sprintf("%d", s );
	PcdSize = "-" + PcdSize;
}

void ConfigDialog::pressPcdMode( int m ) {

	PcdMode = mode_list.at( m );
}

void ConfigDialog::selectThumb( int t )
{

	optstr.ThumSize = ( t * 2 + 6 ) * 10;
}

void ConfigDialog::selectToolIcon( int i)
{
	optstr.ToolIcon = i;
}

void ConfigDialog::setOptions( opt_struct optionstr ) {

	optstr = optionstr;

	win_width->setValue( optstr.WinWidth );
	win_height->setValue( optstr.WinHeight );

	win_bgcolor->setColor( optstr.WinColor );
	setFix();

	if( optstr.ShowLoop == TRUE )
		dia_loop->setChecked( TRUE );
	else { dia_loop->setChecked( FALSE );
	}

	if( optstr.ShowAction == 1 )
		showvalue_1();
	else if( optstr.ShowAction == 2 )
		showvalue_2();
	else if( optstr.ShowAction == 3 )
		showvalue_3();

	slider->setValue( optstr.ShowTime );
	lcd->display(optstr.ShowTime);

	pcd_size->find( optstr.PcdOpt.right(1).toInt(0) )->animateClick();
	pcd_mode->find( mode_list.find( optstr.PcdOpt.left(4) ) )->animateClick();
	thum_size->find((optstr.ThumSize/10 - 6) / 2)->animateClick();
	tb_style->find(optstr.ToolIcon)->animateClick();
}

opt_struct ConfigDialog::getOptions() { 

	optstr.PcdOpt = PcdMode + " " + PcdSize;
	return optstr;
}
