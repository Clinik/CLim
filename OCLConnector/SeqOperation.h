#include "OCLSequence.h"

class SeqOperation : public OCLSequence {

public:
	void addKernels()
	{
		addKernel("test2", { "data/CLim.hcl", "test2.cl" }, true, 1, 1, 1);
	}

};