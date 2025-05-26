#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include "MyDataBase.h"
class InputDialog : public QDialog {
	Q_OBJECT

public:
	struct FormData {
		QString barcodeID;
		QString name;
		double price;
		int quantity;
		QString supplier;
	};

	explicit InputDialog(const QString& barCodeID = "",QWidget * parent = nullptr)
		: QDialog(parent) {
		setWindowTitle(QStringLiteral("填写数据"));

		// 创建控件
		QLabel* labelBarcode = new QLabel(QStringLiteral("ID:"));
		QLineEdit* editBarcode = new QLineEdit;
		if (!barCodeID.isEmpty())
		{
			editBarcode->setText(barCodeID);
		}

		QLabel* labelName = new QLabel(QStringLiteral("名称:"));
		QLineEdit* editName = new QLineEdit;

		QLabel* labelPrice = new QLabel(QStringLiteral("单价:"));
		QLineEdit* editPrice = new QLineEdit;

		QLabel* labelQuantity = new QLabel(QStringLiteral("数量:"));
		QLineEdit* editQuantity = new QLineEdit;

		QLabel* labelSupplier = new QLabel(QStringLiteral("供应商:"));
		QComboBox* editSupplier = new QComboBox;
		// 添加供应商选项
		auto supplierList = MyDataBase::GetInstance().GetSuppliers();
		for (const auto& supplier : supplierList)
		{
			editSupplier->addItem(supplier.name);
		}
		// 设置默认选项
		editSupplier->setCurrentIndex(0);

		// 按钮
		QPushButton* buttonOk = new QPushButton(QStringLiteral("确定"));
		QPushButton* buttonCancel = new QPushButton(QStringLiteral("取消"));

		// 布局
		QVBoxLayout* mainLayout = new QVBoxLayout;
		QHBoxLayout* buttonLayout = new QHBoxLayout;

		mainLayout->addWidget(labelBarcode);
		mainLayout->addWidget(editBarcode);
		mainLayout->addWidget(labelName);
		mainLayout->addWidget(editName);
		mainLayout->addWidget(labelPrice);
		mainLayout->addWidget(editPrice);
		mainLayout->addWidget(labelQuantity);
		mainLayout->addWidget(editQuantity);
		mainLayout->addWidget(labelSupplier);
		mainLayout->addWidget(editSupplier);

		buttonLayout->addStretch();
		buttonLayout->addWidget(buttonOk);
		buttonLayout->addWidget(buttonCancel);
		mainLayout->addLayout(buttonLayout);

		setLayout(mainLayout);

		// 信号与槽
		connect(buttonOk, &QPushButton::clicked, this, [this, editBarcode, editName, editPrice, editQuantity, editSupplier]() {
			formData.barcodeID = editBarcode->text();
			formData.name = editName->text();
			formData.price = editPrice->text().toDouble();
			formData.quantity = editQuantity->text().toInt();
			formData.supplier = editSupplier->currentText();
			accept();
			});

		connect(buttonCancel, &QPushButton::clicked, this, &QDialog::reject);
	}


	FormData getFormData() const {
		return formData;
	}
	// 创建对话框函数
	static InputDialog* createInputDialog(const QString& barCodeID = "", QWidget* parent = nullptr) {
		return new InputDialog(barCodeID,parent);
	}

private:
	FormData formData;
};



//只添加数量的对话框
class QuantityDialog : public QDialog {
	Q_OBJECT
public:
	struct FormData {
		int quantity;
	};
	explicit QuantityDialog(QWidget* parent = nullptr)
		: QDialog(parent) {
		setMinimumSize(300, 100);
		setWindowTitle(QStringLiteral("已存在商品添加"));
		// 创建控件
		QLabel* labelQuantity = new QLabel(QStringLiteral("数量:"));
		QLineEdit* editQuantity = new QLineEdit;
		// 按钮
		QPushButton* buttonOk = new QPushButton(QStringLiteral("确定"));
		QPushButton* buttonCancel = new QPushButton(QStringLiteral("取消"));
		// 布局
		QVBoxLayout* mainLayout = new QVBoxLayout;
		QHBoxLayout* buttonLayout = new QHBoxLayout;
		mainLayout->addWidget(labelQuantity);
		mainLayout->addWidget(editQuantity);
		buttonLayout->addStretch();
		buttonLayout->addWidget(buttonOk);
		buttonLayout->addWidget(buttonCancel);
		mainLayout->addLayout(buttonLayout);
		setLayout(mainLayout);
		// 信号与槽
		connect(buttonOk, &QPushButton::clicked, this, [this, editQuantity]() {
			formData.quantity = editQuantity->text().toInt();
			accept();
			});
		connect(buttonCancel, &QPushButton::clicked, this, &QDialog::reject);
	}
	FormData getFormData() const {
			return formData;
		}

	static QuantityDialog* createQuantityDialog(QWidget* parent = nullptr) {
		return new QuantityDialog(parent);
	}
private:
	FormData formData;
};


// 创建对话框函数

#endif