#include "OCLSequence.h"

void OCLSequence::setConnector(OCLConnector &connector) {
	this->connector = &connector;
}

void OCLSequence::setImageKernelArgs() {
	for each (CLimKernel kernel in kernels)
	{
		if (kernel.isImageKernel)
		{
			addKernelArg(kernel, mem_object.cl_data, 0);
			addKernelArg(kernel, mem_object.width, 1);
			addKernelArg(kernel, mem_object.height, 2);
		}
	}
}

void OCLSequence::addKernelArg(CLimKernel &kernel, cl_mem argPtr, size_t argIndex) {

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

void OCLSequence::addKernel(const char* kernelName, 
	const std::initializer_list<const char*> sources, 
	bool isImageKernel, 
	const size_t workDim, const size_t localSize, const size_t globalSize) {

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
	kernel.setWorkSizes(workDim, localSize, globalSize);
	if (isImageKernel)
	{
		kernel.isImageKernel = isImageKernel;
	}
	
	kernels.push_back(kernel);
}

void OCLSequence::runKernel(CLimKernel &kernel) {
	printf("Running kernel...\n");
	cl_int error;

	cl_event event;
	error = clEnqueueNDRangeKernel(connector->queue, kernel.kernel, 1, NULL, &(kernel.localSize), &(kernel.globalSize), 0, NULL, &event);
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

CLimKernel OCLSequence::makeKernelFromSource(const char* kernel_src, const char* kernelName) const {
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

void OCLSequence::execute() {
	setImageKernelArgs();
	for each (CLimKernel kernel in kernels)
	{
		runKernel(kernel);
	}
}