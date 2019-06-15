#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <iomanip>
#include "number/commonfunction.h"


using namespace std;

#ifndef DATETOOL_H
#define DATETOOL_H


//////////////////////////////////////////////////////////////////////////
//时间 年 月 日
//
//////////////////////////////////////////////////////////////////////////
class CDate
{
public:

	//构造函数
	CDate();
	CDate(int year, int month, int day);
	CDate(int year, int month, int day, int hour, int minute, int second);
	//析构函数
	~CDate();
	//拷贝构造函数
	CDate(const CDate& d);//必须传引用，传值会引发无穷递归调用

	//赋值重载
	CDate& operator=(const CDate& d);

	bool operator ==(const CDate& d)const; //判断两个日期相等
	bool operator !=(const CDate& d)const;//判断两个日期不相等
	bool operator>(const CDate& d)const; // > 
	bool operator>=(const CDate& d)const;// >= 
	bool operator<(const CDate& d)const; // <
	bool operator<=(const CDate& d)const; //<=


	bool SetDay(const string& _date);//设置日期

	bool SetTime(const string& _time);

	bool IsOnTheSameDay(const CDate& d);
	CDate GetFrontWeekDay(int day, int week);//获得前几个星期的周几日期

	int GetWeekDay();//获得当日的星期数
	int GetMonth();

	string GetDay()const;//获得日期字符串
	string GetDateTime()const;//获得日期时间字符串
	string GetTime()const;//获得日期字符串
	string GetDay(const string& splitter)const;//获得日期字符串

	bool IsLeapYear(int year);//是否是润年

	int GetDayByYearAndMonth(int year, int month);//判断返回的月份的天数
	//日期加上天数，并不会改变原来的日期值
	CDate operator+(int dayNum);
	CDate& operator+=(int days);// +=,会改变原来的值，进而在原来的值上面继续加指定的天数
	CDate& operator++(); //前置++
	CDate operator++(int);//后置++ 
	//日期减去天数，并不会改变原来的日期值
	CDate operator-(int dayNum);
	CDate& operator-=(int dayNum);//-=,会改变原来的值，进而在原来的值上面继续减指定的天数
	CDate& operator--();//前置– 
	CDate operator--(int);//后置 –
	int operator-(const CDate& d); // 两个日期相隔天数 

	int _year;
	int _month;
	int _day;

	int _hour;
	int _minute;
	int _second;
private:
	string _strdate;
	string _strTime;
};






#endif