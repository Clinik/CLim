#include <stdio.h>
#include <iostream>
#include <GL/glew.h>

#include "OCLConnector.h"
#include "Misc.h"

#include <vector>
#include <iosfwd>

CLimKernel OCLConnector::makeKernelFromSource(const char* kernel_src, const char* kernelName) const {
	cl_int error;

	cl_program program;
	CLimKernel climKernel;

	/************************************************************************/
	/* Make a program from the kernel source                                */
	/************************************************************************/
	printf("Making program from source...\n");
	const char *source = kernel_src;
	program = clCreateProgramWithSource(context, 1, &source, NULL, &error);
	if (!program)
	{
		printf("Error: Failed to create program!\n \t %s \n", oclErrorString(error));
		return climKernel;
	}
	printf("Program successfully created!\n\n");

	/************************************************************************/
	/* Build the program                                                    */
	/************************************************************************/
	printf("Building program...\n");
	error = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];

		printf("Error: Failed to build program, displaying build log:\n");
		clGetProgramBuildInfo(program, deviceUsed, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		return climKernel;
	}
	printf("Program successfully built!\n\n");

	/************************************************************************/
	/* Create a kernel from the program                                     */
	/************************************************************************/
	printf("Creating kernel...\n");
	climKernel.kernel = clCreateKernel(program, kernelName, &error);
	if (!climKernel.kernel || error != CL_SUCCESS)
	{
		printf("Error: Failed to create kernel!\n \t %s \n", oclErrorString(error));
		return climKernel;
	}
	printf("Kernel: \"%s\" successfully created!\n\n", kernelName);

	climKernel.name = kernelName;
	return climKernel;
}

void OCLConnector::addKernel(const char* kernelName, const std::initializer_list<const char*> sources) {
	
	std::string src = "";

	for (const char* srcPart : sources) {
		std::ifstream sourceFile(srcPart);
		std::string kernelSource(
			std::istreambuf_iterator<char>(sourceFile),
			(std::istreambuf_iterator<char>()));
		src += kernelSource;
	}
	const char* cstr_src = src.c_str();
	kernels.push_back(makeKernelFromSource(cstr_src, kernelName));
}
