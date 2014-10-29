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
#include "OCLSequence.h"
#include "CLim.h"

using namespace clim;

int main() {

	OCLSequence oclSequence;
	oclSequence.execute();

	system("pause");
}