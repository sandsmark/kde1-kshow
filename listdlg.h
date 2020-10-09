/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	listdlg.h
*/

// QT
#include <qdialog.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qstring.h>
#include <qstrlist.h>
#include <qlistbox.h>
#include <qpushbt.h>
#include <qlined.h>
#include <qfileinf.h>

// KDE
#include <kapp.h>

class ListDialog : public QDialog {

Q_OBJECT
  
public:
	ListDialog( QStrList fileList, QWidget *parent );
	~ListDialog() {};	

	void setEntryNum( int );

private slots:
	void selectEntry( int );

private:
	QStrList	liste;
	QPushButton	*a_bt, *b_bt, *c_bt, *d_bt;
	QPushButton	*ok_bt;
	QLabel		*fenster;
	QListBox	*ls_box;			
	QLineEdit	*ph_box;
};

