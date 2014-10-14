#define STRINGIFY(A) #A

std::string kernel2_source = STRINGIFY(

#pragma OPENCL EXTENSION cl_amd_printf:enable \n

__kernel void test(__global unsigned int* flag)
{
	unsigned int ID0 = get_global_id(0);
	printf((__constant char *)"Kernel2 running, flag: %u \n", *flag);
}

);