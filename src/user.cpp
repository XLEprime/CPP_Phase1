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

QString UserManage::addBalance(const QJsonObject &token, int addend) const
{
    if (addend >= (int)1e9 || addend <= (int)-1e9)
        return "单次余额改变量不能超过1000000000";

    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";

    if (userMap[username]->getBalance() + addend < 0)
        return "余额不能为负";

    if (userMap[username]->getBalance() + addend > (int)1e9)
        return "余额上限为1000000000";

    qDebug() << "修改用户 " << username << " 成功, 余额为 " << userMap[username]->getBalance() + addend;
    db->modifyUserBalance(username, userMap[username]->getBalance() + addend);
    userMap[username]->addBalance(addend);
    return {};
}

bool UserManage::transferBalance(const QJsonObject &token, int balance, const QString &dstUser) const
{
    if (balance >= (int)1e9 || balance <= (int)-1e9)
        return "单次余额改变量不能超过1000000000";

    if (!db->queryUserByName(dstUser))
    {
        qCritical() << "无法查到另一个用户 " << dstUser;
        return false;
    }

    int dstBalance = db->queryBalanceByName(dstUser);
    if (dstUser + balance >= (int)1e9)
    {
        qCritical() << "余额不能大于1000000000";
        return false;
    }
    if (dstUser + balance < 0)
    {
        qCritical() << "余额不能为负";
        return false;
    }
    if (!addBalance(token, -balance).isEmpty())
        return false;
    db->modifyUserBalance(dstUser, dstBalance + balance);
    qDebug() << dstUser << "获得金额: " << balance;
    return true;
}

QString UserManage::queryItem(const QJsonObject &token, const QJsonObject &filter, QJsonArray &ret) const
{
    bool ok;
    if (!filter.contains("type"))
        return "缺少type键";
    int cnt;
    QList<QSharedPointer<Item>> result;
    QString username = verify(token);

    switch (filter["type"].toInt())
    {
    case 0:
        if (userMap[username]->getUserType() != ADMINISTRATOR)
            return "非管理员不能查看所有物品";
        cnt = itemManage->queryAll(result);
        break;
    case 1:
    {
        int id = -1;
        Time sendingTime(-1, -1, -1), receivingTime(-1, -1, -1);
        QString dstName("");
        if (filter.contains("id"))
            id = filter["id"].toInt();
        if (filter.contains("sendingTime_Year"))
            sendingTime.year = filter["sendingTime_Year"].toInt();
        if (filter.contains("sendingTime_Month"))
            sendingTime.month = filter["sendingTime_Month"].toInt();
        if (filter.contains("sendingTime_Day"))
            sendingTime.day = filter["sendingTime_Day"].toInt();
        if (filter.contains("receivingTime_Year"))
            receivingTime.year = filter["receivingTime_Year"].toInt();
        if (filter.contains("receivingTime_Month"))
            receivingTime.month = filter["receivingTime_Month"].toInt();
        if (filter.contains("receivingTime_Day"))
            receivingTime.day = filter["receivingTime_Day"].toInt();
        if (filter.contains("dstName"))
            dstName = filter["dstName"].toString();
        cnt = itemManage->queryByFilter(result, id, sendingTime, receivingTime, username, dstName);
    }
    break;
    case 2:
    {
        int id = -1;
        Time sendingTime(-1, -1, -1), receivingTime(-1, -1, -1);
        QString srcName("");
        if (filter.contains("id"))
            id = filter["id"].toInt();
        if (filter.contains("sendingTime_Year"))
            sendingTime.year = filter["sendingTime_Year"].toInt();
        if (filter.contains("sendingTime_Month"))
            sendingTime.month = filter["sendingTime_Month"].toInt();
        if (filter.contains("sendingTime_Day"))
            sendingTime.day = filter["sendingTime_Day"].toInt();
        if (filter.contains("receivingTime_Year"))
            receivingTime.year = filter["receivingTime_Year"].toInt();
        if (filter.contains("receivingTime_Month"))
            receivingTime.month = filter["receivingTime_Month"].toInt();
        if (filter.contains("receivingTime_Day"))
            receivingTime.day = filter["receivingTime_Day"].toInt();
        if (filter.contains("srcName"))
            srcName = filter["srcName"].toString();
        cnt = itemManage->queryByFilter(result, id, sendingTime, receivingTime, srcName, username);
    }
    break;
    default:
        return "type键的值有误";
        break;
    }

    for (const QSharedPointer<Item> &item : result)
    {
        QJsonObject itemJson;
        itemJson.insert("id", item->getId());
        itemJson.insert("cost", item->getCost());
        itemJson.insert("state", item->getState());
        itemJson.insert("sendingTime_Year", item->getSendingTime().year);
        itemJson.insert("sendingTime_Month", item->getSendingTime().month);
        itemJson.insert("sendingTime_Day", item->getSendingTime().day);
        itemJson.insert("receivingTime_Year", item->getReceivingTime().year);
        itemJson.insert("receivingTime_Month", item->getReceivingTime().month);
        itemJson.insert("receivingTime_Day", item->getReceivingTime().day);
        itemJson.insert("srcName", item->getSrcName());
        itemJson.insert("dstName", item->getDstName());
        itemJson.insert("description", item->getDescription());
        ret.append(itemJson);
    }
    return {};
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

QString UserManage::logout(const QJsonObject &token)
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    qDebug() << "用户 " << username << " 登出";
    userMap.remove(username);
    return {};
}

QString UserManage::changePassword(const QJsonObject &token, const QString &newPassword) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    qDebug() << "用户 " << username << " 修改密码为 " << newPassword;
    db->modifyUserPassword(username, newPassword);
    return {};
}

QString UserManage::getUserInfo(const QJsonObject &token, QJsonObject &ret) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    qDebug() << "获取用户" << username << " 的信息";
    ret.insert("username", username);
    ret.insert("type", userMap[username]->getUserType());
    ret.insert("balance", userMap[username]->getBalance());
    return {};
}
