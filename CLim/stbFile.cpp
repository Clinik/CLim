#include "stbFile.h"
#include <iostream>
//#include <fstream>
#include <string>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "IO/stb_image.h"
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "IO/stb_image_write.h"
#endif

void stbFile::load_from_file(std::string &fileName) {
	//int width, height, elementCount;
	int elementCount;

	FILE *file = fopen(fileName.c_str(), "rb");
	
	if (file == NULL) {
		perror(fileName.c_str());
		printf("Error %d \n", errno);
	}
	data = stbi_load_from_file(file, &width, &height, &elementCount, 3);
	
	fclose(file);
}

