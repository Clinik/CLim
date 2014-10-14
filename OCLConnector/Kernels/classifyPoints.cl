#define STRINGIFY(A) #A

std::string kernel_source = STRINGIFY(

#pragma OPENCL EXTENSION cl_amd_printf:enable \n

/*
*	Kernel ami elhelyezi a pontokat a hozzajuk legkozelebb eso klaszterbe
*		pos : pontok pozicioi
*		color : pontok szinei
*		pos_clusters : klaszterek pozicioi
*		color_clusters : klaszterek szinei
*		corresp : pont - klaszter megfeleltetesek
*		sum_helper : minden klasztert beleir a (klaszter_id*num_points) resztomb pontID. helyere
*/
__kernel void classifyPoints(
					__global float4* pos, 
					__global float4* color,
					__global float4* pos_clusters,
					__global float4* color_clusters,
					int num_points, 
					int num_clusters,
					__global unsigned int* corresp,
					__global float4* sum_helper,
					__global float4* velocities
					//__global unsigned int* corresp,
					//__local float* tmpDist,
					//__global float4* pos_clusters_new
					//__global float4* pos_clusters_new,
					)
{

	unsigned int ID0 = get_global_id(0);
	unsigned int ID1 = get_global_id(1);

	unsigned int ID = ID0;

	pos[ID].x += velocities[ID].x;
	pos[ID].y += velocities[ID].y;
	pos[ID].z += velocities[ID].z;

	if(pos[ID].x > 2.0f)
		pos[ID].x = 0.0f;

	if(pos[ID].y > 2.0f)
		pos[ID].y = 0.0f;
	
	if(pos[ID].z > 2.0f)
		pos[ID].z = 0.0f;

	float min_dst = 2000.0f;
	for(int i = 0; i < num_clusters; i++)
	{
		float dst = (pos[ID].x-pos_clusters[i].x) * (pos[ID].x-pos_clusters[i].x) + 
					(pos[ID].y-pos_clusters[i].y) * (pos[ID].y-pos_clusters[i].y) +
					(pos[ID].z-pos_clusters[i].z) * (pos[ID].z-pos_clusters[i].z);

		if(dst <= min_dst)
		{
			min_dst = dst;
			color[ID] = color_clusters[i];
			corresp[ID] = i;
		}
	}

	sum_helper[(corresp[ID]*num_points)+ID] = pos[ID];
	sum_helper[(corresp[ID]*num_points)+ID].w = 1.0f;
	
	/*
	barrier(CLK_GLOBAL_MEM_FENCE);
	// Idaig megvannak a pont -> klaszter megfeleltetesek
	// Ez lenne a "CPU like" megvalositas az atlagokra, de nagyon lassu igy paralell reductiont hasznalok inkabb
	if(ID < num_clusters)
	{
		float2 sum;
		sum.x = 0.0f;
		sum.y = 0.0f;
		int count = 0;
		for(int i = 0; i < num_points; i++)
		{
			if(ID == corresp[i])
			{
				sum.x += pos[i].x;
				sum.y += pos[i].y;
				count++;
			}
		}
		pos_clusters[ID].x = sum.x/count;
		pos_clusters[ID].y = sum.y/count;
		printf("ID: %u, <%f, %f>\n", ID, pos_clusters[ID].x, pos_clusters[ID].y);
	}*/ 
}

/*
*	Kernel ami kiszamolja a klaszterek uj poziciojat a hozzajuk tartozo pontok alapjan
*		buffer : (pontok szama)*(klaszterek szama) darab "koordinata"
*		scratch : abba megy a paralell redu. eredmenye
*		length : pontok szama
*		result : eredmeny
*		cluster_id : melyik klasztert vesszuk
*		pos_ clusters : vegen ebbe mennek az uj koordinatak
*/
__kernel void PRsum(
			__global float4* buffer,
            __local float4* scratch,
            __const int length,
            __global float4* result,
			int cluster_id,
			__global float4* pos_clusters) {

  int global_index = get_global_id(0);
  global_index += length * cluster_id;
  int length_new = length * (cluster_id+1);
  float4 accumulator;
  accumulator.x = 0.0f;
  accumulator.y = 0.0f;
  accumulator.z = 0.0f;
  accumulator.w = 0.0f;

  while (global_index < length_new) {
    float4 element = buffer[global_index];
    accumulator.x = accumulator.x + element.x;
	accumulator.y = accumulator.y + element.y;
	accumulator.z = accumulator.z + element.z;
	accumulator.w = accumulator.w + element.w;
	buffer[global_index].x = 0.0f;
	buffer[global_index].y = 0.0f;
	buffer[global_index].z = 0.0f;
	buffer[global_index].w = 0.0f;
    global_index += get_global_size(0);
  }

  // Parallel reduction
  int local_index = get_local_id(0);
  scratch[local_index] = accumulator;
  barrier(CLK_LOCAL_MEM_FENCE);
  for(int offset = get_local_size(0) / 2;
      offset > 0;
      offset = offset / 2) {
    if (local_index < offset) {
      float4 other = scratch[local_index + offset];
      float4 mine = scratch[local_index];
      scratch[local_index].x = mine.x + other.x;
	  scratch[local_index].y = mine.y + other.y;
	  scratch[local_index].z = mine.z + other.z;
	  scratch[local_index].w = mine.w + other.w;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  if (local_index == 0) {
    result[get_group_id(0)] = scratch[0];
  }

  if(get_global_id(0) == 0){
	if(result[0].w != 0.0f) {
		//printf("---><%f>\n", (result[0].x / result[0].w));
		pos_clusters[cluster_id].x = result[0].x / result[0].w;
		pos_clusters[cluster_id].y = result[0].y / result[0].w;
		pos_clusters[cluster_id].z = result[0].z / result[0].w;
	}
}

printf("%f, %f, %f", result[get_group_id(0)].x, result[get_group_id(0)].y, result[get_group_id(0)].z);
}
);