#include <vector>
#include "CLSequence.h"
#include "CLConnector.h"


class CLGraph {
public:
	class Transmitter;


	static CLConnector CLGraph::clContext;
	static std::vector<Transmitter*> CLGraph::edges;

	CLGraph();
	
	class Node {
		public:
			CLSequence *sequence;

		public:
			Node(CLSequence &sequence);

			void connect(CLGraph::Node &node);

	};

	class Transmitter {
		public:
			CLGraph::Node* from;
			CLGraph::Node* to;
		public:
			Transmitter(CLGraph::Node *n1, CLGraph::Node *n2) {
				this->from = n1;
				this->to = n2;
			}
			
			void attach();
	};

	
public:

	static void execute();
};