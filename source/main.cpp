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

#include <windows.h>
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

inline 
u8* get_resource_pointer(u8* base, u32 roff)
{
	return base + roff - 13;
}

void insure_dirs_exists(u8* filepath)
{
	u32 directories_count = 0;
	u8 filepath_b[512];
	memcpy((char*)filepath_b, (char*)filepath, strlen((char*)filepath));
	u8* cur = filepath;
	while(*cur++ != '\0'){
		if (*cur == '/') directories_count++;
	}
	if(directories_count == 0) return;
	
	// get first '/' from the end
	//while(*cur-- != '/');
	
	// final path
	u8 dir[512] = {0};
	u32 size = 0;
	u32 offset= 0;
	char* next_token = nullptr;
	u8* token = (u8*)strtok_s((char*)filepath_b, "/",&next_token);
	for(u32 i = 0; i < directories_count; i++)
	{
		size = strlen((char*)token);
		//memcpy(dir + offset, token, size);
		strcat_s((char*)dir, strlen((char*)dir) + size + 1, (char*)token);
		dir[strlen((char*)dir)] = '/';
		CreateDirectoryA((char*)dir, nullptr);
		token = (u8*)strtok_s(NULL, "/", &next_token);
	}
	//printf("%s\n", dir);
}

void ExtractPakFiles2(const char * filepath)
{
	//std::chrono::duration<float> timer;
	//cProfiler c(timer);
	
	u8* pak_file;
	if (!read_entire_file(filepath, &(void*)pak_file)){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}
	PakHeader* pak = (PakHeader*)pak_file;
	printf("Tag = %.4s\n", pak->Tag);
	printf("IDK = %d\n", pak->IDK);
	printf("Version = %d\n", pak->Version);
	printf("ResNum = %d\n", pak->ResNum);
	
	u8* offset = pak_file + sizeof(PakHeader);
	
	
	FilesMetaData* files = (FilesMetaData *)malloc(sizeof(FilesMetaData) * pak->ResNum);
	u32 maximum_file_size = 0;
	for(u32 i = 0; i < pak->ResNum; i++)
	{
		files[i].filename = offset;
		while(*offset++ != '\0');
		files[i].roff = *(u32*)offset; offset += 4;
		files[i].size = *(u32*)offset; offset += 4;
		files[i].v1   = *(u32*)offset; offset += 4;
		files[i].v2   = *(u32*)offset; offset += 4;
		
		if (files[i].size > maximum_file_size) maximum_file_size = files[i].size;
	}
	
	if (true)
	{
		for(u32 i = 0; i < pak->ResNum; i++)
		{
			printf("v1[0x%08X], v2[0x%X], [%s]\n", files[i].v1, files[i].v2_1, files[i].filename);
		}
	}
	
	
	// get i'th file from roff
	// files[i].roff + offset - 13 = the address of the i'th file's data
	
	
	
	
#if 0	
	if (pak->Tag[3] == 'A'){
		for(u32 i = 0; i < pak->ResNum; i++)
		{
			u8* OUT_DIR[] = "output/";
			u32 out_dir_size = strlen((char*)OUT_DIR);
			char* output_file_path[512] = {0};
			//memcpy(output_file_path, OUT_DIR, out_dir_size );
			strcat_s((char*)output_file_path, 512, (char*)OUT_DIR);
			strcat_s((char*)output_file_path, 512 - out_dir_size, (char*) files[i].filename);
			
			insure_dirs_exists((u8*)output_file_path);
			
			open_file(outfile, (char*)output_file_path, "wb");
			if(outfile){
				fwrite(files[i].roff + offset - 13, files[i].size, 1, outfile);
				
				fclose(outfile);
			}
		}
	}
	else
	{
		constexpr u32 res_num = 588;
		u8* filedata = get_resource_pointer(offset, files[res_num].roff);
		u8* output = (u8*)malloc(sizeof(u8) * files[res_num].size);
		memset(output, 0, files[res_num].size);
		u32 size = files[res_num].size;
		
		u32 PackedBlockSize, UnpackedBlockSize;
		u32 ioffset = 0;
		
		// What i found was that inflate has around 4:1 ratio of compression in avarage
		constexpr int MaxPackedBlockSize = 4096 + 1024;
		constexpr int MaxUnpackedBlockSize = MaxPackedBlockSize * 4;
		char packed[MaxPackedBlockSize], unpacked[MaxUnpackedBlockSize];
		
		while(1){
			if (size <= 0)
			{
				filedata += sizeof(u32);
				break;
			}
			PackedBlockSize   = *(u32*) filedata; filedata += sizeof(u32);
			UnpackedBlockSize = *(u32*) filedata; filedata += sizeof(u32);
			
			
			assertm(MaxPackedBlockSize >= PackedBlockSize,
					"PackedBlockSize is bigger than you think");
			assertm(MaxUnpackedBlockSize >= UnpackedBlockSize,
					"UnpackedBlockSize is bigger than you think");
			
			memcpy(packed, filedata, PackedBlockSize);
			filedata += PackedBlockSize;
			
			decompress(packed, PackedBlockSize, unpacked, UnpackedBlockSize);
			memcpy(output + ioffset, unpacked, UnpackedBlockSize);
			
			size -= UnpackedBlockSize;
			ioffset += UnpackedBlockSize;
		}
		
		open_file(outfile, "./test.dds", "wb");
		
		fwrite(output, files[res_num].size, 1, outfile);
		
		fclose(outfile);
	}
#endif
	
	free(files);
	free(pak_file);
	
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
			//ExtractAllFiles(filePath);
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

