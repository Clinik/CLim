#ifndef CLIM_KERNEL
#define CLIM_KERNEL

#include <string>

#include "CL/cl.hpp"
#include "CLim.h"
#include "Misc.h"

class OCLConnector;

class CLimKernel
{
public:

	typedef struct clim_mem_object {

		cl::Buffer cl_data;
		int width;
		int height;

	} clim_mem_object;

	clim_mem_object mem_object;
	std::string source;
	cl::Kernel kernel;
	cl::Event event;

	CLimKernel() : kernel(NULL), event(NULL){};
	CLimKernel(std::string s, cl::Kernel k, cl::Event e)
		: source(s), kernel(k), event(e) {};
	~CLimKernel(){};

	void valami(){};

	template <typename T>
	void loadImageForCL(clim::CLim<T> &image, OCLConnector &connector){
		cl_int error;
		clim_mem_object o;
		
		try
		{
			o.cl_data = cl::Buffer(connector.context, CL_MEM_READ_WRITE, image.size() * sizeof(T), NULL, &error);

			printf("Adat -> GPU\n");

			error = connector.queue.enqueueWriteBuffer(o.cl_data, CL_TRUE, 0, image.size() * sizeof(T), &image._data[0], NULL, &event);
		}
		catch (cl::Error er) {
			printf("ERROR @ loadData: %s(%s)\n", er.what(), oclErrorString(er.err()));
		}

		o.width = image._width;
		o.height = image._height;
		mem_object = o;
	}
};

#endif

