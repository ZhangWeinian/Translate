#pragma once

#include "stdafx.h"

#include "TranslateGUI.h"

#include <QtWidgets/QApplication>
#include <qapplication.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	TranslateGUI w;

	w.show();

	return QApplication::exec();
}
