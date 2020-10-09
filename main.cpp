/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	main.cpp
*/

#include "kshow.h"
#include "formats.h"

QList<KShowWindow> KShowWindow::windowList;

KApplication	*app;
FormatManager	*formatMngr;

int main( int argc, char **argv ) {

	app = new KApplication ( argc, argv, "kshow" );
	
	KShowWindow *kShow = new KShowWindow();
	KShowWindow::windowList.append( kShow );

	formatMngr= new FormatManager();
	kShow->show();

	QStrList argList;

	if( argc > 1 ) {		
		for ( int i=0; i < argc; i++ )
			argList.append( argv[i] );

		kShow->argLoad( argList );
	}

	return app->exec();
}
