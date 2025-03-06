/* ========================================================================
   $File: rpc.h
   $Date: 2025-02-09
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/
#pragma once

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t b8;
typedef int32_t b32;

typedef float  f32;
typedef double  f64;

typedef float  float32;
typedef double float64;

// : u32
enum ChunkType  {
    STRUCT										= 0x00000001, // Core: A generic section that stores data for its parent.
    STRING										= 0x00000002, // Core: Stores a 4-byte aligned ASCII string.
    EXTENSION									= 0x00000003, // Core: A container for non-standard extensions of its parent section.
    CAMERA										= 0x00000005, // Core: Contains a camera (unused in GTA games).
    TEXTURE										= 0x00000006, // Core: Stores the sampler state of a texture.
    MATERIAL									= 0x00000007, // Core: Defines a material to be used on a geometry.
    MATERIAL_LIST								= 0x00000008, // Core: Container for a list of materials.
    ATOMIC_SECTION								= 0x00000009, // Core: 
    PLANE_SECTION								= 0x0000000A, // Core: 
    WORLD										= 0x0000000B, // Core: The root section of the level geometry.
    SPLINE										= 0x0000000C, // Core: 
    MATRIX										= 0x0000000D, // Core: 
    FRAME_LIST									= 0x0000000E, // Core: Container for a list of frames. A frame holds the transformation that is applied to an Atomic.
    GEOMETRY									= 0x0000000F, // Core: A platform-independent container for meshes.
    CLUMP										= 0x00000010, // Core: The root section for a 3D model.
    LIGHT										= 0x00000012, // Core: Stores different dynamic lights.
    UNICODE_STRING								= 0x00000013, // Core: 
    ATOMIC										= 0x00000014, // Core: Defines the basic unit for the RenderWare graphics pipeline. Generally speaking, an Atomic can be directly converted to a single draw call.
    RASTER										= 0x00000015, // Core: Stores a platform-dependent (i.e. native) texture image.
    TEXTURE_DICTIONARY							= 0x00000016, // Core: A container for texture images (also called raster).
    ANIMATION_DATABASE							= 0x00000017, // Core: 
    IMAGE										= 0x00000018, // Core: An individual texture image.
    SKIN_ANIMATION								= 0x00000019, // Core: 
    GEOMETRY_LIST								= 0x0000001A, // Core: A container for a list of geometries.
    ANIM_ANIMATION								= 0x0000001B, // Core: 
    TEAM										= 0x0000001C, // Core: 
    CROWD										= 0x0000001D, // Core: 
    DELTA_MORPH_ANIMATION						= 0x0000001E, // Core: 
    RIGHT_TO_RENDER								= 0x0000001F, // Core: Stores the render pipeline the engine uses to draw an atomic or material.
    MULTI_TEXTURE_EFFECT_NATIVE					= 0x00000020, // Core: 
    MULTI_TEXTURE_EFFECT_DICTIONARY				= 0x00000021, // Core: 
    TEAM_DICTIONARY								= 0x00000022, // Core: 
    PLATFORM_INDEPENDENT_TEXTURE_DICTIONARY		= 0x00000023, // Core: 
    TABLE_OF_CONTENTS							= 0x00000024, // Core: 
    PARTICLE_STANDARD_GLOBAL_DATA				= 0x00000025, // Core: 
    ALTPIPE										= 0x00000026, // Core: 
    PLATFORM_INDEPENDENT_PEDS					= 0x00000027, // Core: 
    PATCH_MESH									= 0x00000028, // Core: 
    CHUNK_GROUP_START							= 0x00000029, // Core: 
    CHUNK_GROUP_END								= 0x0000002A, // Core: 
    UV_ANIMATION_DICTIONARY						= 0x0000002B, // Core: 
    COLL_TREE									= 0x0000002C, // Core: 
    METRICS_PLG									= 0x00000101, // Toolkit: 
    SPLINE_PLG									= 0x00000102, // Toolkit: 
    STEREO_PLG									= 0x00000103, // Toolkit: 
    VRML_PLG									= 0x00000104, // Toolkit: 
    MORPH_PLG									= 0x00000105, // Toolkit: 
    PVS_PLG										= 0x00000106, // Toolkit: 
    MEMORY_LEAK_PLG								= 0x00000107, // Toolkit: 
    ANIMATION_PLG								= 0x00000108, // Toolkit: 
    GLOSS_PLG									= 0x00000109, // Toolkit: 
    LOGO_PLG									= 0x0000010A, // Toolkit: 
    MEMORY_INFO_PLG								= 0x0000010B, // Toolkit: 
    RANDOM_PLG									= 0x0000010C, // Toolkit: 
    PNG_IMAGE_PLG								= 0x0000010D, // Toolkit: 
    BONE_PLG									= 0x0000010E, // Toolkit: 
    VRML_ANIM_PLG								= 0x0000010F, // Toolkit: 
    SKY_MIPMAP_VAL								= 0x00000110, // Toolkit: Stores MipMap parameters for the PS2 version of the engine (codenamed Sky).
    MRM_PLG										= 0x00000111, // Toolkit: 
    LOD_ATOMIC_PLG								= 0x00000112, // Toolkit: 
    ME_PLG										= 0x00000113, // Toolkit: 
    LIGHTMAP_PLG								= 0x00000114, // Toolkit: 
    REFINE_PLG									= 0x00000115, // Toolkit: 
    SKIN_PLG									= 0x00000116, // Toolkit: 
    LABEL_PLG									= 0x00000117, // Toolkit: 
    PARTICLES_PLG								= 0x00000118, // Toolkit: 
    GEOMTX_PLG									= 0x00000119, // Toolkit: 
    SYNTH_CORE_PLG								= 0x0000011A, // Toolkit: 
    STQPP_PLG									= 0x0000011B, // Toolkit: 
    PART_PP_PLG									= 0x0000011C, // Toolkit: 
    COLLISION_PLG								= 0x0000011D, // Toolkit: 
    HANIM_PLG									= 0x0000011E, // Toolkit: 
    USER_DATA_PLG								= 0x0000011F, // Toolkit: 
    MATERIAL_EFFECTS_PLG						= 0x00000120, // Toolkit: 
    PARTICLE_SYSTEM_PLG							= 0x00000121, // Toolkit: 
    DELTA_MORPH_PLG								= 0x00000122, // Toolkit: 
    PATCH_PLG									= 0x00000123, // Toolkit: 
    TEAM_PLG									= 0x00000124, // Toolkit: 
    CROWD_PP_PLG								= 0x00000125, // Toolkit: 
    MIP_SPLIT_PLG								= 0x00000126, // Toolkit: 
    ANISOTROPY_PLG								= 0x00000127, // Toolkit: Stores the anisotropy for a texture filter.
    GCN_MATERIAL_PLG							= 0x00000129, // Toolkit: 
    GEOMETRIC_PVS_PLG							= 0x0000012A, // Toolkit: 
    XBOX_MATERIAL_PLG							= 0x0000012B, // Toolkit: 
    MULTI_TEXTURE_PLG							= 0x0000012C, // Toolkit: 
    CHAIN_PLG									= 0x0000012D, // Toolkit: 
    TOON_PLG									= 0x0000012E, // Toolkit: 
    PTANK_PLG									= 0x0000012F, // Toolkit: 
    PARTICLE_STANDARD_PLG						= 0x00000130, // Toolkit: 
    PDS_PLG										= 0x00000131, // Toolkit: 
    PRTADV_PLG									= 0x00000132, // Toolkit: 
    NORMAL_MAP_PLG								= 0x00000133, // Toolkit: 
    ADC_PLG										= 0x00000134, // Toolkit: 
    UV_ANIMATION_PLG							= 0x00000135, // Toolkit: 
    CHARACTER_SET_PLG							= 0x00000180, // Toolkit: 
    NOHS_WORLD_PLG								= 0x00000181, // Toolkit: 
    IMPORT_UTIL_PLG								= 0x00000182, // Toolkit: 
    SLERP_PLG									= 0x00000183, // Toolkit: 
    OPTIM_PLG									= 0x00000184, // Toolkit: 
    TL_WORLD_PLG								= 0x00000185, // Toolkit: 
    DATABASE_PLG								= 0x00000186, // Toolkit: 
    RAYTRACE_PLG								= 0x00000187, // Toolkit: 
    RAY_PLG										= 0x00000188, // Toolkit: 
    LIBRARY_PLG									= 0x00000189, // Toolkit: 
    PLG_2D										= 0x00000190, // Toolkit: 
    TILE_RENDER_PLG								= 0x00000191, // Toolkit: 
    JPEG_IMAGE_PLG								= 0x00000192, // Toolkit: 
    TGA_IMAGE_PLG								= 0x00000193, // Toolkit: 
    GIF_IMAGE_PLG								= 0x00000194, // Toolkit: 
    QUAT_PLG									= 0x00000195, // Toolkit: 
    SPLINE_PVS_PLG								= 0x00000196, // Toolkit: 
    MIPMAP_PLG									= 0x00000197, // Toolkit: 
    MIPMAPK_PLG									= 0x00000198, // Toolkit: 
    FONT_2D										= 0x00000199, // Toolkit: 
    INTERSECTION_PLG							= 0x0000019A, // Toolkit: 
    TIFF_IMAGE_PLG								= 0x0000019B, // Toolkit: 
    PICK_PLG									= 0x0000019C, // Toolkit: 
    BMP_IMAGE_PLG								= 0x0000019D, // Toolkit: 
    RAS_IMAGE_PLG								= 0x0000019E, // Toolkit: 
    SKIN_FX_PLG									= 0x0000019F, // Toolkit: 
    VCAT_PLG									= 0x000001A0, // Toolkit: 
    PATH_2D										= 0x000001A1, // Toolkit: 
    BRUSH_2D									= 0x000001A2, // Toolkit: 
    OBJECT_2D									= 0x000001A3, // Toolkit: 
    SHAPE_2D									= 0x000001A4, // Toolkit: 
    SCENE_2D									= 0x000001A5, // Toolkit: 
    PICK_REGION_2D								= 0x000001A6, // Toolkit: 
    OBJECT_STRING_2D							= 0x000001A7, // Toolkit: 
    ANIMATION_PLG_2D							= 0x000001A8, // Toolkit: 
    ANIMATION_2D								= 0x000001A9, // Toolkit: 
    KEYFRAME_2D									= 0x000001B0, // Toolkit: 
    MAESTRO_2D									= 0x000001B1, // Toolkit: 
    BARYCENTRIC									= 0x000001B2, // Toolkit: 
    PLATFORM_INDEPENDENT_TEXTURE_DICTIONARY_TK	= 0x000001B3, // Toolkit: 
    TOC_TK										= 0x000001B4, // Toolkit: 
    TPL_TK										= 0x000001B5, // Toolkit: 
    ALTPIPE_TK									= 0x000001B6, // Toolkit: 
    ANIMATION_TK								= 0x000001B7, // Toolkit: 
    SKIN_SPLIT_TOOKIT							= 0x000001B8, // Toolkit: 
    COMPRESSED_KEY_TK							= 0x000001B9, // Toolkit: 
    GEOMETRY_CONDITIONING_PLG					= 0x000001BA, // Toolkit: 
    WING_PLG									= 0x000001BB, // Toolkit: 
    GENERIC_PIPELINE_TK							= 0x000001BC, // Toolkit: 
    LIGHTMAP_CONVERSION_TK						= 0x000001BD, // Toolkit: 
    FILESYSTEM_PLG								= 0x000001BE, // Toolkit: 
    DICTIONARY_TK								= 0x000001BF, // Toolkit: 
    UV_ANIMATION_LINEAR							= 0x000001C0, // Toolkit: 
    UV_ANIMATION_PARAMETER						= 0x000001C1, // Toolkit: 
    BIN_MESH_PLG								= 0x0000050E, // World: 
    NATIVE_DATA_PLG								= 0x00000510, // World: 
    EARS_MATERIAL_DATA							= 0x0000EA13, // EA Redwood Shores: 
    EA_REDWOOD_SHORES_1							= 0x0000EA15, // EA Redwood Shores: 
    EARS_MESH_PLUGIN							= 0x0000EA16, // EA Redwood Shores: 
    EA_REDWOOD_SHORES_2							= 0x0000EA20, // EA Redwood Shores: 
    EARS_ZONE_PLUGIN							= 0x0000EA28, // EA Redwood Shores: 
    EARS_LT_MAP_2								= 0x0000EA2D, // EA Redwood Shores: 
    EARS_RP_PARTIAL_INSTANCE					= 0x0000EA2E, // EA Redwood Shores: 
    EARS_TEXTURE_PLUGIN							= 0x0000EA2F, // EA Redwood Shores: 
    EARS_MESH									= 0x0000EA33, // EA Redwood Shores: Stores mesh data.
    EARS_ATOMIC_PLUGIN							= 0x0000EA40, // EA Redwood Shores: 
    EARS_DISPLAY_LIST							= 0x0000EA44, // EA Redwood Shores: 
    EARS_RP_SHADER								= 0x0000EA45, // EA Redwood Shores: 
    EARS_RP_ALCHEMY								= 0x0000EA80, // EA Redwood Shores: 
    EARS_MORPH_TARGET_DATA						= 0x0000EA92, // EA Redwood Shores: 
    ZMODELER_LOCK								= 0x0000F21E, // ZModeler: Unofficial extension that stores a password that protects the file from being modified when opened in ZModeler. Ignored by other applications like RWAnalyze and the GTA games.
    THQ_ATOMIC									= 0x00CAFE40, // THQ: Jimmy Neutron: Attack of the Twonkies custom. Atomic rendering flags.
    THQ_MATERIAL								= 0x00CAFE45, // THQ: Jimmy Neutron: Attack of the Twonkies custom. Extended material parameters.
    ATOMIC_VISIBILITY_DISTANCE					= 0x0253F200, // Rockstar: 
    CLUMP_VISIBILITY_DISTANCE					= 0x0253F201, // Rockstar: 
    FRAME_VISIBILITY_DISTANCE					= 0x0253F202, // Rockstar: 
    PIPELINE_SET								= 0x0253F2F3, // Rockstar: Stores the render pipeline used to draw objects with Rockstar-specific plug-ins.
    TEXDICTIONARY_LINK							= 0x0253F2F5, // Rockstar: 
    SPECULAR_MATERIAL							= 0x0253F2F6, // Rockstar: Stores a specularity map.
    EFFECT_2D									= 0x0253F2F8, // Rockstar: Used to attach various GTA-specific effects to models, for example to enable script interaction or particle effects.
    EXTRA_VERT_COLOR							= 0x0253F2F9, // Rockstar: Stores an additional array of vertex colors, that are used in GTA during night-time to simulate some effects of dynamic global lighting.
    COLLISION_MODEL								= 0x0253F2FA, // Rockstar: Stores a collision model.
    GTA_HANIM									= 0x0253F2FB, // Rockstar: 
    REFLECTION_MATERIAL							= 0x0253F2FC, // Rockstar: Enables advanced environment mapping.
    BREAKABLE									= 0x0253F2FD, // Rockstar: Contains a mesh that is used to render objects that are breakable (like windows or tables).
    FRAME_NAME									= 0x0253F2FE, // Rockstar: Stores the name of a frame within a Frame List.
};

static_assert(sizeof(ChunkType) == 4);

static
const char* GetChunkTypeName(ChunkType chunkType) {
    switch (chunkType) {
        case STRUCT: return "STRUCT";
        case STRING: return "STRING";
        case EXTENSION: return "EXTENSION";
        case CAMERA: return "CAMERA";
        case TEXTURE: return "TEXTURE";
        case MATERIAL: return "MATERIAL";
        case MATERIAL_LIST: return "MATERIAL_LIST";
        case ATOMIC_SECTION: return "ATOMIC_SECTION";
        case PLANE_SECTION: return "PLANE_SECTION";
        case WORLD: return "WORLD";
        case SPLINE: return "SPLINE";
        case MATRIX: return "MATRIX";
        case FRAME_LIST: return "FRAME_LIST";
        case GEOMETRY: return "GEOMETRY";
        case CLUMP: return "CLUMP";
        case LIGHT: return "LIGHT";
        case UNICODE_STRING: return "UNICODE_STRING";
        case ATOMIC: return "ATOMIC";
        case RASTER: return "RASTER";
        case TEXTURE_DICTIONARY: return "TEXTURE_DICTIONARY";
        case ANIMATION_DATABASE: return "ANIMATION_DATABASE";
        case IMAGE: return "IMAGE";
        case SKIN_ANIMATION: return "SKIN_ANIMATION";
        case GEOMETRY_LIST: return "GEOMETRY_LIST";
        case ANIM_ANIMATION: return "ANIM_ANIMATION";
        case TEAM: return "TEAM";
        case CROWD: return "CROWD";
        case DELTA_MORPH_ANIMATION: return "DELTA_MORPH_ANIMATION";
        case RIGHT_TO_RENDER: return "RIGHT_TO_RENDER";
        case MULTI_TEXTURE_EFFECT_NATIVE: return "MULTI_TEXTURE_EFFECT_NATIVE";
        case MULTI_TEXTURE_EFFECT_DICTIONARY: return "MULTI_TEXTURE_EFFECT_DICTIONARY";
        case TEAM_DICTIONARY: return "TEAM_DICTIONARY";
        case PLATFORM_INDEPENDENT_TEXTURE_DICTIONARY: return "PLATFORM_INDEPENDENT_TEXTURE_DICTIONARY";
        case TABLE_OF_CONTENTS: return "TABLE_OF_CONTENTS";
        case PARTICLE_STANDARD_GLOBAL_DATA: return "PARTICLE_STANDARD_GLOBAL_DATA";
        case ALTPIPE: return "ALTPIPE";
        case PLATFORM_INDEPENDENT_PEDS: return "PLATFORM_INDEPENDENT_PEDS";
        case PATCH_MESH: return "PATCH_MESH";
        case CHUNK_GROUP_START: return "CHUNK_GROUP_START";
        case CHUNK_GROUP_END: return "CHUNK_GROUP_END";
        case UV_ANIMATION_DICTIONARY: return "UV_ANIMATION_DICTIONARY";
        case COLL_TREE: return "COLL_TREE";
        case METRICS_PLG: return "METRICS_PLG";
        case SPLINE_PLG: return "SPLINE_PLG";
        case STEREO_PLG: return "STEREO_PLG";
        case VRML_PLG: return "VRML_PLG";
        case MORPH_PLG: return "MORPH_PLG";
        case PVS_PLG: return "PVS_PLG";
        case MEMORY_LEAK_PLG: return "MEMORY_LEAK_PLG";
        case ANIMATION_PLG: return "ANIMATION_PLG";
        case GLOSS_PLG: return "GLOSS_PLG";
        case LOGO_PLG: return "LOGO_PLG";
        case MEMORY_INFO_PLG: return "MEMORY_INFO_PLG";
        case RANDOM_PLG: return "RANDOM_PLG";
        case PNG_IMAGE_PLG: return "PNG_IMAGE_PLG";
        case BONE_PLG: return "BONE_PLG";
        case VRML_ANIM_PLG: return "VRML_ANIM_PLG";
        case SKY_MIPMAP_VAL: return "SKY_MIPMAP_VAL";
        case MRM_PLG: return "MRM_PLG";
        case LOD_ATOMIC_PLG: return "LOD_ATOMIC_PLG";
        case ME_PLG: return "ME_PLG";
        case LIGHTMAP_PLG: return "LIGHTMAP_PLG";
        case REFINE_PLG: return "REFINE_PLG";
        case SKIN_PLG: return "SKIN_PLG";
        case LABEL_PLG: return "LABEL_PLG";
        case PARTICLES_PLG: return "PARTICLES_PLG";
        case GEOMTX_PLG: return "GEOMTX_PLG";
        case SYNTH_CORE_PLG: return "SYNTH_CORE_PLG";
        case STQPP_PLG: return "STQPP_PLG";
        case PART_PP_PLG: return "PART_PP_PLG";
        case COLLISION_PLG: return "COLLISION_PLG";
        case HANIM_PLG: return "HANIM_PLG";
        case USER_DATA_PLG: return "USER_DATA_PLG";
        case MATERIAL_EFFECTS_PLG: return "MATERIAL_EFFECTS_PLG";
        case PARTICLE_SYSTEM_PLG: return "PARTICLE_SYSTEM_PLG";
        case DELTA_MORPH_PLG: return "DELTA_MORPH_PLG";
        case PATCH_PLG: return "PATCH_PLG";
        case TEAM_PLG: return "TEAM_PLG";
        case CROWD_PP_PLG: return "CROWD_PP_PLG";
        case MIP_SPLIT_PLG: return "MIP_SPLIT_PLG";
        case ANISOTROPY_PLG: return "ANISOTROPY_PLG";
        case GCN_MATERIAL_PLG: return "GCN_MATERIAL_PLG";
        case GEOMETRIC_PVS_PLG: return "GEOMETRIC_PVS_PLG";
        case XBOX_MATERIAL_PLG: return "XBOX_MATERIAL_PLG";
        case MULTI_TEXTURE_PLG: return "MULTI_TEXTURE_PLG";
        case CHAIN_PLG: return "CHAIN_PLG";
        case TOON_PLG: return "TOON_PLG";
        case PTANK_PLG: return "PTANK_PLG";
        case PARTICLE_STANDARD_PLG: return "PARTICLE_STANDARD_PLG";
        case PDS_PLG: return "PDS_PLG";
        case PRTADV_PLG: return "PRTADV_PLG";
        case NORMAL_MAP_PLG: return "NORMAL_MAP_PLG";
        case ADC_PLG: return "ADC_PLG";
        case UV_ANIMATION_PLG: return "UV_ANIMATION_PLG";
        case CHARACTER_SET_PLG: return "CHARACTER_SET_PLG";
        case NOHS_WORLD_PLG: return "NOHS_WORLD_PLG";
        case IMPORT_UTIL_PLG: return "IMPORT_UTIL_PLG";
        case SLERP_PLG: return "SLERP_PLG";
        case OPTIM_PLG: return "OPTIM_PLG";
        case TL_WORLD_PLG: return "TL_WORLD_PLG";
        case DATABASE_PLG: return "DATABASE_PLG";
        case RAYTRACE_PLG: return "RAYTRACE_PLG";
        case RAY_PLG: return "RAY_PLG";
        case LIBRARY_PLG: return "LIBRARY_PLG";
        case PLG_2D: return "PLG_2D";
        case TILE_RENDER_PLG: return "TILE_RENDER_PLG";
        case JPEG_IMAGE_PLG: return "JPEG_IMAGE_PLG";
        case TGA_IMAGE_PLG: return "TGA_IMAGE_PLG";
        case GIF_IMAGE_PLG: return "GIF_IMAGE_PLG";
        case QUAT_PLG: return "QUAT_PLG";
        case SPLINE_PVS_PLG: return "SPLINE_PVS_PLG";
        case MIPMAP_PLG: return "MIPMAP_PLG";
        case MIPMAPK_PLG: return "MIPMAPK_PLG";
        case FONT_2D: return "FONT_2D";
        case INTERSECTION_PLG: return "INTERSECTION_PLG";
        case TIFF_IMAGE_PLG: return "TIFF_IMAGE_PLG";
        case PICK_PLG: return "PICK_PLG";
        case BMP_IMAGE_PLG: return "BMP_IMAGE_PLG";
        case RAS_IMAGE_PLG: return "RAS_IMAGE_PLG";
        case SKIN_FX_PLG: return "SKIN_FX_PLG";
        case VCAT_PLG: return "VCAT_PLG";
        case PATH_2D: return "PATH_2D";
        case BRUSH_2D: return "BRUSH_2D";
        case OBJECT_2D: return "OBJECT_2D";
        case SHAPE_2D: return "SHAPE_2D";
        case SCENE_2D: return "SCENE_2D";
        case PICK_REGION_2D: return "PICK_REGION_2D";
        case OBJECT_STRING_2D: return "OBJECT_STRING_2D";
        case ANIMATION_PLG_2D: return "ANIMATION_PLG_2D";
        case ANIMATION_2D: return "ANIMATION_2D";
        case KEYFRAME_2D: return "KEYFRAME_2D";
        case MAESTRO_2D: return "MAESTRO_2D";
        case BARYCENTRIC: return "BARYCENTRIC";
        case PLATFORM_INDEPENDENT_TEXTURE_DICTIONARY_TK: return "PLATFORM_INDEPENDENT_TEXTURE_DICTIONARY_TK";
        case TOC_TK: return "TOC_TK";
        case TPL_TK: return "TPL_TK";
        case ALTPIPE_TK: return "ALTPIPE_TK";
        case ANIMATION_TK: return "ANIMATION_TK";
        case SKIN_SPLIT_TOOKIT: return "SKIN_SPLIT_TOOKIT";
        case COMPRESSED_KEY_TK: return "COMPRESSED_KEY_TK";
        case GEOMETRY_CONDITIONING_PLG: return "GEOMETRY_CONDITIONING_PLG";
        case WING_PLG: return "WING_PLG";
        case GENERIC_PIPELINE_TK: return "GENERIC_PIPELINE_TK";
        case LIGHTMAP_CONVERSION_TK: return "LIGHTMAP_CONVERSION_TK";
        case FILESYSTEM_PLG: return "FILESYSTEM_PLG";
        case DICTIONARY_TK: return "DICTIONARY_TK";
        case UV_ANIMATION_LINEAR: return "UV_ANIMATION_LINEAR";
        case UV_ANIMATION_PARAMETER: return "UV_ANIMATION_PARAMETER";
        case BIN_MESH_PLG: return "BIN_MESH_PLG";
        case NATIVE_DATA_PLG: return "NATIVE_DATA_PLG";
        case EARS_MATERIAL_DATA: return "EARS_MATERIAL_DATA";
        case EA_REDWOOD_SHORES_1: return "EA_REDWOOD_SHORES_1";
        case EARS_MESH_PLUGIN: return "EARS_MESH_PLUGIN";
        case EA_REDWOOD_SHORES_2: return "EA_REDWOOD_SHORES_2";
        case EARS_ZONE_PLUGIN: return "EARS_ZONE_PLUGIN";
        case EARS_LT_MAP_2: return "EARS_LT_MAP_2";
        case EARS_RP_PARTIAL_INSTANCE: return "EARS_RP_PARTIAL_INSTANCE";
        case EARS_TEXTURE_PLUGIN: return "EARS_TEXTURE_PLUGIN";
        case EARS_MESH: return "EARS_MESH";
        case EARS_ATOMIC_PLUGIN: return "EARS_ATOMIC_PLUGIN";
        case EARS_DISPLAY_LIST: return "EARS_DISPLAY_LIST";
        case EARS_RP_SHADER: return "EARS_RP_SHADER";
        case EARS_RP_ALCHEMY: return "EARS_RP_ALCHEMY";
        case EARS_MORPH_TARGET_DATA: return "EARS_MORPH_TARGET_DATA";
        case ZMODELER_LOCK: return "ZMODELER_LOCK";
        case THQ_ATOMIC: return "THQ_ATOMIC";
        case THQ_MATERIAL: return "THQ_MATERIAL";
        case ATOMIC_VISIBILITY_DISTANCE: return "ATOMIC_VISIBILITY_DISTANCE";
        case CLUMP_VISIBILITY_DISTANCE: return "CLUMP_VISIBILITY_DISTANCE";
        case FRAME_VISIBILITY_DISTANCE: return "FRAME_VISIBILITY_DISTANCE";
        case PIPELINE_SET: return "PIPELINE_SET";
        case TEXDICTIONARY_LINK: return "TEXDICTIONARY_LINK";
        case SPECULAR_MATERIAL: return "SPECULAR_MATERIAL";
        case EFFECT_2D: return "EFFECT_2D";
        case EXTRA_VERT_COLOR: return "EXTRA_VERT_COLOR";
        case COLLISION_MODEL: return "COLLISION_MODEL";
        case GTA_HANIM: return "GTA_HANIM";
        case REFLECTION_MATERIAL: return "REFLECTION_MATERIAL";
        case BREAKABLE: return "BREAKABLE";
        case FRAME_NAME: return "FRAME_NAME";
        default: return "UNKNOWN_CHUNK_TYPE";
    }
};

struct RWHeader {
    ChunkType type;
    u32 size;
    u32 libId;
};


struct RwRGBA {
    u8 r,g,b,a;
};


struct RwV3d {
    f32 x, y, z;
};

struct RpTriangle {
    u16 vertex1, vertex2, materialId, vertex3;
};

struct RwSphere {
    f32 x, y, z, radius; 
};

struct RwTexCoords {
    f32 u,v;
};

template<typename T> 
struct SizedChunk {
    u32 size;
    T* data;
};

using StringChunk = SizedChunk<char>;
using ExtensionChunk = SizedChunk<u8>;


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
    s32        frame_count;
    FrameData* frame_data;
};


enum RwTextureFilterMode {
    TEXTUREFILTER_NAFILTERMODE = 0,      // Filtering disabled
    TEXTUREFILTER_NEAREST = 1,           // Point-sampled
    TEXTUREFILTER_LINEAR = 2,            // Bilinear filtering
    TEXTUREFILTER_MIPNEAREST = 3,        // Nearest mipmap (point-sampled per pixel)
    TEXTUREFILTER_MIPLINEAR = 4,         // Bilinear per-pixel mipmap
    TEXTUREFILTER_LINEARMIPNEAREST = 5,  // Mipmap interpolation (point-sampled)
    TEXTUREFILTER_LINEARMIPLINEAR = 6    // Trilinear filtering
};

enum RwTextureAddressMode {
    TEXTUREADDRESS_NATEXTUREADDRESS = 0, // No tiling (undefined behavior)
    TEXTUREADDRESS_WRAP = 1,             // Repeat (Tile in U/V direction)
    TEXTUREADDRESS_MIRROR = 2,           // Mirror in U/V direction
    TEXTUREADDRESS_CLAMP = 3,            // Clamp to edge
    TEXTUREADDRESS_BORDER = 4            // Clamp to border color
};



struct TextureInfo {
    union {
        u32 info; // 4 bytes
        struct {
            u8 filtering        : 8;
            u8 u                : 4;
            u8 v                : 4;
            u8 isUsingMipLevels : 1;
        };
    };
};

static_assert(sizeof(TextureInfo) == 4 && "This should be of size 4 bytes");

struct Texture {
    TextureInfo info;
    StringChunk textureName;
    StringChunk alphaLayerName;
    ExtensionChunk* extensions;
};

struct Material {
    s32 flags; // unsued
    RwRGBA color;
    s32 unused;
    s32 isTextured;
    float ambient;
    float specular;
    float diffuse;
    
    // if isTextured {
    Texture texture;
};

struct MaterialList {
    u32 materialCount;
    u8* materialIndecies;  //[materialCount * sizeof(u32)];
    Material* materials;// [materialCount];
};

struct Morph {
    RwSphere boundingSphere;
    u32 hasVertices;
    u32 hasNormals;
    // if hasVertices
    RwV3d vertices;
    // if hasNormals
    RwV3d* normals;
};

struct GeometryFormat {
    b8 rpGEOMETRYTRISTRIP              : 1; // Is triangle strip (if disabled, it will be a triangle list)
    b8 rpGEOMETRYPOSITIONS             : 1; // Vertex translation
    b8 rpGEOMETRYTEXTURED              : 1; // Texture coordinates
    b8 rpGEOMETRYPRELIT                : 1; // Vertex colors
    b8 rpGEOMETRYNORMALS               : 1; // Store normals
    b8 rpGEOMETRYLIGHT                 : 1; // Geometry is lit (dynamic and static)
    b8 rpGEOMETRYMODULATEMATERIALCOLOR : 1; // Modulate material color
    b8 rpGEOMETRYTEXTURED2             : 1; // Texture coordinates 2
    u8 unused                          : 8; // Unused/reserved bits
    u8 numTexSets                      : 8; // Texture count
    u8 unused2                         : 7; // Unused/reserved bits
    b8 rpGEOMETRYNATIVE                : 1; // Native Geometry
};
static_assert(sizeof(GeometryFormat) == 4, "GeometryFormat must only be of size 4");


static
void printGeometryFormat(GeometryFormat gf) {
    printf("{ ");
    if (gf.rpGEOMETRYTRISTRIP)              printf("TRISTRIP | ");
    if (gf.rpGEOMETRYPOSITIONS)             printf("POSITIONS | ");
    if (gf.rpGEOMETRYTEXTURED)              printf("TEXTURED | ");
    if (gf.rpGEOMETRYPRELIT)                printf("PRELIT | ");
    if (gf.rpGEOMETRYNORMALS)               printf("NORMALS | ");
    if (gf.rpGEOMETRYLIGHT)                 printf("LIGHT | ");
    if (gf.rpGEOMETRYMODULATEMATERIALCOLOR) printf("MODULATEMATERIALCOLOR | ");
    if (gf.rpGEOMETRYTEXTURED2)             printf("TEXTURED2 | ");
    if (gf.rpGEOMETRYNATIVE)                printf("NATIVE | ");
    printf("numTexSets(%u) }", gf.numTexSets);
}

struct Geometry {
    GeometryFormat format;
    s32 numTriangles;
    s32 numVertices;
    s32 numMorphTargets;

    // if Version < 0x34000
    float ambient;
    float specular;
    float diffuse;

    RwRGBA*       prelitcolor;// [numVertices];
    RwTexCoords*  texCoords;  // [numVertices];
    RpTriangle*   triangles;  // [numTriangles];

    Morph* morphs;// [numMorphTargets]
    MaterialList materialList;
};

struct GeometryList {
    u32 geometryCount;
    Geometry* geometries;
};


struct Atomic {
    unsigned int frameIndex;     // Index of the frame within the clump's frame list.
    unsigned int geometryIndex;  // Index of geometry within the clump's geometry list.
    unsigned int flags;          // See below.
    unsigned int unused;         // Typically 0.
};






