#ifndef OCL_CONNECTOR_H
#define OCL_CONNECTOR_H

#include <vector>
#include <iostream>
#include <fstream>

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.h"
#include "CLim.h"

#include "CLimKernel.h"

class OCLConnector {

protected:
	/************************************************************************/
	/* Specialization tools                                                 */
	/************************************************************************/
	char* kernelPath = "Kernels/";

	virtual std::string getKernelPath(std::string name) {
		return (std::string(kernelPath) + std::string(name.c_str()));
	}

	virtual void addKernels() = 0;
	virtual void runKernels() = 0;

	/************************************************************************/
	/* Pointer argument means that we need to alloc space in the GPU's		*/
	/*		global memory													/*
	/************************************************************************/
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
		
		argPtr = clCreateBuffer(context, flags, size * sizeof(Targ), NULL, &error);

		printf("Moving data to the GPU...\n");
		error = clEnqueueWriteBuffer(queue, argPtr, CL_TRUE, 0, size * sizeof(Targ), &arg[0], 0, NULL, NULL);
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

	void addKernelArg(CLimKernel &kernel, cl_mem argPtr, size_t argIndex = NULL) {

		cl_int error;

		printf("Setting buffer arg\n");

		if (argIndex == NULL) {
			argIndex = kernel.numArgs++;
		}

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(cl_mem), &argPtr);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
		printf("Kernel arguments successfully set!\n\n");
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

	/************************************************************************/
	/* Variables to connect with the OpenCl context                         */
	/************************************************************************/
	cl_device_id deviceUsed;
	cl_context context;
	cl_command_queue  queue;
	
	/************************************************************************/
	/* The kernels to will work with in the queue                           */
	/************************************************************************/
	std::vector<CLimKernel> kernels;
	
	/************************************************************************/
	/* Struct to keep track of the image data in the device memory          */
	/************************************************************************/
	typedef struct clim_mem_object {

		cl_mem cl_data;
		int width;
		int height;

	} clim_mem_object;
	clim_mem_object mem_object;

public:

	/************************************************************************/
	/* In the constructor we initialize the context to work with            */
	/************************************************************************/
	OCLConnector() {

		cl_int error;

		printf("------------------------Loading OpenCL----------------------------\n\n");
		cl_platform_id platforms;

		printf("Getting platform IDs....\n");
		error = clGetPlatformIDs(1, &platforms, NULL);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to get platform IDs: %s!\n", oclErrorString(error));
		}
		printf("Platform IDs successfully obtained!\n\n");
		
		/************************************************************************/
		/* Get devices                                                          */
		/************************************************************************/
		printf("Getting devices....\n");
		int gpu = 0;
		error = clGetDeviceIDs(platforms, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &deviceUsed, NULL);

		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to get device IDs: %s!\n", oclErrorString(error));
		}
		printf("Devices successfully obtained!\n\n");

		/************************************************************************/
		/* Create a context                                                     */
		/************************************************************************/
		printf("Creating context...\n");
		context = clCreateContext(0, 1, &deviceUsed, NULL, NULL, &error);
		if (!context)
		{
			printf("ERROR: Failed to create context!\n");
		}
		printf("Context successfully created\n\n");
		
		/************************************************************************/
		/* Create command queue                                                 */
		/************************************************************************/
		printf("Creating a command queue....\n");
		queue = clCreateCommandQueue(
			context,
			deviceUsed,
#ifdef OCL_PROFILING
			CL_QUEUE_PROFILING_ENABLE,
#else 
			NULL,
#endif
			&error);
		if (!queue)
		{
			printf("ERROR: Failed to create a command commands!\n");
		}
		printf("Command queue successfully created!\n\n");
		printf("OpenCl is ready to operate!\n");
		printf("------------------------------------------------------------------\n\n");
	}
	~OCLConnector(){};
	
	/************************************************************************/
	/* Function definitions                                                 */
	/************************************************************************/
	void addKernel(const char* kernelName, const std::initializer_list<const char*> sources);

	CLimKernel makeKernelFromSource(const char* kernel_src, const char* kernelName) const;

	void runKernel(int index) const {
		cl_int error;
		
		printf("------------------------Kernel: \"%s\" invoked----------------------------\n\n", kernels[index].name);
		/************************************************************************/
		/* Setting kernel args                                                  */
		/************************************************************************/
		/*
		printf("Setting kernel arguments...\n");
		error = clSetKernelArg(kernels[index].kernel, 0, sizeof(cl_mem), &mem_object.cl_data);
		error |= clSetKernelArg(kernels[index].kernel, 1, sizeof(uint), &mem_object.width);
		error |= clSetKernelArg(kernels[index].kernel, 2, sizeof(uint), &mem_object.height);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
		printf("Kernel arguments successfully set!\n");
		*/
		/************************************************************************/
		/* Run kernel                                                           */
		/************************************************************************/
		printf("Running kernel...\n");
		size_t local = 1;
		cl_event event;
		error = clEnqueueNDRangeKernel(queue, kernels[index].kernel, 1, NULL, &local, &local, 0, NULL, &event);
		if (error)
		{
			printf("Error: Failed to execute kernel!\n \t %s \n", oclErrorString(error));
		}
#ifdef OCL_PROFILING
		else {
			clWaitForEvents(1, &event);

			cl_ulong start, end;
			clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);

			double timeSec = 1.e-9 * (end - start);
			double timeMs = 1.e-6 * (end - start);

			printf("Kernel runtime: %f seconds or %f ms.\n\n", timeSec, timeMs);
		}
#else
		printf("Profiling not enabled, to display the kernel run time: #define OCL_PROFILING before includeing this file. (It implies event waiting...)\n\n");
#endif // OCL_PROFILING
	}

	void runKernel(CLimKernel &kernel, const size_t workDim, const size_t localSize, const size_t globalSize) {
		printf("Running kernel...\n");
		cl_int error;

		cl_event event;
		error = clEnqueueNDRangeKernel(queue, kernel.kernel, 1, NULL, &localSize, &globalSize, 0, NULL, &event);
		if (error)
		{
			printf("Error: Failed to execute kernel!\n \t %s \n", oclErrorString(error));
		}
#ifdef OCL_PROFILING
		else {
			clWaitForEvents(1, &event);

			cl_ulong start, end;
			clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
			clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);

			double timeSec = 1.e-9 * (end - start);
			double timeMs = 1.e-6 * (end - start);

			printf("Kernel runtime: %f seconds or %f ms.\n\n", timeSec, timeMs);
		}
#else
		printf("Profiling not enabled, to display the kernel run time: #define OCL_PROFILING before includeing this file. (It implies event waiting...)\n\n");
#endif // OCL_PROFILING
	}

	/************************************************************************/
	/* Neccesary template function to push the data to the GPU              */
	/************************************************************************/
	template <typename T>
	void loadImageForCL(clim::CLim<T> &image){

		cl_int error;
		clim_mem_object o;

		o.cl_data = clCreateBuffer(context, CL_MEM_READ_WRITE, image.size() * sizeof(T), NULL, &error);

		printf("Moving image data to the GPU...\n");
		error = clEnqueueWriteBuffer(queue, o.cl_data, CL_TRUE, 0, image.size() * sizeof(T), &image._data[0], 0, NULL, NULL);
		if (error != CL_SUCCESS)
		{
			printf("Error: Failed to write to source array!\n \t %s \n", oclErrorString(error));
		}
		printf("Image data successfully copied to GPU's global memory!\n\n");

		o.width = image._width;
		o.height = image._height;
		mem_object = o;
	}


	void execute() {
		addKernels();
		runKernels();
	}
};

#endif