////////////////////////////////////////////////////
//
// Transparent support for GIF files in Qt Pixmaps,
// using GIFLIB library.
//
// Much of this code is duplicated from gif2x11.c, which is distributed
// with GIFLIB.
//
// Sirtaj Kang, Oct 1996.
//
// Current Problems:
// * No GIF extension blocks supported.
// * Lousy error reporting.
// * Buffers entire image in mem before moving to QImage
// * Skipped IODevice completely. How to get around this?
//
// GIF write support and transparency support added by Richard J. Moore
// Note that Qt will support gif files internally in version 1.3 so this
// code is not worth spending much time on.
// moorer@cs.man.ac.uk
//
// $Id: gif.cpp,v 1.5 1997/12/19 12:29:27 rich Exp $


#include <kdebug.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include <qimage.h>

#include "gif.h"

extern "C" {
#include <gif_lib.h>
};

void read_gif_file(QImageIO * imageio)
{
  int i, j, Size, Row, Col, Width, Height, ExtCode, Count;
  GifRecordType RecordType;
  GifByteType *Extension;
  GifRowType *ScreenBuffer;
  GifFileType *GifFile;

  QImage image;
  ColorMapObject *Colourmap;
  int trans= -1; // Index of the transparant colour, -1 if no transparent colour
/*  unsigned int *ui_row; */
  unsigned char *uc_row;
  int InterlacedOffset[] =
  {0, 4, 2, 1},		/* The way Interlaced image should. */
    InterlacedJumps[] =
    {8, 8, 4, 2};		/* be read - offsets and jumps... */


    // Initialise GIF struct and read init block

    if ((GifFile = DGifOpenFileName(imageio->fileName())) == NULL)
      return;

    // Create the new image using read dimensions.
    // Currently hardcoded to 8 bit.

    image.create(GifFile->SWidth, GifFile->SHeight, 8,
		 GifFile->SColorMap->ColorCount,
		 QImage::BigEndian);

    /* Allocate the screen as vector of column of rows. We cannt allocate    */
    /* the all screen at once, as this broken minded CPU can allocate up to  */
    /* 64k at a time and our image can be bigger than that:                  */
    /* Note this screen is device independent - its the screen as defined by */
    /* the GIF file parameters itself.                                       */

    if ((ScreenBuffer = (GifRowType *)
	 malloc(GifFile->SHeight * sizeof(GifRowType *))) == NULL)
      return;

    Size = GifFile->SWidth * sizeof(GifPixelType);	/* Bytes in one row */
    if ((ScreenBuffer[0] = (GifRowType) malloc(Size)) == NULL)	/* First row */
      return;

    for (i = 0; i < GifFile->SWidth; i++)	/* Set color to BackGround */
      ScreenBuffer[0][i] = GifFile->SBackGroundColor;

    for (i = 1; i < GifFile->SHeight; i++) {
      /* Allocate the other rows, and set their color to background too: */
      if ((ScreenBuffer[i] = (GifRowType) malloc(Size)) == NULL)
	return;

      memcpy(ScreenBuffer[i], ScreenBuffer[0], Size);
    }

    /* Scan the content of the GIF file and load the image(s) in: */

    do {
      if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
	PrintGifError();
	return;
      }
      switch (RecordType) {
      case IMAGE_DESC_RECORD_TYPE:
	if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
	  PrintGifError();
	  return;
	}
	Row = GifFile->Image.Top;	/* Image Position relative to Screen. */
	Col = GifFile->Image.Left;
	Width = GifFile->Image.Width;
	Height = GifFile->Image.Height;
	if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
	    GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
KDEBUG(KDEBUG_INFO, 3000, "Image is not confined to screen dimension, aborted.\n");
	  return;
	}
	if (GifFile->Image.Interlace) {
	  /* Need to perform 4 passes on the images: */
	  for (Count = i = 0; i < 4; i++)
	    for (j = Row + InterlacedOffset[i]; j < Row + Height;
		 j += InterlacedJumps[i]) {
	      if (DGifGetLine(GifFile, &ScreenBuffer[j][Col],
			      Width) == GIF_ERROR) {
		PrintGifError();
		return;
	      }
	    }
	} else {
	  for (i = 0; i < Height; i++) {
	    if (DGifGetLine(GifFile, &ScreenBuffer[Row++][Col],
			    Width) == GIF_ERROR) {
	      PrintGifError();
	      return;
	    }
	  }
	}
	break;
      case EXTENSION_RECORD_TYPE:
	/* Skip any extension blocks in file: */
	if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR) {
	  PrintGifError();
	  return;
	}
	if ( ExtCode == 249 )	// Graphic Control Ext
	  {
KDEBUG(KDEBUG_INFO, 3000, "Found graphic control extension\n");
	    // extract tranparent pixel stuff
	    if (Extension[1] & 1) {
	      trans= Extension[4];
KDEBUG1(KDEBUG_INFO, 3000, "Set trans to %d\n", Extension[4]);
	    }
	  }
	do {
	  if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR) {
	    PrintGifError();
	    return;
	  }
	  if (Extension != NULL) {
	    if ( ExtCode == 249 )	// Graphic Control Ext
	    {
KDEBUG(KDEBUG_INFO, 3000, "Found graphic control extension in next\n");
	    // extract tranparent pixel stuff
	    if (Extension[1] & 1) {
	      trans= Extension[4];
KDEBUG1(KDEBUG_INFO, 3000, "Set trans to %d\n", Extension[4]);
	    }
	    }
	  }
	} while	 (Extension != NULL);
	break;
      case TERMINATE_RECORD_TYPE:
	break;
      default:		/* Should be traps by DGifGetRecordType. */
	break;
      }
    }
    while (RecordType != TERMINATE_RECORD_TYPE);

    /* Set QImage colour map */

    Colourmap = (GifFile->Image.ColorMap
		 ? GifFile->Image.ColorMap
		 : GifFile->SColorMap);

    for (j = 0; j < Colourmap->ColorCount; j++) {
      /* Set colour to rgb and set as opaque */
      image.setColor(j, qRgb(Colourmap->Colors[j].Red,
			     Colourmap->Colors[j].Green,
			     Colourmap->Colors[j].Blue) | 0xff000000);
    }

    /* Copy image from Row buffers */
    for (j = 0; j < GifFile->SHeight; j++) {
      uc_row = (unsigned char *) image.scanLine(j);
      for (i = 0; i < GifFile->SWidth; i++) {
	*uc_row++ = ScreenBuffer[j][i];
      }
    }
    if (trans != -1) {
      /* Make the transparent colour transparent */
      image.setColor(trans, qRgb(Colourmap->Colors[trans].Red,
			     Colourmap->Colors[trans].Green,
			     Colourmap->Colors[trans].Blue) & 0x00ffffff);
      image.setAlphaBuffer(TRUE);
    }

    /* Commit image */

    imageio->setImage(image);
    imageio->setStatus(0);

    if (DGifCloseFile(GifFile) == GIF_ERROR)
      PrintGifError();

}				/*read_gif_file */


