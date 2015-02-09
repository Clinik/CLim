#ifndef OCL_SEQUENCE_H
#define OCL_SEQUENCE_H

#include <list>
#include <vector>
#include <map>
#include "CLim.h"
#include "CLimKernel.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include "CLConnector.h"
#include "Misc.h"

class CLSequence {
public:
	CLSequence() {};
	CLConnector *connector;

	/************************************************************************/
	/* The kernels to work with in the queue                                */
	/************************************************************************/
	std::vector<CLimKernel> kernels;

public:

	void setConnector(CLConnector &connector);


	/************************************************************************/
	/* Structure to keep track of the image data in the device memory       */
	/************************************************************************/
	typedef struct clim_mem_object {

	public:
		void construct(cl_mem data, int w, int h) {
			cl_data = data;
			width = w;
			height = h;
		}

		void construct(clim_mem_object &object) {
			construct(object.cl_data, object.width, object.height);
		}

		cl_mem cl_data;
		int width;
		int height;

	} clim_mem_object;

private:
	void setImageKernelArgs();
	
protected:

	std::vector<clim_mem_object> dataSources;
	std::map<clim_mem_object*, CLimKernel*> kernelToMemObj;
	std::vector<clim_mem_object> kernelOutputs;

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
		cl_mem_flags flags = CL_MEM_READ_WRITE, size_t width = NULL, size_t height = NULL) {

		//printf("Setting pointer arg\n");

		cl_mem argPtr;
		cl_int error;

		argPtr = clCreateBuffer(connector->context, flags, size * sizeof(Targ), NULL, &error);
		if (flags != CL_MEM_READ_ONLY)
		{
			//printf("Moving data to the GPU...\n");
			error = clEnqueueWriteBuffer(connector->queue, argPtr, CL_TRUE, 0, size * sizeof(Targ), &arg[0], 0, NULL, NULL);
			if (error != CL_SUCCESS)
			{
				printf("Error: Failed to write to source array!\n \t %s \n", oclErrorString(error));
			}
			//printf("Data successfully copied to GPU's global memory!\n");
		}
		else {
			clim_mem_object mem_arg;
			mem_arg.construct(argPtr, width, height);
			kernelOutputs.push_back(mem_arg);
		}

		return argPtr;
	}

	template<typename Targ>
	cl_mem addKernelArg(CLimKernel &kernel,
		Targ* arg, size_t size,
		size_t argIndex = -1, cl_mem_flags flags = CL_MEM_READ_WRITE) {
		cl_int error;
		
		cl_mem argPtr = allocateGlobalMemobject(arg, size, flags);

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(cl_mem), &argPtr);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}

		//printf("Kernel arguments successfully set!\n\n");

		return argPtr;
	}

	void addKernelArg(CLimKernel &kernel, cl_mem argPtr, size_t argIndex = NULL);

	template<typename Targ>
	void addKernelArg(CLimKernel &kernel, Targ arg, size_t argIndex = NULL) {
		//printf("Setting stack arg\n");
		cl_int error;

		if (argIndex == NULL) {
			argIndex = kernel.numArgs++;
		}

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(Targ), &arg);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
		//printf("Kernel arguments successfully set!\n\n");
	}
	
	template<typename Targ>
	void addKernelArgLocal(CLimKernel &kernel, size_t size, size_t argIndex = NULL) {

		//printf("Setting local arg\n");
		cl_int error;

		if (argIndex == NULL) {
			argIndex = kernel.numArgs++;
		}

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(Targ), NULL);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
		//printf("Kernel arguments successfully set!\n\n");
	}

	void addKernel(const char* kernelName, 
		const std::initializer_list<const char*> sources, bool isImageKernel, 
		const NDRange &ndRange, const NDRange &localndRange);

	void runKernel(CLimKernel &kernel);

	CLimKernel makeKernelFromSource(const char* kernel_src, const char* kernelName) const;

public:

	template<typename T>
	clim_mem_object addDataSource(clim::CLim<T> &dataSource) {
		cl_mem argPtr = allocateGlobalMemobject(dataSource._data, dataSource.size());
		//mem_object.construct(argPtr, dataSource._width, dataSource._height);
		clim_mem_object memObj;
		memObj.construct(argPtr, dataSource._width, dataSource._height);
		return memObj;
	}

	template<typename T>
	void addDataSource(clim::CLim<T> &dataSource, CLimKernel &kernel) {
		cl_mem argPtr = allocateGlobalMemobject(dataSource._data, dataSource.size());
		clim_mem_object memObj;
		memObj.construct(argPtr, dataSource._width, dataSource._height);
		//kernelToMemObj.insert(&kernel, memObj);
	}

	void addDataSource(cl_mem transferable, size_t width, size_t height) {
		/*CLSequence::clim_mem_object transferable;
		transferable.construct(handle, NULL, NULL);*/
		clim_mem_object t;
		t.construct(transferable, width, height);
		dataSources.push_back(t);
		size_t outSize = 512*512*3;
		unsigned char* data = new unsigned char[outSize];
		addKernelArg(kernels[0], transferable, 0);
		/*
		clim_mem_object mem_arg;
		mem_arg.construct(transferable, 512, 512);
		kernelOutputs.push_back(mem_arg);*/
	}

	virtual void addKernels() = 0;

	/* 
	* This method called after the kernels in this sequence are done executing
	* Variables that depend on the kernel execution will get their final value at this phase:
	*	kernelOutputs - vector of output objects [CL memory handle, width, height of the object]
	*/
	virtual void postExecute() {

	}

	void initKernelArgs();

	virtual void execute();

	std::vector<clim_mem_object> getKernelOutputs() {
		return kernelOutputs;
	}
};

#endif