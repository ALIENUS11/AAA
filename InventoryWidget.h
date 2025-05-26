#pragma once

#include <QWidget>
#include "ui_InventoryWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InventoryWidgetClass; };
QT_END_NAMESPACE

class InventoryWidget : public QWidget
{
	Q_OBJECT

public:
	InventoryWidget(QWidget *parent = nullptr);
	~InventoryWidget();

	void Init();

public slots:
	void OnDataReceived(const QByteArray &data);
	void Refresh();

private:
	Ui::InventoryWidgetClass *ui;
};
