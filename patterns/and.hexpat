struct Str {
    u32 size;
    char string[size];
    if (size > 0)
        padding[2]; 
};

Str ss[505] @ 0x0;