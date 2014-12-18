__kernel void histogram() {
	unsigned int ID0 = get_global_id(0);
	
	printf((__constant char *)
		"hist running %u \n", 
		ID0);
}