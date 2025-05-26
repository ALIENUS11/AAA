#pragma once

#include <QWidget>
#include "ui_Suppier.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Supplier; };
QT_END_NAMESPACE

class SupplierWidget  : public QWidget
{
	Q_OBJECT

public:
	SupplierWidget(QWidget *parent = nullptr);
	~SupplierWidget();

	void setAdmin(bool isAdmin);

private:
	Ui::Supplier* ui;
	bool isAdmin = false;

};
