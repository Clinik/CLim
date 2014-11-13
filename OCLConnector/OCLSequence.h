#ifndef OCL_SEQUENCE_H
#define OCL_SEQUENCE_H

#include <list>
#include <vector>
#include "CLim.h"
#include "CLimKernel.h"

#include <iostream>
#include <fstream>
#include "OCLConnector.h"

class OCLSequence {
public:
	OCLSequence() {};
	OCLConnector *connector;

	/************************************************************************/
	/* The kernels to will work with in the queue                           */
	/************************************************************************/
	std::vector<CLimKernel> kernels;

public:

	void setConnector(OCLConnector &connector);

	/************************************************************************/
	/* Structure to keep track of the image data in the device memory       */
	/************************************************************************/
	typedef struct clim_mem_object {

		cl_mem cl_data;
		int width;
		int height;

	} clim_mem_object;
	clim_mem_object mem_object;
private:
	void setImageKernelArgs();
	
protected:

	/************************************************************************/
	/* Specialization tools                                                 */
	/************************************************************************/
	char* kernelPath = "Kernels/";

	std::string getKernelPath(std::string name) {
		return (std::string(kernelPath) + std::string(name.c_str()));
	}

	template<typename Targ>
	cl_mem allocateGlobalMemobject(
		Targ* arg, size_t size,
		cl_mem_flags flags = CL_MEM_READ_WRITE) {

		printf("Setting pointer arg\n");

		cl_mem argPtr;
		cl_int error;

		argPtr = clCreateBuffer(connector->context, flags, size * sizeof(Targ), NULL, &error);

		printf("Moving data to the GPU...\n");
		error = clEnqueueWriteBuffer(connector->queue, argPtr, CL_TRUE, 0, size * sizeof(Targ), &arg[0], 0, NULL, NULL);
		if (error != CL_SUCCESS)
		{
			printf("Error: Failed to write to source array!\n \t %s \n", oclErrorString(error));
		}
		printf("Data successfully copied to GPU's global memory!\n");

		return argPtr;
	}

	template<typename Targ>
	cl_mem addKernelArg(CLimKernel &kernel,
		Targ* arg, size_t size,
		size_t argIndex = -1, cl_mem_flags flags = CL_MEM_READ_WRITE) {

		printf("Setting pointer arg\n");

		if (argIndex == -1) {
			argIndex = kernel.numArgs++;
		}

		cl_mem argPtr;
		cl_int error;

		argPtr = clCreateBuffer(connector->context, flags, size * sizeof(Targ), NULL, &error);

		printf("Moving data to the GPU...\n");
		error = clEnqueueWriteBuffer(connector->queue, argPtr, CL_TRUE, 0, size * sizeof(Targ), &arg[0], 0, NULL, NULL);
		if (error != CL_SUCCESS)
		{
			printf("Error: Failed to write to source array!\n \t %s \n", oclErrorString(error));
		}
		printf("Data successfully copied to GPU's global memory!\n");

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(cl_mem), &argPtr);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
		printf("Kernel arguments successfully set!\n\n");

		return argPtr;
	}

	void addKernelArg(CLimKernel &kernel, cl_mem argPtr, size_t argIndex = NULL);

	template<typename T>
	void addImageSource(clim::CLim<T> &image) {
		cl_mem argPtr = allocateGlobalMemobject(image._data, image.size());
		mem_object.cl_data = argPtr;
		mem_object.width = image._width;
		mem_object.height = image._height;
	}

	template<typename Targ>
	void addKernelArg(CLimKernel &kernel, Targ arg, size_t argIndex = NULL) {
		printf("Setting stack arg\n");
		cl_int error;

		if (argIndex == NULL) {
			argIndex = kernel.numArgs++;
		}

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(Targ), &arg);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
		printf("Kernel arguments successfully set!\n\n");
	}
	
	template<typename Targ>
	void addKernelArgLocal(CLimKernel &kernel, size_t size, size_t argIndex = -1) {

		printf("Setting local arg\n");
		cl_int error;

		if (argIndex == -1) {
			argIndex = kernel.numArgs++;
		}

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(Targ), NULL);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
		printf("Kernel arguments successfully set!\n\n");
	}

	void addKernel(const char* kernelName, const std::initializer_list<const char*> sources, bool isImageKernel, const size_t workDim, const size_t localSize, const size_t globalSize);

	void runKernel(CLimKernel &kernel);

	CLimKernel makeKernelFromSource(const char* kernel_src, const char* kernelName) const;

public:

	virtual void addKernels() = 0;

	void execute();
};

#endif