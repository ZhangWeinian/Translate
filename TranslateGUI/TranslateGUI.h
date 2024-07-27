#pragma once

#include "stdafx.h"

#include "ui_TranslateGUI.h"

#include <memory>
#include <version>

#include <QtWidgets/QMainWindow>
#include <qmainwindow.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>



#ifndef _STD
	#define _STD ::std::
#endif	// !_STD



QT_BEGIN_NAMESPACE

namespace Ui
{
	class TranslateGUIClass;
};	// namespace Ui

QT_END_NAMESPACE

class TranslateGUI: public QMainWindow
{
	Q_OBJECT

public:
	TranslateGUI(QWidget *parent = nullptr);

	~TranslateGUI() = default;

private:
	_STD unique_ptr<Ui::TranslateGUIClass> ui;
};
