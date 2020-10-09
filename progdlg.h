/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de.de

	progdlg.h
*/

#ifndef PROGDLG_H 
#define PROGDLG_H 

// QT
#include <qdialog.h>
#include <qwidget.h>
#include <qstring.h>
#include <qpushbt.h>
#include <qprogbar.h>
#include <qstrlist.h>
#include <qfileinf.h>
#include <qdir.h>
#include <qimage.h>
#include <qprogdlg.h>
#include <qtimer.h>

// KDE
#include <kapp.h>

class ProgDialog : public QProgressDialog {

Q_OBJECT
  
public:
	ProgDialog( QStrList liste, QWidget *parent=0, const char *name=0 );
	~ProgDialog() {};	

	QStrList getListe() const;

private slots:
	void action();
	void fillListe();

private:
	QFileInfo	fileinfo;
	QPushButton	*cc_bt;
	QProgressBar	*ProgBar;
	QStrList	fileList;
	QStrList	picsList;
	QString		fileUrl;
	QTimer		*timer;

signals:
	void closed();

};

#endif // PROGDLG_H 
