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

	size_t workDim;
	size_t *localSize; 
	size_t *globalSize;

	const char* source;
	const char* name;

	cl_kernel kernel;
	//clim_mem_object mem_object;

	CLimKernel() : 
		kernel(NULL), name(NULL), isImageKernel(false), workDim(1), localSize(NULL), globalSize(NULL){};

	CLimKernel(const char* s, cl_kernel k, const char* n, bool isImageKernel = false) 
		: source(s), kernel(k), name(n), isImageKernel(isImageKernel), workDim(1) {};

	~CLimKernel(){};

	void setWorkSizes(const size_t workDim, size_t globalSize[], size_t localSize[])
	{
		this->workDim = workDim;
		this->localSize = localSize;
		this->globalSize = globalSize;
	}


};

#endif