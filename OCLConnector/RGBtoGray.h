#include "CLSequence.h"
#include "CLim.h"
#include "CLimKernel.h"
#include <vector>
#include <map>

class RGBtoGray : public CLSequence {

	int outSize = 512 * 512 * 3;
	//cl_uchar16 *data;
	uchar *data;

public:

	RGBtoGray() {
		data = new uchar[outSize];
	}

	virtual void addKernels()
	{
		addKernel("RGBtoGray", { "RGBtoGray.cl" }, false, NDRange(512, 512), NDRange(1, 1));
		addKernelArg(kernels[0], &data[0], outSize, 1, CL_MEM_READ_ONLY);
	}

	void postExecute() {
		uchar *d = new unsigned char[outSize];
		uchar *d3 = new unsigned char[outSize*3];

		cl_int error;

		clFinish(connector->queue);
		error = clEnqueueReadBuffer(connector->queue,
			kernelOutputs[0].cl_data, CL_TRUE, 0, outSize * sizeof(uchar), d, 0, NULL, NULL);

		cpuHistogram(d);

		for (int i = 0; i < outSize; i++)
		{
			d3[i * 3] = d[i];
			d3[i * 3 + 1] = d[i];
			d3[i * 3 + 2] = d[i];
		}
		stbFile f;
		std::string fileName = ("../images/grayimg.png");
		f.write_to_file(std::string(fileName.c_str()), 512, 512, 3, d3);
	}

	void cpuHistogram(uchar *d)
	{
		int sum = 0;
		int *hData = new int[256]();
		for (int i = 0; i < 512; ++i)
		{
			for (int j = 0; j < 512; ++j)
			{
				hData[d[i * 512 + j]]++;
			}
		}
		for (int i = 0; i < 256; ++i)
		{
			printf("%i ", hData[i]);
			sum += hData[i];
		}
		printf("\n------------------CPU HIST END(%i)-----------------\n", sum);
		printf("\n");
	}

};