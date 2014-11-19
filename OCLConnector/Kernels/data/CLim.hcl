#pragma OPENCL EXTENSION cl_amd_printf:enable
//#pragma OPENCL EXTENSION cl_intel_printf :enable

//#include "CLim.hcl"

typedef struct CLim {
	__global unsigned int* _data;
	unsigned int _width;
	unsigned int _height;

} CLim;

void init(CLim* image, __global unsigned int* data, int width, int height) {

	image->_data = data;
	image->_width = width;
	image->_height = height;

	printf((__constant char *)"Image init: w(%u), h(%u), d(%u) \n", 
			width, height, data[1]);
		
}