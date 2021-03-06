#ifndef CLIM_H
#define  CLIM_H

#define for_each_pixel(clim,px,type) \
for (type *px = (clim)._data; px < (clim)._data + (clim).size(); ++px)

#define STB_IMAGE_IMPLEMENTATION
#include "IO/stb_image.h"

namespace clim {

template<typename T = float>
struct CLim
{
	unsigned int _width, _height, _channels;

	T *_data;

public:
	CLim() : _width(0), _height(0), _channels(0), _data(0) {};

#ifdef clim_converters
#include clim_converters
#endif
	
	CLim(const unsigned int w, const unsigned int h = 1, const unsigned int c = 1) {

		const unsigned long dataSize = (unsigned long) w * h * c;
		if (dataSize)
		{
			_width = w; _height = h; _channels = c;
			try {
				_data = new T[dataSize];
			}
			catch (...) {
				// TODO
				// Exception
			}
		}
		else {
			*this = CLim();
		}
	}

	CLim<T>& operator=(const CLim<T>& im){
		return assign(im._data, im._width, im._height, im._channels);
	}

	CLim<T>& assign(const T* data, const unsigned int w, const unsigned int h, const unsigned int c) {
		const unsigned long dataSize = (unsigned long)w * h * c;

		try {
			_data = new T[dataSize];
			_width = w; _height = h; _channels = c;
			std::memcpy(_data, data, dataSize*sizeof(T));
		}
		catch (...) {
			// TODO
			// Exception
		}
		return *this;
	}

	T& operator()(const unsigned int x, const unsigned int y) {
		const unsigned long o = offset(x, y, c);
		return _data[o];
	}

	const T& operator()(const unsigned int x, const unsigned int y) const {
		const unsigned long o = offset(x, y, c);
		return _data[o];
	}

	T& operator()(const unsigned int x, const unsigned int y, const unsigned int c) {
		const unsigned long o = offset(x, y, c);
		if (o < size())
		{
			return _data[o];
		}
		else {
			// TODO
			// OUTofBounds
		}
	}

	const T& operator()(const unsigned int x, const unsigned int y, const unsigned int c) const {
		const unsigned long o = offset(x, y, c);
		if (o < size())
		{
			return _data[o];
		}
		else {
			// TODO
			// OUTofBounds
		}
	}
	
	T& get(const unsigned int x, const unsigned int y, const unsigned int c) const {
		const unsigned long o = offset(x, y, c);
		if (o < size())
		{
			return _data[o];
		}
		else {
			// TODO
			// OUTofBounds
		}
	}

	unsigned long offset(const unsigned int x, const unsigned int y, const unsigned int c = 0) const {
		//return x + y * (long)_width + c * (long)_width * (long)_height;
		return (x + y * _width) * _channels + c;
	}

	unsigned int size() const {
		return _width * _height * _channels;
	}

	void set_all(const T value) {
		for_each_pixel(*this, px, T) \
			*px = value;
	}

	// TODO
	// Remove this is only for testing
	void print() {
		for_each_pixel(*this, px, T) \
			std::cout << (int)*px << " ";
	}
	
	void load_from_file(const std::string &fileName) {

		int width, height, elementCount;
		FILE *file = fopen(fileName.c_str(), "rb");
		if (!file)
			//std::cout << "File not found" << std::endl;
			throw 20;
			// TODO filenotfound exception

		unsigned char* data = stbi_load_from_file(file, &width, &height, &elementCount, 3);

		assign(data, width, height, 3);
		
		fclose(file);
	}
	
	~CLim() {
		std::cout << "destroy" << std::endl;
	}

};

}

#endif