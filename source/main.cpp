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
#include <vector>
#define LOGANDEXIT(x) { std::cout << x ; return 0;}
#define ERRORANDEXIT(x) { std::cerr << x ; return 0;}
#define assertm(exp, msg) assert((msg, exp))

std::atomic<int> progress = 0;
std::atomic<int> maxNum = 100;

#define foreach(x) for(int a = 0; a < x ;  a++)


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
	std::cout << "V1 = " << pak.unknown1 << "\n";
	std::cout << "V2 = " << pak.unknown2 << "\n";
	std::cout << "ResNum = " << pak.ResNum << "\n";

	FilesData* filesData = new FilesData[pak.ResNum];
	for (int a = 0; a < pak.ResNum; a++)
	{
		auto& filedata = filesData[a];
		char* filename = new char[120];
		memset(filename, 0, 120);

		char ch = 0;
		int size = 0;

		while((ch = file.get()) != '\0')
			filedata.filename[size++] = ch;

		filedata.filename[size] = '\0';
		
		file.read((char*)&filedata.ROFF, sizeof(long));
		file.read((char*)&filedata.Size, sizeof(long));
		file.read((char*)&filedata.V1, sizeof(long));
		file.read((char*)&filedata.V2, sizeof(long));

		std::cout << "Filename: " << filedata.filename;
		std::cout << " roff: " << filedata.ROFF;
		std::cout << " size: " << filedata.Size;
		std::cout << " v1: " << /*std::hex <<*/ filedata.V1;
		std::cout << " v2: " << /*std::hex << */filedata.V2 << std::dec;
		// if(filedata.V2 == 29766566)
		// 	std::cout << " Photo 256X256";
		// else if(filedata.V2 == 29766561)
		// 	std::cout << " Photo 64 x 64";
		std::cout << "\n";
		
		
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

		// Check why this is not working ?? 
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
		}
		outfile.write(data, filesData[a].Size);
		outfile.close();
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
#if 0
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
#endif
	return 0;
}

void PrintData(CSFFBSData& data)
{
	switch(data.type)
	{
	  case 327679:
		  std::cout << "\tValue      : " << *((float*)(&data.value)) << "\n";
		  break;
	  case 262143:
		  std::cout << "\tValue      : " << data.value << "\n";
		  break;
	  case 393215:
		  std::cout << "\tValue      : String index " << data.value << " from string table\n";
		  break;
	  default:
		  std::cout << "\tUnhandled Data type      : " << data.type << "\n";
		  return;
	}
	std::cout << "\tType       : " << GetTypeFromCode(data.type) << "\n";
}
void Test()
{
	const char* gest =
		"D:\\Husam\\Games\\CSF\\data\\Ambush\\MENUS\\Retratos\\FotoGest.fbs";
	const char* general =
		"D:\\Husam\\Games\\CSF\\data\\Ambush\\MENUS\\Retratos\\FotoGeneral.fbs";
	const char* red =
		"D:\\Husam\\Games\\Commandos Strike Force C\\Config\\Red.cfg";
	const char* Punteria =
		"D:\\Husam\\Games\\Commandos Strike Force C\\Config\\Punteria.cfg";
	const char* Ver =
		"D:\\Husam\\Games\\Commandos Strike Force C\\Config\\JuegoVer.cfg";
	const char* su =
		"D:\\Husam\\Games\\CSF\\data\\Ambush\\Gfx\\1FU.sp";
	const char * filepath = Punteria;

	
	std::fstream file(filepath, std::fstream::in | std::fstream::binary);
	std::cout << "Opening  " << filepath << "\n";	
	if(!file.is_open())
	{
		std::cout << "[" <<filepath <<  "] was not found !! \n";
		return;
	}

	std::cout << "Sizeof(CSFFBSHeader) : " << sizeof(CSFFBSHeader) << "\n";
	std::cout << "Sizeof(CSFFBSSubHeader) : " << sizeof(CSFFBSSubHeader) << "\n";
	std::cout << "Sizeof(CSFFBSDataHeader) : " << sizeof(CSFFBSDataHeader) << "\n";
	std::cout << "Sizeof(CSFFBSData) : " << sizeof(CSFFBSData) << "\n";
	// Get file size
	file.seekg (0, file.end);
	int length = file.tellg();
	file.seekg (0, file.beg);

	
	CSFFBSHeader Header;
	file.read((char*)&Header, sizeof(Header));

	std::cout << "Type         : " << Header.Type << "\n";
	std::cout << "ListCount    : " << Header.ListCount << "\n";
	std::cout << "Unknown 1    : " << Header.unknown1 << "\n";
	std::cout << "Total Count  : " << Header.StringEntryCount << "\n";
	std::cout << "String Count : " << Header.StringValueCount << "\n";
	std::cout << "--------------------------------------\n";
	
	CSFFBSSubHeader SubHeader;
	ListData listData;
	file.read((char*)&SubHeader, sizeof(SubHeader));
	std::cout << "Unknown 1    : " << SubHeader.Unknown1 << "\n";
	std::cout << "Type         : " << GetTypeFromCode(SubHeader.Type) << "\n";
	std::cout << "--------------------------------------\n";
	
	if(SubHeader.Unknown1 == 1) // if list
	{
		file.read((char*)&listData, sizeof(listData));
		std::cout << "Unknown1     : " << listData.Unknown1 << "\n";
		std::cout << "Item Count   : " << listData.InnerItemCount << "\n";
		std::cout << "Unknown2     : " << listData.Unknown2 << "\n";
	}

	int ElementsToRead = 0;
	if(SubHeader.Unknown1 == 1)
		ElementsToRead = listData.InnerItemCount;
	else
		ElementsToRead = Header.StringEntryCount;
	for(int a = 0; a < ElementsToRead ; a++) 
	{
		CSFFBSDataHeader dataHeader;
		file.read((char*)&dataHeader, sizeof(dataHeader));
		std::cout << "Unknown3    : " << dataHeader.Unknown3<< "\n";
		std::cout << "Indecator    : " << dataHeader.Indecator<< "\n";
		std::cout << "Serial       : " << dataHeader.Serial << "\n";

		if(dataHeader.Indecator  == -1){
			CSFFBSData data;
			file.read((char*)&data, sizeof(data));
			PrintData(data);

		}  else if (dataHeader.Indecator  == 15){
			std::cout << "Get the fuck out from here\n.... \n For now" ;
			return;
		} else {

			CSFFBSData data;
			for(int b = 0 ; b < dataHeader.Indecator; b++)
			{
				file.read((char*)&data, sizeof(data));
				PrintData(data);
			}

		}
	}
    // this part is working well, we just need to find the offset to start reading the strings
    // @incomplete(Husam):  this should also be aware that not all the strings is entries

	// @Todo(Husam):  this needs to be tweaked because some files has multiple entires
	//                with the same string
	std::vector<char *> Entries; Entries.reserve(Header.StringEntryCount);
	std::vector<char *> Values; Values.reserve(Header.StringValueCount);
	int size = 0;
	for(int a = 0; a < Header.StringEntryCount ; a++)
	{
		char* name = new char[50];
		file.read((char*)&size, 4);
		if (size == 0)
			memset(name, '\0', 50);
		else
			file.read(name,size);

		Entries.push_back(name);
	}
	for(int a = 0; a < Header.StringValueCount ; a++)
	{
		char* name = new char[50];
		file.read((char*)&size, 4);
		if (size == 0)
			memset(name, '\0', 50);
		else
			file.read(name,size);

		Values.push_back(name);
	}
	
	
	const char* outputPath = "Test.txt"  ;
	std::fstream outfile(outputPath, std::fstream::out);

	outfile << "[\n";
	for(int a = 0; a < ElementsToRead; a++)
	{
		outfile << Entries[a] << "\n";
	}
	outfile << "]\n";
	
	file.close();
	outfile.close();
	for(int a = 0; a < Header.StringEntryCount ; a++)
		if(Entries[a] != nullptr)
			delete Entries[a];
	for(int a = 0; a < Header.StringValueCount ; a++)
		if(Values[a] != nullptr)
			delete Values[a];
}

