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
//#include "OCLSequence.h"
#include "CLim.h"

#include "Affine.h"

using namespace clim;

int main() {
	/*
	Affine oclSequence;
	Affine oclSequence3;

	OCLSequence seq;
	seq.addTask(oclSequence);
	seq.addTask(oclSequence3);
	//oclSequence.execute();
	printf("size: %d\n", seq.sequence.size());
	seq.removeTask(oclSequence3);
	printf("size: %d\n", seq.sequence.size());

	oclSequence.execute();*/
	OCLConnector c;

	Affine a;
	a.setConnector(c);
	c.addSequence(a);
	a.addKernels();
	a.execute();
	system("pause");
}