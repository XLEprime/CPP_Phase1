/**
 * @file item.h
 * @author Haolin Yang
 * @brief 物品和物品管理
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 * @note 定义了物品和物品管理类.
 * @note 物品类暂不需要继承(pahse2开始需要).
 * @note 物品类中包含两种状态:已签收，待签收(pahse2开始有三种)，包含6种属性:寄送时间、接受时间、寄件用户、收件用户、物品状态、物品描述
 */

#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QSharedPointer>
#include <ctime>

enum ITEM_STATE
{
    RECEIVED,        //已签收
    PENDINGREVEICING //待签收
};

class Database;

/**
 * @brief 物品基础类
 */
class Item
{
protected:
    int id;                  // 物品ID 主键
    ITEM_STATE state;        //物品状态
    struct tm collectedTime; //寄送时间
    struct tm receivedTime;  //接受时间
    QString srcName;         //寄件用户的用户名
    QString dstName;         //收件用户的用户名
    QString description;     //物品描述

public:
    /**
     * @brief 删除默认构造函数
     */
    Item() = delete;

    /**
     * @brief 构造函数
     *
     * @param _id 物品ID 主键
     * @param _state 物品状态
     * @param _collectedTime 寄送时间
     * @param _receivedTime 接受时间
     * @param _srcName 寄件用户的用户名
     * @param _dstName 收件用户的用户名
     * @param _description 物品描述
     * @note 注意是否使用std::move
     */
    Item(int _id, ITEM_STATE _state, struct tm _collectedTime, struct tm _receivedTime, QString _srcName, QString _dstName, QString _description) : id(_id), state(_state), collectedTime(_collectedTime), receivedTime(_receivedTime), srcName(_srcName), dstName(_dstName), description(_description) {}

    ~Item() = default;
};

#endif