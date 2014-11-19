#ifndef ImOperation_H
#define ImOperation_H

#include "OCLSequence.h"

class ImOperation : public OCLSequence{
public:
	void addKernels()
	{
		
		addKernel("test", { "data/CLim.hcl", "test.cl" }, true, 1, 1, 1);

		//addKernel("plain", { "data/CLim.hcl", "plainKernel.cl" }, true, 1, 1, 1);
	}

};

#endif