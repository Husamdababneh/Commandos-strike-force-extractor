/* ========================================================================
   $File: main.h
   $Date: 2020-03-20
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   ========================================================================*/

#pragma once

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float  f32;
typedef double  f64;

typedef float  float32;
typedef double float64;

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


enum  : long
{
	GFX    = 0x1c633a1,
	MENUS  = 0x1c633a6,
	MAPS   = 0x1c633ac,
	MODELS = 0x1c633a7,
	BDD    = 0x1c633af
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

struct FilesData
{
	char Filename[120];
	long ROFF;// resouce offset in file 
	long Size;
	long Unknown1;
	long WhereItIs; // ?? 
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

struct CSFFBSBrackets
{
	std::stack<int> col;
	std::stack<int> arr;
	std::stack<int> turn2;
	
	bool turn = false; // true -> arr , false -> col
};

#pragma warning(disable: 4200)

struct SString {
	u32 size;
	u8 str[];
};
