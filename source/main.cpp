/*
  main.cpp -- First try for Commando Strike force Pak files extractor
*/

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <stdio.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <map>
#include <stack>


#include "functions.h"
//
#include "old.cpp"

// new 


#define open_file(handle, path, mode) FILE* handle; fopen_s(&handle, path,mode);

u64 read_entire_file(FILE* file, void** data_return)
{
	assert(file);
	int descriptor = fileno(file);
	
	struct stat file_stats;
	int result = fstat(descriptor, &file_stats);
	if (result == -1) return -1;
	
	u64 length = file_stats.st_size;
	
	unsigned char* data = new unsigned char[length];
	
	fseek(file, 0, SEEK_SET);
	u64 success = fread((void*)data, length, 1, file);
	if (success < 1) {
		delete[] data;
		return -1;
	}
	
	*data_return = data;
	return length;
}

u64 read_entire_file(const char* filepath, void** data_return)
{
	open_file(file, filepath, "rb");
	
	if (!file)
	{
		printf("Couldn't find file [%s]\n", filepath);
		return false;
	}
	
	u64 result = read_entire_file(file, data_return);
	fclose(file);
	return result;
}

#include "ExtractCSFFBS.cpp"



void ExtractPakFiles2(const char * filepath)
{
	//std::chrono::duration<float> timer;
	//cProfiler c(timer);
	
	u8* filedata;
	if (!read_entire_file(filepath, &(void*)filedata)){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}
	PakHeader* pak = (PakHeader*)filedata;
	printf("Tag = %.4s\n", pak->Tag);
	printf("IDK = %d\n", pak->IDK);
	printf("Version = %d\n", pak->Version);
	printf("ResNum = %d\n", pak->ResNum);
	
	u8* offset = filedata + sizeof(PakHeader);
	u8* cursor = offset;
	
	for(u32 i = 0; i < pak->ResNum; i++)
	{
		while(true){
			if(*offset == '\0'){ 
				offset = offset + 1;
				break;
			}
			offset = offset + 1;
		}
		offset = offset + 4;
		offset = offset + 4;
		offset = offset + 4;
		offset = offset + 4;
		
		//offset = offset + 1;
	}
	
	for(u32 i = 0; i < pak->ResNum; i++)
	{
		u8* filename = cursor;
		while(true){
			if(*cursor == '\0'){
				cursor= cursor+ 1;
				break;
			}
			cursor= cursor+ 1;
		}
		
		u32 roff = *(u32*)cursor; cursor += 4;
		u32 size = *(u32*)cursor; cursor += 4;
		u32 v1 = *(u32*)cursor;   cursor += 4;
		u32 v2 = *(u32*)cursor;   cursor += 4;
		//cursor= cursor+ 1;
		printf("ROff [%d], Size[%06d], V1[%d], V2[%d], Next [%d], FN [%s]\n", roff , size, v1, v2 ,
			   roff + size,
			   filename );
	}
	
	
	//FilesData* data = (u8**)malloc(sizeof(u8*) * strings_count );
	/* 
		int maximumFileSize = 0;
		FilesData* filesData = new FilesData[pak.ResNum];
		long lastitem = 0;
		for (int a = 0; a < pak.ResNum; a++)
		{
			auto& filedata = filesData[a];
			char filename[120];
			memset(filename, 0, 120); 
			
			char ch = 0;
			int size = 0;
			
			while((ch = file.get()) != '\0')
				filedata.Filename[size++] = ch;
			
			filedata.Filename[size] = '\0';
			
			file.read((char*)&filedata.ROFF, sizeof(long));
			file.read((char*)&filedata.Size, sizeof(long));
			file.read((char*)&filedata.Unknown1, sizeof(long));
			file.read((char*)&filedata.WhereItIs, sizeof(long));
			
			// std::cout << "roff: " << filedata.ROFF;
			// std::cout << "\tsize:" << std::setw(8) << filedata.Size;
			// std::cout << " v1: "  << std::hex  << filedata.Unknown1;
			// std::cout << "\tv2: "  << std::hex  << (int)filedata.WhereItIs << std::dec;
			// std::cout << "\tFilename: " << filedata.Filename;
			// std::cout << "\n";
			
			
			lastitem = filedata.Unknown1;
			
			if (filedata.Size > maximumFileSize)
				maximumFileSize = filedata.Size;
		}	
		//return 0;
		
		maxNum = pak.ResNum;
		char* data = new char[maximumFileSize];
		for (int a = 0; a < pak.ResNum; a++)
		{
			std::filesystem::path path{ "./Data/" };
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
		std::cout << "Finished: " << filepath  << "\n";
		file.close();
		delete[] filesData;
		return true;
		 */
	
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
		{
			ExtractPakFiles2(filePath);
			break;
		}
		case 'h':
		case 'H':
		{
			ExtractAllFiles(filePath);
			break;
		}
		case 'c':
		case 'C':
		{
			CompressPAC(filePath);
			break;}
		case '3':
		{
			RPCConvert(filePath);
			break;
		}
		case 't':
		case 'T':
		{
			ExtractCSFFBS(filePath);
			break;
		}
		default:
		std::cout << "Unknown command [" << whattodo << "]\n" ;
	}
	
	return 0;
}

