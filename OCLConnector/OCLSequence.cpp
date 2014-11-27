#include "CLSequence.h"

void CLSequence::setConnector(CLConnector &connector) {
	this->connector = &connector;
}

void CLSequence::setImageKernelArgs() {
	/*
	for (auto& mapping : kernelToMemObj)
	{
		addKernelArg(*(mapping.second), (mapping.first->cl_data));
	}*/
	for each (CLimKernel kernel in kernels)
	{
		if (kernel.isImageKernel)
		{
			addKernelArg(kernel, dataSources[0].cl_data);
			addKernelArg(kernel, dataSources[1].cl_data);
			//addKernelArg(kernel, dataSources[0].width, 1);
			//addKernelArg(kernel, dataSources[0].height, 2);
		}
	}

}

void CLSequence::addKernelArg(CLimKernel &kernel, cl_mem argPtr, size_t argIndex) {

	cl_int error;

	printf("Setting buffer arg\n");

	if (argIndex == NULL) {
		argIndex = kernel.numArgs;
	}

	error = clSetKernelArg(kernel.kernel, argIndex, sizeof(cl_mem), &argPtr);
	if (error != CL_SUCCESS) {
		printf("ERROR: Failed to set kernel arguments!\n \t %s \n", oclErrorString(error));
	}

	printf("Kernel arguments[%d] successfully set!\n\n", kernel.numArgs);
	kernel.numArgs = kernel.numArgs + 1;
}

void CLSequence::addKernel(const char* kernelName, 
	const std::initializer_list<const char*> sources, 
	bool isImageKernel, 
	const size_t workDim, size_t globalSize[], size_t localSize[]) {

	std::string src = "";

	for (const char* srcPart : sources) {
		std::string fullPath = std::string(kernelPath) + std::string(srcPart);
		std::ifstream sourceFile(fullPath.c_str());
		std::string kernelSource(
			std::istreambuf_iterator<char>(sourceFile),
			(std::istreambuf_iterator<char>()));
		src += kernelSource;
	}
	const char* cstr_src = src.c_str();
	CLimKernel kernel = makeKernelFromSource(cstr_src, kernelName);
	kernel.setWorkSizes(workDim, globalSize, localSize);
	if (isImageKernel)
	{
		kernel.isImageKernel = isImageKernel;
	}
	
	kernels.push_back(kernel);
}

void CLSequence::runKernel(CLimKernel &kernel) {
	printf("Running kernel: %s...\n", kernel.name);
	cl_int error;

	size_t globalThreads[] = { 3, 3 };
	size_t localThreads[] = { 3, 1 };

	cl_event event;
	error = clEnqueueNDRangeKernel(
		connector->queue, kernel.kernel, 
		2,
		NULL,
		globalThreads, localThreads,
		0, NULL, &event);
	
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
	printf("Profiling not enabled, to display the kernel run time: #define OCL_PROFILING before including this file. (It implies event waiting...)\n\n");
#endif // OCL_PROFILING
}

CLimKernel CLSequence::makeKernelFromSource(const char* kernel_src, const char* kernelName) const {
	cl_int error;

	cl_program program;
	CLimKernel climKernel;

	/************************************************************************/
	/* Make a program from the kernel source                                */
	/************************************************************************/
	printf("Making program from source...\n");
	const char *source = kernel_src;
	program = clCreateProgramWithSource(connector->context, 1, &source, NULL, &error);
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
	error = clBuildProgram(program, 1, &(connector->deviceUsed), NULL, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];

		printf("Error: Failed to build program, displaying build log:\n");
		clGetProgramBuildInfo(program, connector->deviceUsed, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
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

void CLSequence::initKernelArgs() {
	for each (CLimKernel kernel in kernels)
	{
		setImageKernelArgs();
	}

}

void CLSequence::execute() {
	initKernelArgs();
	for each (CLimKernel kernel in kernels)
	{
		runKernel(kernel);
	}
}