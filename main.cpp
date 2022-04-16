/**
 * @file main.cpp
 * @author Haolin Yang
 * @brief 主程序
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <QtCore>
#include <QTextStream>
#include "include/user.h"

int main()
{
    Database db("defaultConnection", "users.txt");
    ItemManage itemManage(&db);
    UserManage userManage(&db, &itemManage);

    QTextStream istream(stdin);
    QTextStream ostream(stdout);

    QJsonValue token(QJsonValue::Null);

    QVector<QString> userType{"CUSTOMER", "ADMINISTRATOR"};
    QString input;

    qInfo() << "欢迎使用";

    while (true)
    {
        istream.readLineInto(&input);
        QStringList args = input.split(" ");
        args[0] = args[0].toLower();
        bool ok = false;

        if (args[0] == "help" && args.size() == 1)
        {
            qInfo() << "注册: register <用户名> <密码>";
            qInfo() << "登录: login <用户名> <密码>";
            qInfo() << "登出: logout";
            qInfo() << "修改密码: changepassword <新密码>";
            qInfo() << "查看个人信息: info";
            qInfo() << "充值: addbalance <增加量>";
            qInfo() << "查询快递: query <物品单号> <寄送时间年> <寄送时间月> <寄送时间日> <接收时间年> <接收时间月> <接收时间日> <寄件用户的用户名> <收件用户的用户名>";
            qInfo() << "    若要查询所有符合该条件的物品，则该条件用*代替。注意此功能仅限管理员使用。";
            qInfo() << "查找发出的快递: querysrc <物品单号> <寄送时间年> <寄送时间月> <寄送时间日> <接收时间年> <接收时间月> <接收时间日> <收件用户的用户名>";
            qInfo() << "查找将收到的快递: querysrc <物品单号> <寄送时间年> <寄送时间月> <寄送时间日> <接收时间年> <接收时间月> <接收时间日> <寄件用户的用户名>";
        }
        else if (args[0] == "register" && args.size() == 3) // register 用户名 密码 在phase2开始添加type
        {
            if (!token.isNull())
            {
                qInfo() << "当前已有用户登录，请登出后重试。";
                continue;
            }
            QString ret = userManage.registerUser(args[1], args[2], 0);
            if (ret.isEmpty())
                qInfo() << "用户 " << args[1] << " 注册成功";
            else
                qInfo() << "用户 " << args[1] << " 注册失败" << ret;
        }
        else if (args[0] == "login" && args.size() == 3) // login 账号 密码
        {
            if (!token.isNull())
            {
                qInfo() << "当前已有用户登录，请登出后重试。";
                continue;
            }
            QJsonObject retToken;
            QString ret = userManage.login(args[1], args[2], retToken);
            if (ret.isEmpty())
            {
                qInfo() << "用户 " << args[1] << " 登录成功";
                token = retToken;
            }
            else
                qInfo() << "用户 " << args[1] << " 登录失败" << ret;
        }
        else if (args[0] == "logout" && args.size() == 1)
        {
            if (token.isNull())
            {
                qInfo() << "当前没有用户登录，请登录后重试。";
                continue;
            }
            QString ret = userManage.logout(token.toObject());
            if (ret.isEmpty())
                qInfo() << "已登出";
            else
                qInfo() << "登出失败";
        }
        else if (args[0] == "changepassword" && args.size() == 2)
        {
            if (token.isNull())
            {
                qInfo() << "当前没有用户登录，请登录后重试。";
                continue;
            }
            QString ret = userManage.changePassword(token.toObject(), args[1]);
            if (ret.isEmpty())
                qInfo() << "修改密码成功";
            else
                qInfo() << "修改密码失败 " << ret;
        }
        else if (args[0] == "info" && args.size() == 1)
        {
            if (token.isNull())
            {
                qInfo() << "当前没有用户登录，请登录后重试。";
                continue;
            }
            QJsonObject retInfo;
            QString ret = userManage.getUserInfo(token.toObject(), retInfo);
            if (ret.isEmpty())
            {
                qInfo() << "查询用户信息成功 用户名为 " << retInfo["username"] << " 类型为 " << userType[retInfo["type"].toInt()] << " 余额为 " << retInfo["balance"];
            }
        }
        else if (args[0] == "addbalance" && args.size() == 2 && args[1].toInt(&ok) && ok)
        {
            if (token.isNull())
            {
                qInfo() << "当前没有用户登录，请登录后重试。";
                continue;
            }
            QString ret = userManage.addBalance(token.toObject(), args[1].toInt());
            if (ret.isEmpty())
                qInfo() << "余额充值成功";
            else
                qInfo() << "余额充值失败 " << ret;
        }
        else if (args[0] == "query" && args.size() == 10 && args[1].toInt(&ok) && ok && args[2].toInt(&ok) && ok && args[3].toInt(&ok) && ok && args[4].toInt(&ok) && ok && args[5].toInt(&ok) && ok && args[6].toInt(&ok) && ok && args[7].toInt(&ok) && ok)
        {
            if (token.isNull())
            {
                qInfo() << "当前没有用户登录，请登录后重试。";
                continue;
            }
            //未完待续
        }

        else
            qInfo() << "指令输入有误，请输入help查看帮助";
    }

    return 0;
}