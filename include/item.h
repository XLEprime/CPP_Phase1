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
#include "database.h"

const int RECEIVED = 1;          //已签收
const int PENDING_REVEICING = 2; //待签收


// 时间类
class Time
{
public:
    int year;  //年
    int month; //月
    int day;   //日

    Time() = default;
    Time(int _year, int _month, int _day) : year(_year), month(_month), day(_day){};
    ~Time() = default;
};

class Database;

/**
 * @brief 物品基类
 */
class Item
{
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
    Item(int _id,
         int _state,
         Time _sendingTime,
         Time _receivingTime,
         QString _srcName,
         QString _dstName,
         QString _description)
        : id(_id),
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
     * @return const int 物品状态
     */
    const int getItemState() const { return state; }

    /**
     * @brief 获得寄送时间
     * @return const Time& 寄送时间
     */
    const Time &getSendingTime() const { return sendingTime; }

    /**
     * @brief 获得接收时间
     * @return const Time& 接收时间
     */
    const Time &getReceivingTime() const { return receivingTime; }

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

protected:
    int id;              // 物品ID 主键
    int cost;            //价格 phase1中为15元一件
    int state;           //物品状态
    Time sendingTime;    //寄送时间
    Time receivingTime;  //接收时间
    QString srcName;     //寄件用户的用户名
    QString dstName;     //收件用户的用户名
    QString description; //物品描述
};

/**
 * @brief 物品管理类
 */
class ItemManage
{
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
     * @brief 插入一个Item，会自动分配id.
     *
     * @param cost 快递花费
     * @param state 物品状态
     * @param sendingTime 寄送时间
     * @param receivingTime 接收时间
     * @param srcName 寄件用户的用户名
     * @param dstName 收件用户的用户名
     * @param description 物品描述
     *
     * @note pahse默认cost为15
     */
    void insertItem(
        const int cost,
        const int state,
        const Time &sendingTime,
        const Time &receivingTime,
        const QString &srcName,
        const QString &dstName,
        const QString &description);

    /**
     * @brief 查询所有物品
     * @param result 用于返回结果
     * @return int 查到符合条件的数量
     */
    int queryAll(QList<QSharedPointer<Item>> &result) const;

    /**
     * @brief 根据条件查询物品
     * @param result 用于返回结果
     * @param id 物品单号
     * @param sendingTime 寄送时间
     * @param receivingTime 接收时间
     * @param srcName 寄件用户的用户名
     * @param dstName 收件用户的用户名
     * @return int 查到符合条件的数量
     */
    int queryByFilter(QList<QSharedPointer<Item>> &result, int id = -1, const Time &sendingTime = Time(-1, -1, -1), const Time &receivingTime = Time(-1, -1, -1), const QString &srcName = "", const QString &dstName = "") const;

    /**
     * @brief 从数据库中删除对应id的物品
     * @param id 物品单号
     */
    void deleteItem(int id) const;

private:
    Database *db; //数据库
    int total;    //物品ID允许的最大值
};
#endif