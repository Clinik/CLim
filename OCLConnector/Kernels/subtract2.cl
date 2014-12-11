__kernel void subtract2(
	__global const unsigned char *in1,
	__global const unsigned char *in2,
	__global unsigned char *out)
{
	unsigned int ID0 = get_global_id(0);
	unsigned int ID1 = get_global_id(1);
	unsigned int ID3 = get_global_id(2);

	unsigned int idx = (ID0 + ID1 * get_global_size(1)) * 3 + ID3;
	
	char newValue = 0;
		
	if(in2[idx] < in1[idx])
		newValue = in1[idx] - in2[idx];

	out[idx] = newValue;
	
}
