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

    u64 originalSize;
};

#define PeekType(buffer, type)    (type*)_PeekSize   (buffer, sizeof(type), __LINE__)
#define PeekSize(buffer, size)           _PeekSize   (buffer, size        , __LINE__)
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

u64 report_position(StreamingBuffer* sb) {
    return sb->originalSize - sb->size;
}


u32
libraryIDPack(u32 version, u32 build)
{
	if(version <= 0x31000)
		return version>>8;
	return (version-0x30000 & 0x3FF00) << 14 | (version & 0x3F) << 16 |
	       (build & 0xFFFF);
}

u32
libraryIDUnpackVersion(u32 libid)
{
	if(libid & 0xFFFF0000)
		return (libid>>14 & 0x3FF00) + 0x30000 |
		       (libid>>16 & 0x3F);
	return libid<<8;
}

u32
libraryIDUnpackBuild(u32 libid)
{
	if(libid & 0xFFFF0000)
		return libid & 0xFFFF;
	return 0;
}



void parse_chunk(StreamingBuffer* sb);
void parse_atomic(StreamingBuffer* sb);

void ConsumeExtensions(StreamingBuffer* sb);

void ConsumeExtensions(StreamingBuffer* sb) {
    RWHeader* extHeader = PeekType(sb, RWHeader);
    while(extHeader->type == EXTENSION) {
        ConsumeType(sb, RWHeader);
        ConsumeSize(sb, extHeader->size);

        if (sb->size == 0) return;
        
        extHeader = PeekType(sb, RWHeader);
    }
}

Clump* parse_clump(StreamingBuffer* sb) {
    RWHeader clumpHeader = *ConsumeType(sb, RWHeader);
    RWHeader clumpStructHeader = *ConsumeType(sb, RWHeader);
    Clump* clumpStruct = ConsumeType(sb, Clump);
    // printf("   atomics_count: %d\n", clumpStruct->atomics_count);
    // printf("   lights_count : %d\n", clumpStruct->lights_count);
    // printf("   cameras_count: %d\n", clumpStruct->cameras_count);
    return clumpStruct;
}

void parse_atomic(StreamingBuffer* sb) {
    RWHeader atomicHeader = *ConsumeType(sb, RWHeader);
    RWHeader atomicStructHeader = *ConsumeType(sb, RWHeader);
    Atomic* atomic = ConsumeType(sb, Atomic);
    // printf("Atomic Struct:\n");
    // printf("  frameIndex  : %u\n", atomic->frameIndex);
    // printf("  geometryIndex: %u\n", atomic->geometryIndex);
    // printf("  flags        : %u\n", atomic->flags);
    ConsumeExtensions(sb);
}

#if 0
void* parse_struct(StreamingBuffer* sb, RWHeader header) {
    RWHeader structHeader = *ConsumeType(sb, RWHeader);
    return ConsumeSize(sb, header.size);
}
#endif
void parse_frame_list(StreamingBuffer* sb){
    auto frameListHeader = *ConsumeType(sb, RWHeader);
    u32 sbCheckPoint = sb->size;
    auto frameListStruct = *ConsumeType(sb, RWHeader);
    FrameList frame_list;
    frame_list.frame_count = *ConsumeType(sb, s32);
    frame_list.frame_data  = new FrameData[frame_list.frame_count];
    for (u32 i = 0; i < frame_list.frame_count; i++) {
        frame_list.frame_data[i] = *ConsumeType(sb, FrameData);
    };

    // Idealy this should parse chunck but we know if there are things here they should be extensions
    u32 counter = 0;
    while(sbCheckPoint - sb->size < frameListHeader.size ) {
        RWHeader* extensionHeader = ConsumeType(sb, RWHeader);
        printf("Streaming Buffer position [0x%llX]\n", report_position(sb));
        RWHeader* extensionStruct = PeekType(sb, RWHeader);
        if (extensionHeader->type != EXTENSION) {
            printf("Expected EXTENSION chunk... exiting\n");
            exit(-1);
        }
        counter++;
        ConsumeSize(sb, extensionHeader->size);
    }

    // if (counter) printf("  Found %d EXTENSION chunks in FrameListData\n", counter);
}

#if 0
void parse_geometry(StreamingBuffer* sb){
    RWHeader header = *ConsumeType(sb, RWHeader); // consume Geometry struct info
    auto sizeCheckPoint = sb->size;
    ConsumeType(sb, RWHeader); // consume struct info
};
#endif

