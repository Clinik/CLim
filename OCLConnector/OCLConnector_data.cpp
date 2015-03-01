#include <stdio.h>
#include <iostream>
#include "GL/glew.h"

#include "CLConnector.h"
#include "CLSequence.h"
#include "CLGraph.h"

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
	// Setup the sequence's kernels
	for each (CLSequence *sequence in sequences)
	{
		sequence->addKernels();
	}

	// Make the connections between the sequences
	for each (CLGraph::Transmitter* transmitter in CLGraph::edges)
	{
		transmitter->attach();
	}
	// Run the sequences
	for each (CLSequence *sequence in sequences)
	{
		sequence->execute();
		sequence->postExecute();
	}
}
