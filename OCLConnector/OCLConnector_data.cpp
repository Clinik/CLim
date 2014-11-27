#include <stdio.h>
#include <iostream>
#include "GL/glew.h"

#include "CLConnector.h"
#include "CLSequence.h"

#include <thread>
#include <vector>
#include <iosfwd>

void CLConnector::addSequence(CLSequence &sequence) {
	sequence.connector = this;
	sequences.push_back(&sequence);
}

void CLConnector::removeSequence(CLSequence &sequence) {
	sequences.remove_if(
		[&](const CLSequence *current){
		return(current == &sequence);
	});
}

void CLConnector::execute() {

	for each (CLSequence *sequence in sequences)
	{
		sequence->addKernels();
	}

	for each (CLSequence *sequence in sequences)
	{
		sequence->execute();
		sequence->postExecute();
	}
}
