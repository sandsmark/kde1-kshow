/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	listdlg.h
*/

#ifndef LISTDLG_H 
#define LISTDLG_H 

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
#include <qimage.h>

// KDE
#include <kapp.h>
#include <kfileinfo.h>
#include <kfiledialog.h>
#include <kmsgbox.h> 

class ListDialog : public QDialog {

Q_OBJECT
  
public:
	ListDialog( QStrList fileList, QWidget *parent );
	~ListDialog() {};	

	void setEntryNum( int );
	QStrList getNewList() const;
	int getSelectNum() const;

private slots:
	void selectEntry( int );
	void pressOk();
	void pressBtA();
	void pressBtB();
	void pressBtC();
	void pressBtD();
	void addFileUrl();

private:
	bool		changeList;
	QStrList	liste;
	QString		fileUrl;
	
	QPushButton	*a_bt, *b_bt, *c_bt, *d_bt;
	QPushButton	*ok_bt;
	QLabel		*fenster;
	QListBox	*ls_box;			
	QLineEdit	*ph_box;
};

#endif // LISTDLG_H 
