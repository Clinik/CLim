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

/// <summary>	A sequence defines a list of kernel which will run after each other in the same command queue. </summary>
class CLSequence {
public:
	CLSequence() {};
	/// <summary>	The connector which will execute the sequence's kernels. </summary>
	CLConnector *connector;

	/// <summary>	The kernels in the sequence. </summary>
	std::vector<CLimKernel> kernels;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets a connector. </summary>
	///
	/// <param name="connector"> The connector to execute the kernels. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void setConnector(CLConnector &connector);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Structure to keep track of the image data in the device memory. </summary>
	///
	/// <remarks>	User, 2015.02.28. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef struct clim_mem_object {

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> 	
		/// 		Constructs a memory object from the given memory pointer and the width, height of the array. 
		/// </summary>
		///
		/// <param name="data">	The data. </param>
		/// <param name="w">   	The width. </param>
		/// <param name="h">   	The height. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void construct(cl_mem data, int w, int h) {
			cl_data = data;
			width = w;
			height = h;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructs another memory object from the given reference. </summary>
		///
		/// <param name="object">	[in,out] The object. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void construct(clim_mem_object &object) {
			construct(object.cl_data, object.width, object.height);
		}

		/// <summary>	The OpenCL memory object. </summary>
		cl_mem cl_data;
		/// <summary>	The width. </summary>
		int width;
		/// <summary>	The height. </summary>
		int height;

	} clim_mem_object;

private:
	/// <summary>	Sets image kernel arguments automatically if the kernel is processing an image. </summary>
	void setImageKernelArgs();
	
