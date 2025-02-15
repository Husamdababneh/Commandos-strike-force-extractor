/* ========================================================================
   $File: raw.cpp
   $Date: 2023-06-29
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"

struct Vec3 {
    float32 r,g,b;
};

struct RawImage {
    u32 width, height;
    Vec3* data; // this with be width*height
};



void convertRaw(const char *filepath)
{
	void* filedataD;
	if (!read_entire_file(filepath, &filedataD)){
		printf("Couldn't read %.s", filepath);
		exit(-1);
	}

	u8* filedata = (u8*)filedataD;
    
    RawImage image;
    image.width = *(filedata + 0);
    image.height = *(filedata + 4);
    image.data = (Vec3*)filedata + 8;

    u64 channels = 3;
    u64 stride = channels * image.width;
    
    
    int test =  stbi_write_png("test.png",
                               image.width,
                               image.height,
                               channels,
                               image.data,
                               stride);

}
