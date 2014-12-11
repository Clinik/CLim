#include "CLSequence.h"
#include "CLim.h"
#include "CLimKernel.h"
#include <vector>
#include <map>

class ImSubtract : public CLSequence {

	unsigned char *data;
	int outSize;

	int outWidth, outHeight;

	std::string name;
	std::string fileName;
	NDRange global;
	NDRange local;

public:
	ImSubtract(const std::string kernelname, const NDRange &global, const NDRange &local) {
		name = kernelname.c_str();
		fileName = (name + ".cl").c_str();
		this->global = global;
		this->local = local;
	}


	template<typename T>
	void addImages(clim::CLim<T> im1, clim::CLim<T> im2) {
		dataSources.push_back(addDataSource(im1));
		dataSources.push_back(addDataSource(im2));
		outWidth = im1._width;
		outHeight = im1._height;
		outSize = im1._width * im1._height*3;
		data = new unsigned char[outSize];
	}

	void addKernels()
	{
		addKernel(name.c_str(), { fileName.c_str() }, true, global, local);
		
		// Input
		kernelToMemObj.insert(std::make_pair(&dataSources[0], &kernels[0]));
		kernelToMemObj.insert(std::make_pair(&dataSources[1], &kernels[0]));
		// Output
		addKernelArg(kernels[0], &data[0], outSize, 2, CL_MEM_READ_ONLY);
	}

	void postExecute() {
		unsigned char *d = new unsigned char[outSize];
		cl_int error;
		
		clFinish(connector->queue);
		error = clEnqueueReadBuffer(connector->queue,
		kernelOutputs[0], CL_TRUE, 0, outSize * sizeof(unsigned char), d, 0, NULL, NULL);
		
		stbFile f;
		std::string fileName = ("../images/" + name + ".png");
		f.write_to_file(std::string(fileName.c_str()), outWidth, outHeight, 3, d);
	}


};