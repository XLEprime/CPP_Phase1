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
// #include "item.h"

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
     * @brief Construct a new User object
     *
     * @param _balance 余额
     */
    User(int _balance) : balance(_balance){};

    enum USER_TYPE
    {
        CUSTOMER,     //用户类
        ADMINISTRATOR //管理员
    };

    /**
     * @brief Get the User Type object
     *
     * @return USER_TYPE 返回UserType
     */
    virtual USER_TYPE getUserType() const = 0;

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
     * @brief Construct a new Customer object
     *
     * @param _balance 余额
     */
    Customer(int _balance) : User(_balance) {}

    /**
     * @brief Get the User Type object
     *
     * @return USER_TYPE 返回UserType
     */
    USER_TYPE getUserType() const override { return CUSTOMER; }
};

/**
 * @brief 管理员类
 */
class Administrator : public User
{
public:
    Administrator() = default;

    /**
     * @brief Construct a new Administrator object
     *
     * @param _balance 余额
     */
    Administrator(int _balance) : User(_balance) {}

    /**
     * @brief Get the User Type object
     *
     * @return USER_TYPE 返回UserType
     */
    USER_TYPE getUserType() const override { return ADMINISTRATOR; }
};

/**
 * @brief 用户管理类, 类似于工厂模式, 对象的创建和对象的使用分离.
 */
class UserManage
{
private:
    QMap<QString, QSharedPointer<User>> userMap; //用户名到用户对象的映射.
    Database *database;                          //数据库

    /**
     * @brief 用户鉴权
     *
     * @param token 凭据
     * @return QString 鉴权成功则返回用户名，失败则返回空串.
     * @note 空串可用isEmpty判断.
     */
    QString tokenVerify(const QJsonObject &token) const;
};

#endif