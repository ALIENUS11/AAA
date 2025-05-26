#include "CashRegister.h"
#include <QtWidgets/QApplication>
#include <QThread>
#include <QFile>
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	CashRegister c;
	c.show();

	QFile file(QCoreApplication::applicationDirPath() + "/style.qss");
	if (file.open(QIODevice::ReadOnly))
	{
		qApp->setStyleSheet(QString::fromLatin1(file.readAll()));
		file.close();
	}
	return a.exec();
}
