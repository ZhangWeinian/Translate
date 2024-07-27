#pragma once

#include "stdafx.h"

#include "TranslateGUI.h"
#include "ui_TranslateGUI.h"

#include <memory>
#include <string>
#include <vector>
#include <version>

#include <qmainwindow.h>
#include <qwidget.h>

TranslateGUI::TranslateGUI(QWidget *parent): QMainWindow(parent), ui(_STD make_unique<Ui::TranslateGUIClass>())
{
	ui->setupUi(this);
}
