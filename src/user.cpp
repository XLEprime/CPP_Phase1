/**
 * @file user.cpp
 * @author Haolin Yang
 * @brief
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../include/user.h"

QString UserManage::verify(const QJsonObject &token) const
{
    if (!token.contains("username") ||
        !userMap.value(token["username"].toString(), nullptr) ||
        !token.contains("iss") ||
        token["iss"] != "Haolin Yang")
    {
        qWarning() << "用户验证失败";
        return {};
    }
    else
    {
        qDebug() << "用户 " << token["username"] << " 验证成功";
        return token["username"].toString();
    }
}

QString UserManage::changeBalance(const QJsonObject &token, int balanceChange) const
{
    if (balanceChange >= (int)1e9 || balanceChange <= (int)-1e9)
        return "单次余额改变量不能超过1000000000";

    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";

    if (userMap[username]->getBalance() + balanceChange < 0)
        return "余额不能为负";

    if (userMap[username]->getBalance() + balanceChange > (int)1e9)
        return "余额上限为1000000000";

    qDebug() << "修改用户 " << username << " 成功, 余额为 " << userMap[username]->getBalance() + balanceChange;
    db->modifyUserBalance(username, userMap[username]->getBalance() + balanceChange);
    userMap[username]->changeBalance(balanceChange);
    return {};
}

bool UserManage::changeBalance(const QJsonObject &token, int balanceChange, const QString &dstUser) const
{
    if (balanceChange >= (int)1e9 || balanceChange <= (int)-1e9)
        return "单次余额改变量不能超过1000000000";

    if (!db->queryUserByName(dstUser))
    {
        qCritical() << "无法查到另一个用户 " << dstUser;
        return false;
    }

    int dstBalance = db->queryBalanceByName(dstUser);
    if (dstUser + balanceChange >= (int)1e9)
    {
        qCritical() << "余额不能大于1000000000";
        return false;
    }
    if (dstUser + balanceChange < 0)
    {
        qCritical() << "余额不能为负";
        return false;
    }
    if (!changeBalance(token, -balanceChange).isEmpty())
        return false;
    db->modifyUserBalance(dstUser, dstBalance + balanceChange);
    qDebug() << dstUser << "获得金额: " << balanceChange;
    return true;
}

QString UserManage::registerUser(const QString &username, const QString &password, int type) const
{
    if (username.isEmpty() || username.size() > 10)
        return "用户名长度应该在1~10之间";
    if (db->queryUserByName(username))
        return "该用户名已被注册";
    if (type == ADMINISTRATOR)
        return "管理员类不支持注册";
    db->insertUser(username, password, type, 0);
    qDebug() << "用户 " << username << " 注册成功";
    return {};
}

QString UserManage::login(const QString &username, const QString &password, QJsonObject &token)
{
    QString retPassword;
    int retType;
    int retBalance;
    if (db->queryUserByName(username, retPassword, retType, retBalance) && retPassword == password)
    {
        switch (retType)
        {
        case CUSTOMER:
            if (!userMap[username])
                userMap[username] = QSharedPointer<Customer>::create(retBalance);
            break;
        case ADMINISTRATOR:
            if (!userMap[username])
                userMap[username] = QSharedPointer<Administrator>::create(retBalance);
            break;
        default:
            return "数据库中用户类型错误";
            break;
        }
        token.insert("iss", "Haolin Yang");
        token.insert("username", username);
        return {};
    }
    else
        return "用户名或密码错误";
}
