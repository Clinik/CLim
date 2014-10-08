#ifndef CLIMDISPLAY_H
#define CLIMDISPLAY_H

#include <QtWidgets/QApplication>
#include "ui_climdisplay.h"

class CLimDisplay : public QMainWindow
{
	Q_OBJECT

public:
	CLimDisplay(QWidget *parent = 0);
	~CLimDisplay();

private:
	Ui::CLimDisplayClass ui;
};

#endif // CLIMDISPLAY_H
