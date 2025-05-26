#include "MyDataBase.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include <QVariant>
#include <QCoreApplication>
#include <QDebug>
MyDataBase::MyDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QCoreApplication::applicationDirPath() +  "/myDataBase.db");
    if (!db.open())
    {
        emit error(db.lastError().text());
        return;
    }
    
    //创建表   有 ID  商品名称   商品价格  商品数量  供应商
    QSqlQuery query;
    if(!query.exec("CREATE TABLE IF NOT EXISTS goods (barcode TEXT, name TEXT, price REAL, quantity INTEGER, supplier TEXT)"))
    {
        emit error(query.lastError().text());
    }
    
    //创建表   有 ID  售出数量 售出时间  
    if(!query.exec("CREATE TABLE IF NOT EXISTS sales (barcode TEXT,quantity INTEGER, saleTime TEXT)"))
    {
        emit error(query.lastError().text());
    }
    
    //创建表  供应商列表 
    if (!query.exec("CREATE TABLE IF NOT EXISTS supplier (name TEXT, phone TEXT, address TEXT)"))
    {
        emit error(query.lastError().text());
    }
}

MyDataBase::~MyDataBase()
{
    db.close();
}

//查询商品名
QString MyDataBase::GetGoodsName(const QString& barCode)
{
    QSqlQuery query;
    if(!query.exec(QString("SELECT name FROM goods WHERE barcode = '%1'").arg(barCode)))
    {
        emit error(query.lastError().text());
    }
    if(query.next())//查询是否有结果
    {
        return query.value(0).toString();
    }
    return QString();
}

//获取商品的价格
double MyDataBase::GetGoodsPrice(const QString& barCode)
{
    QSqlQuery query;
    if(!query.exec(QString("SELECT price FROM goods WHERE barcode = '%1'").arg(barCode)))
    {
        emit error(query.lastError().text());  
    }
    if(query.next())
    {
        return query.value(0).toDouble();
    }
    return 0.0;
}

//获取商品
Good MyDataBase::GetGoods(const QString& barCode)
{
    QSqlQuery query;
    if(!query.exec(QString("SELECT * FROM goods WHERE barcode = '%1'").arg(barCode)))
    {
        emit error(query.lastError().text());
    }
    if(query.next())
    {
        Good good;
        good.barCode = query.value(0).toString();
        good.name = query.value(1).toString();
        good.price = query.value(2).toDouble();
        good.number = query.value(3).toInt();
        good.supplier = query.value(4).toString();
        return good;
    }
    return Good();
}

//增加新商品
void MyDataBase::AddNewGoods(const QString& barCode, const QString& Name, const double& price, const int& number)
{
    QSqlQuery query;
    if(!query.exec(QString("INSERT INTO goods (barcode, name, price, quantity, supplier) VALUES ('%1', '%2', %3, %4,'')").arg(barCode).arg(Name).arg(price).arg(number)))
    {
        qDebug() << query.lastError().text();
        emit error(query.lastError().text());
    }
}

//为商品添加供应商
void MyDataBase::AddSupplierForGoods(const QString& barCode, const QString& supplierName)
{
    QSqlQuery query;
    if (!query.exec(QString("UPDATE goods SET supplier = '%1' WHERE barcode = '%2'").arg(supplierName).arg(barCode)))
    {
       emit error(query.lastError().text());
    }
}

//增加已有商品库存
void MyDataBase::AddGoods(const QString& barCode, const int& number)
{
    QSqlQuery query;
    if(!query.exec(QString("UPDATE goods SET quantity = quantity + %1 WHERE barcode = '%2'").arg(number).arg(barCode)))
    {
        emit error(query.lastError().text());
    }
}

//减少已有商品库存
void MyDataBase::ReduceGoods(const QString& barCode, const int& number)
{
    QSqlQuery query;
    if(!query.exec(QString("UPDATE goods SET quantity = quantity - %1 WHERE barcode = '%2'").arg(number).arg(barCode)))
    {
        emit error(query.lastError().text());
    }
}

//添加出库记录
void MyDataBase::AddSellRecord(const QString& barCode, const int& number, const double& price)
{
    QSqlQuery query;
    if(!query.exec(QString("INSERT INTO sales (barcode, quantity, saleTime) VALUES ('%1', %2, '%3')").arg(barCode).arg(number).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))))
    {
        emit error(query.lastError().text());
    }
}

//获取所有出库记录
QList<SellRecord> MyDataBase::GetSellRecord()
{
    QList<SellRecord> list;
    QSqlQuery query;
    if(!query.exec("SELECT * FROM sales"))//查询所有出库记录z
    {
        emit error(query.lastError().text());
        return list;
    }
    while(query.next())
    {
        SellRecord record;
        record.barCode = query.value(0).toString();
        record.number = query.value(1).toInt();
        record.time = query.value(2).toString();

        record.price = GetGoodsPrice(record.barCode) * record.number;
        record.name = GetGoodsName(record.barCode);

        record.supplier = GetSupplierForGoods(record.barCode).name;
        list.append(record);
    }
    return list;
}

//获取所有商品
QList<Good> MyDataBase::GetGoods()
{
    QList<Good> list;
    QSqlQuery query;
    if(!query.exec("SELECT * FROM goods"))
    {
        emit error(query.lastError().text());
        return list;
    }
    while(query.next())
    {
        Good good;
        good.barCode = query.value(0).toString();
        good.name = query.value(1).toString();
        good.price = query.value(2).toDouble();
        good.number = query.value(3).toInt();
        good.supplier = query.value(4).toString();
        list.append(good);
    }
    return list;
}

//获取所有供应商
QList<Supplier> MyDataBase::GetSuppliers()
{
    QList<Supplier> list;
    QSqlQuery query;
    if (!query.exec("SELECT * FROM supplier"))
    {
        emit error(query.lastError().text());
        return list;
    }
    while (query.next())
    {
        Supplier supplier;
        supplier.name = query.value(0).toString();
        supplier.phone = query.value(1).toString();
        supplier.address = query.value(2).toString();
        list.append(supplier);
    }
    return list;
}

//添加供应商
bool MyDataBase::AddSupplier(const QString& name, const QString& phone, const QString& address)
{
    QList <Supplier> list = GetSuppliers();
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].name == name)
        {
            emit error(QStringLiteral("供应商已存在"));
            return false;
        }
    }

    QSqlQuery query;
    if (!query.exec(QString("INSERT INTO supplier (name, phone, address) VALUES ('%1', '%2', '%3')").arg(name).arg(phone).arg(address)))
    {
        emit error(query.lastError().text());
        return false;
    }

    return true;
}

//删除供应商
bool MyDataBase::DeleteSupplier(const QString& name)
{
    QSqlQuery query;
    if (!query.exec(QString("DELETE FROM supplier WHERE name = '%1'").arg(name)))
    {
        emit error(query.lastError().text());
        return false;
    }
    return true;
}

//为商品添加供应商信息
Supplier MyDataBase::GetSupplierForGoods(const QString& barCode)
{
    QSqlQuery query;
    if (!query.exec(QString("SELECT supplier FROM goods WHERE barcode = '%1'").arg(barCode)))
    {
        emit error(query.lastError().text());
        return Supplier();
    }
    if (query.next())
    {
        Supplier supplier;
        supplier.name = query.value(0).toString();
        return supplier;
    }
}
