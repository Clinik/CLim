#pragma OPENCL EXTENSION cl_amd_printf:enable
__kernel void sobel(__global uchar16 *in, __global uchar *out) 
{
	
	uint iXstride = get_global_size(0);
	uint iIndex = (get_global_id(1) * iXstride + get_global_id(0)) * 3 + 1;
	uint oXstride = iXstride * 3;
	uint oIndex = iIndex;
	
	uint a,				c, 
		  d,				f,
		  g,				i;

	uint16 b, h, e;
	
	a = ((__global uchar*)(in + iIndex))[-1];
	b = convert_uint16(in[iIndex]);
	c = ((__global uchar*)(in + iIndex))[16];

	if(get_global_id(1) == 1 && get_global_id(0) == 1) {
		printf((__constant char *)" %u \n", b.s01);
	}

	iIndex += oXstride;
	d = ((__global uchar*)(in + iIndex))[-1];
	e = convert_uint16(in[iIndex]);
	f = convert_float(((__global uchar*)(in + iIndex))[16]);
	
	iIndex += oXstride;
	g = ((__global uchar*)(in + iIndex))[-1];
	h = convert_uint16(in[iIndex]);
	i = ((__global uchar*)(in + iIndex))[16];

	/*float16 sobelX =	a +			c * -1 +
					d * 2 +		f * -2 +
					g +			i * -1;

	float16 sobelY =	a +			b * 2 +		c +
					g * -1 +	h * -2 +	i * -1;
					*/
	uint16 sobelX, sobelY;
	
	sobelX = (uint16)(a, b.s0123, b.s456789ab, b.scde) -      (uint16)(b.s123, b.s4567, b.s89abcdef, c) + 
	  2.0f*(uint16)(d, e.s0123, e.s456789ab, e.scde) - 2.0f*(uint16)(e.s123, e.s4567, e.s89abcdef, f) + 
		   (uint16)(g, h.s0123, h.s456789ab, h.scde) -      (uint16)(h.s123, h.s4567, h.s89abcdef, i);

	sobelY = (uint16)(a, b.s0123, b.s456789ab, b.scde) + 2.0f*b + (uint16)(b.s123, b.s4567, b.s89abcdef, c) - 
	       (uint16)(g, h.s0123, h.s456789ab, h.scde) - 2.0f*h - (uint16)(h.s123, h.s4567, h.s89abcdef, i);
		   
	/*uint ix;
	for(ix=0;ix<3;ix++) {
		out[iIndex] = convert_uchar16( min((float16)255.0f, sqrt(convert_float16(sobelX * sobelX + sobelY * sobelY))) );
	}*/

}