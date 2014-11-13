#include <stdio.h>
#include <iostream>
#include "GL/glew.h"

#include "OCLConnector.h"
//#include "Misc.h"

#include <vector>
#include <iosfwd>

void OCLConnector::addSequence(OCLSequence &sequence) {
	//sequence.connector = this;
	sequences.push_back(&sequence);
}

void OCLConnector::removeSequence(OCLSequence &sequence) {
	sequences.remove_if(
		[&](const OCLSequence *current){
		return(current == &sequence);
	});
}
