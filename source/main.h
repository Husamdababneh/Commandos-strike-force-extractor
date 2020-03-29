/* ========================================================================
   $File: main.h
   $Date: 2020-03-20
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   ========================================================================*/
#pragma once
#include <iostream>
extern const char * GetTypeFromCode(int code)
{
	switch(code)
	{
	  case 1:
		  return  "Not Expected";
	  case 2: case 196607:
		  return  "List";
	  case 3: case 262143:
		  return "Int";
	  case 4: case 327679:
		  return "Float";
	  case 5: case 393215:
		  return "String";
	  default:
		  std::cout << "Error : Type = " << code << "\n";
		  return  "Unknown Data type";
	}
}

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

/*
  FF FF 00 01 = unknown ?? 
  FF FF 00 02 = new indent ?? 
  FF FF 00 03 = int (which can be used as a boolean) (4294902528)
  FF FF 00 04 = float (4294902784) 327679
  FF FF 00 05 = str (4294903040) 393215
*/

struct CSFFBSHeader
{
	char Type[6];
	// there is a 2 byte padding here ? 
	long ListCount; // ?? maybe
	long unknown1;
	long StringEntryCount;
	long StringValueCount;
	long unknown;
};

struct CSFFBSSubHeader
{
	//long Zero; // ??,
	long Unknown1; // ? maybe array indecattor ? 
	long Type;
};

struct ListData
{
	long Unknown1;
	long InnerItemCount;
	long Unknown2;
};

struct CSFFBSDataHeader
{
	long Unknown3;
	long Indecator;
	long Serial ; // you can use this without the type
};

struct CSFFBSData
{
	long Unknown;
	long value;
	long type;
};


