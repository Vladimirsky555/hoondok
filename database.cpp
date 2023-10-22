#include "mainwindow.h"
#include "database.h"

#define sdbd "jino"


DataBase::DataBase(QObject *parent) : QObject(parent){}
DataBase::~DataBase(){}

void DataBase::connectdb()
{	
	db = QSqlDatabase::addDatabase("QMYSQL", sdbd);
	db.setHostName("*******.vps.myjino.ru");
	db.setDatabaseName("dbname");
	db.setPort(*****);
	db.setUserName("login");
	db.setPassword("password");

	if (!db.open()) {
		QMessageBox mbox;
		mbox.setText(db.lastError().text());
		mbox.setWindowTitle(db.lastError().text());
		mbox.setIcon(QMessageBox::Critical);
		mbox.exec();
	} else {
		this->opendb();
	}
}


bool DataBase::restoredb()
{
	return true;
}

bool DataBase::opendb()
{
	if(db.open()){
		return true;
	} else {
		return false;
	}
}

void DataBase::closedb()
{
	db.close();
	db.removeDatabase(sdbd);
}

qint32 DataBase::dbclose()
{
	db.close();
	db.removeDatabase(sdbd);
	return 0;
}






