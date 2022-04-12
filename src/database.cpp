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

const QString &Database::getPrimaryKeyByTableName(const QString &tableName)
{
    static QString username("username");
    static QString id("id");
    if (tableName == "user")
        return username;
    else
        return id;
}

Database::Database(const QString &connectionName, const QString &fileName)
{
    db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    db.setDatabaseName("MyDataBase.db");
    db.open();

    if (!db.tables().contains("user")) //若不包含user，则创建。
    {
        QSqlQuery sqlQuery(db);
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

    if (!db.tables().contains("item")) //若不包含item，则创建。
    {
        QSqlQuery sqlQuery(db);
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
            qCritical() << "item表创建失败" << sqlQuery.lastError();
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

void Database::modifyData(const QString &tableName, const QString &id, const QString &key, int value) const
{
    QSqlQuery sqlQuery(db);
    sqlQuery.prepare("UPDATE " + tableName + " SET " + key + " = :value WHERE " + getPrimaryKeyByTableName(tableName) + " = :id");
    sqlQuery.bindValue(":value", value);
    sqlQuery.bindValue(":id", id);

    exec(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "数据库: " << id << " -> "
                    << value
                    << " 修改失败" << sqlQuery.lastError();
    else
        qDebug() << "数据库: " << id << " -> "
                 << value
                 << " 修改成功";
}

void Database::modifyData(const QString &tableName, const QString &id, const QString &key, const QString value) const
{
    QSqlQuery sqlQuery(db);
    sqlQuery.prepare("UPDATE " + tableName + " SET " + key + " = :value WHERE " + getPrimaryKeyByTableName(tableName) + " = :id");
    sqlQuery.bindValue(":value", value);
    sqlQuery.bindValue(":id", id);

    exec(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "数据库: " << id << " -> "
                    << value
                    << " 修改失败" << sqlQuery.lastError();
    else
        qDebug() << "数据库: " << id << " -> "
                 << value
                 << " 修改成功";
}

void Database::insertUserData(const QString &username, const QString &password, bool type, int balance)
{
    QSqlQuery sqlQuery(db);
    sqlQuery.prepare("INSERT INTO user VALUES(:username, :password, :type, :balance)");
    sqlQuery.bindValue(":username", username);
    sqlQuery.bindValue(":password", password);
    sqlQuery.bindValue(":type", type);
    sqlQuery.bindValue(":balance", balance);

    exec(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "数据库:插入user " << username << " 失败" << sqlQuery.lastError();
    else
        qDebug() << "数据库:插入user " << username << " 成功";
}

bool Database::queryUserByName(const QString &username) const
{
    QSqlQuery sqlQuery(db);
    sqlQuery.prepare("SELECT * FROM user WHERE username = :username");
    sqlQuery.bindValue(":username", username);

    exec(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "数据库:查找user " << username << " 失败" << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << "数据库:查找user " << username << " 成功";
        if (sqlQuery.next() && usernameSet.find(username) != usernameSet.constEnd())
        {
            qDebug() << "文件中查找到用户"
                     << "username";
            return true;
        }
        qFatal("数据库和文件不同步");
        return false;
    }
}

void Database::modifyUserpassword(const QString &username, const QString &password) const
{
    modifyData("user", username, "password", password);
}


