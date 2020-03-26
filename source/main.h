/* ========================================================================
   $File: main.h
   $Date: 2020-03-20
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   ========================================================================*/
#pragma once



struct PakHeader
{
	long packtype;
	long PackedBlockSize;
	long UnpackedBlockSize;
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
	char Tag [7]; // Tag With Null Terminating Chararcter
	long unknown1;
	long unknown2;
	char unknown3[12];
	char Flag; //??
	
};
	
struct CSFFVSName
{
	long StringSize;
//	char name[StringSize];
};


	
