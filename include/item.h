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
 * @note 物品类中包含两种状态:已签收，待签收(pahse2开始有三种)，包含6种属性:寄送时间、接收时间、寄件用户、收件用户、物品状态、物品描述
 */

#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QSharedPointer>
#include <ctime>

enum ITEM_STATE
{
    RECEIVED,         //已签收
    PENDING_REVEICING //待签收
};

class Database;

/**
 * @brief 物品基类
 */
class Item
{
protected:
    int id;                  // 物品ID 主键
    int cost;                //价格 phase1中为15元一件
    ITEM_STATE state;        //物品状态
    time_t sendingTime;   //寄送时间
    time_t receivingTime; //接收时间
    QString srcName;         //寄件用户的用户名
    QString dstName;         //收件用户的用户名
    QString description;     //物品描述

public:
    /**
     * @brief 删除默认构造函数
     * @note 用不上
     */
    Item() = delete;

    /**
     * @brief 构造函数
     *
     * @param _id 物品ID 主键
     * @param _state 物品状态
     * @param _sendingTime 寄送时间
     * @param _receivingTime 接收时间
     * @param _srcName 寄件用户的用户名
     * @param _dstName 收件用户的用户名
     * @param _description 物品描述
     * @note 注意是否使用std::move
     */
    Item(
        int _id,
        ITEM_STATE _state,
        time_t _sendingTime,
        time_t _receivingTime,
        QString _srcName,
        QString _dstName,
        QString _description) : id(_id),
                                cost(15),
                                state(_state),
                                sendingTime(_sendingTime),
                                receivingTime(_receivingTime),
                                srcName(_srcName),
                                dstName(_dstName),
                                description(_description) {}

    ~Item() = default;

    /**
     * @brief 获得物品id
     * @return int 物品id
     */
    int getId() const { return id; }

    /**
     * @brief 获得物品状态
     * @return const ITEM_STATE 物品状态
     */
    const ITEM_STATE getItemState() const { return state; }

    /**
     * @brief 获得寄送时间
     * @return const time_t& 寄送时间
     */
    const time_t &getSendingTime() const { return sendingTime; }

    /**
     * @brief 获得接收时间
     * @return const time_t& 接收时间
     */
    const time_t &getReceivingTime() const { return receivingTime; }

    /**
     * @brief 获得寄件用户的用户名
     * @return const QString& 寄件用户的用户名
     */
    const QString &getSrcName() const { return srcName; }

    /**
     * @brief 获得收件用户的用户名
     * @return const QString& 收件用户的用户名
     */
    const QString &getDstName() const { return dstName; }

    /**
     * @brief 获得描述信息
     * @return const QString& 描述信息
     */
    const QString &getDescription() const { return description; }
};

/**
 * @brief 物品管理类
 */
class ItemManage
{
private:
    Database *database; //数据库
    int total;          //物品ID允许的最大值

public:
    /**
     * @brief 删除默认构造函数
     * @note 用不上
     */
    ItemManage() = delete;

    /**
     * @brief 构造函数
     * @param _database 数据库
     */
    ItemManage(Database *_database);

    /**
     * @brief 插入一个Item
     *
     * @param cost 快递花费
     * @param state 物品状态
     * @param sendingTime 寄送时间
     * @param receivingTime 接收时间
     * @param srcName 寄件用户的用户名
     * @param dstName 收件用户的用户名
     * @param description 物品描述
     * @return true 插入成功
     * @return false 插入失败
     *
     * @note pahse默认cost为15
     */
    bool insertItem(
        const int cost,
        const ITEM_STATE state,
        const time_t &sendingTime,
        const time_t &receivingTime,
        const QString &srcName,
        const QString &dstName,
        const QString &description);
};

    //queryAll

    //queryByFilter

    //queryBySrcName

    //queryByDstName

    //deleteItem
#endif