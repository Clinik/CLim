#ifndef ImOperation_H
#define ImOperation_H

#include "CLSequence.h"

class Histogram : public CLSequence {

	cl_int binSize;
	cl_int groupSize;
	cl_int subHistgCnt;

	int outSize;
	int imageDims = 512;
	uint *out;
	uint *hostOut;

public:

	Histogram() {
		binSize = 256;
		groupSize = 16;
		subHistgCnt = (imageDims * imageDims) / (binSize * groupSize);
		outSize = binSize * subHistgCnt;
		out = new uint[outSize];
		hostOut = new uint[binSize]();
	}

	virtual void addKernels()
	{
		addKernel("histogram", { "histogram.cl" }, false, NDRange((imageDims * imageDims) / binSize), NDRange(groupSize));
		addKernelArgLocal<uchar>(kernels[0], groupSize * binSize * sizeof(cl_uchar), 1);
		addKernelArg(kernels[0], &out[0], outSize, 2, CL_MEM_READ_ONLY);
	}

	void postExecute() {
		uint *d = new uint[outSize];
		
		cl_int error;

		clFinish(connector->queue);
		error = clEnqueueReadBuffer(connector->queue,
			kernelOutputs[0].cl_data, CL_TRUE, 0, outSize * sizeof(uint), d, 0, NULL, NULL);

		int width = (imageDims / binSize) * binSize;
		int height = (imageDims / groupSize) * groupSize;

		int sum = 0;
		for (int i = 0; i < subHistgCnt; ++i)
		{
			for (int j = 0; j < binSize; ++j)
			{
				hostOut[j] += d[i * binSize + j];
			}
		}

		for (int i = 0; i < binSize; ++i)
		{
			printf("%i ", hostOut[i]);
			sum += hostOut[i];
		}
		printf("\n------------------GPU HIST END(%i)-----------------\n", sum);
	}

};

#endif