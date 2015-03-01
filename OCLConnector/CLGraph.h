#include <vector>
#include "CLSequence.h"
#include "CLConnector.h"


/// <summary>	A cl graph representing the sequences as nodes and connections between them. </summary>
class CLGraph {
public:
	class Transmitter;


	static CLConnector CLGraph::clContext;
	static std::vector<Transmitter*> CLGraph::edges;

	CLGraph();
	
	/// <summary>	A node containing a sequence. </summary>
	class Node {
		public:
			CLSequence *sequence;

		public:
			Node(CLSequence &sequence);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Connects the given node with this node. </summary>
			///
			/// <param name="node">	[in,out] The node to connect. </param>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			void connect(CLGraph::Node &node);

	};

	/// <summary>	A transmitter acting as an edge between sequences. </summary>
	class Transmitter {
		public:
			CLGraph::Node* from;
			CLGraph::Node* to;
		public:
			Transmitter(CLGraph::Node *n1, CLGraph::Node *n2) {
				this->from = n1;
				this->to = n2;
			}
			
			/// <summary>	
			/// 		Attaches the edge's nodes together by mapping the sources's outputs with the sink's inputs. 
			/// </summary>
			void attach();
	};

	
public:

	/// <summary>	Executes the graph. </summary>
	static void execute();
};