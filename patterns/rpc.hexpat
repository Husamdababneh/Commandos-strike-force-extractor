enum ChunkType : u32 {
    STRUCT = 0x00000001, // Core: A generic section that stores data for its parent.
    STRING = 0x00000002, // Core: Stores a 4-byte aligned ASCII string.
    EXTENSION = 0x00000003, // Core: A container for non-standard extensions of its parent section.
    CAMERA = 0x00000005, // Core: Contains a camera (unused in GTA games).
    TEXTURE = 0x00000006, // Core: Stores the sampler state of a texture.
    MATERIAL = 0x00000007, // Core: Defines a material to be used on a geometry.
    MATERIAL_LIST = 0x00000008, // Core: Container for a list of materials.
    ATOMIC_SECTION = 0x00000009, // Core: 
    PLANE_SECTION = 0x0000000A, // Core: 
    WORLD = 0x0000000B, // Core: The root section of the level geometry.
    SPLINE = 0x0000000C, // Core: 
    MATRIX = 0x0000000D, // Core: 
    FRAME_LIST = 0x0000000E, // Core: Container for a list of frames. A frame holds the transformation that is applied to an Atomic.
    GEOMETRY = 0x0000000F, // Core: A platform-independent container for meshes.
    CLUMP = 0x00000010, // Core: The root section for a 3D model.
    LIGHT = 0x00000012, // Core: Stores different dynamic lights.
    UNICODE_STRING = 0x00000013, // Core: 
    ATOMIC = 0x00000014, // Core: Defines the basic unit for the RenderWare graphics pipeline. Generally speaking, an Atomic can be directly converted to a single draw call.
    RASTER = 0x00000015, // Core: Stores a platform-dependent (i.e. native) texture image.
    TEXTURE_DICTIONARY = 0x00000016, // Core: A container for texture images (also called raster).
    ANIMATION_DATABASE = 0x00000017, // Core: 
    IMAGE = 0x00000018, // Core: An individual texture image.
    SKIN_ANIMATION = 0x00000019, // Core: 
    GEOMETRY_LIST = 0x0000001A, // Core: A container for a list of geometries.
    ANIM_ANIMATION = 0x0000001B, // Core: 
    TEAM = 0x0000001C, // Core: 
    CROWD = 0x0000001D, // Core: 
    DELTA_MORPH_ANIMATION = 0x0000001E, // Core: 
    RIGHT_TO_RENDER = 0x0000001F, // Core: Stores the render pipeline the engine uses to draw an atomic or material.
    MULTI_TEXTURE_EFFECT_NATIVE = 0x00000020, // Core: 
    MULTI_TEXTURE_EFFECT_DICTIONARY = 0x00000021, // Core: 
    TEAM_DICTIONARY = 0x00000022, // Core: 
    PLATFORM_INDEPENDENT_TEXTURE_DICTIONARY = 0x00000023, // Core: 
    TABLE_OF_CONTENTS = 0x00000024, // Core: 
    PARTICLE_STANDARD_GLOBAL_DATA = 0x00000025, // Core: 
    ALTPIPE = 0x00000026, // Core: 
    PLATFORM_INDEPENDENT_PEDS = 0x00000027, // Core: 
    PATCH_MESH = 0x00000028, // Core: 
    CHUNK_GROUP_START = 0x00000029, // Core: 
    CHUNK_GROUP_END = 0x0000002A, // Core: 
    UV_ANIMATION_DICTIONARY = 0x0000002B, // Core: 
    COLL_TREE = 0x0000002C, // Core: 
    METRICS_PLG = 0x00000101, // Toolkit: 
    SPLINE_PLG = 0x00000102, // Toolkit: 
    STEREO_PLG = 0x00000103, // Toolkit: 
    VRML_PLG = 0x00000104, // Toolkit: 
    MORPH_PLG = 0x00000105, // Toolkit: 
    PVS_PLG = 0x00000106, // Toolkit: 
    MEMORY_LEAK_PLG = 0x00000107, // Toolkit: 
    ANIMATION_PLG = 0x00000108, // Toolkit: 
    GLOSS_PLG = 0x00000109, // Toolkit: 
    LOGO_PLG = 0x0000010A, // Toolkit: 
    MEMORY_INFO_PLG = 0x0000010B, // Toolkit: 
    RANDOM_PLG = 0x0000010C, // Toolkit: 
    PNG_IMAGE_PLG = 0x0000010D, // Toolkit: 
    BONE_PLG = 0x0000010E, // Toolkit: 
    VRML_ANIM_PLG = 0x0000010F, // Toolkit: 
    SKY_MIPMAP_VAL = 0x00000110, // Toolkit: Stores MipMap parameters for the PS2 version of the engine (codenamed Sky).
    MRM_PLG = 0x00000111, // Toolkit: 
    LOD_ATOMIC_PLG = 0x00000112, // Toolkit: 
    ME_PLG = 0x00000113, // Toolkit: 
    LIGHTMAP_PLG = 0x00000114, // Toolkit: 
    REFINE_PLG = 0x00000115, // Toolkit: 
    SKIN_PLG = 0x00000116, // Toolkit: 
    LABEL_PLG = 0x00000117, // Toolkit: 
    PARTICLES_PLG = 0x00000118, // Toolkit: 
    GEOMTX_PLG = 0x00000119, // Toolkit: 
    SYNTH_CORE_PLG = 0x0000011A, // Toolkit: 
    STQPP_PLG = 0x0000011B, // Toolkit: 
    PART_PP_PLG = 0x0000011C, // Toolkit: 
    COLLISION_PLG = 0x0000011D, // Toolkit: 
    HANIM_PLG = 0x0000011E, // Toolkit: 
    USER_DATA_PLG = 0x0000011F, // Toolkit: 
    MATERIAL_EFFECTS_PLG = 0x00000120, // Toolkit: 
    PARTICLE_SYSTEM_PLG = 0x00000121, // Toolkit: 
    DELTA_MORPH_PLG = 0x00000122, // Toolkit: 
    PATCH_PLG = 0x00000123, // Toolkit: 
    TEAM_PLG = 0x00000124, // Toolkit: 
    CROWD_PP_PLG = 0x00000125, // Toolkit: 
    MIP_SPLIT_PLG = 0x00000126, // Toolkit: 
    ANISOTROPY_PLG = 0x00000127, // Toolkit: Stores the anisotropy for a texture filter.
    GCN_MATERIAL_PLG = 0x00000129, // Toolkit: 
    GEOMETRIC_PVS_PLG = 0x0000012A, // Toolkit: 
    XBOX_MATERIAL_PLG = 0x0000012B, // Toolkit: 
    MULTI_TEXTURE_PLG = 0x0000012C, // Toolkit: 
    CHAIN_PLG = 0x0000012D, // Toolkit: 
    TOON_PLG = 0x0000012E, // Toolkit: 
    PTANK_PLG = 0x0000012F, // Toolkit: 
    PARTICLE_STANDARD_PLG = 0x00000130, // Toolkit: 
    PDS_PLG = 0x00000131, // Toolkit: 
    PRTADV_PLG = 0x00000132, // Toolkit: 
    NORMAL_MAP_PLG = 0x00000133, // Toolkit: 
    ADC_PLG = 0x00000134, // Toolkit: 
    UV_ANIMATION_PLG = 0x00000135, // Toolkit: 
    CHARACTER_SET_PLG = 0x00000180, // Toolkit: 
    NOHS_WORLD_PLG = 0x00000181, // Toolkit: 
    IMPORT_UTIL_PLG = 0x00000182, // Toolkit: 
    SLERP_PLG = 0x00000183, // Toolkit: 
    OPTIM_PLG = 0x00000184, // Toolkit: 
    TL_WORLD_PLG = 0x00000185, // Toolkit: 
    DATABASE_PLG = 0x00000186, // Toolkit: 
    RAYTRACE_PLG = 0x00000187, // Toolkit: 
    RAY_PLG = 0x00000188, // Toolkit: 
    LIBRARY_PLG = 0x00000189, // Toolkit: 
    PLG_2D = 0x00000190, // Toolkit: 
    TILE_RENDER_PLG = 0x00000191, // Toolkit: 
    JPEG_IMAGE_PLG = 0x00000192, // Toolkit: 
    TGA_IMAGE_PLG = 0x00000193, // Toolkit: 
    GIF_IMAGE_PLG = 0x00000194, // Toolkit: 
    QUAT_PLG = 0x00000195, // Toolkit: 
    SPLINE_PVS_PLG = 0x00000196, // Toolkit: 
    MIPMAP_PLG = 0x00000197, // Toolkit: 
    MIPMAPK_PLG = 0x00000198, // Toolkit: 
    FONT_2D = 0x00000199, // Toolkit: 
    INTERSECTION_PLG = 0x0000019A, // Toolkit: 
    TIFF_IMAGE_PLG = 0x0000019B, // Toolkit: 
    PICK_PLG = 0x0000019C, // Toolkit: 
    BMP_IMAGE_PLG = 0x0000019D, // Toolkit: 
    RAS_IMAGE_PLG = 0x0000019E, // Toolkit: 
    SKIN_FX_PLG = 0x0000019F, // Toolkit: 
    VCAT_PLG = 0x000001A0, // Toolkit: 
    PATH_2D = 0x000001A1, // Toolkit: 
    BRUSH_2D = 0x000001A2, // Toolkit: 
    OBJECT_2D = 0x000001A3, // Toolkit: 
    SHAPE_2D = 0x000001A4, // Toolkit: 
    SCENE_2D = 0x000001A5, // Toolkit: 
    PICK_REGION_2D = 0x000001A6, // Toolkit: 
    OBJECT_STRING_2D = 0x000001A7, // Toolkit: 
    ANIMATION_PLG_2D = 0x000001A8, // Toolkit: 
    ANIMATION_2D = 0x000001A9, // Toolkit: 
    KEYFRAME_2D = 0x000001B0, // Toolkit: 
    MAESTRO_2D = 0x000001B1, // Toolkit: 
    BARYCENTRIC = 0x000001B2, // Toolkit: 
    PLATFORM_INDEPENDENT_TEXTURE_DICTIONARY_TK = 0x000001B3, // Toolkit: 
    TOC_TK = 0x000001B4, // Toolkit: 
    TPL_TK = 0x000001B5, // Toolkit: 
    ALTPIPE_TK = 0x000001B6, // Toolkit: 
    ANIMATION_TK = 0x000001B7, // Toolkit: 
    SKIN_SPLIT_TOOKIT = 0x000001B8, // Toolkit: 
    COMPRESSED_KEY_TK = 0x000001B9, // Toolkit: 
    GEOMETRY_CONDITIONING_PLG = 0x000001BA, // Toolkit: 
    WING_PLG = 0x000001BB, // Toolkit: 
    GENERIC_PIPELINE_TK = 0x000001BC, // Toolkit: 
    LIGHTMAP_CONVERSION_TK = 0x000001BD, // Toolkit: 
    FILESYSTEM_PLG = 0x000001BE, // Toolkit: 
    DICTIONARY_TK = 0x000001BF, // Toolkit: 
    UV_ANIMATION_LINEAR = 0x000001C0, // Toolkit: 
    UV_ANIMATION_PARAMETER = 0x000001C1, // Toolkit: 
    BIN_MESH_PLG = 0x0000050E, // World: 
    NATIVE_DATA_PLG = 0x00000510, // World: 
    EARS_MATERIAL_DATA = 0x0000EA13, // EA Redwood Shores: 
    EA_REDWOOD_SHORES_1 = 0x0000EA15, // EA Redwood Shores: 
    EARS_MESH_PLUGIN = 0x0000EA16, // EA Redwood Shores: 
    EA_REDWOOD_SHORES_2 = 0x0000EA20, // EA Redwood Shores: 
    EARS_ZONE_PLUGIN = 0x0000EA28, // EA Redwood Shores: 
    EARS_LT_MAP_2 = 0x0000EA2D, // EA Redwood Shores: 
    EARS_RP_PARTIAL_INSTANCE = 0x0000EA2E, // EA Redwood Shores: 
    EARS_TEXTURE_PLUGIN = 0x0000EA2F, // EA Redwood Shores: 
    EARS_MESH = 0x0000EA33, // EA Redwood Shores: Stores mesh data.
    EARS_ATOMIC_PLUGIN = 0x0000EA40, // EA Redwood Shores: 
    EARS_DISPLAY_LIST = 0x0000EA44, // EA Redwood Shores: 
    EARS_RP_SHADER = 0x0000EA45, // EA Redwood Shores: 
    EARS_RP_ALCHEMY = 0x0000EA80, // EA Redwood Shores: 
    EARS_MORPH_TARGET_DATA = 0x0000EA92, // EA Redwood Shores: 
    ZMODELER_LOCK = 0x0000F21E, // ZModeler: Unofficial extension that stores a password that protects the file from being modified when opened in ZModeler. Ignored by other applications like RWAnalyze and the GTA games.
    THQ_ATOMIC = 0x00CAFE40, // THQ: Jimmy Neutron: Attack of the Twonkies custom. Atomic rendering flags.
    THQ_MATERIAL = 0x00CAFE45, // THQ: Jimmy Neutron: Attack of the Twonkies custom. Extended material parameters.
    ATOMIC_VISIBILITY_DISTANCE = 0x0253F200, // Rockstar: 
    CLUMP_VISIBILITY_DISTANCE = 0x0253F201, // Rockstar: 
    FRAME_VISIBILITY_DISTANCE = 0x0253F202, // Rockstar: 
    PIPELINE_SET = 0x0253F2F3, // Rockstar: Stores the render pipeline used to draw objects with Rockstar-specific plug-ins.
    TEXDICTIONARY_LINK = 0x0253F2F5, // Rockstar: 
    SPECULAR_MATERIAL = 0x0253F2F6, // Rockstar: Stores a specularity map.
    EFFECT_2D = 0x0253F2F8, // Rockstar: Used to attach various GTA-specific effects to models, for example to enable script interaction or particle effects.
    EXTRA_VERT_COLOR = 0x0253F2F9, // Rockstar: Stores an additional array of vertex colors, that are used in GTA during night-time to simulate some effects of dynamic global lighting.
    COLLISION_MODEL = 0x0253F2FA, // Rockstar: Stores a collision model.
    GTA_HANIM = 0x0253F2FB, // Rockstar: 
    REFLECTION_MATERIAL = 0x0253F2FC, // Rockstar: Enables advanced environment mapping.
    BREAKABLE = 0x0253F2FD, // Rockstar: Contains a mesh that is used to render objects that are breakable (like windows or tables).
    FRAME_NAME = 0x0253F2FE, // Rockstar: Stores the name of a frame within a Frame List.
};

