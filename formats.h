#include <qimage.h>

class FormatRecord {
public:
	enum {InternalFormat = 1, ReadFormat = 2, WriteFormat = 4};
	const char *formatName;
	unsigned int flags;
	char *magic;
	char *glob;
	char *suffix;  
	image_io_handler read_format;
	image_io_handler write_format;
};

class FormatManager {
public:
    static FormatManager *instance() {
        static FormatManager inst;
        return &inst;
    }
	virtual ~FormatManager();
   
	const char *allImagesGlob();
	QStrList *formats();
	const char *glob(const char *format);
	const char *suffix(const char *format); 

protected:
	FormatManager();
	QList <FormatRecord> list;
	virtual void init(FormatRecord formatlist[]);
	QStrList names;
	QString globAll;
};

