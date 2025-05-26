#include "Supplier.h"
#include "MyDataBase.h"
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>
SupplierWidget::SupplierWidget(QWidget *parent)
	: QWidget(parent), ui(new Ui::Supplier)
{
	ui->setupUi(this);

	//初始化tableWidget

	ui->tableWidget->setColumnCount(3);
	ui->tableWidget->setHorizontalHeaderLabels(QStringList() << QStringLiteral("供应商名称") << QStringLiteral("供应商地址") << QStringLiteral("供应商电话"));
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择整行
	//拉伸
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	auto suppliers = MyDataBase::GetInstance().GetSuppliers();//// 从数据库中获取所有供应商
	//显示所有供应商
	for (auto& supplier : suppliers)
	{
		int row = ui->tableWidget->rowCount();
		ui->tableWidget->insertRow(row);
		ui->tableWidget->setItem(row, 0, new QTableWidgetItem(supplier.name));
		ui->tableWidget->setItem(row, 1, new QTableWidgetItem(supplier.phone));
		ui->tableWidget->setItem(row, 2, new QTableWidgetItem(supplier.address));

		//居中
		for (int i = 0; i < 3; i++)
		{
			ui->tableWidget->item(row, i)->setTextAlignment(Qt::AlignCenter);
		}
	}

	//添加新的供应商信息
	connect(ui->pb_add, &QPushButton::clicked, [this]()
		{
			QDialog* dialog = new QDialog(this);
			dialog->setWindowTitle(QStringLiteral("添加供应商"));
			QFormLayout* layout = new QFormLayout(dialog);
			//设置对话框布局并添加输入框
			QLineEdit* nameEdit = new QLineEdit(dialog);
			QLineEdit* addressEdit = new QLineEdit(dialog);
			QLineEdit* phoneEdit = new QLineEdit(dialog);
			layout->addRow(QStringLiteral("供应商名称:"), nameEdit);
			layout->addRow(QStringLiteral("供应商地址:"), addressEdit);
			layout->addRow(QStringLiteral("供应商电话:"), phoneEdit);
			//添加确定和取消按钮
			QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
			layout->addWidget(buttonBox);

			//添加供应商的逻辑
			connect(buttonBox, &QDialogButtonBox::accepted, [this, dialog, nameEdit, addressEdit, phoneEdit]
				{
					bool supplier =MyDataBase::GetInstance().AddSupplier(nameEdit->text(), addressEdit->text(), phoneEdit->text());//添加供应商到数据库
					if (supplier)
					{
						//新供应商添加到表格
						int row = ui->tableWidget->rowCount();
						ui->tableWidget->insertRow(row);
						ui->tableWidget->setItem(row, 0, new QTableWidgetItem(nameEdit->text()));
						ui->tableWidget->setItem(row, 1, new QTableWidgetItem(addressEdit->text()));
						ui->tableWidget->setItem(row, 2, new QTableWidgetItem(phoneEdit->text()));
						for (int i = 0; i < 3; i++)
						{
							ui->tableWidget->item(row, i)->setTextAlignment(Qt::AlignCenter);
						}
						ui->tableWidget->setCurrentCell(-1, -1);
						dialog->accept();
					}
					else
					{
						QMessageBox::warning(this, QStringLiteral("添加供应商失败"), QStringLiteral("供应商已存在"));
					}});
			connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
			dialog->exec();

		});

	//删除供应商
	connect(ui->pb_delete, &QPushButton::clicked, [this]()
		{
			int row = ui->tableWidget->currentRow();
			if (row >= 0)
			{
				if (MyDataBase::GetInstance().DeleteSupplier(ui->tableWidget->item(row, 0)->text()) == false)
				{
					QMessageBox::warning(this, QStringLiteral("删除供应商失败"), QStringLiteral("供应商不存在"));
					return;
				}
				else
				{
					ui->tableWidget->removeRow(row);
					ui->tableWidget->setCurrentCell(-1, -1);
					QMessageBox::information(this, QStringLiteral("删除供应商成功"), QStringLiteral("供应商已删除"));
				}
			}
			else
			{
				QMessageBox::warning(this, QStringLiteral("删除供应商失败"), QStringLiteral("请选择要删除的供应商"));
			}
		});
}

SupplierWidget::~SupplierWidget()
{}

//权限设置
void SupplierWidget::setAdmin(bool isAdmin)
{
	if (isAdmin)
	{
		ui->pb_add->setEnabled(true);
		ui->pb_delete->setEnabled(true);
	}
	else
	{
		ui->pb_add->setEnabled(false);
		ui->pb_delete->setEnabled(false);
	}
}