struct ChunkHeader {
    ChunkType type;
    u32 size;
    u32 version;
};

struct ExtensionChunk {
    ChunkHeader header;
    u8 data[header.size];
};

struct Chunk {
    ChunkHeader header;
    if (header.type == ChunkType::CLUMP) {
        // do nothing
    }
};

struct Clump {
    u32 atomics_count;
    u32 lights_count;
    u32 cameras_count;
};


struct TVector3F {
    float x,y,z;
};

struct TMatrix3x3F {
    TVector3F right, up, at;
};

struct FrameData {
    TMatrix3x3F rotation_matrix;
    TVector3F   position;
    s32         parent_index;
    s32         flags;
};

struct FrameList {
    ChunkHeader header;
    ChunkHeader structInfo;
    u32 frameCount;
    FrameData frameData[frameCount];
    ExtensionChunk extensions[frameCount];
};

enum GeometryFormat : u32 {
    rpGEOMETRYTRISTRIP = 0x00000001, //     rpGEOMETRYTRISTRIP  Is triangle strip (if disabled it will be an triangle list)
    rpGEOMETRYPOSITIONS = 0x00000002, //    rpGEOMETRYPOSITIONS Vertex translation
    rpGEOMETRYTEXTURED = 0x00000004, //     rpGEOMETRYTEXTURED  Texture coordinates
    rpGEOMETRYPRELIT = 0x00000008, //   rpGEOMETRYPRELIT    Vertex colors
    rpGEOMETRYNORMALS = 0x00000010, //  rpGEOMETRYNORMALS   Store normals
    rpGEOMETRYLIGHT = 0x00000020, //    rpGEOMETRYLIGHT Geometry is lit (dynamic and static)
    rpGEOMETRYMODULATEMATERIALCOLOR = 0x00000040, //    rpGEOMETRYMODULATEMATERIALCOLOR Modulate material color
    rpGEOMETRYTEXTURED2 = 0x00000080, //    rpGEOMETRYTEXTURED2 Texture coordinates 2
    rpGEOMETRYNATIVE = 0x01000000, //   rpGEOMETRYNATIVE    Native Geometry
};

