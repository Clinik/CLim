#ifndef QTIM_H
#define QTIM_H

#include <QtGui/QImage>
#include "ui_climdisplay.h"

using namespace clim;

template<typename T>
class QTim : public QImage {

public:
	CLim<T> CLimage;

	QTim() {

	}

	QTim(CLim<T> CLimage) {
		this->CLimage = CLimage;
	}

	void loadFile(const std::string &fileName) {
		CLimage.load_from_file(fileName);
	}

	void writeFile(const char* fileName)
	{
		CLimage.write_to_file(fileName);
	}

	QImage getQImage() {
		QImage qimage(CLimage._width, CLimage._height, QImage::Format_RGB32);

		for (int i = 0; i < CLimage._width; i++)
		{
			for (int j = 0; j < CLimage._height; j++)
			{
				QRgb rgb = qRgb(CLimage(i, j, 0),
					CLimage(i, j, 1),
					CLimage(i, j, 2));
				qimage.setPixel(i, j, rgb);
			}
		}
		
		return qimage;
	}

	QImage getQGrayImage() {
		QImage qimage(CLimage._width, CLimage._height, QImage::Format_RGB32);

		for (int i = 0; i < CLimage._width; i++)
		{
			for (int j = 0; j < CLimage._height; j++)
			{
				QRgb rgb = qRgb(CLimage(i, j, 0),
					CLimage(i, j, 0),
					CLimage(i, j, 0));
				qimage.setPixel(i, j, rgb);
			}
		}

		return qimage;
	}

	QPixmap getDisplay(QImage::Format format) {
		QImage image;

		if (QImage::Format_RGB32 == format) {
			image = getQImage();
		} else 
		if (QImage::Format_Indexed8 == format)
		{
			image = getQGrayImage();
		}
		
		return QPixmap::fromImage(image);
	}

	QTim<T> toGrayScale() {
		if (CLimage._data)
		{
			return CLimage.toGrayScale();
		}
	}

};

#endif