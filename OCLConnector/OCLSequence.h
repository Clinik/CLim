#include "OCLConnector.h"

/*********************************************************************************/
/* Sample kernel sequence consisting of a 2 operations                           */
/*********************************************************************************/
class OCLSequence : public OCLConnector {
public:

	void addKernels()
	{
		clim::CLim<int> image(200, 100, 3);
		image.set_all(200);
		
		// First kernel
		addKernel("test", { getKernelPath("data/CLim.hcl").c_str(), getKernelPath("test.cl").c_str() });
		cl_mem argPtr = addKernelArg(kernels[0], image._data, 10);
		addKernelArg(kernels[0], image._width);
		addKernelArg(kernels[0], image._height);
		addKernelArgLocal<cl_float>(kernels[0], 100);

		// Second kernel
		addKernel("test2", { getKernelPath("data/CLim.hcl").c_str(), getKernelPath("test2.cl").c_str() });
		addKernelArg(kernels[1], argPtr);
		addKernelArg(kernels[1], image._width);
		addKernelArg(kernels[1], image._height);
	}

	void runKernels()
	{
		runKernel(kernels[0], 1, 1, 1);
		runKernel(kernels[1], 1, 1, 1);
	}


};