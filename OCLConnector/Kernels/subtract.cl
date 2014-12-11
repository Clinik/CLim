__kernel void subtract(
	__global const unsigned char *in1,
	__global const unsigned char *in2,
	__global unsigned char *out)
{
	unsigned int ID0 = get_global_id(0);
	unsigned int ID1 = get_global_id(1);
	
	unsigned int idx = (ID0 + ID1 * get_global_size(1)) * 3;
	
	unsigned int i;
	for(i=0;i<3;i++) {
		
		char newValue = 0;
		
		if(in2[idx+i] < in1[idx+i])
			newValue = in1[idx+i] - in2[idx+i];

		out[idx+i] = newValue;
	}
	
}
