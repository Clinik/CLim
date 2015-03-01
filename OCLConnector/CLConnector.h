#ifndef OCL_CONNECTOR_H
#define OCL_CONNECTOR_H
#define OCL_PROFILING
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.h"
#include "CLim.h"
#include "CLimKernel.h"

class CLSequence;

class CLConnector {
public:
	/************************************************************************/
	/* Variables to connect with the OpenCl context                         */
	/************************************************************************/
	cl_device_id deviceUsed;
	cl_context context;
	cl_command_queue  queue;

	/************************************************************************/
	/* Device infos                                                         */
	/************************************************************************/
	cl_int MAX_WORKGROUP_SIZE;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor.
	/// 		Initiating the OpenCL platform, a device on it and a command queue. 
	/// </summary>
	///
	/// <param name="deviceType">	Type of the device. 
	/// 		Possible values:
	/// 			CL_DEVICE_TYPE_CPU,
	///				CL_DEVICE_TYPE_GPU (default)							
	/// </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	CLConnector(cl_device_type deviceType = CL_DEVICE_TYPE_CPU) {
		cl_int error;

		// Get platform info
		cl_platform_id platformsStore[100];
		cl_uint platformsNum = 0;
		error = clGetPlatformIDs(100, platformsStore, &platformsNum);

		printf("Platform count: %d \n", platformsNum);
		for (int i = 0; i < platformsNum; i++)
		{
			char buffer[10240];
			printf("  -- %d --\n", i);
			error = clGetPlatformInfo(platformsStore[i], CL_PLATFORM_PROFILE, 10240, buffer, NULL);
			printf("  PROFILE = %s\n", buffer);
			error = clGetPlatformInfo(platformsStore[i], CL_PLATFORM_VERSION, 10240, buffer, NULL);
			printf("  VERSION = %s\n", buffer);
			error = clGetPlatformInfo(platformsStore[i], CL_PLATFORM_NAME, 10240, buffer, NULL);
			printf("  NAME = %s\n", buffer);
			error = clGetPlatformInfo(platformsStore[i], CL_PLATFORM_VENDOR, 10240, buffer, NULL);
			printf("  VENDOR = %s\n", buffer);
			error = clGetPlatformInfo(platformsStore[i], CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL);
			printf("  EXTENSIONS = %s\n", buffer);
		}
		
		printf("------------------------Loading OpenCL----------------------------\n\n");
		cl_platform_id platforms;

		printf("Getting platform IDs....\n");
		error = clGetPlatformIDs(1, &platforms, NULL);
		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to get platform IDs: %s!\n", oclErrorString(error));
		}
		printf("Platform IDs successfully obtained!\n\n");
		
		initDevice(platforms, deviceType);

	}
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	
	/// 		Initialises the device on the given platform with the specified device type. 	
	/// </summary>
	///
	/// <param name="platforms"> 	The platform. </param>
	/// <param name="deviceType">	Type of the device. </param>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void initDevice(cl_platform_id platforms, cl_device_type deviceType)
	{
		cl_int error;

		// Get the device infos
		cl_device_id devicesStore[100];
		cl_uint devicesNum = 0;

		error = clGetDeviceIDs(platforms, CL_DEVICE_TYPE_ALL, 100, devicesStore, &devicesNum);

		// Print the available devices
		printf("OpenCL devices:\n");
		for (int i = 0; i < devicesNum; i++)
		{
			char buffer[10240];
			cl_uint buf_uint;
			cl_ulong buf_ulong;
			printf("  -- %d \n", i);
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
			printf("  DEVICE_NAME = %s\n", buffer);
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL);
			printf("  DEVICE_VENDOR = %s\n", buffer);
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL);
			printf("  DEVICE_VERSION = %s\n", buffer);
			error = clGetDeviceInfo(devicesStore[i], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL);
			printf("  DRIVER_VERSION = %s\n", buffer);
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL);
			printf("  DEVICE_MAX_COMPUTE_UNITS = %u\n", (unsigned int)buf_uint);
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(buf_uint), &buf_uint, NULL);
			printf("  CL_DEVICE_MAX_WORK_GROUP_SIZE = %u\n", (unsigned int)buf_uint);
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(buf_uint), &buf_uint, NULL);
			printf("  CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS = %u\n", (unsigned int)buf_uint);
			size_t wItemSizes[3];
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(wItemSizes), &wItemSizes, NULL);
			printf("  CL_DEVICE_MAX_WORK_ITEM_SIZES = %u x %u x %u %\n", wItemSizes[0], wItemSizes[1], wItemSizes[2]);
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL);
			printf("  DEVICE_MAX_CLOCK_FREQUENCY = %u\n", (unsigned int)buf_uint);
			error = clGetDeviceInfo(devicesStore[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
			printf("  DEVICE_GLOBAL_MEM_SIZE = %llu\n", (unsigned long long)buf_ulong);
		}

		// Get the device to use
		printf("Getting devices....\n");
		error = clGetDeviceIDs(platforms, deviceType, 1, &deviceUsed, NULL);

		if (error != CL_SUCCESS) {
			printf("ERROR: Failed to get device IDs: %s!\n", oclErrorString(error));
		}
		printf("Devices successfully obtained!\n\n");

		// Create a context on the used device
		printf("Creating context...\n");
		context = clCreateContext(0, 1, &deviceUsed, NULL, NULL, &error);
		if (!context)
		{
			printf("ERROR: Failed to create context!\n");
		}
		printf("Context successfully created\n\n");

		// Create command queue on the context and device
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
public:
	~CLConnector(){};
	
protected:
	/// <summary>	The sequences to run on the command queue.
	/// 		List of sequences defines an order which the kernels will run.
	/// </summary>
	std::list<CLSequence*> sequences;
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a sequence. </summary>
	///
	/// <param name="sequence"> The sequence to add to the sequences list. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void addSequence(CLSequence &sequence);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Removes the sequence described by sequence. </summary>
	///
	/// <param name="sequence">	The sequence to remove from the sequences list. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void removeSequence(CLSequence &sequence);

	void execute();
};

#endif