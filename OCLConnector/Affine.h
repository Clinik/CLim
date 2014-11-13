#ifndef AFFINE_H
#define AFFINE_H

#include "OCLSequence.h"

class Affine : public OCLSequence{
public:
	void addKernels()
	{
		clim::CLim<int> image(200, 100, 3);
		image.set_all(200);
		addImageSource(image);

		//addKernel("test", { "data/CLim.hcl", "test.cl" }, true, 2, 2, 2);
		//addKernelArgLocal<cl_float>(kernels[0], 100, 3);

		addKernel("plain", { "data/CLim.hcl", "plainKernel.cl" }, true, 1, 1, 1);
		
		
	}

};

#endif