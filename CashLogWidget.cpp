#include "CashLogWidget.h"
#include "MyDataBase.h"
#include <QDateTime>
CashLogWidget::CashLogWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::CashLogWidgetClass())
{
	ui->setupUi(this);
	
	ui->tableWidget->setColumnCount(6);
	QStringList header;
	header << QStringLiteral("ID") << QStringLiteral("商品名称") <<  QStringLiteral("数量")<<QStringLiteral("出库日期")  << QStringLiteral("价格") << QStringLiteral("供应商");
	ui->tableWidget->setHorizontalHeaderLabels(header);
	
	Load();
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	//查询过滤
	connect(ui->lineEdit, &QLineEdit::textChanged, [this]()
		{
			if (ui->comboBox->currentIndex() == 0) //根据供应商查询
			{
			   //只显示供应商为lineEdit的行
				for (int i = 0; i < ui->tableWidget->rowCount(); i++)
				{
					if (ui->tableWidget->item(i, 5)->text().contains(ui->lineEdit->text()))
					{
						ui->tableWidget->showRow(i);
					}
					else
					{
						ui->tableWidget->hideRow(i);
					}
				}
			}
			else if (ui->comboBox->currentIndex() == 1) //根据商品名称查询
			{
				for (int i = 0; i < ui->tableWidget->rowCount(); i++)
				{
					if (ui->tableWidget->item(i, 1)->text().contains(ui->lineEdit->text()))
					{
						ui->tableWidget->showRow(i);
					}
					else
					{
						ui->tableWidget->hideRow(i);
					}
				}
			}
			else if (ui->comboBox->currentIndex() == 2) //根据时间晚于查询
			{
				for (int i = 0; i < ui->tableWidget->rowCount(); i++)
				{
					//转成时间格式
					QDateTime dateTime = QDateTime::fromString(ui->tableWidget->item(i, 3)->text(), "yyyy-MM-dd hh:mm:ss");

					if (dateTime > QDateTime::fromString(ui->lineEdit->text(), "yyyy-MM-dd hh:mm:ss"))
					{
						ui->tableWidget->showRow(i);
					}
					else
					{
						ui->tableWidget->hideRow(i);
					}
				}
			}
		});
}

CashLogWidget::~CashLogWidget()
{
	delete ui;
}

void CashLogWidget::Load()
{
	auto list = MyDataBase::GetInstance().GetSellRecord();//从数据库中获取所有的商品销售记录
	for(auto& item : list)
	{
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(item.barCode));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(item.name));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(item.number)));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(item.time));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(item.price)));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, new QTableWidgetItem(item.supplier));

		//居中
		for (int i = 0; i < 6; i++)
		{
			ui->tableWidget->item(ui->tableWidget->rowCount() - 1, i)->setTextAlignment(Qt::AlignCenter);
		}
	}
}

void CashLogWidget::Refresh()
{
	ui->tableWidget->setRowCount(0);
	Load();
}