protected:

	/// <summary>	The image data sources for the kernels processing an image. </summary>
	std::vector<clim_mem_object> dataSources;
	/// <summary>	The kernel outputs containing the processed output data. </summary>
	std::vector<clim_mem_object> kernelOutputs;

	/// <summary>	The root folder which the kernels are in. </summary>
	char* kernelPath = "Kernels/";

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets kernel's full path. 
	/// 		The kernel's full path is a string consists of the "kernelPath" directory and the kernels name.	
	/// </summary>
	///
	/// <param name="name">	The name of the kernel. </param>
	///
	/// <returns>	Full path of the kernel. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string getKernelPath(std::string name) {
		return (std::string(kernelPath) + std::string(name.c_str()));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Allocate data in the device's global memory. </summary>
	///
	/// <typeparam name="Targ">	Type of the data. </typeparam>
	/// <param name="arg">   	If non-null, the argument. </param>
	/// <param name="size">  	The size of the data array. </param>
	/// <param name="flags"> 	The OpenCL flags. </param>
	/// <param name="width"> 	The width of the data. </param>
	/// <param name="height">	The height of the data. </param>
	///
	/// <returns> A cl_mem pointing to the allocated data. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Targ>
	cl_mem allocateGlobalMemobject(
		Targ* arg, size_t size,
		cl_mem_flags flags = CL_MEM_READ_WRITE, size_t width = NULL, size_t height = NULL) {

		cl_mem argPtr;
		cl_int error;

		// Open a buffer to move the data
		argPtr = clCreateBuffer(connector->context, flags, size * sizeof(Targ), NULL, &error);
		// In case of CL_MEM_READ_ONLY flag the data buffer will be an output and doesn't need to fill with data
		if (flags != CL_MEM_READ_ONLY)
		{
			// Moving data to the GPU
			error = clEnqueueWriteBuffer(connector->queue, argPtr, CL_TRUE, 0, size * sizeof(Targ), &arg[0], 0, NULL, NULL);
			if (error != CL_SUCCESS)
			{
				printf("Error: Failed to write to source array!\n \t %s \n", oclErrorString(error));
			}
		}
		else {
			// Push the memory object to the list of outputs, which can be processed after the kernel done executing
			clim_mem_object mem_arg;
			mem_arg.construct(argPtr, width, height);
			kernelOutputs.push_back(mem_arg);
		}

		return argPtr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a kernel argument and allocates the data in the client's memory. </summary>
	///
	/// <typeparam name="Targ">	Type of the data. </typeparam>
	/// <param name="kernel">  	The kernel to set the argument. </param>
	/// <param name="arg">	   	If non-null, the data. </param>
	/// <param name="size">	   	The size of the data array. </param>
	/// <param name="argIndex">	Index of the argument, the next available argument index if not given. </param>
	/// <param name="flags">   	The OpenCL flags. </param>
	///
	/// <returns> A cl_mem pointing to the allocated data. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Targ>
	cl_mem addKernelArg(CLimKernel &kernel,
		Targ* arg, size_t size,
		size_t argIndex = -1, cl_mem_flags flags = CL_MEM_READ_WRITE) {
		cl_int error;
		
		// Allocate memory for the data
		cl_mem argPtr = allocateGlobalMemobject(arg, size, flags);
		// Set the kernel argument
		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(cl_mem), &argPtr);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}

		return argPtr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a kernel argument with an already existing memory object. </summary>
	///
	/// <param name="kernel"> The kernel. </param>
	/// <param name="argPtr">  The pointer to the memory object. </param>
	/// <param name="argIndex"> Index of the argument, the next available argument index if not given. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void addKernelArg(CLimKernel &kernel, cl_mem argPtr, size_t argIndex = NULL);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> Adds a kernel argument with a given template type. </summary>
	///
	/// <typeparam name="Targ">	Type of the targ. </typeparam>
	/// <param name="kernel">  	[in,out] The kernel. </param>
	/// <param name="arg">	   	The argument. </param>
	/// <param name="argIndex">	Index of the argument, the next available argument index if not given. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Targ>
	void addKernelArg(CLimKernel &kernel, Targ arg, size_t argIndex = NULL) {
		cl_int error;

		if (argIndex == NULL) {
			argIndex = kernel.numArgs++;
		}

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(Targ), &arg);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	
	/// 		Adds a kernel argument with the type specified withing the template wildcards and with the size given. 
	/// </summary>
	///
	/// <typeparam name="Targ">	Type of the targ. </typeparam>
	/// <param name="kernel">  	The kernel. </param>
	/// <param name="size">	   	The size of the data. </param>
	/// <param name="argIndex">	Index of the argument, the next available argument index if not given. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Targ>
	void addKernelArgLocal(CLimKernel &kernel, size_t size, size_t argIndex = NULL) {

		cl_int error;

		if (argIndex == NULL) {
			argIndex = kernel.numArgs++;
		}

		error = clSetKernelArg(kernel.kernel, argIndex, sizeof(Targ), NULL);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a kernel to the sequence. </summary>
	///
	/// <param name="kernelName">   	Name of the kernel. </param>
	/// <param name="sources">			The source code of the kernel. </param>
	/// <param name="isImageKernel">	true if this object is image kernel. </param>
	/// <param name="ndRange">			NDRange object specifying the global group sizes. </param>
	/// <param name="localndRange"> 	NDRange object specifying the global group sizes. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void addKernel(const char* kernelName, 
		const std::initializer_list<const char*> sources, bool isImageKernel, 
		const NDRange &ndRange, const NDRange &localndRange);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the kernel with the OpenCL context. </summary>
	///
	/// <param name="kernel">	The kernel. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void runKernel(CLimKernel &kernel);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Makes kernel from source code. </summary>
	///
	/// <param name="kernel_src">	The kernel source. </param>
	/// <param name="kernelName">	Name of the kernel. </param>
	///
	/// <returns>	A CLimKernel compiled. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	CLimKernel makeKernelFromSource(const char* kernel_src, const char* kernelName) const;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	
	/// 		Makes a reusable memory object from the given image, also allocating memory for it on the client. 
	/// </summary>
	///
	/// <typeparam name="T">	Type of the Clim object. </typeparam>
	/// <param name="dataSource">	The data source. </param>
	///
	/// <returns>	A clim_mem_object. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	clim_mem_object addDataSource(clim::CLim<T> &dataSource) {
		cl_mem argPtr = allocateGlobalMemobject(dataSource._data, dataSource.size());
		clim_mem_object memObj;
		memObj.construct(argPtr, dataSource._width, dataSource._height);
		return memObj;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a data source to 'kernel'. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="dataSource">	[in,out] The data source. </param>
	/// <param name="kernel">	 	[in,out] The kernel. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	void addDataSource(clim::CLim<T> &dataSource, CLimKernel &kernel) {
		cl_mem argPtr = allocateGlobalMemobject(dataSource._data, dataSource.size());
		clim_mem_object memObj;
		memObj.construct(argPtr, dataSource._width, dataSource._height);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Given a memory object adds it as a kernel arguments. </summary>
	///
	/// <param name="transferable">	The memory object. </param>
	/// <param name="width">	   	The width. </param>
	/// <param name="height">	   	The height. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void addDataSource(cl_mem transferable, size_t width, size_t height) {
		clim_mem_object t;
		t.construct(transferable, width, height);
		dataSources.push_back(t);
		size_t outSize = 512*512*3;
		unsigned char* data = new unsigned char[outSize];
		addKernelArg(kernels[0], transferable, 0);
	}

	virtual void addKernels() = 0;

	/* 
	* This method called after the kernels in this sequence are done executing
	* Variables that depend on the kernel execution will get their final value at this phase:
	*	kernelOutputs - vector of output objects [CL memory handle, width, height of the object]
	*/
	virtual void postExecute() {

	}

	/// <summary>	Initialises the kernel arguments. </summary>
	void initKernelArgs();

	/// <summary>	Executes the kernels in the sequence. </summary>
	virtual void execute();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets kernel outputs. </summary>
	///
	/// <returns>	The kernel outputs. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<clim_mem_object> getKernelOutputs() {
		return kernelOutputs;
	}
};

#endif