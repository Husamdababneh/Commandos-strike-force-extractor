struct PakHeader {
	char Tag[4];
	u32 IDK;
	u32 Version;
	u32 ResNum;
};

struct Resource { // todo
	char fn [];
	u32 roff;
	u32 size;
	u32 v1;
	u32 v2;
};

struct pacFile {
	PakHeader header;
	Resource  resources[header.ResNum];
	
};
pacFile file @ 0x00;