/* ========================================================================
   $File: rpc.cpp
   $Date: 2025-01-28
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/

#include "rpc.h"

struct StreamingBuffer {
    u8* content;
    u64 size;
};

#define PeekType(buffer, type) (type*)_PeekSize(buffer, sizeof(type), __LINE__)
#define ConsumeType(buffer, type) (type*)_ConsumeSize(buffer, sizeof(type), __LINE__)
#define ConsumeSize(buffer, size)        _ConsumeSize(buffer, size        , __LINE__)

void* _PeekSize(StreamingBuffer* buffer, u64 size, u64 callerLine = 0) {
    if (buffer->size < size) {
        if (callerLine == 0) {
            printf("Buffer ended\n");
        } else {
            printf("Buffer ended at line (%lld)\n", callerLine);
        };
        exit(-1); // assert here
    }

    if (size < 0) {
        if (callerLine == 0) {
            printf("Cannot Consume size less that 0\n");
        } else {
            printf("Cannot Consume size less that 0 at line (%lld)\n", callerLine);
        }
        exit(-1); // assert here
    };
    return buffer->content;
}

void* _ConsumeSize(StreamingBuffer* buffer, u64 size, u64 callerLine = 0)
{
    void* result = _PeekSize(buffer, size, callerLine);
    buffer->content += size;
    buffer->size    -= size;   
    return result;
}



void parse_chunk(StreamingBuffer* sb);

void parse_clump(StreamingBuffer* sb) {
    // Parse nested chunks within the Clump
    while (sb->size > 0) {
        parse_chunk(sb);
    }
}

void* parse_struct(StreamingBuffer* sb, RWHeader header) {
    return ConsumeSize(sb, header.size);
}

void parse_frame_list(StreamingBuffer* sb){
    auto frameListStruct = *ConsumeType(sb, RWHeader);
    FrameList frame_list;
    frame_list.frame_count = *ConsumeType(sb, s32);
    frame_list.frame_data  = new FrameData[frame_list.frame_count];
    for (u32 i = 0; i < frame_list.frame_count; i++) {
        frame_list.frame_data[i] = *ConsumeType(sb, FrameData);
    };

    RWHeader* header = PeekType(sb, RWHeader);
    u32 counter = 0;
    while(header->type == EXTENSION) {
        counter++;
        auto ext = ConsumeType(sb, RWHeader);
        ConsumeSize(sb, ext->size);
        header = PeekType(sb, RWHeader);
    };
    printf("\tFound %d EXTENSION chunks in FrameListData\n", counter);
    //exit(0);
}

void parse_geometry_list(StreamingBuffer* sb){
    
};

void parse_chunk(StreamingBuffer* sb) {
    RWHeader header = *ConsumeType(sb, RWHeader);

    printf("Found a [%s] chunk with size (%d)\n", GetChunkTypeName(header.type), header.size);
    switch(header.type){
      case CLUMP: {
          parse_clump(sb);
      } break;
          
      case STRUCT: {
          parse_struct(sb, header);
      }break;

      case FRAME_LIST: {
          parse_frame_list(sb);
      }break;
      case GEOMETRY_LIST: {
          parse_geometry_list(sb);
      }break;
      default: {
          if (header.size)
              ConsumeSize(sb, header.size);
      }break;
    }
    
};

bool RpcConvertNew(const char* filepath) {
    u8* filedata;
    u64 size = read_entire_file(filepath, &(void*)filedata);
    if (!size){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}
    
    StreamingBuffer sb;
    sb.content = filedata;
    sb.size = size;

    parse_chunk(&sb);
    return false;
};



