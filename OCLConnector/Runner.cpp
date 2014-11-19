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
#include "SeqOperation.h"

using namespace clim;

#include <thread>

int main() {
	OCLConnector clContext;
	ImOperation imOperation;
	SeqOperation seqOperation;
	
	clContext.addSequence(seqOperation);
	clContext.addSequence(imOperation);
	

	clim::CLim<int> image(200, 100, 3);
	image.set_all(200);
	imOperation.addDataSource(image);

	//seqOperation.addDataSource(imOperation);
	seqOperation.addDataSource(image);
	clContext.execute();
	
	system("pause");
}