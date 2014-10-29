__kernel void test2(__global const unsigned int *data, int width, int height)
{
	unsigned int ID0 = get_global_id(0);
	CLim im; 
	init(&im, data, width, height);

	printf((__constant char *)
		"test2 running, init image: w(%u), h(%u), data[1]: %u \n", 
		im._width, im._height, im._data[1]);

}
