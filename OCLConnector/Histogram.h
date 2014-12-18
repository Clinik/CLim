#ifndef ImOperation_H
#define ImOperation_H

#include "CLSequence.h"

class Histogram : public CLSequence{
public:
	void addKernels()
	{
		addKernel("histogram", { "histogram.cl" }, false, NDRange(1, 1), NDRange(1, 1));
	}

};

#endif