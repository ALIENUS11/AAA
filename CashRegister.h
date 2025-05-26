#pragma once

#include <QtWidgets/QWidget>
#include "ui_CashRegister.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CashRegisterClass; };
QT_END_NAMESPACE

class CashRegister : public QWidget
{
    Q_OBJECT

public:
    CashRegister(QWidget *parent = nullptr);
    ~CashRegister();
        
    void InitPort();

private:
    Ui::CashRegisterClass *ui;
    struct CashRegisterPrivate;
    CashRegisterPrivate *p;
};
