#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>

#include "GL/glew.h"
#include "GL/glut.h"

#include <vector>
#include <map>
#include <string>
#include <time.h>

#include "SSH.h"
/*
#define OCL_PROFILING
#include "CLim.h"
#include "CLConnector.h"

#include "ImOperation.h"
#include "Multioperation.h"
#include "PlainOperation.h"
#include "ImSubtract.h"
/*
using namespace clim;

void cpuSubtract(clim::CLim<unsigned char> &im1, clim::CLim<unsigned char> &im2) {
	clock_t t1, t2;
	t1 = clock();

	for (int i = 0; i < im1.size(); i++)
	{
		char newValue = 0;
		if (im1._data[i] > im2._data[i])
		{
			newValue = im1._data[i] - im2._data[i];
		}
	}
			
	t2 = clock();
	float diff((float)t2 - (float)t1);
	printf("CPU runtime: %f seconds \n\n", diff / CLOCKS_PER_SEC);
}

void subtractExample() {
	CLConnector clContext;
	

	clim::CLim<unsigned char> image1(std::string("../images/lena.jpg"));
	clim::CLim<unsigned char> image2(std::string("../images/lenaR.jpg"));
	
	ImSubtract subtract(std::string("subtract"), NDRange(510, 510), NDRange(1, 1));
	//ImSubtract subtract2(std::string("subtract2"), NDRange(510, 510, 3), NDRange(1, 1, 1));
	clContext.addSequence(subtract);
	//clContext.addSequence(subtract2);

	subtract.addImages(image1, image2);
	//subtract2.addImages(image1, image2);

	clContext.execute();
		
	cpuSubtract(image1, image2);
}
*/
int main() {

	//subtractExample();
	SSH ssh;
	ssh.doit();
	system("pause");
}

