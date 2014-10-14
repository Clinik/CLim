#ifndef OCL_CONNECTOR_H
#define OCL_CONNECTOR_H

#include <vector>
#include <iostream>
#include <fstream>

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#include "CLimKernel.h"

using namespace std;

/*
*	OpenCL loaderem
*/
class OCLConnector {
public:
	/*
	 *	Funkcio definiciok
	 */
	OCLConnector();		//... Default konstruktor
	~OCLConnector(){};		//... Destr.

	vector<string> kernelSources;

	void loadKernel(std::string kernel_source);
	void loadProgram(string kernel_src);
	void runKernel();
	void pop();
	
	void addKernel(string kernelName, initializer_list<string> sources);
	CLimKernel makeKernelFromSource(string kernel_src, string kernelName);
	void runKernel(int index);
	void runKernels();
	
	cl_event events[3];
	cl::Buffer cl_data;
	
	cl_int error;

	unsigned int deviceUsed;
	vector<cl::Device> devices;

	cl::Context context;
	cl::CommandQueue queue;
	cl::CommandQueue queues[3];

	cl::Event event;
	cl::Buffer cl_flag;
	cl::Memory flag;

	vector<CLimKernel> kernels;
};

#endif