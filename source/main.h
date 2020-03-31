/* ========================================================================
   $File: main.h
   $Date: 2020-03-20
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   ========================================================================*/

#pragma once

#include <iostream>

#if 0
enum  : long
{ 
	NO_DATA = 65535,  // 0ffff
	PAIR    = 65552,
	LIST    = 196607, // 2ffff
	INT     = 262143, // 3ffff
	FLOAT   = 327679, // 4ffff
	STRING  = 393215  // 5ffff
};
#else
enum  : long
{ 
	NO_DATA = 0,
	PAIR    = 1,
	LIST    = 2,
	INT     = 3,
	FLOAT   = 4,
	STRING  = 5 
};
#endif
struct PakHeader
{
	long packtype;
	long unknown1;
	long unknown2;
	long ResNum;
};

struct FilesData
{
	char filename[120];
	long ROFF;
	long Size;
	long V1;
	long V2;
};

struct CSFFBSHeader
{
	char Tag[6];
	// there is a 2 byte padding here ? 
	long Version; // ?? maybe
	long NumberOfItems; // * 12) + 24= Offset Of Where Data Begins -v
	long StringEntryCount;
	long StringValueCount;
	// long Unknown2;
	// long ListSize; // ? maybe array indecattor ? 
	// long Type;
};

struct ListData
{
	long Indicator;
	union {
		// Data 
		long IntegerValue;
		float FloatValue;
		// String 
		long StringIndex;
		// List 
		long InnerItemCount;
	};
	//long Type;
	short ItemStringIndex;
	short Type;
	// union {
	// 	// Data
	// 	long Type;
	// 	// Header
	// 	short ItemStringIndex; // you can use this without the type
	// };

};

struct CSFFBSDataHeader
{
	long Unknown; // maybe this is what i'm looking for ?? 
	long ListCount;
	union {
		long Type;
		short StringIndex, junk ; // you can use this without the type
		
	};
	
};

struct CSFFBSData
{
	long Unknown;
	union {
		long IntegerValue;
		float FloatValue;
	};
	union {
		long type;
		short StringIndex, junk ; // you can use this without the type
		
	};
};


