#ifndef ImOperation_H
#define ImOperation_H

#include "CLSequence.h"

class ImOperation : public CLSequence{
public:
	void addKernels()
	{
		//addKernel("test", { "data/CLim.hcl", "test.cl" }, true, 1, 1, 1);
	}

};

#endif