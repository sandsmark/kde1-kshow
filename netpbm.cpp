/*
	Program : kshow
	Author  : Ralf Berger
	E-Mail  : rberger@fun.horx.de

	netpbm.cpp
*/

#include "kshow.h"
#include "netpbm.h"

#define CMDBUFLEN  4096

extern QString KShowWindow::PcdOpt;

void import_graphic (char *filter, QImageIO *image)
{
	char * tmpFileName;
	QImage myimage;
	char cmdBuf [CMDBUFLEN];
	tmpFileName = tmpnam(NULL);
	sprintf (cmdBuf, "%s %s > %s", filter, image->fileName(), tmpFileName);
	system (cmdBuf);
	myimage.load (tmpFileName);
	unlink (tmpFileName);
	image->setImage (myimage);
	image->setStatus (0);
}

void read_pcx (QImageIO *image)
{
  import_graphic ("pcxtoppm", image);
}

void read_ilbm (QImageIO *image)
{
  import_graphic ("ilbmtoppm", image);
}

void read_tga (QImageIO *image)
{
  import_graphic ("tgatoppm", image);
}

void read_pcd (QImageIO *image) {

	QString pcdopt;
	pcdopt = "hpcdtoppm " + KShowWindow::PcdOpt;
	char *line = qstrdup(pcdopt);
	import_graphic( line, image);
}

void read_tiff (QImageIO *image) {

	import_graphic("tifftopnm", image);
}
