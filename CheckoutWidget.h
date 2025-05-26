#pragma once

#include <QWidget>
#include "ui_CheckoutWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CheckoutWidgetClass; };
QT_END_NAMESPACE

class CheckoutWidget : public QWidget
{
	Q_OBJECT

public:
	CheckoutWidget(QWidget *parent = nullptr);
	~CheckoutWidget();
	
	void Calculate();
	QStringList CheckStock(); // 新增方法，返回库存不足的商品列表
	//更新库存
	void UpdateStock();
	//清空
	void Clear();
signals:
	void CheckoutFinished();
public slots:
	void OnDataReceived(const QByteArray &data);
private:
	Ui::CheckoutWidgetClass *ui;
};
