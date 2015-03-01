#ifndef CLIM_KERNEL
#define CLIM_KERNEL

#include <string>

#include "CL/cl.h"
#include "CLim.h"
#include "Misc.h"

/// <summary>	Wrapper class for the OpenCL kernel </summary>
class CLimKernel
{
	
public:
	/// <summary> true if this object is image kernel. 
	/// 		  Indicates thats the kernel contains an image to process, the image is a "Clim" object
	/// </summary>
	bool isImageKernel;
	/// <summary>	Number of arguments. </summary>
	size_t numArgs = 0;
	/// <summary>	The global group size. </summary>
	NDRange globalndRange;
	/// <summary>	The local group size. </summary>
	NDRange localndRange;

	/// <summary>	Source code for the kernel. </summary>
	const char* source;
	/// <summary>	The name of the kernel, which also the function name in the source. </summary>
	const char* name;

	/// <summary>	The OpenCl kernel reference. </summary>
	cl_kernel kernel;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Basic constructor. 
	/// 		Initializes the kernel with default values.	
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	CLimKernel() : 
		kernel(NULL), name(NULL), isImageKernel(false), globalndRange(1), localndRange(1){};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor.
	/// 		Initializes the kernel with the given parameters
	/// </summary>
	///
	/// <param name="_source">		 	Source for the kernel. </param>
	/// <param name="_kernel">		 	The OpenCL kernel. </param>
	/// <param name="_name">		 	The name of the kernel. </param>
	/// <param name="_isImageKernel">	true if this object is image kernel. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	CLimKernel(const char* _source, cl_kernel _kernel, const char* _name, bool _isImageKernel = false) 
		: source(_source), kernel(_kernel), name(_name), isImageKernel(_isImageKernel), globalndRange(1), localndRange(1){};

	~CLimKernel(){};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the global group size. </summary>
	///
	/// <param name="ndRange">	The nd range. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void setglobalNDRange(const NDRange &ndRange)
	{
		this->globalndRange = ndRange;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the local group size. </summary>
	///
	/// <param name="ndRange">	The nd range. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void setlocalNDRange(const NDRange &ndRange)
	{
		this->localndRange = ndRange;
	}

};

#endif