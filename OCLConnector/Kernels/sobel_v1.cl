#pragma OPENCL EXTENSION cl_amd_printf:enable
__kernel void sobel(__global uchar *in, __global uchar *out) 
{
	
	uint iXstride = get_global_size(0);
	uint iIndex = (get_global_id(1) * iXstride + get_global_id(0));
	uint oXstride = iXstride;
	uint oIndex = iIndex;
	
	float a,		b,		c, 
		  d,				f,
		  g,		h,		i;

	a = in[iIndex - 1];
	b = in[iIndex];
	c = in[iIndex + 1];
	iIndex += oXstride;
	d = in[iIndex - 1];
	f = in[iIndex + 1];
	iIndex += oXstride;
	g = in[iIndex - 1];
	h = in[iIndex];
	i = in[iIndex + 1];


	int sobelX =	a +			c * -1 +
					d * 2 +		f * -2 +
					g +			i * -1;

	int sobelY =	a +			b * 2 +		c +
					g * -1 +	h * -2 +	i * -1;

	float sX = (float) sobelX;
	float sY = (float) sobelY;
	uint sobelGrad = min((uint)255, (uint) sqrt(sX * sX + sY * sY));

	uint ix;
	for(ix=0;ix<3;ix++) {
		out[iIndex + ix] = sobelGrad;
	}
	
}