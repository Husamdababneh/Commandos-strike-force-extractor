/*
  main.cpp -- First try for Commando Strike force Pak files extractor
*/

#include "functions.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <stdio.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

std::atomic<int> progress = 0;
std::atomic<int> maxNum = 100;

bool ExtractPakFiles(const char * filepath)
{

	std::cout << "Opening  " << filepath << "\n";
	std::cout << "Sizeof(PakHeader)" << sizeof(PakHeader) << "\n";
	std::fstream file(filepath, std::fstream::in | std::fstream::binary);

	if (!file.is_open())
		LOGANDEXIT("Error : cannot open " << filepath << "\n");
	
	int filesize = std::filesystem::file_size(filepath);
	PakHeader pak = { 0 };
	int filesOffset;
	int maximumFileSize = 0;
	file.read((char*)&pak, sizeof(PakHeader));
	std::cout << "File type = ";
	std::cout.write((char*)&pak.Tag, 4) << "\n";
	// std::cout << "V1 = " << pak.Unknown1 << "\n";
	// std::cout << "V2 = " << pak.Unknown2 << "\n";
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
			filedata.Filename[size++] = ch;

		filedata.Filename[size] = '\0';
		
		file.read((char*)&filedata.ROFF, sizeof(long));
		file.read((char*)&filedata.Size, sizeof(long));
		file.read((char*)&filedata.Unknown1, sizeof(long));
		file.read((char*)&filedata.Unknown2, sizeof(long));

		
		//std::cout << "roff: " << filedata.ROFF;
		//std::cout << "\tsize:" << std::setw(8) << filedata.Size;
		std::cout << "v1: "  << std::hex  << filedata.Unknown1;
		std::cout << "\tv2: "  << std::hex  << (int)filedata.Unknown2 << std::dec;
		std::cout << "\tFilename: " << filedata.Filename;
		std::cout << "\n";
		
		
		if (filedata.Size > maximumFileSize)
			maximumFileSize = filedata.Size;
	}
	std::cout << "File offset = " << std::hex << file.tellg() <<std::dec <<"\n";
	return 0;
	maxNum = pak.ResNum;
	char* data = new char[maximumFileSize];
	for (int a = 0; a < pak.ResNum; a++)
	{
		std::filesystem::path path{ "D:\\Husam\\Games\\CSF\\Data" };
		path /= std::filesystem::path(filepath).stem();
		path /= filesData[a].Filename;
		std::filesystem::create_directories(path.parent_path());
		std::fstream outfile(path, std::fstream::out | std::fstream::binary);

		if (((char*)&pak.Tag)[3] == 'A')
		{
			file.read(data, filesData[a].Size);
		}
		else
		{
			long PackedBlockSize, UnpackedBlockSize;
			int size = filesData[a].Size;
			char junk[sizeof(long)];
			int offset = 0;
			
			// What i found was that inflate has around 4:1 ratio of compression
			// for avarage files
			constexpr int MaxPackedBlockSize = 4096 + 1024;
			constexpr int MaxUnpackedBlockSize = MaxPackedBlockSize * 4;
			char packed[MaxPackedBlockSize], unpacked[MaxUnpackedBlockSize];
			// @Incomplete(Husam):Performance and more general way ??.
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
	file.close();
	return true;
}
// We Have script for that <3
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

// @Incomplete(Husam):Parameter
void ExtractCSFFBS(const char *filepath)
{
	std::fstream file(filepath, std::fstream::in | std::fstream::binary);
	std::cout << "Opening  " << filepath << "\n";
	
	if(!file.is_open())
	{
		std::cout << "[" <<filepath <<  "] was not found !! \n";
		return;
	}

	// Get file size
	file.seekg (0, file.end);
	int length = file.tellg();
	file.seekg (0, file.beg);


	CSFFBSHeader Header;
	file.read((char*)&Header, sizeof(Header));

	// std::cout << "Tag          : " << Header.Tag << "\n";
	// std::cout << "Version    : " << Header.Version << "\n";
	// std::cout << "NumberOfItems: " << Header.NumberOfItems << "\n";
	// std::cout << "Total Count  : " << Header.StringEntryCount << "\n";
	// std::cout << "String Count : " << Header.StringValueCount << "\n";
	// std::cout << "--------------------------------------\n";
	
	// Get the strings first so you don't need to store the data in a container 
	file.seekg(Header.NumberOfItems * 12 + 24 );
	std::vector<char *> Entries; Entries.reserve(Header.StringEntryCount);
	std::vector<char *> Values; Values.reserve(Header.StringValueCount);
	int size = 0;
	for(int a = 0; a < Header.StringEntryCount ; a++)
	{
		file.read((char*)&size, 4);
		char* name = new char[size + 1];
		file.read(name,size);
		Entries.push_back(name);
	}
	for(int a = 0; a < Header.StringValueCount ; a++)
	{
		file.read((char*)&size, 4);
		char* name = new char[size + 1];
		if (size == 0)
			name[0] = '\0';
		else
			file.read(name,size);
		Values.push_back(name);
	}
	
	file.seekg(24);
	int numberOfItems = 0;
	const char* outputPath = "Test.txt"  ;
	std::fstream outfile(outputPath, std::fstream::out);
	bool insidePair = false;
	bool flipTheBool = false;
	int numberofPair = 0;
	for(int a = 0; a < Header.NumberOfItems; a++) 
	{
		CSFFBSData data;
		file.read((char*)&data, sizeof(data));
		if(data.Type == HEADER)
		{
			std::cout << Entries[data.ItemStringIndex] << " ";	
		}
		else // if(data.Type == 0)
		{
			if(data.Type == STRING)
				std::cout << "\"" <<  Values[data.StringIndex] << "\"";
			else if(data.Type == INT)
				std::cout <<  data.IntegerValue;
			else if(data.Type == FLOAT)
				std::cout << std::fixed << data.FloatValue;		
			else if(data.Type == LIST)
			{
				if(data.ItemStringIndex != -1)
					std::cout << Entries[data.ItemStringIndex] << "\n";
				else
					std::cout << "We hit that point\n";
			}
			else if(data.Type == COLLECTION)
			{
				std::cout << Entries[data.ItemStringIndex] << " (";
				numberofPair = data.InnerItemCount; 
				flipTheBool = true;
				continue;
			}


			
			if(numberofPair == 0)
				std::cout << "\n";
			else 
			{
				if (numberofPair == 1)
					std::cout << ")\n";
				else
					std::cout << ", ";
				numberofPair--;
			}
		}
	}
	

	outfile << "]\n";
	file.close();
	outfile.close();
	for(int a = 0; a < Header.StringEntryCount ; a++)
		if(Entries[a] != nullptr)
			delete[] Entries[a];
	for(int a = 0; a < Header.StringValueCount ; a++)
		if(Values[a] != nullptr)
			delete[] Values[a];

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
	//long type = 1095450960;
	std::fstream outfile ("GlobalES.pak", std::fstream::out | std::fstream::binary);
	PakHeader header = {"PAKA", 1, rescount};
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
	  case 't':
	  case 'T':
		  ExtractCSFFBS(filePath);
		  break;
	  default:
		  std::cout << "Unknown command [" << whattodo << "]\n" ;
	}

	return 0;
}

