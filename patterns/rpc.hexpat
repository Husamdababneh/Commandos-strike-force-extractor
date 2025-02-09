
enum BlockType : u32 {
    INTEGER   = 0x00000001,  // 1
    Type_3   = 0x00000003,  // 3
    Type_14  = 0x0000000E,  // 14
    Type_15  = 0x0000000F,  // 15
    RPC_FILE = 0x00000010,  // RPC_FILE
    Type_20  = 0x00000014,  // 20
    MESH_DATA  = 0x0000001A   // 26
};

struct Block {
    BlockType type; // ?? idk
    u32 size; // 
    padding[4]; // 469893175 - 37 00 02 1C
    if (type == BlockType::INTEGER) {
        u32 data[size / sizeof(u32)];
    }
};

struct IdentificationBlock {
    BlockType type; // ?? idk
    u32 size; // 
    padding[4]; // 469893175 - 37 00 02 1C
    if (type != BlockType::RPC_FILE) {
        u8 data[size];
    }
};

struct IdentificationHeader {
    BlockType type; // ?? idk
    u32 size; // 
    padding[4]; // 469893175 - 37 00 02 1C
};

struct RpcHeader {
    BlockType type;
    u32 filesize;
    padding[4]; // 469893175 - 37 00 02 1C

};

struct Transforms {
    float matrix[12];
    s32 _parent;
    // padding[4];
    u32 unknown;
};

struct Section0xFFFFFF00 {
    u32 unkown1;
    // u32 _3700021C;
    padding[4]; // 469893175 - 37 00 02 1C
    u32 unknown2;
    u32 hasName;
    if (hasName == 1) {
        u32 id;
        u32 len;
        char name[len];
    }            
};


struct Object {
    u32 type; // ?? idk
    u32 size; // 
    padding[4]; // 469893175 - 37 00 02 1C
    u8 data[size];
};

struct Texcoords {
    float U;
    float V;
};

struct Vertex3 {
    float x,y,z;
};

struct Face {
    u16 index1;
    u16 index2;
    u16 faceMat;
    u16 index3;
};



struct theOneHundredByteStruct {
    u32 unknown1;
    u32 unknown2;
    
    padding[4]; // 469893175 - 37 00 02 1C
    u32 unknown3;
    u32 unknown4;
    u32 unknown5;
    
    padding[4]; // 469893175 - 37 00 02 1C
    u32 unknown6;
    u32 unknown7;
    
    padding[4]; // 469893175 - 37 00 02 1C
    u32 unknown8;
    u32 unknown9;
    u32 unknown10;
    
    padding[4]; // 469893175 - 37 00 02 1C
    u32 unknown11;
    u32 unknown12;
    
    padding[4]; // 469893175 - 37 00 02 1C
    u32 unknown13_[8];
    u8 unknown[100 - (4 * 25)];
};


#define ONEZEROZEROSTRUCT true

struct TextureData {
    IdentificationHeader header;
    
    //IdentificationHeader header2;
       
    u8 data[header.size];
};

struct StringView {
    u32 size;
    char string[size];
    u32 unknown1;
    u32 unknown2;
    u32 unknown3;
};

struct StringTable {
    u32 count;
    StringView strings[count];
};


struct AfterTextures {
    IdentificationHeader meta1;
        IdentificationHeader meta2;
        u32 unknown1;
        u32 unknown2;
        u32 unknown3;
        u32 unknown4;
        u8 data[meta2.size - 16];
        
        IdentificationHeader meta3;
        u8 data2[meta3.size];
        // StringTable strings;
};



struct Model { // todo
    IdentificationHeader meta1;
    IdentificationHeader meta2;

    if (false) {
        u32 something;
        u32 num_faces;
        u32 num_vertices;
        u32 unknown3; // num_textures?
        
        Texcoords uvs[num_vertices];
        Face faces[num_faces];
        padding[24];
        Vertex3 vertcies[num_vertices];
        Vertex3 vertex_normals[num_vertices];
    } else {
        u8 d[meta2.size];
    }
    
    IdentificationHeader meta3;
    IdentificationHeader meta4;

    u32 texture_count;
    padding[4];
    //padding[texture_count * 4]; /// FF * texture_count ?? 
    TextureData textures[texture_count];   
    AfterTextures at;
};



struct Model2 {
    IdentificationHeader header;
    //    IdentificationHeader header2;
    //    u32 info_id;
    //    u32 model_id;
    u8 a[header.size];
    
};

struct NObjects {
    IdentificationHeader header1;
    u8 data[header1.size];
};

struct RpcFile {
    RpcHeader header;
    Block b1;
        
        IdentificationHeader block2Header;
            IdentificationHeader block21Header;
                u32 num_objects;
                Transforms transforms[num_objects];
                NObjects objects[num_objects];
       
        IdentificationHeader block3Header;
            IdentificationHeader block31Header;
            u32 num_models;
            Model models[num_models];
            Model2 model[num_models];
            
       // IdentificationBlock block4Header;
        
      //IdentificationHeader lastBlock;
};

struct SizedBlock {
    u32 type; // maybe ?? 
    u32 size;
    padding[4];
    u32 something;
    u32 something2;
    //u8 block[size];
};

struct RpcFile2 {
    IdentificationBlock header;
    IdentificationBlock section1; // section1
    IdentificationBlock section2; // section1
    IdentificationBlock section3; // section1
    IdentificationBlock section4; // section1
    IdentificationBlock section5; // section1
    IdentificationBlock section6; // section1
};

RpcFile file @0x0;
//AfterTextures at @0x000017D8;
// IdentificationBlock block @0x00000000;
//IdentificationBlock block2 @0x0000000C;
// RpcFile2 file2 @0x0;
//IdentificationBlock block3 @0x00000024;

// 0x000032F5