void write_gif_file(QImageIO *imageio)
{
  int i, /*j,*/ status;
  GifFileType *GifFile;
  ColorMapObject *screenColourmap;
  ColorMapObject *imageColourmap;

KDEBUG(KDEBUG_INFO, 3000, "write_gif_file()\n");

  imageColourmap= MakeMapObject(256, NULL);
  if (!imageColourmap) {
KDEBUG(KDEBUG_INFO, 3000, "Could not create image colour map\n");
    return;
  }

  screenColourmap= MakeMapObject(256, NULL);
  if (!screenColourmap) {
KDEBUG(KDEBUG_INFO, 3000, "Could not create screen colour map\n");
    return;
  }

KDEBUG1(KDEBUG_INFO, 3000, "Made Colourmap size 256, image colours= %d\n", imageio->image().numColors());

  for (i= 0; i < 256; i++) {
    if (i <imageio->image().numColors()) {
      imageColourmap->Colors[i].Red= qRed(imageio->image().color(i));
      imageColourmap->Colors[i].Green= qGreen(imageio->image().color(i));
      imageColourmap->Colors[i].Blue= qBlue(imageio->image().color(i));
    }
    else {
      imageColourmap->Colors[i].Red= 0;
      imageColourmap->Colors[i].Green= 0;
      imageColourmap->Colors[i].Blue= 0;
    }
  }

  for (i= 0; i < 256; i++) {
    if (i <imageio->image().numColors()) {
      screenColourmap->Colors[i].Red= qRed(imageio->image().color(i));
      *>imageSoolourmap->u5mo].Green= 0;
      imageColou0  }

  for (i= 0; i < 256; i++) {
    if (0  }

  for (i= 0; i < 256; i++) {
 e().color(i));
    }
 ) == GI(KDterlacedJumps[] =
    {8, 8, 4, 2};, 0 (!imageCo
      imourmapF (ict(256, NUolourmap;
  Col;urmapF (ict(256, NUlourmap) {
KDEBU;KDEBUG_INFO, 3000, "Could not create i->kd) ==  return;
  }

  screenColourmINFO, 3000, "Could notage eEvenh > Gifesc status;
 (DGif(KDterPut = 0; , &RecordTyp* Make thageio->image().coindow_h* Make thageio->image().co= window* Make thnCol Make th0l Make thlourmap) {
KDEBU;K!imageC (DGif = (.setOKimourmapE 0x00ffffff);
      i;KDEBUG_INFO, 3000, "Could not create image colour mar *allImage return;
  }

  screenColourmINFO, 3000, "Could notage eEveurmap)r *allImage returs;
 (DGif(KDterPutfFile, &RecordTypl Make t,
  },Make tageio->image().coindow_h* Make tageio->image().co= window* Make t },Make tageiop) {
KDEBU;K!imageC (DGif = (.setOKimourmapE 0x00ffffff);
      i;Kn;
  }

  screenColourmINFO, 3000, "Could nonstaeEveurmap)rata returs;
  }

    /*  (DGif  rec    ageio->image().co= window)ageColourmap- (DGif(KDterPutj][Col],
			  _widtgeio->image().co) | 0xff0iw* Maktgeio->image().cob (GiPer0xff0;
    }K!imageC (DGif = (.setOKimourmapRUE);
    }

    rmapE 0x00ffffff);
      i;Kn;
  }

  screenC)	// Gr 0x00ffffff);
      im= (.setOKimourmapRUE);
    }

    rmap }

  screenC)	ta(), ".." ted:
   QList <FormatRecord> list;
   virtual void init(FormatRecord formatlist[]);
   QStrList names;
   QString globAll;
};

#endif // FORMATS_H
