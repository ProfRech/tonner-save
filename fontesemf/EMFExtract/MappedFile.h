#ifndef MAPPED_FILE_H_INCLUDED
#define MAPPED_FILE_H_INCLUDED

#include <string>
#include <windows.h>

class MappedFile
{
public:
	enum Flags {
		NO_FLAGS = 0,
		EXTENDED_FLAG = 1 << 0,
		READONLY_FLAG = 1 << 1
	};
	off_t length;
	const char * contents;
	std::string filename;
	MappedFile();
	MappedFile(const std::string & filename, Flags flags = EXTENDED_FLAG);
	virtual ~MappedFile();
private:
	HANDLE hMapFile;
	Flags flags;
	char zeroed[4096];
};

#endif // MAPPED_FILE_H_INCLUDED