void CompressPAC(const char * folderPath)
{
	std::filesystem::path path{ folderPath };
	if (!std::filesystem::is_directory(folderPath))
	{
		std::cout << "not direcotry, Make sure that the path doesn't end with a backslash(\\)!!\n";
	}

	int rescount = 0;
	for(auto& p: std::filesystem::recursive_directory_iterator(path))
		if(!std::filesystem::is_directory(p))
			rescount++;



	std::cout << "Res Count = " << rescount << "\n";
	long type = 1095450960;
	std::fstream outfile ("GlobalES.pak", std::fstream::out | std::fstream::binary);
	PakHeader header = {type, 5,1, rescount};
	outfile.write((char*)&header, sizeof(header));

	long offset = 13;
	
	long V2 = 29766561;
	for(auto& p: std::filesystem::recursive_directory_iterator(path))
	{
		char filename[120];
		
		if(!std::filesystem::is_directory(p))
		{
			long size = std::filesystem::file_size(p);

			size_t filenameSize = strlen(p.path().string().c_str()) - strlen(folderPath);
			strcpy(filename, p.path().string().c_str() + strlen(folderPath) + 1);
			
			outfile.write(filename, filenameSize);
			outfile.write((char*)&offset, sizeof(long)); // offset ?? 
			outfile.write((char*)&size, sizeof(long)); // size
			outfile.write((char*)&V2, sizeof(long)); // unknown
			outfile.write((char*)&V2, sizeof(long)); // unknown 
			offset+=size;
		}
	}

	for(auto& p: std::filesystem::recursive_directory_iterator(path))
	{
		
		if(!std::filesystem::is_directory(p))
		{
			long size = std::filesystem::file_size(p);
			std::fstream myfile(p.path().string().c_str(),
								std::ios::in | std::ios::binary);
			char* data = new char[size];
			myfile.read(data, size);
			outfile.write(data, size);
			delete[] data;
		}
	}
	outfile.close();
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
	  case 'c':
	  case 'C':
		  CompressPAC(filePath);
		  break;
	  case '3':
		  RPCConvert(filePath);
		  break;
	  case 'f':
	  case 'F':
		  ExtractCSFFBS(filePath);
		  break;
	  case 't':
	  case 'T':
		  Test();
		  break;
	  default:
		  std::cout << "Unknown command [" << whattodo << "]\n" ;
	}

	return 0;
}

