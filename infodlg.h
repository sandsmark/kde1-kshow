/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de.de

	infodlg.h
*/

// QT
#include <qdialog.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qlined.h>
#include <qstring.h>
#include <qfileinf.h>
#include <qpushbt.h>

// KDE
#include <kapp.h>

class InfoDialog : public QDialog {

Q_OBJECT
  
public:
	InfoDialog( const char *filename, QWidget *parent );
	~InfoDialog() {};	

private slots:

private:
	QString		image_name, image_path, image_width, image_height, image_format;
	QString		image_length, image_comment;

	QPushButton	*ok_bt;
	QLabel		*fenster;
	QLabel		*lb_name, *lb_path, *lb_format, *lb_width, *lb_height, *lb_length, *lb_comment;
	QLabel		*tx_name, *tx_path, *tx_format, *tx_width, *tx_height, *tx_length;
	QLineEdit	*tx_comment;

			
};

