#include "OCLJunction.h"

class MultiOperation : public OCLJunction {

public:
	void addKernels()
	{
		addKernel("test2", { "data/CLim.hcl", "test2.cl" }, true, 1, 1, 1);
	}
};