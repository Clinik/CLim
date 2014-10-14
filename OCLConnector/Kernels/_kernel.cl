#define STRINGIFY(A) #A

std::string kernel1_source = STRINGIFY(

#pragma OPENCL EXTENSION cl_amd_printf:enable \n

	typedef struct {
		unsigned char* _data;
		unsigned int _width;
		unsigned int _height;
	} CLim;

	/*void init(__global const unsigned char* data, int width, int height) {

		*_data = data;
		_width = width;
		_height = height;

		printf((__constant char *)"Kernel1 init: w(%u), h(%u) \n", _width, _height);
		
	}*/
	

__kernel void kernel1(__global unsigned int* flag, __global unsigned int *p, __global CLim *image)
{
	unsigned int ID0 = get_global_id(0);
	
	
	//int *data_start = p;
	//CLim im;

	//MyClass wrapper;

	//CLim_init(&im);
	//im.init();
	//im.data = p;
	//im.data = p;
	printf((__constant char *)"Kernel1 running, modifying flag: %u \n", *flag);
	//printf((__constant char *)"Kernel1 running, data: %u \n", wrapper->get());
	printf((__constant char *)"Kernel1 running, param: %u \n", p[1]);

	//*flag = (*flag)*3;
	/*
	float j = 1000;
	for (size_t i = 0; i < 2000000; i++)
	{
		j = sin(j);
		
		//printf((__constant char *)"sin.... %f \n", j);
	}
	//if (ID0 == 0)
		//printf((__constant char *)"new flag value: %u \n", *flag);*/
}

__kernel void kernel2(__global unsigned int* flag)
{
	unsigned int ID0 = get_global_id(0);
	printf((__constant char *)"Kernel2 running, flag: %u \n", *flag);
	//printf((__constant char *)"Kernel1 running, data: %u \n", im.data);
}

__kernel void kernel3(__global unsigned int* flag)
{
	unsigned int ID0 = get_global_id(0);

	printf((__constant char *)"Kernel3 running, flag: %u \n", *flag);
}

);