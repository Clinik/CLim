#include "OCLSequence.h"

class OCLJunction : public OCLSequence {

	std::vector<clim_mem_object*> dataSources;

public:

	template<typename T>
	void addDataSource(clim::CLim<T> &dataSource) {
		cl_mem argPtr = allocateGlobalMemobject(dataSource._data, dataSource.size());
		mem_object.construct(argPtr, dataSource._width, dataSource._height);
	}

	void addDataSource(OCLSequence &dataSource) {
		dataSources.push_back(&(dataSource.mem_object));
	}

	void setCurrentSource(clim_mem_object *dataSource) {
		mem_object = *dataSource;
	}

	/*
	* Specialize the execute method to run the kernels on ALL the data sources (a.k.a. images)
	*/
	void execute() {
		// For each source
		for each (clim_mem_object *source in dataSources)
		{
			// Set current source 
			setCurrentSource(source);
			for each (CLimKernel kernel in kernels)
			{
				initKernelArgs();
				runKernel(kernel);
			}
		}
	}

};