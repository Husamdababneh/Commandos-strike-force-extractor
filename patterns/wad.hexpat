enum ChunkType : u32{
 HEADER = 2050,
 DESC   = 2051,
 DATA   = 2052
};

struct WadChunkHeader
{
    u32 version;
    u32 p1, p2;
};

struct WadChunkDescriptor 
{
    u32 v2;
    u32 headerSize;
    u32 p4, p5;
    u32 sampleRate;
    padding[9];
    u8 channels;
    padding[74];
    u32 type;
    u32 p6,p7,p8,p9;
    //padding[16];
    char name[]; // cstring
    u8 unknownChunkOfData[(headerSize - 112 ) - sizeof(name)];
};

struct WadChunkData {
    u32 v3;
    u32 dataSize;
    padding[4];
    //u8 data[dataSize];
    padding[dataSize];
};

struct WadChunk
{
    WadChunkHeader header;
    WadChunkDescriptor desc;
    WadChunkData data;
};

u32 numberOfChunks = 413;
//u32 numberOfChunks = 2;
WadChunk file[numberOfChunks] @ 0x0;

// sample.sampleRate * bitsPerSample * sample.channels / 8
// bitsPerSample * sample.channels / 8
// 4 + 12 + 4