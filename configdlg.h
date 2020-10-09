/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	configdlg.h
*/

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

// KDE
#include <kapp.h>
#include <ktabctl.h>
#include <kcolorbtn.h>
#include <kspinbox.h>
#include <kintegerline.h>
#include <kslider.h>

struct opt_struct{

	bool WinFix;
	QString WinWidth;
	QString WinHeight;
	QColor WinColor;
	int ShowAction;
	int ShowTime;
	bool ShowLoop;
};

class ConfigDialog : public QDialog {

Q_OBJECT
  
public:
	ConfigDialog( QWidget *parent );
	~ConfigDialog() {};	

	void setOptions(struct opt_struct optstr);
	struct opt_struct getOptions();
	bool		WinFix;
	QString		WinWidth;
	QString		WinHeight;
	QColor		WinColor;
	int		ShowAction;
	int		ShowTime;
	bool		ShowLoop;

private slots:
	void press_use();
	void press_save();
	void click_win_fix();
	void setFix();
	void colorchange( const QColor &newColor );
	void widthchange( const char *newWidth );
	void heightchange( const char *newHeight );
	void showvalue_1();
	void showvalue_2();
	void showvalue_3();
	void click_dia_loop();
	void timechange( int newtime );

private:
	struct opt_struct optstr;
	QPushButton	*use;
	QPushButton	*save;
	QPushButton	*cancel;
	KTabCtl		*karten;
	QWidget		*window;
	QWidget		*diashow;
	KConfig		*config;	

// Window Settings
	QCheckBox	*win_fix;
	QLabel		*lb_width, *lb_height, *lb_bgcolor;
	QLineEdit	*win_width, *win_height;
	KColorButton	*win_bgcolor;

// Diashow Settings
	QLabel		*lb_tx, *lb_sk;
	QLCDNumber	*lcd;
	KSlider		*slider;
	QButtonGroup	*bg;
	QCheckBox	*dia_for, *dia_bac, *dia_ran, *dia_loop;
};

