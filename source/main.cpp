/*
  main.cpp -- First try for Commando Strike force Pak files extractor
*/

#include "main.h"
#include "zlib.h"
#include "zconf.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cassert>
#include <stdio.h>
#include <thread>
#include <atomic>
#include <chrono>
#define LOGANDEXIT(x) { std::cout << x ; return 0;}
#define ERRORANDEXIT(x) { std::cerr << x ; return 0;}
#define assertm(exp, msg) assert((msg, exp))

std::atomic<int> progress = 0;
std::atomic<int> maxNum = 100;

void ProgressBar()
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


bool ExtractPakFiles(const char * filepath)
{

	std::cout << "Opening  " << filepath << "\n";
	std::fstream file(filepath, std::fstream::in | std::fstream::binary);

	if (!file.is_open())
		LOGANDEXIT("Error : cannot open " << filepath << "\n");
	
	int filesize = std::filesystem::file_size(filepath);
	PakHeader pak = { 0 };
	int filesOffset;
	int maximumFileSize = 0;
	file.read((char*)&pak, sizeof(PakHeader));
	std::cout << "File type = ";
	std::cout.write((char*)&pak.packtype,4) << "\n";
	std::cout << "V1 = " << pak.PackedBlockSize << "\n";
	std::cout << "V2 = " << pak.UnpackedBlockSize << "\n";
	std::cout << "ResNum = " << pak.ResNum << "\n";

	FilesData* filesData = new FilesData[pak.ResNum];
	for (int a = 0; a < pak.ResNum; a++)
	{
		auto& filedata = filesData[a];
		char* filename = new char[120];
		memset(filename, 0, 120);

		char ch = 0;
		int size = 0;
		for(char ch = 0; (ch = file.get()) != '\0'; size++)
			filedata.filename[size] = ch;
		filedata.filename[size] = '\0';
		file.read((char*)&filedata.ROFF, sizeof(long));
		file.read((char*)&filedata.Size, sizeof(long));
		file.read((char*)&filedata.V1, sizeof(long));
		file.read((char*)&filedata.V2, sizeof(long));
		
		
		if (filedata.Size > maximumFileSize)
			maximumFileSize = filedata.Size;
	}
	maxNum = pak.ResNum;
	//std::thread t1(ProgressBar);
	char* data = new char[maximumFileSize];
	for (int a = 0; a < pak.ResNum; a++)
	{
		std::filesystem::path path{ "D:\\Husam\\Games\\CSF\\Data" };
		path /= std::filesystem::path(filepath).stem();
		path /= filesData[a].filename;
		std::filesystem::create_directories(path.parent_path());
		std::fstream outfile(path, std::fstream::out | std::fstream::binary);

		if (((char*)&pak.packtype)[3] == 'A')
		{
			file.read(data, filesData[a].Size);
		}
		else
		{
			long PackedBlockSize, UnpackedBlockSize;
			int size = filesData[a].Size;
			char junk[sizeof(long)];
			int offset = 0;
			
			// What i found was that deflate and inflate has around 4:1 ratio of compression
			// for avarage files
			constexpr int MaxPackedBlockSize = 4096 + 1024;
			constexpr int MaxUnpackedBlockSize = MaxPackedBlockSize * 4;
			char packed[MaxPackedBlockSize], unpacked[MaxUnpackedBlockSize];
			
			while(1){
				if (size <= 0)
				{
					file.read(junk, sizeof(long));
					break;
				}
				file.read((char*)&PackedBlockSize, sizeof(long));
				file.read((char*)&UnpackedBlockSize, sizeof(long));

				assertm(MaxPackedBlockSize >= PackedBlockSize,
						"PackedBlockSize is bigger than you think");
				assertm(MaxUnpackedBlockSize >= UnpackedBlockSize,
						"UnpackedBlockSize is bigger than you think");

				file.read(packed, PackedBlockSize);

				decompress(packed, PackedBlockSize, unpacked, UnpackedBlockSize);
				memcpy(data + offset, unpacked, UnpackedBlockSize);
				
				size -= UnpackedBlockSize;
				offset += UnpackedBlockSize;
			}
			outfile.write(data, filesData[a].Size);
			outfile.close();
			
		}
		progress++;
	}
	//t1.join();
	file.close();
	return true;
}

/*
  RPC FileFormat
  
  4  - string_identifier == 16
  20 - unknown
  4  - models count 
  8  - unknown
  4  - objects count 

  (12*4             +    4          +      4 ) * objects
  transformation        parent     +      padding? 

*/

bool RPCConvert(const char * filepath)
{
	std::cout << "Opening  " << filepath << "\n";
	std::fstream file(filepath, std::fstream::in | std::fstream::binary);

	if (!file.is_open())
		LOGANDEXIT("Error : cannot open " << filepath << "\n");

	unsigned long string_identifier;
	file.read((char*)&string_identifier, sizeof(long));
	if(string_identifier != 16)
		LOGANDEXIT("Error : Not Vaild RPC File : string_identifier is not 16\n");

	char unknown1[20] = {0};
	file.read(unknown1, 20);

	long ModelsCount;
	file.read((char*)&ModelsCount, sizeof(long));
	
	char unknown2[8]= {0};
	file.read(unknown1, 8);

	long ObjectsCount;
	file.read((char*)&ObjectsCount, sizeof(long));
	
	std::cout << "Model count : " << ModelsCount << "\n";
	std::cout << "Object  count : " << ObjectsCount << "\n";
	file.close();
	return true;
}

int ExtractCSFFBS(const char* filepath)
{
	std::cout << "Opening  " << filepath << "\n";
	std::fstream file(filepath, std::fstream::in | std::fstream::binary);

	if (!file.is_open())
		LOGANDEXIT("Error : cannot open " << filepath << "\n");
	int filesize = std::filesystem::file_size(filepath);

	CSFFBSHeader header = {0};
	file.read((char*)&header, sizeof(CSFFBSHeader));

	std::cout << "Header Tag = " << header.Tag << "\n";
	std::cout << "Header Unknown = " << header.unknown1 << "\n";
	std::cout << "Header Unknown = " << header.unknown2 << "\n";
	std::cout << "Header Flag = " << (int)header.Flag;
	file.close();
	return 0;
}

int main(int argc, char** argv)
{

	// TODO(husam): make Usage usefull
	if (argc < 3)
		ERRORANDEXIT("Usage:" <<  argv[0] << " -<e/3/f>"  << " <filename>.pak\n");
	
	const char* whattodo = argv[1];
	const char* filePath = argv[2];

	switch(whattodo[1])
	{
	  case 'e':
	  case 'E':
		  ExtractPakFiles(filePath);
		  break;
	  case '3':
		  RPCConvert(filePath);
		  break;
	  case 'f':
	  case 'F':
		  ExtractCSFFBS(filePath);
		  break;
	  default:
		  std::cout << "Unknown command [" << whattodo << "]\n" ;
	}

	return 0;
}

