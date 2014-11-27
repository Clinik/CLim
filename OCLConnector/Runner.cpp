#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>

#include "GL/glew.h"
#include "GL/glut.h"

#include <vector>
#include <map>

#define OCL_PROFILING
#include "CLim.h"
#include "CLConnector.h"

#include "ImOperation.h"
#include "Multioperation.h"
#include "PlainOperation.h"
#include "ImSubtract.h"

using namespace clim;

#include <thread>

void subtractExample() {
	CLConnector clContext;
	

	clim::CLim<int> image1(3, 3);
	image1.set_all(30);
	clim::CLim<int> image2(3, 3);
	image2.set_all(10);
	//image1.load_from_file(std::string("lena.jpg"));
	
	ImSubtract subtract;
	clContext.addSequence(subtract);

	subtract.addImages(image1, image2);
	clContext.execute();
}

int main() {

	subtractExample();
	system("pause");
}

