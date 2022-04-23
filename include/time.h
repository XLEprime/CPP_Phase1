/**
 * @file time.h
 * @author Haolin Yang
 * @brief 时间类和时间管理类的声明
 * @version 0.1
 * @date 2022-04-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef TIME_H
#define TIME_H

#include <ctime>

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

//时间管理类
class TimeManage
{
public:
    /**
     * @brief 构造函数，根据当前操作系统的时间初始化物流系统的年月日
     */
    TimeManage();

    ~TimeManage() = default;

    /**
     * @brief加快物流系统时间，单位:天
     *
     * @param dayNum要加快的天数
     * @return QString 成功则返回空串，否则返回错误信息
     */
    QString addDays(int dayNum);

    /**
     * @brief 判断某时间是否到达，以物流系统时间为判据。
     *
     * @param time 时间
     * @return true 达到
     * @return false 未到达
     */
    bool isDue(const Time time) const;

    int getCurYear() const { return curTime.year; };

    int getCurMonth() const { return curTime.month; };

    int getCurDay() const { return curTime.day; };

private:
    Time curTime; //物流系统当前时间
};

#endif