bitfield GeometryFormat2 {
    rpGEOMETRYTRISTRIP : 1;               // Is triangle strip (if disabled, it will be a triangle list)
    rpGEOMETRYPOSITIONS : 1;              // Vertex translation
    rpGEOMETRYTEXTURED : 1;               // Texture coordinates
    rpGEOMETRYPRELIT : 1;                 // Vertex colors
    rpGEOMETRYNORMALS : 1;                // Store normals
    rpGEOMETRYLIGHT : 1;                  // Geometry is lit (dynamic and static)
    rpGEOMETRYMODULATEMATERIALCOLOR : 1;  // Modulate material color
    rpGEOMETRYTEXTURED2 : 1;              // Texture coordinates 2
    unused1: 8;                           // Unused/reserved bits
    numTexSets : 8;
    unused : 7;                           // Unused/reserved bits
    rpGEOMETRYNATIVE : 1;                 // Native Geometry
};

struct RwRGBA {
    u8 r,g,b,a;
};


struct RwV3d {
    float x, y, z;
};

struct RpTriangle {
    u16 vertex2, vertex1, materialId, vertex3;
};

struct RwSphere {
    float x, y, z, radius; 
};

struct RwTexCoords {
    float u,v;
};

struct Morphs<auto count> {
    RwSphere boundingSphere;
    u32 hasVertices;
    u32 hasNormals;
    if (hasVertices) {
        RwV3d vertices[count];
    }
    if (hasNormals) {
        RwV3d normals[count];
    }
};
/*
Texture filtering modes

0 - FILTERNAFILTERMODE (filtering is disabled)
1 - FILTERNEAREST (Point sampled)
2 - FILTERLINEAR (Bilinear)
3 - FILTERMIPNEAREST (Point sampled per pixel mip map)
4 - FILTERMIPLINEAR (Bilinear per pixel mipmap)
5 - FILTERLINEARMIPNEAREST (MipMap interp point sampled)
6 - FILTERLINEARMIPLINEAR (Trilinear)
Texture addressing modes

0 - TEXTUREADDRESSNATEXTUREADDRESS (no tiling)
1 - TEXTUREADDRESSWRAP (tile in U or V direction)
2 - TEXTUREADDRESSMIRROR (mirror in U or V direction)
3 - TEXTUREADDRESSCLAMP
4 - TEXTUREADDRESSBORDER

*/
bitfield TextureInfo {
    filtering: 8;
    u:4;
    v:4;
    isUsingMipLevels: 1;
    pad: 15;
};

