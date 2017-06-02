#include "FontsManager.h"
#include "FontProperties.h"

FontsManager::FontsManager(void)
{
}

FontsManager::~FontsManager(void)
{
}

string FontsManager::GetFontsDirectory()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory ( &osvi, sizeof(OSVERSIONINFOEX) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

	if( !(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
		bOsVersionInfoEx = GetVersionEx( (OSVERSIONINFO *) &osvi);
	}

	if( bOsVersionInfoEx ) 
	{		 
		if( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			{
				unsigned char path[1024];
				DWORD tipo,size = 1024;
				HKEY key;
				if( RegOpenKeyExA( HKEY_USERS ,".DEFAULT\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",0, KEY_READ, &key) == ERROR_SUCCESS)
				{
					if( RegQueryValueExA(key,"Fonts",NULL,&tipo,path,&size) == ERROR_SUCCESS)
					{
						return (char*)path;
					}
				}
			}
		}
	}

	char dir[MAX_PATH] = {0};
	GetWindowsDirectoryA( dir , MAX_PATH + 1);
	strcat( dir, "\\Fonts" );
	return dir;
}

bool FontsManager::LoadLocalFonts()
{
	WIN32_FIND_DATAA wf;
	HANDLE hFind;

	string fontsDir = this->GetFontsDirectory();
	string strSearch = fontsDir;
	strSearch += "\\*.*tf";
	
	hFind = FindFirstFileA( strSearch.c_str(), &wf );
	
	while( hFind != INVALID_HANDLE_VALUE  ) 
	{
		string fileName = fontsDir;
		fileName += "\\";
		fileName += wf.cFileName;

		FontProperties objFontProperties;
		if (objFontProperties.Load(fileName) == true)
		{
			this->AddFont(objFontProperties.GetFamily());
		}

		if( !FindNextFileA( hFind, &wf ) ) 
			break; 
	}
	FindClose( hFind );

	return true;
}

bool FontsManager::InstalledFont(string fontFamily)
{
	vector<string>::iterator iter = this->fontList.begin();
	for (;iter != this->fontList.end(); iter++)
	{
		if (_stricmp(iter->c_str(), fontFamily.c_str()) == 0)
			return true;
	}

	return false;
}

void FontsManager::AddFont(string fontFamily)
{
	if (this->InstalledFont(fontFamily) == false)
		this->fontList.push_back(fontFamily);
}

int FontsManager::GetTotalFonts()
{
	return this->fontList.size();
}

string FontsManager::GetFontList()
{
	string ret;

	vector<string>::iterator iter = this->fontList.begin();
	for (;iter != this->fontList.end(); iter++)
	{
		ret.append(*iter);
		ret.append("¬");
	}

	return ret;
}
