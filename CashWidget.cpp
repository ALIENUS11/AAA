#include "CashWidget.h"

CashWidget::CashWidget(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::CashWidgetClass())
{
	ui->setupUi(this);
}

CashWidget::~CashWidget()
{
	delete ui;
}
