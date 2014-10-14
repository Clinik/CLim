
__kernel void kernel3(__global unsigned int* flag)
{
	unsigned int ID0 = get_global_id(0);

	printf((__constant char *)"Kernel3 running, flag: %u \n", *flag);
}