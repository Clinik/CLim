#ifndef CLIM_H
#define  CLIM_H

#include <string>

#include "stbFile.h"

typedef unsigned int uint;
typedef unsigned char uchar;

#define for_each_pixel(clim,px,type) \
for (type *px = (clim)._data; px < (clim)._data + (clim).size(); ++px)

namespace clim {
	template<typename T = float>
	class CLim
	{
	public:
		unsigned int _width, _height, _channels;

		T *_data;

		CLim() : _width(0), _height(0), _channels(0), _data(0) {};

#ifdef clim_converters
#include clim_converters
#endif

		CLim(std::string &fileName) {
			load(fileName);
		}

		CLim(const unsigned int w, const unsigned int h = 1, const unsigned int c = 1) {

			const unsigned long dataSize = (unsigned long)w * h * c;
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

		void allocData(int w, int h, int c) {
			const unsigned long dataSize = (unsigned long)w * h * c;

			try {
				_data = new T[dataSize];
				_width = w; _height = h; _channels = c;
			}
			catch (...) {
				// TODO
				// Exception
			}
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

		// Remove this is only for testing
		void print() {
			for_each_pixel(*this, px, T)
				std::cout << (int)*px << " ";
		}

		void load(std::string &fileName) {
			stbFile f;
			f.load_from_file(fileName);
			allocData(f.width, f.height, 3);
			std::copy(f.data, f.data + size(), _data);
		}

		void save() {
			stbFile f;
		}

		~CLim() {
		}

	};
}

#endif