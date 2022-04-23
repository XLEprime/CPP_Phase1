/**
 * @file item.cpp
 * @author Haolin Yang
 * @brief
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../include/item.h"
#include "../include/database.h"

int ItemManage::insertItem(
    const int cost,
    const int state,
    const Time &sendingTime,
    const Time &receivingTime,
    const QString &srcName,
    const QString &dstName,
    const QString &description)
{
    qDebug() << "添加物品 ";
    db->insertItem(++total, cost, state, sendingTime, receivingTime, srcName, dstName, description);
    return total;
}

int ItemManage::queryAll(QList<QSharedPointer<Item>> &result) const
{
    qDebug() << "查询所有物品";
    return db->queryItemByFilter(result, -1, Time(-1, -1, -1), Time(-1, -1, -1), "", "");
}

int ItemManage::queryByFilter(QList<QSharedPointer<Item>> &result, int id, const Time &sendingTime, const Time &receivingTime, const QString &srcName, const QString &dstName) const
{
    qDebug() << "按条件查询 ";
    return db->queryItemByFilter(result, id, sendingTime, receivingTime, srcName, dstName);
}

void ItemManage::deleteItem(int id) const
{
    qDebug() << "按条件查询 ";
    db->deleteItem(id);
}