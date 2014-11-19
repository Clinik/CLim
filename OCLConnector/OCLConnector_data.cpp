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

	std::vector<std::thread> threads;
	int i = 0;
	for each (OCLSequence *sequence in sequences)
	{
		std::thread t(&OCLSequence::initKernelArgs, sequence, i==0 ? true : false);
		i++;
		threads.push_back(std::move(t));
	}
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}
	
	for each (OCLSequence *sequence in sequences)
	{
		sequence->execute();
	}
}