struct StringChunk {
    ChunkHeader header;
    if (header.size > 0)
        char name[header.size];
};

struct Texture {
    ChunkHeader header;
    ChunkHeader textureStructinfo;
    TextureInfo info;
    StringChunk textureName;
    StringChunk alphaLayerName;
};

struct Material {
    ChunkHeader header;
    ChunkHeader materialStructInfo;
    s32 flags; // unsued
    RwRGBA color;
    s32 unused;
    u32 isTextured;
    float ambient;
    float specular;
    float diffuse;
    
    if (isTextured) {
        Texture texture;
    }
    ExtensionChunk extension;
    
};

struct MaterialList {
    ChunkHeader header;
    ChunkHeader structInfo;
    u32 materialCount;
    u8 data[materialCount * sizeof(u32)];
    Material materials[materialCount];
};

struct Geometry {
    ChunkHeader header;
        ChunkHeader geometryStructInfo;
            GeometryFormat2 format;
            s32 numTriangles;
            s32 numVertices;
            s32 numMorphTargets;
            if (header.version < 0x34000) {
                float ambient;
                float specular;
                float diffuse;
            }
            if (format.rpGEOMETRYNATIVE == 0) {
                if (format.rpGEOMETRYPRELIT) {
                    RwRGBA   prelitcolor[numVertices];
                }
                if (format.numTexSets) {
                    RwTexCoords  texCoords[numVertices];
                }
                
                RpTriangle triangles[numTriangles];
            }
            
            Morphs<numVertices> morphs[numMorphTargets];
            
        MaterialList materialList;
};

struct GeometryList {
    ChunkHeader header;
    ChunkHeader structInfo;
    u32 geometryCount;
    Geometry geometries[1];
};

struct RpcFile {
    Chunk root; // clump
        ChunkHeader clump_struct; //
            Clump clump;
        FrameList frameList;
        GeometryList geometryList; //
};
RpcFile file@0x0;






