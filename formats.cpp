#include "formats.h"
#include "netpbm.h"

extern void read_jpeg_jfif(QImageIO *);

static int numFormats= 11;
//static int numFormats= 8;

static FormatRecord formatlist[]= {
  {
    "GIF",
    FormatRecord::InternalFormat | FormatRecord::ReadFormat | FormatRecord::WriteFormat,
    "^GIF[0-9][0-9][a-z]",
    "*.gif",
    "gif",
    0, 0,
  },
  {
    "JPEG",
    FormatRecord::ReadFormat,
    "^\377\330\377\340",		// \377\330\377\356 for jpg
    "*.jpeg *.jpg",
    "jpeg",
    read_jpeg_jfif, 0,
  },
  {
    "BMP",
    FormatRecord::InternalFormat | FormatRecord::ReadFormat | FormatRecord::WriteFormat,
    "^BM",
    "*.bmp",
    "bmp",
    0, 0,
  },
  {	 
    "XBM",
    FormatRecord::InternalFormat | FormatRecord::ReadFormat | FormatRecord::WriteFormat,
    0,
    "*.xbm",
    "xbm",
    0, 0,
  },
  {
    "XPM",
    FormatRecord::InternalFormat | FormatRecord::ReadFormat | FormatRecord::WriteFormat,
    "^/* XPM*/",                      //  /*\ XPM\*/
    "*.xpm",
    "xpm",
    0, 0,
  },
  {
    "PNM",
    FormatRecord::InternalFormat | FormatRecord::ReadFormat | FormatRecord::WriteFormat,
    0,	
    "*.pbm *.pgm",
    "pnm",
    0, 0
  },
  {
    "PPM",
    FormatRecord::InternalFormat | FormatRecord::ReadFormat | FormatRecord::WriteFormat,
    0,	
    "*.ppm",
    "ppm",
    0, 0
  },

// new formats , you most include netpbmtools
  {
    "ILBM",
    FormatRecord::ReadFormat,
    "^FORM....ILBM",
    "*.iff *.ilbm",
    "iff",
    read_ilbm , 0
  },
  {
    "PCX",
    FormatRecord::ReadFormat,
    "\012[0\2\3\4\5]\001",
    "*.pcx",
    "pcx",
    read_pcx , 0
  },
  {
    "TIFF",
    FormatRecord::ReadFormat,
    "^I[I][*]",
    "*.tif",
    "tif",
    read_tiff , 0
  },
  {
    "PCD",
    FormatRecord::ReadFormat,
    "\xff\xff\xff\xff\xff\xff\xff",    // PCD_IPI=image / PCD_OPA =uebersicht
    "*.pcd",
    "pcd",
    read_pcd , 0
  }
};

FormatManager::FormatManager()
{
   list.setAutoDelete(TRUE);
   init(formatlist);
}

FormatManager::~FormatManager()
{
}

void FormatManager::init(FormatRecord formatlist[])
{
   int i;
   FormatRecord *rec;
   
   // Build format list
   for (i= 0; i < numFormats; i++) {
     list.append(&formatlist[i]);
     names.append(formatlist[i].formatName);
     globAll.append(formatlist[i].glob);
     if (i < numFormats-1)
       globAll.append(" ");
   };
   
   // Register them with Qt
   for (rec= list.first(); rec != 0; rec= list.next()) {
     if ( (rec->flags & FormatRecord::InternalFormat) == 0L)
       QImageIO::defineIOHandler(rec->formatName, rec->magic,
				 0, 
				 rec->read_format, rec->write_format);
   }
}
	  
QStrList *FormatManager::formats()
{
  return &names;
}

const char *FormatManager::allImagesGlob()
{
  return globAll;
}
 
const char *FormatManager::glob(const char *format)
{
  FormatRecord *rec;
  QString name(format);
  QString curr;
  bool done= FALSE;

  rec= list.first();
  do { 
    curr= rec->formatName;
    if (curr == name)
      done= TRUE;
    else 
      rec= list.next();
  } while (!done && (rec != 0));

  if (done)
    return rec->glob;
  else
    return 0;
}

const char *FormatManager::suffix(const char *format)
{
  FormatRecord *rec;
  QString name(format);
  QString curr;
  bool done= FALSE;

  rec= list.first();
  do { 
    curr= rec->formatName;
    if (curr == name)
      done= TRUE;
    else 
      rec= list.next();
  } while (!done && (rec != 0));

  if (done)
    return rec->suffix;
  else
    return 0;
}
