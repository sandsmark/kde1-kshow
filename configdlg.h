/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	configdlg.h
*/

#ifndef CONFIGDLG_H 
#define CONFIGDLG_H 

#include <stdlib.h>
#include <time.h>

// QT
#include <qdialog.h>
#include <qtabdlg.h>
#include <qpushbt.h>
#include <qchkbox.h>
#include <qlabel.h>
#include <qlined.h>
#include <qbttngrp.h>
#include <qlcdnum.h>
#include <qspinbox.h>

// KDE
#include <kapp.h>
#include <ktabctl.h>
#include <kcolorbtn.h>
#include <kslider.h>

struct opt_struct{

	bool	WinFix;
	int	WinWidth;
	int	WinHeight;
	QColor	WinColor;
	int	ShowAction;
	int	ShowTime;
	bool	ShowLoop;
	QString	PcdOpt;
	int	ThumSize;
	int	ToolIcon;
};

#define KdeTbI 1
#define AppTbI 2

class ConfigDialog : public QDialog {

Q_OBJECT
  
public:
	ConfigDialog( QWidget *parent );
	~ConfigDialog() {};	

	void setOptions(opt_struct optionstr);
	opt_struct getOptions();

private slots:
	void press_use();
	void press_save();
	void click_win_fix();
	void setFix();
	void colorchange( const QColor &newColor );
	void widthchange( int newWidth );
	void heightchange( int newHeight );
	void showvalue_1();
	void showvalue_2();
	void showvalue_3();
	void click_dia_loop();
	void timechange( int newtime );

	void pressPcdSize( int s );
	void pressPcdMode( int m );
	void selectThumb( int t);
	void selectToolIcon( int i);

private:
	QPushButton	*use;
	QPushButton	*save;
	QPushButton	*cancel;
	KTabCtl		*karten;
	QWidget		*window;
	QWidget		*diashow;
	QWidget		*pcdimage;
	QWidget		*album;
	QWidget		*privat;
	KConfig		*config;	

// Window Settings
	QCheckBox	*win_fix;
	QLabel		*lb_width, *lb_height, *lb_bgcolor;
	QSpinBox	*win_width, *win_height;
	KColorButton	*win_bgcolor;

// Diashow Settings
	QLabel		*lb_tx, *lb_sk;
	QLCDNumber	*lcd;
	KSlider		*slider;
	QButtonGroup	*bg;
	QCheckBox	*dia_for, *dia_bac, *dia_ran, *dia_loop;

// PCD Image Settings
	
	QPushButton	*pcd_a, *pcd_b, *pcd_c;
	QPushButton	*pcd_rgb, *pcd_gray, *pcd_ycc;
	QButtonGroup	*pcd_size, *pcd_mode;
	QStrList	mode_list;
	QString		PcdSize, PcdMode;

// Thumbnail Settings
	QPushButton	*thum_0, *thum_1, *thum_2;
	QButtonGroup	*thum_size;

// Private Settings
	QButtonGroup	*tb_style;
	QPushButton	*tb_kde, *tb_app;
	QLabel		*lb_kde, *lb_app;

protected:
	opt_struct	optstr;
};

#endif // CONFIGDLG_H 
