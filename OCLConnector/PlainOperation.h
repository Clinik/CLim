#include "CLSequence.h"

class PlainOperation : public CLSequence {

	int outSize = 510 * 510 * 3;
	unsigned char *data;

public:

	PlainOperation() {
		data = new unsigned char[outSize];
	}

	virtual void addKernels()
	{
		addKernel("sobel", { "sobelKernel.cl" }, false, NDRange(510, 510), NDRange(1, 1));
		addKernelArg(kernels[0], &data[0], outSize, 1, CL_MEM_READ_ONLY);
	}

	void postExecute() {
		unsigned char *d = new unsigned char[outSize];
		
		cl_int error;

		clFinish(connector->queue);
		error = clEnqueueReadBuffer(connector->queue,
			kernelOutputs[0].cl_data, CL_TRUE, 0, outSize * sizeof(unsigned char), d, 0, NULL, NULL);

		stbFile f;
		std::string fileName = ("../images/sobel.png");
		f.write_to_file(std::string(fileName.c_str()), 510, 510, 3, d);
	}

};