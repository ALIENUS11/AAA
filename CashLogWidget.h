#pragma once

#include <QWidget>
#include "ui_CashLogWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CashLogWidgetClass; };
QT_END_NAMESPACE

class CashLogWidget : public QWidget
{
	Q_OBJECT

public:
	CashLogWidget(QWidget *parent = nullptr);
	~CashLogWidget();
	
	void Load();

public slots:
	void Refresh();
private:
	Ui::CashLogWidgetClass *ui;
};
