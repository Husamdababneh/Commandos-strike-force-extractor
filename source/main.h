/* ========================================================================
   $File: main.h
   $Date: 2020-03-20
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   ========================================================================*/

#pragma once

#define FSB_MAGIC "CSFFBS"

enum : long
{
	GFX    = 0x1C633A1,
	MENUS  = 0x1C633A6,
	MAPS   = 0x1C633AC,
	MODELS = 0x1C633A7,
	BDD    = 0x1C633AF
};

struct PakHeader
{
	char Tag[4];
	long IDK;
	long Version;
	long ResNum;
};

enum : short
{
	UNKNOWN_TYPE = 0
};

struct FilesMetaData
{
	u8* filename;
	// NOTE(Husam): resouce offset in file ->
    //              first you have to get the first offset by looping over the resources header,
    //              save the offset , add it to roff to get to the location of the resources
	u32 roff;
	u32 size;
	
	union {
		u32 v1;
		struct { u16 v1_1; u16 v1_2; };
	};
	
	union {
		u32 v2;
		struct { u16 v2_1; u16 v2_2; };
	};
};


enum FBSType : u16
{ 
	CSF_KEY           = 0,
	CSF_COLLECTION    = 1,
	CSF_LIST          = 2,
	CSF_INT           = 3,
	CSF_FLOAT         = 4,
	CSF_STRING        = 5 
};


struct CSFFBSHeader
{
	char Tag[6];
    char padding[2];
	// there is a 2 byte padding here ? 
	long Version; // ?? maybe
	long NumberOfItems; // * 12) + 24 = Offset Of Where Data Begins -v
	long StringEntryCount;
	long StringValueCount;
};

struct CSFFBSData
{
	u32 Indicator;
	union {
		// Data 
		u32 IntegerValue;
		float FloatValue;
		// String 
		u32 StringIndex;
		// List 
		u32 InnerItemCount;
	};
	u16 ItemStringIndex;
	FBSType type;
};

struct CSFFBSFile
{
    CSFFBSHeader* header;
    CSFFBSData*   data;
    u8**        keys;
    u8**        string_table;
};

#pragma warning(disable: 4200)

struct SString {
	u32 size;
	u8 str[];
};


struct PakFile {
    u8* dataToBeFreed;
    PakHeader* pak;
    FilesMetaData* files;
};

PakFile ParsePakFile(const char*);
