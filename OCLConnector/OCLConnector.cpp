#include <stdio.h>
#include <string>
#include <iostream>

#include "OCLConnector.h"
#include "Misc.h"

#if defined WIN32
	#else
	#include <GL/glx.h>
#endif

OCLConnector::OCLConnector()
{
	printf("OpenCL kontextus betoltese\n\n");
	
	// Kerunk egy platformot
	printf("OpenCL platform kerese\n");
	std::vector<cl::Platform> platforms;
	error = cl::Platform::get(&platforms);
	printf("\tcl::Platform[nev]: %s\n", oclErrorString(error));
	printf("\tplatform[meret]: %d\n\n", platforms.size());

	// Karunk a platformtol device-t
	printf("OpenCL platform-tol device kerese\n");
	deviceUsed = 0;
	error = platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
	printf("\tDevicek: %s\n", oclErrorString(error));
	printf("\tDevicek merete: %d\n", devices.size());
	int t = devices.front().getInfo<CL_DEVICE_TYPE>();
	printf("\tTipus: device: %d CL_DEVICE_TYPE_GPU: %d \n\n", t, CL_DEVICE_TYPE_GPU);

	cl_ulong gmsize = devices.front().getInfo< CL_DEVICE_GLOBAL_MEM_SIZE > ();
	printf("\t Global memo merete: %d \n\n", gmsize, CL_DEVICE_TYPE_GPU);

	// Platformtol kerunk egy muveleti kornyezetet
#if defined (__APPLE__) || defined(MACOSX)
	// Apple-vel nem foglalkozok most
#else
#if defined WIN32	// Win 32bit
	printf("Platform: WIN 32bit \n");
	// OpenGL kontextus nelkul
	/*
	cl_context_properties props[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
		0
	};
	*/

	try{
		printf("Kontextus keszitese \n");
		context = cl::Context(CL_DEVICE_TYPE_GPU, NULL);
	}
	catch (cl::Error er) {
		printf("ERROR kontextus keszitese: %s(%d) - %s\n", er.what(), er.err(), oclErrorString(er.err()));
	}
#else	// Win 64bit
	cl_context_properties props[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
		CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
		0
	};
	//cl_context cxGPUContext = clCreateContext(props, 1, &cdDevices[uiDeviceUsed], NULL, NULL, &err);
	try{
		context = cl::Context(CL_DEVICE_TYPE_GPU, props);
	}
	catch (cl::Error er) {
		printf("ERROR: %s(%s)\n", er.what(), oclErrorString(er.err()));
	}
#endif
#endif

	// Parancssor letrehozasa az OpenCL parancsok tovabbitasara
	try{
		
		for (int i = 0; i < 3; i++)
		{
			queue = cl::CommandQueue(context,
				devices[deviceUsed],
				CL_QUEUE_PROFILING_ENABLE,
				&error);
		}
	}
	catch (cl::Error er) {
		printf("ERROR: %s(%d) - %s\n", er.what(), er.err(), oclErrorString(er.err()));
	}

}

