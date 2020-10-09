/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	progdlg.cpp
*/

#include <progdlg.h>
#include <progdlg.moc>

ProgDialog::ProgDialog(QStrList liste, QWidget *parent, const char *name)
	: QProgressDialog( parent, "kShow loading", TRUE ) {

	fileList = liste;
	picsList.clear();
	setCaption( i18n( "kShow search" ) );
	setLabelText(i18n("Images to Directory ..."));
	setCancelButtonText(i18n("Cancel"));
	timer = new QTimer(this);
	connect(timer, SIGNAL( timeout()), SLOT( action() ));
	if ( timer->isActive() )
	    timer->stop();
	timer->start( 0, true );
}

void ProgDialog::action()
{
	int e = fileList.count();
	int i = 0;
	while( i < e ) {
	    setTotalSteps(e);	
	    setProgress(i);
	    if(wasCancelled())
		break;

	    fileUrl = fileList.at( i );
	    if( fileUrl.left(5) == "file:" )
		fileUrl = fileUrl.remove( 0, 5 );
	    fileinfo.setFile( fileUrl );

	    if( fileinfo.isDir() == TRUE ) {
		if(fileinfo.isReadable())
		    fillListe();
	    }
	    else if ( QImageIO::imageFormat( fileUrl ) != 0 ) {
		    picsList.append(fileList.at( i ));
	    }    
	    i++;
	    e = fileList.count();
	}
	setProgress(e);
	emit closed();
	cancel();
}

void ProgDialog::fillListe()
{
	QDir filedir( fileUrl );
	const QFileInfoList *list = filedir.entryInfoList();
	QFileInfoListIterator it( *list );
	QFileInfo *fileinfo;	
	while( ( fileinfo = it.current() ) )
	{
	    if ( strcmp( fileinfo->fileName().data(), "." ) != 0 && 
		 strcmp( fileinfo->fileName().data(), ".." ) != 0 )
	    {
		QString tmp = fileinfo->absFilePath().data();
		tmp = "file:" + tmp;
		fileList.append( tmp );
	    }
	    ++it;
	}
}

QStrList ProgDialog::getListe() const
{
	return picsList;
}

