#include "CLGraph.h"
#include "ImSubtract.h"
#include "PlainOperation.h"
#include "Histogram.h"

class SSH {

public:

	void doit() {

		ImSubtract subtract(std::string("subtract"), NDRange(510, 510), NDRange(1, 1));
		clim::CLim<unsigned char> image1(std::string("../images/lena.jpg"));
		clim::CLim<unsigned char> image2(std::string("../images/lenaR.jpg"));

		PlainOperation plainOp;
		
		Histogram hist;

		CLGraph::Node nSubtract(subtract);
		CLGraph::Node nPlain(plainOp);
		CLGraph::Node nHist(hist);

		subtract.addImages(image1, image2);

		nSubtract.connect(nPlain);
		nSubtract.connect(nHist);
		CLGraph::execute();
	}

};