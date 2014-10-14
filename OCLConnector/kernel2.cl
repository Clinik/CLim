
__kernel void kernel2(__global unsigned int* flag)
{
	unsigned int ID0 = get_global_id(0);
	printf((__constant char *)"Kernel2 running, flag: %u \n", *flag);
	//printf((__constant char *)"Kernel1 running, data: %u \n", im.data);
}
