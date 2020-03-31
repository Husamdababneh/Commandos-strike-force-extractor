/* ========================================================================
   $File: functions.h
   $Date: 2020-03-30
   $Creator: Husam Dababneh
   ========================================================================*/

#pragma once
#include "main.h"
#include "zlib.h"
#include "zconf.h"
#include <cassert>
#define LOGANDEXIT(x) { std::cout << x ; return 0;}
#define ERRORANDEXIT(x) { std::cerr << x ; return 0;}
#define assertm(exp, msg) assert((msg, exp))
/*extern void ProgressBar()
  {
  while(progress < maxNum)
  {
  int barWidth = 70;
  std::cout << "[";
  float realPos = ((float)progress / maxNum);
  int pos = int(barWidth * realPos);
  for (int i = 0; i < barWidth; ++i) {
  if (i < pos) std::cout << "=";
  else if (i == pos) std::cout << ">";
  else std::cout << " ";
  }
  std::cout << "] " << int(realPos* 100)  << " %\r";
  std::cout.flush();
  }
  } 
*/

void zerr(int ret)
{
	std::cerr << "Unsuccesful decompression : ";
	switch (ret) {
	  case Z_ERRNO:
		  if (ferror(stdin))
			  std::cerr << "error reading stdin\n";
		  if (ferror(stdout))
			  std::cerr << "error writing stdout\n";
		  break;
	  case Z_STREAM_ERROR:
		  std::cerr << "invalid compression level\n";
		  break;
	  case Z_DATA_ERROR:
		  std::cerr << "invalid or incomplete deflate data\n";
		  break;
	  case Z_MEM_ERROR:
		  std::cerr << "out of memory\n";
		  break;
	  case Z_VERSION_ERROR:
		  std::cerr << "zlib version mismatch!\n";
	}
	std::cout << "ret : " << ret;
	assertm(false, "Error While Decompression");
}

void decompress(char* source, int sourceSize, char* dest, int destSize)
{

	z_stream infstream;
	infstream.zalloc = Z_NULL;
	infstream.zfree = Z_NULL;
	infstream.opaque = Z_NULL;
	
	infstream.avail_in = (unsigned int)sourceSize; // size of input
	infstream.next_in = (Bytef*)source; // input char array
	
	infstream.avail_out = (unsigned int)destSize; // size of output
	infstream.next_out = (Bytef*)dest; // output char array

	// TODO(husam): handle errors comeing from  (inflate)
	int ret = inflateInit(&infstream);
	if(ret < Z_OK) zerr(ret);
	ret = inflate(&infstream, Z_NO_FLUSH);
	if(ret < Z_OK ) zerr(ret);
	inflateEnd(&infstream);

}

extern const char * GetTypeFromCode(int code)
{
	switch(code)
	{
	  case HEADER:
		  return "Not Expected";
	  case LIST:
		  return "List";
	  case INT:
		  return "Int";
	  case FLOAT:
		  return "Float";
	  case STRING:
		  return "String";
	  case COLLECTION:
		  return "Pair";
	  default:
		  return  "Unknown Data type";
	}
}
