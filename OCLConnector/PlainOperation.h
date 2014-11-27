#include "CLSequence.h"

class PlainOperation : public CLSequence {


	virtual void addKernels()
	{
//		addKernel("plain", { "plainKernel.cl" }, false, 1, 1, 1);
	}

};