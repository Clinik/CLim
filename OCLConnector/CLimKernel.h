#ifndef CLIM_KERNEL
#define CLIM_KERNEL

#include <string>

#include "CL/cl.h"
#include "CLim.h"
#include "Misc.h"

class OCLConnector;

class CLimKernel
{
public:

	const char* source;
	const char* name;
	cl_kernel kernel;

	CLimKernel() : kernel(NULL), name(NULL){};
	CLimKernel(const char* s, cl_kernel k, const char* n)
		: source(s), kernel(k), name(n) {};
	~CLimKernel(){};

};

#endif

