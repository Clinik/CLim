#pragma OPENCL EXTENSION cl_amd_printf:enable
//#include "CLim.hcl"

typedef struct CLim {
	__global const unsigned int* _data;
	unsigned int _width;
	unsigned int _height;

} CLim;

void init(CLim* image, __global const unsigned int* data, int width, int height) {

	image->_data = data;
	image->_width = width;
	image->_height = height;

	printf((__constant char *)"Kernel1 init: w(%u), h(%u), d(%u) \n", 
			width, height, data[1]);
		
}
