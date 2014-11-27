__kernel void subtract(
	__global const unsigned int *in1,
	__global const unsigned int *in2,
	__global unsigned int *out)
{
	unsigned int ID0 = get_global_id(0);
	unsigned int ID1 = get_global_id(1);
	unsigned int idx = ID0 + ID1 * 3;
	out[idx] = in1[idx] - in2[idx];
	printf((__constant char *)"subtract running %u \n", out[0]);
}
