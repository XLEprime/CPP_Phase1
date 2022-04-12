/**
 * @file database.h
 * @author Haolin Yang
 * @brief 数据库类的声明
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 * @note 对于用户部分, 定义了插入用户(注册), 查询用户, 修改用户密码, 修改用户余额的接口.
 * @note 对于物品部分, 定义了插入物品, 查询物品(根据发送人/接收人/时间/快递单号即id), 修改物品信息, 删除物品的接口.
 * @note 额外定义了部分工具接口，有...
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <QFile>
#include <QtSql>

#include "item.h"

class Item;
struct Time;

/**
 * @brief 数据库类
 */
class Database
{
private:
    QSqlDatabase db;           // SQLite数据库
    QFile userFile;            //永久存储用户名文件
    QTextStream stream;        //用户读写用户名的stream
    QSet<QString> usernameSet; //用户名集合

    /**
     * @brief 执行SQL语句
     * @param sqlQuery
     */
    static void exec(const QSqlQuery &sqlQuery);

    /**
     * @brief 通过数据库表名获得该表的主键
     * @param tableName 数据库表名
     * @return const QString& 主键
     *
     * @todo 是否可以不用static
     */
    static const QString &getPrimaryKeyByTableName(const QString &tableName);

    /**
     * @brief 修改数据库中某个记录的值，值为int类型，对应数据库的INT类型。
     * @param tableName 数据库表名
     * @param primaryKey 需要修改的记录的主键
     * @param key 需要修改的键
     * @param value 修改的值
     */
    void modifyData(const QString &tableName, const QString &primaryKey, const QString &key, int value) const;

    /**
     * @brief 修改数据库中某个记录的值，值为QString类型，对应数据库的TEXT类型。
     * @param tableName 数据库表名
     * @param primaryKey 需要修改的记录的主键
     * @param key 需要修改的键
     * @param value 修改的值
     */
    void modifyData(const QString &tableName, const QString &primaryKey, const QString &key, const QString value) const;

public:
    /**
     * @brief 删除默认构造函数
     */
    Database() = delete;

    /**
     * @brief 构造函数
     * @param connectionName 连接名称
     * @param fileName 文件名
     *
     * @note 检查是否存在user、item两个table，如果不存在某个表则创建；同时打开用户名文件，将用户名信息读取到usernameSet中。
     *
     */
    Database(const QString &connectionName, const QString &fileName);

    /**
     * @brief 插入用户条目
     *
     * @param username 用户名
     * @param password 密码
     * @param type 用户类型
     * @param balance 余额
     */
    void insertUser(const QString &username, const QString &password, bool type, int balance);

    /**
     * @brief 根据用户名查询用户
     *
     * @param username 用户名
     * @return true 查询到用户
     * @return false 没查询到用户
     */
    bool queryUserByName(const QString &username) const;

    /**
     * @brief 获得用户名对应的余额
     * @param username
     * @return int 余额
     */
    int queryBalanceByName(const QString &username) const;

    /**
     * @brief 修改用户密码
     *
     * @param username 用户名
     * @param password 新密码
     */
    void modifyUserpassword(const QString &username, const QString &password) const;

    /**
     * @brief 修改用户余额
     *
     * @param username 用户名
     * @param balance 改后的余额
     */
    void modifyUserBalance(const QString &username, int balance) const;

    /**
     * @brief 查询表中主键的最大值
     * @param tableName 数据库表名
     * @return 返回最大主键允许的s值
     */
    int getDBMaxId(const QString &tableName) const;

    /**
     * @brief 插入物品
     *
     * @param id 主键
     * @param cost 价格 phase1中为15元一件
     * @param state 物品状态
     * @param sendingTime 寄送时间
     * @param receivingTime 接收时间
     * @param srcName 寄件用户的用户名
     * @param dstName 收件用户的用户名
     * @param description 物品描述
     */
    void insertItem(int id, int cost, int state, const Time &sendingTime, const Time &receivingTime, const QString &srcName, const QString &dstName, const QString &description);

    /**
     * @brief 根据单号(id)查询物品
     * @param id 物品单号
     * @param result 结果
     * @return true 查询成功
     * @return false 查询失败
     */
    bool queryItemById(int id, int &result) const;

    /**
     * @brief 根据寄件用户查询物品
     */
    bool queryItemBySrcName(int id, int &result) const;

    /**
     * @brief 根据收件用户查询物品
     */
    bool queryItemByDstName(int id, int &result) const;

    /**
     * @brief 根据寄送时间查询物品
     */
    bool queryItemBySendingTime(int id, int &result) const;

    /**
     * @brief 根据接收时间查询物品
     */
    bool queryItemByReceivingTime(int id, int &result) const;

    /**
     * @brief 删除物品
     * @param id 物品单号
     */
    void deleteItem(int id) const;
};

#endif