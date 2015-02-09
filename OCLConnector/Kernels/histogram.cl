#pragma OPENCL EXTENSION cl_amd_printf:enable
#define BIN_SIZE 256

__kernel void histogram(__global uchar* in, __local uchar* shared, __global uint* out) 
{
	size_t localId = get_local_id(0);
    size_t globalId = get_global_id(0);
    size_t groupId = get_group_id(0);
	size_t groupSize = get_local_size(0);

    for(int i = 0; i < BIN_SIZE; ++i)
        shared[localId * BIN_SIZE + i] = 0;

    barrier(CLK_LOCAL_MEM_FENCE);
    
    for(int i = 0; i < BIN_SIZE; ++i)
    {
        uint value = in[globalId * BIN_SIZE + i];
        shared[localId * BIN_SIZE + value]++;
    }
    
    barrier(CLK_LOCAL_MEM_FENCE); 
    
    for(int i = 0; i < BIN_SIZE / groupSize; ++i)
    {
        uint binCount = 0;
        for(int j = 0; j < groupSize; ++j)
            binCount += shared[j * BIN_SIZE + i * groupSize + localId];
            
        out[groupId * BIN_SIZE + i * groupSize + localId] = binCount;
		
    }

}