/**
 * @file user.h
 * @author Haolin Yang
 * @brief 用户和用户管理
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 * @note 定义了用户和用户管理类
 * @note 用户类包括一个抽象的用户基类(其中getUserType函数为纯虚函数), 以及用户类(CUSTOMER)和管理员(ADMINISTRATOR)两个派生类.
 * @note 用户类中保存用户的余额.
 * @note 用户类的对象都是临时对象.
 * @note 用户管理类中定义了大多操作和鉴权.
 * @note 简单的成员函数直接在此文件给出实现.
 * @note 管理员不需要支持注册!!!
 */

#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonValue>

#include "database.h"
#include "item.h"

#define CUSTOMER 1
#define ADMINISTRATOR 2

/**
 * @brief 用户基类
 * @note 纯虚类, 其中getUserType为纯虚函数.
 */
class User
{
protected:
    int balance; //余额

public:
    User() = default;

    /**
     * @brief 构造函数
     *
     * @param _balance 余额
     */
    User(int _balance) : balance(_balance){};

    /**
     * @brief Get the User Type object
     *
     * @return int 返回UserType
     */
    virtual int getUserType() const = 0;

    virtual ~User() = default;
};

/**
 * @brief 用户类
 */
class Customer : public User
{
public:
    Customer() = default;

    /**
     * @brief 构造函数
     * @param _balance 余额
     */
    Customer(int _balance) : User(_balance) {}

    /**
     * @brief 获得用户类
     * @return int 返回UserType
     * @note 1为CUSTOMER 2为ADMINISTRATOR
     */
    int getUserType() const override { return CUSTOMER; }
};

/**
 * @brief 管理员类
 */
class Administrator : public User
{
public:
    Administrator() = default;

    /**
     * @brief 构造函数
     * @param _balance 余额
     */
    Administrator(int _balance) : User(_balance) {}

    /**
     * @brief 获得用户类
     * @return int 返回UserType
     * @note 1为CUSTOMER 2为ADMINISTRATOR
     */
    int getUserType() const override { return ADMINISTRATOR; }
};

/**
 * @brief 用户管理类, 类似于工厂模式, 对象的创建和对象的使用分离.
 */
class UserManage
{
private:
    QMap<QString, QSharedPointer<User>> userMap; //用户名到用户对象的映射.
    Database *database;                          //数据库
    ItemManage *ItemManage;                      //物品管理类

    /**
     * @brief 用户鉴权
     * @param token 凭据
     * @return QString 鉴权成功则返回用户名，失败则返回空串.
     * @note 空串可用isEmpty判断.
     */
    QString verify(const QJsonObject &token) const;

    /**
     * @brief 减少一个用户的余额，增加另一个用户的余额。
     * @param token 第一个用户（减少余额的用户）的token
     * @param balance 减少量
     * @param srcUser 第二个用户（增加余额的用户）的用户名
     * @return true 操作成功
     * @return false 操作失败
     */
    bool changeBalance(const QJsonObject &token, int balance, const QString &dstUser) const;

    //建造itemByJson

public:
    /**
     * @brief 禁止默认的构造函数
     * @note 用不上
     */
    UserManage() = delete;

    // todo: 增加itemmanage的内容
    UserManage(Database *_database) : database(_database) {}

    /**
     * @brief 注册
     *
     * @param username 用户名
     * @param password 密码
     * @param type 注册的账号类型 1为CUSTOMER, 2为EXPRESSMAN(Phase2开始出现)
     * @return QString 如果注册成功，返回空串，否则返回错误信息.
     * @note ADMINISTRATOR不用支持注册.
     * @note register是关键字，不能作为函数名.
     */
    QString registerUser(const QString &username, const QString &password, int type) const;

    /**
     * @brief 登录
     * @param username 用户名
     * @param password 密码
     * @param ret 生成的凭据
     * @return QString 如果登录成功，返回空串，否则返回错误信息.
     */
    QString login(const QString &username, const QString &password, QJsonObject &ret);

    /**
     * @brief 登出
     *
     * @param token 凭据
     * @return QString 如果登出成功，返回空串，否则返回错误信息.
     */
    QString logout(const QJsonObject &token);

    /**
     * @brief 更改密码
     *
     * @param token 凭据
     * @param newPassword 新密码
     * @return QString 如果更改成功，返回空串，否则返回错误信息.
     */
    QString changePassword(const QJsonObject &token, const QString &newPassword) const;

    /**
     * @brief 获取用户信息
     * @param token 凭据
     * @param ret 用户信息
     * @return 如果获取成功，返回空串，否则返回错误信息
     *
     * 用户信息的格式：
     * ```json
     * {
     *    "username": <字符串>,
     *    "type": <整数>,
     *    "balance": <整数>
     * }
     * ```
     */
    QString getUserInfo(const QJsonObject &token, QJsonObject &ret) const;

    /**
     * @brief todo
     *
     * @param token
     * @param balance
     * @return QString
     */
    QString changeBalance(const QJsonObject &token, int balance) const;

    // todo 增加物品 修改物品 查询物品 删除物品
};

#endif