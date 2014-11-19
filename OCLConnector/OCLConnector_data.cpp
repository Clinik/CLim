#include <stdio.h>
#include <iostream>
#include "GL/glew.h"

#include "OCLConnector.h"
#include "OCLSequence.h"

#include <thread>
#include <vector>
#include <iosfwd>

void OCLConnector::addSequence(OCLSequence &sequence) {
	sequence.connector = this;
	sequences.push_back(&sequence);
}

void OCLConnector::removeSequence(OCLSequence &sequence) {
	sequences.remove_if(
		[&](const OCLSequence *current){
		return(current == &sequence);
	});
}

void OCLConnector::execute() {

	for each (OCLSequence *sequence in sequences)
	{
		sequence->addKernels();
	}

	for each (OCLSequence *sequence in sequences)
	{
		sequence->execute();
	}
}
