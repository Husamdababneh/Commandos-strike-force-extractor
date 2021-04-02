struct rpcHeader {
	u32 str_id; // if the is not 16 for some reason, it's not good
	padding [5 * 4];
	u32 num_models;
	padding [8 * 4];
	u32 num_objects;
};

struct Objects { // todo
	float trans[12];
	u32 parent;
	padding[4];
};

struct Objects2 { // todo
	padding[12];
	u32 section_type; // this must equal 4294967040 (1E 01 00 00 / 1F 01 00 00)
	padding[12];
	u32 hasName;
	if (hasName == 1){
		u32 id;
		u32 name_length;
		if (nameLength > 0){
			char name[];
		}
	}
};

struct vertex {
	float X;
	float Y;
	float Z;
};

struct texcoords {
	float U;
	float V;
};

struct face {
	u16 index1;
	u16 index2;
	u16 faceMat;
	u16 index3;
};

struct Texture {
	padding[4];
	u32 section_size;
	padding[76];
	u32 num_texlong;
	padding[4];
	char texture_name[num_texlong];
	padding[section_size - num_texlong - 80];
};

struct Model { // todo
	u32 idk;
	u32 section_size;
	padding[16];
	u16 unknown1;
	u16 hasUV;
	if (hasUV == 0){ // skip model
		padding[section_size - 16];
	}
	u32 num_faces;
	u32 num_vertices;
	padding[4];
	texcoords UVs[num_vertices];
	face      faces[num_faces];
	padding[24];
	vertex vertices[num_vertices];
	vertex normals[num_vertices];
	padding[24];
	u32 num_texture;
	padding[4 * num_texture];
	Texture textures[num_texture];
	u32 ppadding;
	u32 section_size2;
	padding[section_size2 + 4];
	
};

struct rpcFile {
	rpcHeader header;
	if (header.num_objects > 0) {
		Objects objects[header.num_objects];
		Objects2 o2[header.num_objects];
		padding[28]; // i think this is a wrong place ?? but let's try
	}
	if (header.num_models > 0){
		Model models[header.num_models];
	}
};
rpcFile file @ 0x00;
