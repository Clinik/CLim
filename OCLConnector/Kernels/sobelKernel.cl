__kernel void sobel(__global const unsigned char *in, __global unsigned char *out)
{
	unsigned int ID0 = get_global_id(0);
	unsigned int ID1 = get_global_id(1);
	unsigned int idx = (ID0 + ID1 * get_global_size(1)) * 3;

	/*int my[3][3] = {{-1, -2, -1}, 
					{ 0,  0,  0}, 
					{ 1,  2,  1} };*/
	
	int mx[3][3] = {{-1, 0, 1}, 
					{ -2,  0,  2}, 
					{ -1,  0,  1} };
					
	char newValue = 0;
	int i, j;
	
	for(i=-1;i<2;i++) {
		for(j=-1;j<2;j++) {
			int newX = ID0 + i * 3;
			int newY = ID1 + (j*get_global_size(1)) * 3;
			int lIDX = (newX + (newY * get_global_size(1)));
			
			newValue = newValue + in[lIDX] * mx[i+1][j+1];
		}
	}
	
	for(i=0;i<3;i++) {
		if(newValue > 0)
			out[idx + i] = 255;
		else
			out[idx + i] = 0;
	}

}
