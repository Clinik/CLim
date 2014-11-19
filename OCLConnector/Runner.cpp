#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>

#include <GL/glew.h>
#include <GL/glut.h>

#include <vector>
#include <map>

#define OCL_PROFILING
#include "OCLConnector.h"
#include "CLim.h"

#include "ImOperation.h"
#include "Multioperation.h"

using namespace clim;

#include <thread>

int main() {
	OCLConnector clContext;
	ImOperation imOperation1;
	ImOperation imOperation2;
	MultiOperation junction;

	// Init source images
	clim::CLim<int> image1(3, 3);
	image1.set_all(1);
	clim::CLim<int> image2(3, 3);
	image2.set_all(2);
	// Add operations into the context
	clContext.addSequence(imOperation1);
	clContext.addSequence(imOperation2);
	clContext.addSequence(junction);
	// Set image sources
	imOperation1.addDataSource(image1);
	imOperation2.addDataSource(image2);
	// junction dependencies
	junction.addDataSource(imOperation1);
	junction.addDataSource(imOperation2);
	// Invoke context
	clContext.execute();
	
	system("pause");
}