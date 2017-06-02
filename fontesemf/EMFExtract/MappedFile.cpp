#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <io.h>
#include "MappedFile.h"

#define DEBUG_MSG(fmt, ...)

const char * INVALID_MAP_HANDLE = reinterpret_cast<char *>(-1);

MappedFile::MappedFile()
{
	memset(reinterpret_cast<char *>(this->zeroed), 0, sizeof(this->zeroed));

	this->flags = NO_FLAGS;
	this->contents = reinterpret_cast<char *>(this->zeroed);
	this->length = 0;
}

MappedFile::MappedFile(const std::string & filename, Flags flags) : filename(filename), flags(flags)
{
	DEBUG_MSG("mapeando arquivo %s | flags: %i", filename.c_str(), flags);
	memset(reinterpret_cast<char *>(this->zeroed), 0, sizeof(this->zeroed));
	HANDLE hFile;

	// windows 98 não suporta FILE_SHARE_DELETE
	int file_share = /*FILE_SHARE_DELETE |*/ FILE_SHARE_READ | FILE_SHARE_WRITE;
	DWORD access_mode = GENERIC_READ | GENERIC_WRITE;
	if (this->flags & READONLY_FLAG)
	{
		access_mode = GENERIC_READ;
	}
	bool extend_file = true;
	hFile = CreateFileA(this->filename.c_str(), access_mode, file_share, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		int err = GetLastError();
		DEBUG_MSG("primeira tentativa de abrir arquivo falhou: %i", err);
		// nao foi possivel abrir com escrita, tentamos mudar os atributos do arquivo
		if (SetFileAttributesA(this->filename.c_str(), FILE_ATTRIBUTE_NORMAL))
		{
			DEBUG_MSG("atributos setados");
			// conseguimos mudar os atributos, tentamos abrir como escrita novamente
			hFile = CreateFileA(this->filename.c_str(), access_mode, file_share, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				err = GetLastError();
				DEBUG_MSG("segunda tentativa de abrir arquivo falhou: %i", err);
				// e somente leitura entao?
				hFile = CreateFileA(this->filename.c_str(), GENERIC_READ, file_share, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
				{
					err = GetLastError();
					DEBUG_MSG("terceira tentativa de abrir arquivo falhou: %i", err);
					// realmente nao teve jeito, arquivo inexistente ou fora do nosso alcance
					throw "Impossivel abrir arquivo para mapeamento";
				}
				else
				{
					DEBUG_MSG("conseguiu abrir arquivo, mas sem extendê-lo");
					// conseguimos abrir como somente leitura, nao poderemos extender o arquivo
					extend_file = false;
				}
			}
		}
		else
		{
			int err = GetLastError();
			DEBUG_MSG("atributos inalterados: %i", err);
			// atributos não podem ser mudados, tentamos abrir como somente leitura
			hFile = CreateFileA(this->filename.c_str(), GENERIC_READ, file_share, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				int err = GetLastError();
				DEBUG_MSG("segunda tentativa de abrir arquivo falhou: %i", err);
				throw "Impossivel abrir arquivo para mapeamento";
			}
			else
			{
				DEBUG_MSG("conseguiu abrir arquivo, mas sem extendê-lo");
				extend_file = false;
			}
		}
	}

	if (!(this->flags & EXTENDED_FLAG))
	{
		extend_file = false;
	}

	if (this->flags & READONLY_FLAG)
	{
		extend_file = false;
	}

	this->length = GetFileSize(hFile,  NULL);
	DEBUG_MSG("filesize: %i", this->length);
	if (extend_file)
	{
		DEBUG_MSG("tentando extender arquivo");
		if (SetFilePointer(hFile, 0, NULL, FILE_END) != INVALID_SET_FILE_POINTER)
		{
			DEBUG_MSG("SetFilePointer ok");
			DWORD bytes_written;
			if (!WriteFile(hFile, this->zeroed, sizeof(this->zeroed), &bytes_written, NULL))
			{
				int err = GetLastError();
				DEBUG_MSG("WriteFile error: %i", err);
				extend_file = false;
			}
			else
			{
				DEBUG_MSG("WriteFile success");
			}
		}
		else
		{
			int err = GetLastError();
			DEBUG_MSG("SetFilePointer error: %i", err);
			extend_file = false;
		}
	}
	else
	{
		DEBUG_MSG("arquivo não expansível");
	}

	if (extend_file)
		this->hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, this->length + sizeof(this->zeroed), NULL);
	else
		this->hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, this->length, NULL);

	if (this->hMapFile != NULL)
	{
		this->contents = static_cast<char *>(MapViewOfFile(this->hMapFile, FILE_MAP_READ, 0, 0, 0));
		if (this->contents == NULL)
		{
			int err = GetLastError();
			DEBUG_MSG("MapViewOfFile error: %i", err);
			CloseHandle(this->hMapFile);
			this->hMapFile = NULL;
			this->contents = INVALID_MAP_HANDLE;
		}
		else
		{
			DEBUG_MSG("arquivo mapeado com sucesso");
		}
	}
	else
	{
		int err = GetLastError();
		DEBUG_MSG("CreateFileMapping error: %i", err);
	}

	CloseHandle(hFile);

	if (this->contents == INVALID_MAP_HANDLE)
		throw "Nao foi possivel mapear o arquivo";
}

MappedFile::~MappedFile()
{
	DEBUG_MSG("start | length:%i", this->length);
	if (this->length > 0)
	{
		UnmapViewOfFile(this->contents);
		CloseHandle(this->hMapFile);

		//SetFilePointer(this->hFile, 0, NULL, this->length);
		//SetEndOfFile(this->hFile);

		if (!(this->flags & READONLY_FLAG))
		{
			if (this->flags & EXTENDED_FLAG)
			{
				//retornar o tamanho original do arquivo
				int f = _open(this->filename.c_str(), _O_WRONLY);
				if (f != -1)
				{
					_chsize(f, this->length);
					_close(f);
				}
			}
		}
	}
}