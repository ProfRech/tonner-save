#ifndef __FONTSMANAGER_H__
#define __FONTSMANAGER_H__

#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

class FontsManager
{
	string GetFontsDirectory();
	void AddFont(string fontFamily);

	vector<string> fontList;

public:
	FontsManager(void);
	~FontsManager(void);

	bool LoadLocalFonts();
	bool InstalledFont(string fontFamily);

	int GetTotalFonts();
	string GetFontList();

};

#endif // __FONTSMANAGER_H__