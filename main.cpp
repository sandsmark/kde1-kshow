/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	main.cpp
*/

#include "kshow.h"
#include "formats.h"
#include "jpeg.h"

#include <kpreview.h>

QList<KShowWindow> KShowWindow::windowList;

KApplication	*app;
static FormatManager	*formatMngr;

static bool previewHandler (const KFileInfo *fi, const QString fileName,QString &, QPixmap & pi)
{
    doing_previews = true;
    bool ret = pi.load(fileName);
    doing_previews = false;
    return ret;
}

int main( int argc, char **argv ) {

	app = new KApplication ( argc, argv, "kshow" );
        KPreview::registerPreviewModule("IMAGE", previewHandler, PreviewPixmap);
	KShowWindow *kShow = new KShowWindow();
	KShowWindow::windowList.append( kShow );

	formatMngr= FormatManager::instance();
	kShow->show();

	QStrList argList;

	if( argc > 1 ) {		
		for ( int i=0; i < argc; i++ )
			argList.append( argv[i] );

		kShow->argLoad( argList );
	}

	return app->exec();
}
