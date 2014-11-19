__kernel void test(__global unsigned int *data, int width, int height)
{
	unsigned int ID0 = get_global_id(0);
	CLim im; 
	init(&im, data, width, height);
	printf((__constant char *)
		"test1 running, init image: w(%u), h(%u), data[0]: %u \n", 
		im._width, im._height, im._data[0]);

	for(int i=0;i<im._width;i++) {
		for(int j=0;j<im._width;j++) {
			int idx = i*width+j;
			im._data[idx] = im._data[idx] * 2;
			printf((__constant char *)
				"%u ", im._data[idx]);
		}
		char dummy = ".";
		printf((__constant char *)" %c \n",dummy);
	}
}
