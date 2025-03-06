struct Header {
 u32 sig;
 u32 version; // ??
 u32 resNumber; // ?? 
// u32 idk;
};


struct ResName
{
    u32 res1NameSize;
    char res1Name[res1NameSize];

};

struct Resources {
    u32 idk;
    ResName reses[2];
    u8 u[36];
};

struct File {
    Header header;
    Resources resources[header.resNumber];
};

File file @ 0x0;