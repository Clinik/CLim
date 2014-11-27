#include "CLSequence.h"
#include "CLim.h"
#include "CLimKernel.h"
#include <vector>
#include <map>

class ImSubtract : public CLSequence {

	int *data;
	int outSize;

public:

	template<typename T>
	void addImages(clim::CLim<T> im1, clim::CLim<T> im2) {
		dataSources.push_back(addDataSource(im1));
		dataSources.push_back(addDataSource(im2));
		outSize = im1._width * im1._height;
		data = new int[outSize];
		printf("output: %d", data[1]);
	}

	void addKernels()
	{

		size_t *globalThreads;
		size_t *localThreads;
		addKernel("subtract", { "subtract.cl" }, true, 1, NULL, NULL);
		
		// Input
		kernelToMemObj.insert(std::make_pair(&dataSources[0], &kernels[0]));
		kernelToMemObj.insert(std::make_pair(&dataSources[1], &kernels[0]));
		// Output

		addKernelArg(kernels[0], &data[0], outSize, 2, CL_MEM_READ_ONLY);
	}

	void postExecute() {
		size_t imSize = sizeof(data);
		int *d = new int[9]; d[1] = 3;
		cl_int error;

		error = clEnqueueReadBuffer(connector->queue,
			kernelOutputs[0], CL_TRUE, 0, 9 * sizeof(int), d, 0, NULL, NULL);
		clFinish(connector->queue);
		
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++)
				printf("%d ", d[i + j * 3]);
			printf("\n");
		}
	}


};