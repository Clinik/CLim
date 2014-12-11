#ifndef CLIM_KERNEL
#define CLIM_KERNEL

#include <string>

#include "CL/cl.h"
#include "CLim.h"
#include "Misc.h"

class CLimKernel
{
	
public:
	bool isImageKernel;
	size_t numArgs = 0;

	NDRange globalndRange;
	NDRange localndRange;

	const char* source;
	const char* name;

	cl_kernel kernel;

	CLimKernel() : 
		kernel(NULL), name(NULL), isImageKernel(false), globalndRange(1), localndRange(1){};

	CLimKernel(const char* s, cl_kernel k, const char* n, bool isImageKernel = false) 
		: source(s), kernel(k), name(n), isImageKernel(isImageKernel), globalndRange(1), localndRange(1){};

	~CLimKernel(){};

	void setglobalNDRange(const NDRange &ndRange)
	{
		this->globalndRange = ndRange;
	}

	void setlocalNDRange(const NDRange &ndRange)
	{
		this->localndRange = ndRange;
	}

};

#endif