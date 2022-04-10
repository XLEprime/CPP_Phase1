/**
 * @file database.h
 * @author Haolin Yang
 * @brief
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 * @note 数据库类的封装
 * @note 对于用户部分, 定义了插入用户(注册), 查询用户, 修改用户密码, 修改用户余额的接口.
 * @note 对于物品部分, 定义了插入物品, 查询物品, 修改物品信息, 删除物品的接口.
 * @note 额外定义了部分工具接口，有...
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <QFile>
#include <QtSql>

// #include "item.h"

// class Item;

/**
 * @brief 数据库类
 */
class Database
{
private:
    QSqlDatabase database;     // SQLite数据库
    QFile userFile;            //永久存储用户名文件
    QTextStream stream;        //用户读写用户名的stream
    QSet<QString> userNameSet; //用户名集合

    /**
     * @brief 执行SQL语句
     * @param sqlQuery
     */
    static void exec(const QSqlQuery &sqlQuery);

    /**
     * @brief 修改数据库中某个记录的值
     * @param table 表名
     * @param id 需要修改的记录的主键
     * @param key 需要修改的键
     * @param value 修改的值
     */
    void modifyData(const QString &table, const QString &id, const QString &key, int value) const;

public:
    /**
     * @brief 插入用户条目
     *
     * @param username
     * @param passwd
     * @param type
     * @param balance
     */
    void insertUserData(const QString &username, const QString &passwd, bool type, int balance);

    /**
     * @brief 根据用户名查询用户
     *
     * @param username 用户名
     * @return true 查询到用户
     * @return false 没查询到用户
     */
    bool queryUserByName(const QString &username) const;

    /**
     * @brief 修改用户密码
     *
     * @param username 用户名
     * @param passwd 新密码
     */
    void modifyUserPasswd(const QString &username, const QString &passwd) const;

    /**
     * @brief 修改用户余额
     *
     * @param username 用户名
     * @param balance 改后的余额
     */
    void modifyUserBalance(const QString &username, int balance) const;

    /**
     * @brief 查询表中主键的最大值
     * @param table 表名
     * @return 返回最大主键允许的s值
     */
    int getQueryMaxId(const QString &table) const;

    //todo: 关于快递的添加
};

#endif