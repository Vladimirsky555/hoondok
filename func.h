#ifndef FUNC_H
#define FUNC_H

#include <QObject>
#include <QMessageBox>
#include <QString>
#include <QtSql>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

#include "gconst.h"

class QSqlQuery;

class func
{
public:
	func();

	bool checkrequest(QSqlQuery *qry, QString sqq);
	bool checkRegExp(QRegExp rx);
    QString F_StyleSheet(QString obj_name, QString obj_class);
    QString convertMonthes(QString month);
};

#endif // FUNC_H
