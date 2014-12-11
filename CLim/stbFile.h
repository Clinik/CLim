#ifndef STBFILE_H
#define STBFILE_H

#include <string>
#include "IO/stb_image_write.h"

class stbFile {
public:
	stbFile() {

	}
	int width, height;
	unsigned char* data;

	void load_from_file(std::string &fileName);

	//template<typename T>
	//void write_to_file(std::string &fileName, int w, int h, int c, T *d);
	template<typename T>
	void write_to_file(std::string &fileName, int w, int h, int c, T *d) {
		stbi_write_png(fileName.c_str(), w, h, c, d, w*3);
	}
};

#endif