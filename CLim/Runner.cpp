#include <stdio.h>
#include <iostream>
#include <limits.h>

#define clim_converters "CLimConverters.cpp"
#include "CLim.h"

template<typename T>
void toGrayScale(const CLim<T>& src, CLim<T>& dst)
{
	for (int i = 0; i < src._width; ++i)
	{
		for (int j = 0; j < src._height; ++j)
		{
			T r = src(i, j, 0);
			T g = src(i, j, 1);
			T b = src(i, j, 2);

			dst(i, j, 0) = round(0.299*((double)r) + 0.587*((double)g) + 0.114*((double)b));
			std::cout << dst(i, j, 0) << std::endl;
		}
	}

}

int main() {
	//unsigned char
	CLim<unsigned char> image1(1, 1, 3);
	//image1.set_all(100);
	image1(0, 0, 0) = (unsigned char)50;
	image1(0, 0, 1) = (unsigned char)100;
	image1(0, 0, 2) = (unsigned char)200;

	CLim<unsigned char> image2 = image1;
	CLim<unsigned char> image3(1, 1, 1);
	
	toGrayScale(image1, image3);

	image1.print();
	std::cout << image1.size() << std::endl;
	image2.print();
	std::cout << image2.size() << std::endl;
	image3.print();
	std::cout << image3.size() << std::endl;

	
	int nn;
	std::cin >> nn;
}