#ifdef CLIM_H

#ifndef plugin_converters
#define plugin_converters

#include <iostream>

CLim<T> toGrayScale()
{
	CLim<T> gray(_width, _height, 1);
	for (int i = 0; i < (*this)._width; ++i)
	{
		for (int j = 0; j < (*this)._height; ++j)
		{
			T r = src(i, j, 0);
			T g = src(i, j, 1);
			T b = src(i, j, 2);

			_data(i, j, 0) = round(0.299*((double)r) + 0.587*((double)g) + 0.114*((double)b));
		}
	}

}
	
#endif
#endif