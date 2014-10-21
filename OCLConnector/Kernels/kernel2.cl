
__kernel void kernel2()
{
	unsigned int ID0 = get_global_id(0);
	printf((__constant char *)"Kernel2 running %u \n", ID0);
	//printf((__constant char *)"Kernel1 running, data: %u \n", im.data);
}
