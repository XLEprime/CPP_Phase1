/**
 * @file main.cpp
 * @author Haolin Yang
 * @brief
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

        if (args[0] == "register" && args.size() == 3) // register 账号 密码 在phase2开始添加type
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
        else
            qInfo() << "指令输入有误，请输入help查看帮助";
    }

    return 0;
}