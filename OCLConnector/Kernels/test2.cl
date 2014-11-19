__kernel void test2(__global unsigned int *data, int width, int height, __local float* localArg)
{
	unsigned int ID0 = get_global_id(0);
	CLim im; 
	init(&im, data, width, height);
	printf((__constant char *)
		"test2 running, init image: w(%u), h(%u), data[1]: %u \n", 
		im._width, im._height, im._data[1]);

	//unsigned int b = im._data[1];
	im._data[1] = 5;
	printf((__constant char *)
		"test modified im._data[1]: %u \n", im._data[1]);
	float asd;

	for(int i=0;i<100000;i++)
		asd = sinh((float)i);
	printf((__constant char *)
		"test2: %f \n", asd);
	
}
