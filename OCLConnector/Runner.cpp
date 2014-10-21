#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/glut.h>

#define OCL_PROFILING
#include "OCLConnector.h"
#include "CLim.h"

using namespace clim;

int main() {

	OCLConnector oclConnector;

	CLim<uint> image(200, 100, 3);
	image.set_all(200);

	oclConnector.addKernel("kernel1", { "Kernels/data/CLim.hcl", "Kernels/kernel.cl" });
	oclConnector.loadImageForCL(image);

		oclConnector.runKernel(0);
	
	system("pause");
}