void parse_geometry_list(StreamingBuffer* sb, RWHeader header){
    ConsumeType(sb, RWHeader); // consume geometryListHeader
    ConsumeType(sb, RWHeader); // consume struct info
    
    GeometryList geometryList;
    geometryList.geometryCount = *ConsumeType(sb, u32);
    // parse_geometry(sb);
    for(u32 i = 0; i < geometryList.geometryCount; ++i) {
        RWHeader header = *ConsumeType(sb, RWHeader); // consume Geometry struct info
        u32 sbCheckPoint = sb->size;
        RWHeader structInfo = *ConsumeType(sb, RWHeader); // consume Geometry struct info
        auto sizeCheckPoint = sb->size;
        if (header.type != GEOMETRY ) {
            printf("Expected A GEOMETRY Chunk\n");
            exit(-1);
        }

        GeometryFormat format = *ConsumeType(sb, GeometryFormat);
        s32 numTriangles = *ConsumeType(sb, s32);
        s32 numVertices = *ConsumeType(sb, s32);
        s32 numMorphTargets = *ConsumeType(sb, s32);

        // printf("  format          : ");
        // printGeometryFormat(format);
        // printf("\n");
        // printf("  numTriangles    : %d\n", numTriangles);
        // printf("  numVertices     : %d\n", numVertices);
        // printf("  numMorphTargets : %d\n", numMorphTargets);

        if (format.rpGEOMETRYNATIVE == 0) {
            if (format.rpGEOMETRYPRELIT) {
                ConsumeSize(sb, sizeof(RwRGBA) * numVertices);
                printf("rpGEOMETRYPRELIT\n");
            }
            for (u32 texture = 0; texture < format.numTexSets; ++texture) {
                // RwTexCoords* uvs = PeekType(sb, RwTexCoords);
                ConsumeSize(sb, sizeof(RwTexCoords) * numVertices);
            }

            // printf("------------------------------------\n");
            RpTriangle* triangles = (RpTriangle*)ConsumeSize(sb, sizeof(RpTriangle) * numTriangles);
#if STDOUT_OBJ
            for(u32 t = 0; t < numTriangles; ++t) {
                printf("f %d %d %d\n",
                       triangles[t].vertex1 + 1,
                       triangles[t].vertex2 + 1,
                       triangles[t].vertex3 + 1);
            }
#endif // STDOUT_OBJ
        }

        for(u32 morph = 0; morph < numMorphTargets; ++morph) {
            ConsumeType(sb, RwSphere);
            b32 hasVerts = *ConsumeType(sb, b32);
            b32 hasNormals = *ConsumeType(sb, b32);

            if (hasVerts)   {
                RwV3d* vetrices = (RwV3d*)ConsumeSize(sb, sizeof(RwV3d) * numVertices);
#if STDOUT_OBJ
                for (u32 v = 0; v < numVertices; ++v){
                    printf("v %f %f %f\n",
                           vetrices[v].x,
                           vetrices[v].y,
                           vetrices[v].z);                    
                };
#endif // STDOUT_OBJ
            }
            if (hasNormals) {
                // ConsumeSize(sb, sizeof(RwV3d) * numVertices);
                RwV3d* vetrices = (RwV3d*)ConsumeSize(sb, sizeof(RwV3d) * numVertices);
#if STDOUT_OBJ
                for (u32 v = 0; v < numVertices; ++v){c
                    printf("vn %f %f %f\n",
                           vetrices[v].x,
                           vetrices[v].y,
                           vetrices[v].z);                    
                };
#endif // STDOUT_OBJ
            }
            
        }

        // I read somewhere that the ma
        RWHeader materialListHeader = *ConsumeType(sb, RWHeader); 
        RWHeader materialListStructHeader = *ConsumeType(sb, RWHeader);
        u32 materialsCount = *ConsumeType(sb, u32);
        // materials indecies
        u32 arraySize = sizeof(s32) * materialsCount;
        if (materialListStructHeader.size - sizeof(u32) != arraySize) {
            printf("materials indecies is not as expected\n");
            exit(-1);
        }
        ConsumeSize(sb, sizeof(s32) * materialsCount);

        for(u32 materialIt = 0; materialIt < materialsCount; ++materialIt) {
            RWHeader materialHeader = *ConsumeType(sb, RWHeader);
            RWHeader materialStructHeader = *ConsumeType(sb, RWHeader);
            ConsumeType(sb, s32); // flags unused
            RwRGBA color = *ConsumeType(sb, RwRGBA);
            ConsumeType(sb, s32); // unused
            s32 isTextured = *ConsumeType(sb, s32); //
            if (materialStructHeader.libId > 0x30400) {
                f32  ambient  = *ConsumeType(sb, f32);
                f32  specular = *ConsumeType(sb, f32);
                f32  diffuse  = *ConsumeType(sb, f32);
            }

            if (isTextured) {
                RWHeader textureHeader = *ConsumeType(sb, RWHeader);
                RWHeader textureStructHeader = *ConsumeType(sb, RWHeader);
                TextureInfo textureInfo = *ConsumeType(sb, TextureInfo);
                
                // printf("Texture time 0x%llX %lld\n", sb->originalSize - sb->size, sizeof(TextureInfo));
                RWHeader textureNameHeader = *ConsumeType(sb, RWHeader);
                const char* textureName = (const char*)ConsumeSize(sb, textureNameHeader.size);
//                printf("   Texture Name : [%s]\n", textureName);
                RWHeader textureAlphaLayerNameHeader = *ConsumeType(sb, RWHeader);
                const char* textureAlphaLayerName = (const char*)ConsumeSize(sb, textureAlphaLayerNameHeader.size);
                // if(textureAlphaLayerNameHeader.size) printf("   Texture Alpha Layer Name : [%s]\n", textureAlphaLayerName);                
            }

            ConsumeExtensions(sb);
            
        }
        

#if 1
        // Idealy this should parse chunck but we know if there are things here they should be extensions
        u32 counter = 0;
        while(sbCheckPoint - sb->size < header.size ) {
            RWHeader* extensionHeader = ConsumeType(sb, RWHeader);
            if (extensionHeader->type != EXTENSION) {
                printf("Expected EXTENSION chunk... exiting\n");
                exit(-1);
            }
            counter++;
            ConsumeSize(sb, extensionHeader->size);
        }

//        if (counter) printf("  Found %d EXTENSION chunks in FrameListData\n", counter);
#else
        if ((sizeCheckPoint - sb->size) < header.size) {
            printf("Data Still left in chunk\n");
            exit(-1);
        }
#endif

        
    }
//     printf("  Found %d geometries in the list\n", geometryList.geometryCount);
    // exit(0);
};

