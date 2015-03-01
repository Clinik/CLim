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

int main() {

	SSH ssh;
	ssh.doit();
	system("pause");
}

