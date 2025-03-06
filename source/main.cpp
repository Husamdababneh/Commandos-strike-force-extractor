/* ========================================================================
   $File: main.cpp
   $Date: 2020-03-30
   $Creator: Husam Dababneh
   ========================================================================*/

#if 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_IDENTIFIER 16

#include "utils.cpp"
#include "main.h"
#include "ExtractCSFFBS.cpp"
#include "raw.cpp"

#include "rpc.cpp"
#include "gui.cpp"

inline 
u8* get_resource_pointer(u8* base, u32 roff)
{
	return base + roff - 13;
}



void ExtractPakA(PakHeader* pak, FilesMetaData* files, u8* offset)
{
    for(u32 i = 0; i < pak->ResNum; i++)
    {
        const u8* OUT_DIR = "output/"_u8;
        u32 out_dir_size = strlen((char*)OUT_DIR);
        char* output_file_path[512] = {0};
        
        strcat_s((char*)output_file_path, 512, (char*)OUT_DIR);
        strcat_s((char*)output_file_path, 512 - out_dir_size, (char*) files[i].filename);
		
        SV filePath = {3, (u8*)output_file_path};
        
#if 0
        File file = createFile(filePath);

        u8* output = get_resource_pointer(files[i].roff
        u64 wrote_count ;// fwrite(output, 1, file_size, outfile);
        b8 result = writeFile(file, output, file_size, &wrote_count);
       
        closeFile(file);
#endif
    }
}

void ExtractPakC(PakHeader* pak, FilesMetaData* files, u8* offset)
{
    u32 res_count = pak->ResNum;
    for(int resource_index = 0; resource_index < res_count; resource_index++)
    {
        //int it = 1;
        u8* it = get_resource_pointer(offset, files[resource_index].roff);
        u32 file_size = files[resource_index].size;
        u32 size = files[resource_index].size;
        u8* output = (u8*)malloc(sizeof(u8) * size);
        memset(output, 0, size);
		const char* filename = (const char*)files[resource_index].filename;
        u32 PackedBlockSize, UnpackedBlockSize;
        u32 ioffset = 0;
		
        // What i found was that inflate has around 4:1 ratio of compression in avarage
        constexpr int MaxPackedBlockSize = 4096 + 1024;
        constexpr int MaxUnpackedBlockSize = MaxPackedBlockSize * 4;
        char packed[MaxPackedBlockSize], unpacked[MaxUnpackedBlockSize];
		
        while(1){
            if (size <= 0)
            {
                it += sizeof(u32);
                break;
            }
            
            PackedBlockSize   = *(u32*) it;
            it += sizeof(u32);
            
            UnpackedBlockSize = *(u32*) it;
            it += sizeof(u32);
			
            // "PackedBlockSize is bigger than you think"
            assert(MaxPackedBlockSize >= PackedBlockSize);
            // "UnpackedBlockSize is bigger than you think"
            assert(MaxUnpackedBlockSize >= UnpackedBlockSize);
			
            memcpy(packed, it, PackedBlockSize);
            it += PackedBlockSize;
			
            decompress(packed, PackedBlockSize, unpacked, UnpackedBlockSize);
            memcpy(output + ioffset, unpacked, UnpackedBlockSize);
			
            size -= UnpackedBlockSize;
            ioffset += UnpackedBlockSize;
        }

        File file = createFile(to_sv(filename));
        
        u64 wrote_count;
        b8 result = writeFile(file, output, file_size, &wrote_count);
        
        closeFile(file);
    }
}

global 
PakFile ParsePakFile(const char* filepath) {
    PakFile result = {};
	void* fileData;
	if (!read_entire_file(filepath, &fileData)){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}

    u8* pak_file = (u8*)fileData;

    result.dataToBeFreed = pak_file;
    result.pak = (PakHeader*)pak_file;
    result.files = new FilesMetaData[result.pak->ResNum];

    u8* offset = pak_file + sizeof(PakHeader);

	for(u32 i = 0; i < result.pak->ResNum; i++)
	{
		result.files[i].filename = offset;
		while(*offset++ != '\0');
		result.files[i].roff = *(u32*)offset; offset += sizeof(u32);
		result.files[i].size = *(u32*)offset; offset += sizeof(u32);
		result.files[i].v1   = *(u32*)offset; offset += sizeof(u32);
		result.files[i].v2   = *(u32*)offset; offset += sizeof(u32);	
	}
    
    return result;
}

void ExtractPakFiles2(const char * filepath)
{
	
	void* fileData;
	if (!read_entire_file(filepath, &fileData)){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}

    u8* pak_file = (u8*)fileData;
    
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
		files[i].roff = *(u32*)offset; offset += sizeof(u32);
		files[i].size = *(u32*)offset; offset += sizeof(u32);
		files[i].v1   = *(u32*)offset; offset += sizeof(u32);
		files[i].v2   = *(u32*)offset; offset += sizeof(u32);
		
		if (files[i].size > maximum_file_size) maximum_file_size = files[i].size;
	}
	
	if (false)
	{
		for(u32 i = 0; i < pak->ResNum; i++)
		{
			printf("v1[0x%08X], v2[0x%X], [%s]\n", files[i].v1, files[i].v2_1, files[i].filename);
		}
	}
	
	
	// get i'th file from roff
	// files[i].roff + offset - 13 = the address of the i'th file's data
	
	
#if 1
	if (pak->Tag[3] == 'A'){
        ExtractPakA(pak, files, offset);
	} else {
		ExtractPakC(pak, files, offset);
	}
#endif
	
	free(files);
	free(pak_file);
	
}




int main(int argc, char** argv)
{
    const char* exe_name = argv[0];

    if (argc == 1 || (argc == 2 && argv[1][1] == 'w')){
        gui();
        return 0;
    }
    
    // TODO(husam): make Usage usefull
	if (argc < 3){
		printf("Usage: %s -<e/3/f> <filename>.pak\n", exe_name);
        return 0;
    }

	const char* whattodo = argv[1];    
	const char* filePath = argv[2];
	
	switch(whattodo[1])
	{
      case 'e': case 'E': {
          ExtractPakFiles2(filePath);
          break;
      }
      case 'h': case 'H': {
          //ExtractAllFiles(filePath);
          break;
      }
      case 'c': case 'C': {
          printf("Not implemented yet");
          //CompressPAC(filePath);
          break;
      }
      case '3': {
          // RpcConvert(filePath);
          RpcConvertNew(filePath);
          //printf("Not implemented yet");
          break;
      }
      case 't': case 'T': {
          ExtractCSFFBS(filePath);
          break;
      }
      case 'r': case 'R': {
          convertRaw(filePath);
          break;
      }
      case 'w':{
          gui();
          break;
      };
      default:
          printf("Unknown command [%s]\n", whattodo);
          printf("Usage: %s -<e/3/f> <filename>.pak\n", exe_name);
	}
	
	return 0;
}
#else


#include <stdio.h>
#include "utils.cpp"


int main(int argc, char** argv)
{
    SV fullPath = "folder1/folder2/folder3/folder4/first.txt"_sv;
    File file = createFile(fullPath);

    
    DWORD errorCode = GetLastError();
    printf("Error Code: %d\n", errorCode);    
    return 0;
}

#endif
