/**
 * @file time.cpp
 * @author Haolin Yang
 * @brief 时间和时间管理类的实现
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <QDebug>
#include "../include/time.h"

TimeManage::TimeManage()
{
    time_t rawTime;
    time(&rawTime);
    struct tm *tm_curTime = localtime(&rawTime);
    curTime.year = tm_curTime->tm_year + 1900;
    curTime.month = tm_curTime->tm_mon + 1;
    curTime.day = tm_curTime->tm_mday;
    qInfo() << "当前物流系统时间为" << curTime.year << "/" << curTime.month << "/" << curTime.day;
}

QString TimeManage::addDays(int dayNum)
{
    if (dayNum <= 0)
        return "要加快的天数应该为正数";
    curTime.day += dayNum;
    while (curTime.day > 31)
    {
        curTime.day -= 31;
        curTime.month++;
    }
    while (curTime.day > 12)
    {
        curTime.day -= 12;
        curTime.year++;
    }
    qDebug() << "物流系统时间增加" << dayNum << "天，当前物流系统时间为" << curTime.year << "/" << curTime.month << "/" << curTime.day;
    return "";
}

bool TimeManage::isDue(const Time time) const
{
    return ((time.year < curTime.year) || (time.year == curTime.year && time.month < curTime.month) || (time.year == curTime.year && time.month == curTime.month && time.day < curTime.day));
}