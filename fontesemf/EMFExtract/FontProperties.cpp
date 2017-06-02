#include "FontProperties.h"

#define DEBUG_MSG

FontProperties::FontProperties(void)
{
}

FontProperties::~FontProperties(void)
{
}

bool FontProperties::Load(string fileName)
{
	this->copyright = "";
	this->family = "";
	this->name = "";
	this->trademark = "";

	TT_OFFSET_TABLE tableheader;
	TT_TABLE_DIRECTORY tblDir;

	FILE *f = fopen( fileName.c_str(), "rb" );

	if( f == NULL )
	{
		return false;
	}

	fread( &tableheader, sizeof( TT_OFFSET_TABLE ), 1 , f );
	tableheader.uNumOfTables  = SWAPWORD( tableheader.uNumOfTables );
	tableheader.uMajorVersion = SWAPWORD( tableheader.uMajorVersion );
    tableheader.uMinorVersion = SWAPWORD( tableheader.uMinorVersion );
	DEBUG_MSG( " tableheader.uNumOfTables %i  tableheader.uMajorVersion  %i tableheader.uMinorVersion %i" , 
		tableheader.uNumOfTables , 
		tableheader.uMajorVersion ,
		tableheader.uMinorVersion ); 


	// verificando se é uma fonte true tpye
	this->trueType = tableheader.uMajorVersion == 1 ;
		
	for ( int i = 0; i < tableheader.uNumOfTables; i++ )
	{
		char buffer[ 5 ] = {0};

		fread( &tblDir , sizeof( TT_TABLE_DIRECTORY ), 1 , f);
		strncpy( buffer , tblDir.szTag , 4);
		if (_stricmp( buffer , "name" ) == 0)
		{
			tblDir.uLength = SWAPLONG( tblDir.uLength );
			tblDir.uOffset = SWAPLONG( tblDir.uOffset );

			this->GetData(f, &tblDir);
			break;
		}
	}

	fclose( f );
	
	if (this->name.length() == 0)
		this->name = this->family;

	if (this->name.length() == 0)
	{
		char fontFileName[ MAX_PATH ] =""; 		
		strncpy(fontFileName, fileName.c_str(), sizeof(fontFileName) - 1);

		char* lastNameFont = strrchr( fontFileName, '\\' ); 
		if( lastNameFont )
		{
			lastNameFont++; 
			this->name = (char*)lastNameFont;
			this->family = (char*)lastNameFont;
		}
	}

	DEBUG_MSG( "fim" ); 

	return true;
}

bool FontProperties::GetData(FILE * f, TT_TABLE_DIRECTORY * tblDir)
{
	TT_NAME_TABLE_HEADER ttNTHeader;
	TT_NAME_RECORD ttRecord;

	fseek( f , tblDir->uOffset , SEEK_SET );
	fread( &ttNTHeader , sizeof( TT_NAME_TABLE_HEADER ), 1 , f);

	ttNTHeader.uNRCount = SWAPWORD(ttNTHeader.uNRCount);
	ttNTHeader.uStorageOffset = SWAPWORD(ttNTHeader.uStorageOffset);

	char * buffer = NULL;

	for(int i = 0;
		i < ttNTHeader.uNRCount && 
		( this->copyright.length() == 0 || this->family.length() == 0 || this->name.length() == 0 || this->trademark.length() == 0); 
		i++)
	{

		fread( &ttRecord , sizeof( TT_NAME_RECORD ) , 1 , f);

		ttRecord.uNameID = SWAPWORD(ttRecord.uNameID);

		if (ttRecord.uNameID != 1 && ttRecord.uNameID != 0 && ttRecord.uNameID != 7)
			continue;

		ttRecord.uStringLength = SWAPWORD(ttRecord.uStringLength);
		ttRecord.uStringOffset = SWAPWORD(ttRecord.uStringOffset);

		DEBUG_MSG( " ttRecord.uStringLength %i", ttRecord.uStringLength ); 
		buffer = new char [ ttRecord.uStringLength+1 ];
		buffer[ ttRecord.uStringLength ] = '\0';

		int nPos = ftell( f );

		fseek( f , tblDir->uOffset + ttRecord.uStringOffset + ttNTHeader.uStorageOffset , SEEK_SET);
		fread( buffer , ttRecord.uStringLength , 1 , f );

		if(strlen( buffer ) > 0)
		{
			switch( ttRecord.uNameID )
			{
			case 1:
				this->family = buffer;
				break;

			case 0:
				this->copyright = buffer;
				break;

			case 7:
				this->trademark = buffer;
				break;

			case 4:
				this->name = buffer;
				break;
			}
		}

		fseek( f , nPos , SEEK_SET );

		delete [] buffer; 
		buffer = NULL; 
	}

	return true;
}

string FontProperties::GetFamily()
{
	return this->family;
}

string FontProperties::GetName()
{
	return this->name;
}

string FontProperties::GetCopyright()
{
	return this->copyright;
}

string FontProperties::GetTrademark()
{
	return this->trademark;
}

bool FontProperties::IsTrueType()
{
	return this->trueType;
}