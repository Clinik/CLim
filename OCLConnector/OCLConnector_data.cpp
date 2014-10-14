#include <stdio.h>
#include <iostream>
#include <GL/glew.h>

#include "OCLConnector.h"
#include "Misc.h"

#include <vector>
#include <iosfwd>

CLimKernel OCLConnector::makeKernelFromSource(std::string kernel_src, std::string kernelName) {
	// Handle
	int pl;
	cl::Program programFromSource;
	CLimKernel climKernel;

	printf("Making %u. program from source \n\n", kernels.size());

	pl = kernel_src.size();

	try
	{
		cl::Program::Sources source(1,
			std::make_pair(kernel_src.c_str(), pl));

		programFromSource = cl::Program(context, source);

	}
	catch (cl::Error er) {
		printf("ERROR @ making %u. program from source: %s(%s)\n", kernels.size(), er.what(), oclErrorString(er.err()));
	}

	printf("Program build\n");
	try
	{/*
		//const char* buildOption = "-x clc++ ";
		//std::string projectDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
		wchar_t szFilename[200];
		GetModuleFileName(GetModuleHandle(NULL), szFilename, 200);
		char const* temp = getenv("$(SolutionDir)");
		char const* temp2 = getenv("SolutionDir");

		char const* temp3 = getenv("PROJECT_DIR");
		
		const char* buildOption = "-I c:/Includes/";*/
		error = programFromSource.build(devices, NULL);
		
	}
	catch (cl::Error er) {
		printf("ERROR @ building %u. program: %s\n", kernels.size(), oclErrorString(er.err()));
	}
	printf("done building program %u \n", kernels.size());
	std::cout << "BuildStatus: " << programFromSource.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(devices[0]) << std::endl;
	std::cout << "BuildOptions:\t" << programFromSource.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(devices[0]) << std::endl;
	std::cout << "BuildLog:\t " << programFromSource.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;

	try{
		//std::string kernelName = "kernel";
		//kernelName.append(std::to_string(kernels.size()+1));

		climKernel.kernel = cl::Kernel(programFromSource,  kernelName.c_str(), &error);
	}
	catch (cl::Error er) {
		printf("ERROR @ making kernel from %u. program: %s(%s)\n", kernels.size(), er.what(), oclErrorString(er.err()));
	}

	return climKernel;
}

void OCLConnector::addKernel(std::string kernelName, initializer_list<string> sources) {
	
	string src = "";

	for (string srcPart : sources) {
		std::ifstream sourceFile(srcPart);
		std::string kernelSource(
			std::istreambuf_iterator<char>(sourceFile),
			(std::istreambuf_iterator<char>()));
		src += kernelSource;
	}
	
	//CLimKernel kernelToAdd = makeKernelFromSource(src, kernelName);

	kernels.push_back(makeKernelFromSource(src, kernelName));
	//return kernels.back();
}

void OCLConnector::runKernel(int index)
{

	// Kernel argumentumok
	int width = 3;
	int height = 3;
	try
	{
		if (index == 0)
		{
			error = kernels[index].kernel.setArg(0, cl_flag);
			error = kernels[index].kernel.setArg(1, kernels[index].mem_object.cl_data);
			error = kernels[index].kernel.setArg(2, kernels[index].mem_object.width);
			error = kernels[index].kernel.setArg(3, kernels[index].mem_object.height);
		}
		else
		{
			error = kernels[index].kernel.setArg(0, cl_flag);
		}
		
	}
	catch (cl::Error er) {
		printf("ERROR: %s(%s)\n", er.what(), oclErrorString(er.err()));
	}

	printf("Running kernel: %u \n \n", index);
	try
	{
		
		error = queue.enqueueNDRangeKernel(kernels[index].kernel, cl::NullRange,
			cl::NDRange(1),
			cl::NullRange,
			NULL, &(kernels[index].event));
		
		kernels[index].event.wait();
		cl_ulong start =
			kernels[index].event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		cl_ulong end =
			kernels[index].event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
		double time = 1.e-9 * (end - start);
		double startTime = start * 1.e-9;
		double endTime = end * 1.e-9;
		std::cout << " Kernel running time: " << time << std::endl;

	}
	catch (cl::Error er) {
		printf("ERROR: %s(%s)\n @ enqueueNDRangeKernel ", er.what(), oclErrorString(er.err()));
	}
}

void OCLConnector::runKernels()
{
	/*
	cl::Event user_event[3];
	//cl_event events[3];
	for (int i = 0; i < 3;i++)
	{
		user_event[i] = clCreateUserEvent(context(), &error);
		events[i] = user_event[i]();
	}
	std::vector<cl::Event> v1;
	v1.push_back(events[2]);
	std::vector<cl::Event> v2;
	v2.push_back(events[2]);*/
	for (int i = 0; i < 3; i++)
	{
		try
		{
			error = kernels[i].kernel.setArg(0, cl_flag);
		}
		catch (cl::Error er) {
			printf("ERROR: %s(%s)\n", er.what(), oclErrorString(er.err()));
		}
		
		printf("Running kernel: %u \n \n", i);
		try
		{
			
			size_t size = 1;
			error = queue.enqueueNDRangeKernel(kernels[i].kernel, cl::NullRange,
				cl::NDRange(1),
				cl::NullRange,
				NULL, &(kernels[i].event));

		}
		catch (cl::Error er) {
			printf("ERROR: %s(%s)\n @ enqueueNDRangeKernel ", er.what(), oclErrorString(er.err()));
		}
	}
	//clSetUserEventStatus(v1[0](), CL_COMPLETE);
	for (int i = 0; i < 3; ++i) {
		//queues[i].finish();
	}
	/*
	clSetUserEventStatus(events[0], CL_COMPLETE);
	clSetUserEventStatus(events[1], CL_COMPLETE);
	clSetUserEventStatus(events[2], CL_COMPLETE);
	clWaitForEvents(3, events);
	
	for (int i = 0; i < 3; ++i) {
		cl_ulong start =
			kernels[i].event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		cl_ulong end =
			kernels[i].event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
		double time = 1.e-9 * (end - start);
		double startTime = start * 1.e-9;
		double endTime = end * 1.e-9;
		std::cout << i << ". kernel running time: " << time << std::endl;
	}*/
	
}
