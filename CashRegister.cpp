#include "CashRegister.h"
#include "CashLogWidget.h"
#include "CheckoutWidget.h"
#include "InventoryWidget.h"
#include "SerialPortManager.h"
#include "Supplier.h"
#include <QInputDialog>
#include <QMessageBox>
struct CashRegister::CashRegisterPrivate
{
    CashLogWidget *cashLogWidget = new CashLogWidget;
    CheckoutWidget *checkoutWidget = new CheckoutWidget;
    InventoryWidget *inventoryWidget = new InventoryWidget;
    SupplierWidget* supplierWidget = new SupplierWidget;

    bool isBarcodeMode = true;
};

CashRegister::CashRegister(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CashRegisterClass()),p(new CashRegisterPrivate)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("智能仓储系统"));
    setWindowIcon(QIcon(":/image/icon.png"));
    //清空默认的
    ui->stackedWidget->removeWidget(ui->page);
    ui->stackedWidget->removeWidget(ui->page_2);
    //添加界面
    ui->stackedWidget->addWidget(p->inventoryWidget);
    ui->stackedWidget->addWidget(p->checkoutWidget);
    ui->stackedWidget->addWidget(p->cashLogWidget);
    ui->stackedWidget->addWidget(p->supplierWidget);


    ui->pushButton->setText(QStringLiteral("条形码模式"));
    connect(ui->pushButton, &QPushButton::clicked, [this]()
        {
            p->isBarcodeMode = !p->isBarcodeMode;
            if (p->isBarcodeMode)
            {
                ui->pushButton->setText(QStringLiteral("条形码模式"));
            }
            else
            {
                ui->pushButton->setText(QStringLiteral("二维码模式"));
            }
        });

    //切换页面
    connect(ui->pb_Intery, &QPushButton::clicked, [this]()
    {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->pb_calculate, &QPushButton::clicked, [this]()
    {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->pb_log, &QPushButton::clicked, [this]()
    {
        ui->stackedWidget->setCurrentIndex(2);
    });

    //供应商管理界面并设置权限管理
    connect(ui->pb_supplier, &QPushButton::clicked, [this]()
        {
            QInputDialog inputDialog;
            inputDialog.resize(400, 200);
            inputDialog.setWindowTitle(QStringLiteral("请输入管理员密码"));
            inputDialog.setLabelText(QStringLiteral("密码:"));
            inputDialog.setTextEchoMode(QLineEdit::Password);
            if (inputDialog.exec() == QDialog::Accepted)
            {
                if (inputDialog.textValue() == "123456")
                {
                    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("密码正确,进入管理员模式"));
                    p->supplierWidget->setAdmin(true);
                }
                else
                {
                    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("密码错误,进入普通模式"));
                    p->supplierWidget->setAdmin(false);
                }

            }
            else
            {
                QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("默认进入普通模式"));
                p->supplierWidget->setAdmin(false);
            }
            ui->stackedWidget->setCurrentIndex(3);
        });
    ui->stackedWidget->setCurrentIndex(0);

    //区分条形码与二维码
    connect(ui->stackedWidget,&QStackedWidget::currentChanged,[this]()
    {
        disconnect(SerialPortManager::getInstance(),&SerialPortManager::dataReceived, nullptr, nullptr);//清除之前为 dataReceived 信号设置的处理逻辑
        //入库处理
        if(ui->stackedWidget->currentIndex() == 0)
		{
            connect(SerialPortManager::getInstance(), &SerialPortManager::dataReceived, [this](const QByteArray& data)
                {
                    if (p->isBarcodeMode)//条形码模式
                    {
                        if (data.size() != 14)//条形码格式
                        {
                            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无效的条形码"));
                            return;
                        }
                        p->inventoryWidget->OnDataReceived(data);//接收信号
                    }
                    else
                    {
						if (data.size() == 14)
						{
							QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无效的二维码"));
							return;
						}
						p->inventoryWidget->OnDataReceived(data);
                    }
                });
		}
        else if(ui->stackedWidget->currentIndex() == 1)
		{
			connect(SerialPortManager::getInstance(), &SerialPortManager::dataReceived, [this](const QByteArray& data)
				{
					if (p->isBarcodeMode)
					{
						if (data.size() != 14)
						{
							QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无效的条形码"));
							return;
						}
						p->checkoutWidget->OnDataReceived(data);
					}
					else
					{
						if (data.size() == 14)
						{
							QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无效的二维码"));
							return;
						}
						p->checkoutWidget->OnDataReceived(data);
					}
				});
		}
    });


	connect(SerialPortManager::getInstance(), &SerialPortManager::dataReceived, [this](const QByteArray& data)
		{
			if (p->isBarcodeMode)
			{
				if (data.size() != 14)
				{
					QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无效的条形码"));
					return;
				}
				p->inventoryWidget->OnDataReceived(data);
			}
			else
			{
				if (data.size() == 14)
				{
					QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("无效的二维码"));
					return;
				}
				p->inventoryWidget->OnDataReceived(data);
			}
		});

    if (SerialPortManager::getInstance()->openSerialPort("COM5"))
    {
        ui->label_status->setText(QStringLiteral("已连接"));
    }
    else
    {
        ui->label_status->setText(QStringLiteral("未连接"));
    }

    connect(p->checkoutWidget, &CheckoutWidget::CheckoutFinished, p->cashLogWidget, &CashLogWidget::Refresh);
    connect(p->checkoutWidget, &CheckoutWidget::CheckoutFinished, p->inventoryWidget, &InventoryWidget::Refresh);

}

CashRegister::~CashRegister()
{
    delete ui;
    delete p;
}

void CashRegister::InitPort()
{
   
}
