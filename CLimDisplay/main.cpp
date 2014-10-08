#include  <iostream>

#include "climdisplay.h"
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QDebug>

#define clim_converters "modules/CLimConverters.h"
#include "CLim.h"
#include "QTim.h"

using namespace clim;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTim<unsigned char> qtIm;
	qtIm.loadFile("lena.jpg");

	QLabel imageContainer;
	imageContainer.setPixmap(qtIm.getDisplay(QImage::Format_RGB32));
	imageContainer.show();
	
	QTim<unsigned char> qtImGray = qtIm.toGrayScale();

	QLabel imageContainerGray;
	imageContainerGray.setPixmap(qtImGray.getDisplay(QImage::Format_Indexed8));
	imageContainerGray.show();
	
	qtImGray.writeFile("gray.png");
	return a.exec();
}
