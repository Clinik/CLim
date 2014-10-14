#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/glut.h>
#include "OCLConnector.h"
#include "CLim.h"

using namespace clim;

/*
*	OpenCL "loader" osztaly
*/

int main() {

	// OpenCL osztaly letrehozasa
	OCLConnector oclConnector;

	CLim<uint> image(200, 100, 3);
	image.set_all(200);

	oclConnector.addKernel("kernel1", { "Kernels/data/CLim.hcl", "Kernels/kernel.cl" });
	oclConnector.kernels[0].loadImageForCL(image, oclConnector);
	//oclConnector->addKernel(kernelSource, "kernel2");
	//oclConnector->addKernel(kernelSource, "kernel3");
	
//	oclConnector.loadData(2);

	for (int i = 0; i < 1; i++)
	{
		oclConnector.runKernel(i);
	}
	
	system("pause");
}