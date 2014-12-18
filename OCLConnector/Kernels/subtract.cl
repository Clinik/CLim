__kernel void subtract(
	__global const unsigned char *in1,
	__global const unsigned char *in2,
	__global unsigned char *out)
{
	unsigned int ID0 = get_global_id(0);
	unsigned int ID1 = get_global_id(1);
	
	unsigned int idx = (ID0 + ID1 * get_global_size(1)) * 3;
	
	unsigned char gray[3];
	unsigned char lum[3] = {0.21, 0.72, 0.07};

	char newValue = 0;
		
	unsigned int i;
	for(i=0;i<3;i++) {
		
		
		if(in2[idx+i] < in1[idx+i])
			newValue = in1[idx+i] - in2[idx+i];
		
		gray[i] = in1[idx+i];//newValue;
	}

	newValue = gray[0] * 0.21;
	newValue = newValue + gray[1] * 0.72;
	newValue = newValue + gray[2] * 0.07;
	
	for(i=0;i<3;i++) 
		out[idx+i] = newValue;
}
