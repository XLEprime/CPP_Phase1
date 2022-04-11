/**
 * @file database.cpp
 * @author Haolin Yang
 * @brief 数据库类的实现
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../include/database.h"
#include <QDebug>

using namespace std;

void Database::exec(const QSqlQuery &sqlQuery)
{
    qDebug() << "执行SQL语句" << sqlQuery.lastQuery();
    QMap<QString, QVariant> sqlIter(sqlQuery.boundValues());
    for (auto i = sqlIter.begin(); i != sqlIter.end(); i++)
        qDebug() << i.key().toUtf8().data() << ":" << i.value().toString().toUtf8().data();
}

Database::Database(const QString &connectionName, const QString &fileName)
{
    database = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    database.setDatabaseName("MyDataBase.db");
    database.open();

    if (!database.tables().contains("user")) //若不包含user，则创建。
    {
        QSqlQuery sqlQuery(database);
        sqlQuery.prepare("CREATE TABLE user( username TEXT PRIMARY KEY NOT NULL,"
                         "password TEXT NOT NULL,"
                         "type INT NOT NULL,"
                         "balance INT NOT NULL) ");
        exec(sqlQuery);
        if (!sqlQuery.exec())
            qCritical() << "user表创建失败" << sqlQuery.lastError();
        else
            qDebug() << "user表创建成功";
    }
    else
        qDebug() << "user表已存在";

    if (!database.tables().contains("item")) //若不包含item，则创建。
    {
        QSqlQuery sqlQuery(database);
        sqlQuery.prepare("CREATE TABLE item( id INT PRIMARY KEY NOT NULL,"
                         "cost INT NOT NULL,"
                         "type INT NOT NULL,"
                         "state INT NOT NULL,"
                         "sendingTime INT NOT NULL,"
                         "receivingTime INT NOT NULL,"
                         "srcName TEXT NOT NULL,"
                         "dstName TEXT NOT NULL,"
                         "description TEXT NOT NULL) ");
        exec(sqlQuery);
        if (!sqlQuery.exec())
        {
            qCritical() << "item表创建失败" << sqlQuery.lastError();
            exit(1);
        }
        else
            qDebug() << "item表创建成功";
    }
    else
        qDebug() << "item表已存在";

    if (!userFile.open(QIODevice::ReadWrite | QIODevice ::Text))
    {
        qCritical() << "user文件打开失败";
        exit(1);
    }
    stream.setDevice(&userFile);
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        usernameSet.insert(line);
    }
}