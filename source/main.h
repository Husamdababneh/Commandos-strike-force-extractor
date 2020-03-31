/* ========================================================================
   $File: main.h
   $Date: 2020-03-20
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   ========================================================================*/

#pragma once

#include <iostream>
#include <cstddef>
enum : short
{ 
	HEADER        = 0,
	COLLECTION    = 1,
	LIST          = 2,
	INT           = 3,
	FLOAT         = 4,
	STRING        = 5 
};

struct PakHeader
{
	char Tag[6];
	// Padding
	long Version;
	long ResNum;
};

struct FilesData
{
	char Filename[120];
	long ROFF;
	long Size;
	long Unknown1;
	short C601;
	std::byte ThirtyThree;
	std::byte Unknown2;
	//short Unknown2;
};

struct CSFFBSHeader
{
	char Tag[6];
	// there is a 2 byte padding here ? 
	long Version; // ?? maybe
	long NumberOfItems; // * 12) + 24= Offset Of Where Data Begins -v
	long StringEntryCount;
	long StringValueCount;
};

struct CSFFBSData
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
	short ItemStringIndex;
	short Type;
};
