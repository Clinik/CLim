#include "CLGraph.h"
#include "ImSubtract.h"
#include "PlainOperation.h"
#include "RGBtoGray.h"
#include "Histogram.h"

class SSH {

public:

	void doit() {

		ImSubtract subtract(std::string("subtract"), NDRange(512, 512), NDRange(1, 1));
		clim::CLim<unsigned char> image1(std::string("../images/lena2.jpg"));
		clim::CLim<unsigned char> image2(std::string("../images/lena2R.jpg"));

		PlainOperation plainOp;
		RGBtoGray grayOp;
		Histogram hist;

		CLGraph::Node nSubtract(subtract);
		CLGraph::Node nGray(grayOp);
		CLGraph::Node nPlain(plainOp);
		CLGraph::Node nHist(hist);

		subtract.addImages(image1, image2);

		nSubtract.connect(nGray);
		nGray.connect(nPlain);
		nGray.connect(nHist);
		CLGraph::execute();
	}

};