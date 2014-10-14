#include <cmath>
#include <algorithm>
#include <string>

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

//#include <glew.h>
using namespace std;

// The following defines specialized templates to provide a string
// containing the typename
template<class T>
struct TypeName {
	string getName();
private:
	T *t;
};

template<> string TypeName<double>::getName() { return(string("double")); }
template<> string TypeName<float>::getName() { return(string("float")); }
template<> string TypeName<unsigned long>::getName() { return(string("ulong")); }
template<> string TypeName<long>::getName() { return(string("long")); }
template<> string TypeName<unsigned int>::getName() { return(string("uint")); }
template<> string TypeName<int>::getName() { return(string("int")); }
template<> string TypeName<unsigned char>::getName() { return(string("uchar")); }
template<> string TypeName<char>::getName() { return(string("char")); }

// specification of the OclTest template
template <typename TYPE1>
class OclTest {
private:
	// Experiment with RNG and performance
	// performance on various devices (CPU, GPU, etc)
	cl::Kernel kernel;
	cl_int nTests;
	cl_int vLen, vSize;
	TYPE1  *h_oneCnt;
	cl::Buffer d_oneCnt;
	string myType;
	cl::Event event;
	long seed1, seed2;
	int seedOffset;
	unsigned long  nIter;

	// used to calculate entropy
	const double log2of10 = 3.32192809488736234787;
	inline double rt_log2(double x) {
		return log2of10 * log10(x);
	}

public:
	cl::Event *getEventPtr() { return &event; }
	OclTest() {}
	OclTest(cl::CommandQueue& queue, const char* kernelFile,
		int argc, char *argv[])
	{
		cl::Device device = queue.getInfo<CL_QUEUE_DEVICE>();
		cl::Context context = queue.getInfo<CL_QUEUE_CONTEXT>();
		myType = TypeName<TYPE1>().getName();
		cout << "My type is " << myType.c_str() << endl;

		if (argc < 4) {
			cerr << "Ocl kernel use: nTests seed1 seed2 nIterPerTest" << endl;
			exit(EXIT_FAILURE);
		}
		nTests = atol(argv[0]);
		seed1 = atol(argv[1]) + seedOffset;
		seed2 = atol(argv[2]) + seedOffset;
		nIter = atol(argv[3]);
		seedOffset = 0;

		vLen = nTests;
		vSize = vLen * sizeof(TYPE1);

		// Demonstrate using defines in the ocl build
		string buildOptions;
		{ // create preprocessor defines for the kernel
			char buf[256];
			sprintf(buf, "-D TYPE1=%s ", myType.c_str());
			buildOptions += string(buf);
		}
		
		// build the program from the source in the file
		ifstream file(kernelFile);
		string prog(istreambuf_iterator<char>(file),
			(istreambuf_iterator<char>()));
		cl::Program::Sources source(1, make_pair(prog.c_str(),
			prog.length() + 1));
		cl::Program program(context, source);
		file.close();

		try {
			cerr << "buildOptions " << buildOptions << endl;
			cl::vector<cl::Device> foo;
			foo.push_back(device);
			program.build(foo, buildOptions.c_str());
		}
		catch (cl::Error& err) {
			// Get the build log
			cerr << "Build failed! " << err.what()
				<< '(' << err.err() << ')' << endl;
			cerr << "retrieving  log ... " << endl;
			cerr
				<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
				<< endl;
			exit(-1);
		}
		//Get the name of the kernel from the filename
		string kernelName = string(kernelFile)
			.substr(0, string(kernelFile).find(".cl"));
		cerr << "specified kernel: " << kernelName << endl;
		kernel = cl::Kernel(program, kernelName.c_str());

		// set up the kernel inputs
		h_oneCnt = new TYPE1[vLen];
		d_oneCnt = cl::Buffer(context, CL_MEM_READ_WRITE, vSize);

		kernel.setArg(0, nIter);
		kernel.setArg(1, seed1);
		kernel.setArg(2, seed2);
		kernel.setArg(3, d_oneCnt);
	}

	inline void initData(cl::CommandQueue& queue, int seed)
	{
		seedOffset = seed;
	}

	inline cl::Kernel& getKernel() { return(kernel); }
	//NEW methods to return information for queuing work-groups
	cl::NDRange getGlobalWorkItems() {
		return(cl::NDRange(vLen));
	}
	cl::NDRange getWorkItemsInWorkGroup() {
		// Only one work item per workgroup
		return(cl::NDRange(1, 1));
	}

	inline int goldenTest(cl::CommandQueue& queue)
	{
		event.wait();
#ifdef PROFILING
		cl::Device queueDevice = queue.getInfo<CL_QUEUE_DEVICE>();
		std::string deviceName = queueDevice.getInfo<CL_DEVICE_NAME>();
		cl_ulong start =
			event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		cl_ulong end =
			event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
		double time = 1.e-9 * (end - start);
		double startTime = start * 1.e-9;
		double endTime = end * 1.e-9;
		cout << "Kernel (start,end) " << startTime << "," << endTime
			<< " Time for kernel to execute " << time
			<< " device \"" << deviceName << "\"" << endl;
#endif

		// bring data back to the host via a blocking read
		queue.enqueueReadBuffer(d_oneCnt, CL_TRUE, 0, vSize, h_oneCnt);

		// Find the minimum entropy and display some info
		double minBitEntropy = 10;
		double minBitMean = 10;
		for (int i = 0; i < vLen; i++) {
			// calculate entropy and arithmetic mean
			double totalBitCount = 32 * nIter;
			double bitEntropy = 0.;
			double bitProb[2];
			double bitMean = h_oneCnt[i] / totalBitCount;
			bitProb[1] = ((double)h_oneCnt[i]) / totalBitCount;
			bitProb[0] = 1. - bitProb[1];
			if ((bitProb[0] > 0.f) && (bitProb[1] > 0.f)) {
				bitEntropy = bitProb[0] * rt_log2(1. / bitProb[0])
					+ bitProb[1] * rt_log2(1. / bitProb[1]);
			}
			minBitEntropy = min(minBitEntropy, bitEntropy);
			minBitMean = min(minBitMean, bitMean);
		}
		cout << "Minimum bitEntropy " << minBitEntropy
			<< " Minimum bitMean " << minBitMean << endl;
		cout << "Optimum compression would reduce "
			<< "the size of this random bit sequence by "
			<< (1. - minBitEntropy) << '%' << endl;

		if (minBitEntropy >= 0.9999) return(0);
		else return(1);
	}
};