#include "CLSequence.h"

class PlainOperation : public CLSequence {

	int outSize = 512 * 512 * 3;
	//cl_uchar16 *data;
	uchar *data;

public:

	PlainOperation() {
		//data = new cl_uchar16[outSize];
		data = new uchar[outSize];
	}

	virtual void addKernels()
	{
		addKernel("sobel", { "sobel_v1.cl" }, false, NDRange(512, 512), NDRange(1, 1));
		addKernelArg(kernels[0], &data[0], outSize, 1, CL_MEM_READ_ONLY);
	}

	void postExecute() {
		unsigned char *d = new unsigned char[outSize];
		uchar *d3 = new unsigned char[outSize * 3];

		cl_int error;

		clFinish(connector->queue);
		error = clEnqueueReadBuffer(connector->queue,
			kernelOutputs[0].cl_data, CL_TRUE, 0, outSize * sizeof(uchar), d, 0, NULL, NULL);

		for (int i = 0; i < outSize; i++)
		{
			d3[i * 3] = d3[i * 3 + 1] = d3[i * 3 + 2] = d[i];
		}

		stbFile f;
		std::string fileName = ("../images/sobel.png");
		f.write_to_file(std::string(fileName.c_str()), 512, 512, 3, d3);
	}

};