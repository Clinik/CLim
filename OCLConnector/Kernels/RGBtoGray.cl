__kernel void RGBtoGray(__global uchar *in, __global uchar *out) {

	unsigned int ID0 = get_global_id(0);
	unsigned int ID1 = get_global_id(1);
	
	unsigned int idx = (ID0 + ID1 * get_global_size(1)) * 3;
	unsigned int outIdx = (ID0 + ID1 * get_global_size(1));

	char grayValue;
	grayValue = in[idx] * 0.21;
	grayValue = grayValue + in[idx+1] * 0.72;
	grayValue = grayValue + in[idx+2] * 0.07;
	
	out[outIdx] = grayValue;
}