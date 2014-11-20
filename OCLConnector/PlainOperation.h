#include "OCLSequence.h"

class PlainOperation : public OCLSequence {


	virtual void addKernels()
	{
		addKernel("plain", { "plainKernel.cl" }, false, 1, 1, 1);
	}

};