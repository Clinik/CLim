#include "CLGraph.h"
#include "CLSequence.h"

CLConnector CLGraph::clContext;
std::vector<CLGraph::Transmitter*> CLGraph::edges;

CLGraph::Node::Node(CLSequence &sequence) {
	this->sequence = &sequence;
	CLGraph::clContext.addSequence(sequence);
}

void CLGraph::Node::connect(CLGraph::Node &node) {
	CLGraph::Transmitter *transmitter = new CLGraph::Transmitter(this, &node);
	CLGraph::edges.push_back(transmitter);

}

void CLGraph::Transmitter::attach() {
	for each (CLSequence::clim_mem_object mem in this->from->sequence->getKernelOutputs())
	{
		to->sequence->addDataSource(mem.cl_data, 512, 512);
	}
}

void CLGraph::execute() {
	CLGraph::clContext.execute();
}