void parse_chunk(StreamingBuffer* sb) {
    RWHeader header = *PeekType(sb, RWHeader);

    const char* chunkTypeName = GetChunkTypeName(header.type);
    printf("Found a [%s] chunk with size (%d), version (0x%X) and build (0x%X)\n",
           chunkTypeName,
           header.size,
           libraryIDUnpackVersion(header.libId),
           libraryIDUnpackBuild(header.libId));

    if (header.type == EXTENSION && header.size == 0) {
        printf("Found last chunk\n");
        exit(0);
    }   

    if (strcmp(chunkTypeName, "UNKNOWN_CHUNK_TYPE") == 0) {
        printf("Exiting due to %s", chunkTypeName);
        exit(0);
    }
    
    switch(header.type){
      case CLUMP: {
          parse_clump(sb);
      }break;
      case ATOMIC: {
          parse_atomic(sb);
      }break;
#if 0
      case STRUCT: {
          parse_struct(sb, header);
      }break;
#endif
      case FRAME_LIST: {
          parse_frame_list(sb);
      }break;
      case GEOMETRY_LIST: {
          parse_geometry_list(sb, header);
      }break;
      default: {
          if (header.type != CLUMP)
              ConsumeSize(sb, header.size);

          if (header.type != EXTENSION && header.size == 0) {
              printf("Exiting due to incorrect type/size\n");
              exit(-1);
          }
      }break;
    }
};

bool RpcConvertNew(const char* filepath) {
    void* filedata;
    u64 size = read_entire_file(filepath, &filedata);
    if (!size){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}
    
    StreamingBuffer sb;
    sb.content = (u8*)filedata;
    sb.size = size;
    sb.originalSize = size;
    while (sb.size > 0) {
        parse_chunk(&sb);
    }
    printf("End of file %s----------------\n", filepath);
    return false;
};



