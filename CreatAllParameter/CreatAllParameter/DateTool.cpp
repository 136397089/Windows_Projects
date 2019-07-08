#include "stdafx.h"
#include "DateTool.h"
#include <iostream>
using namespace std;

CDate::CDate()
	:_hour(0), _minute(0), _second(0),
	_year(0), _month(0), _day(0)
{

}
//构造函数
CDate::CDate(int year, int month, int day)
	:_hour(0),_minute(0),_second(0)
{
	_year = year;
	_month = month;
	_day = day;
	_hour = 0;
	_minute = 0;
	_second = 0;
	_strdate = "";
	_strTime = "";
}
//析构函数
CDate::~CDate()
{

}
//拷贝构造函数
CDate::CDate(const CDate& d)//必须传引用，传值会引发无穷递归调用
	:_hour(0), _minute(0), _second(0)
{
	_year = d._year;
	_month = d._month;
	_day = d._day;
	_strdate = d._strdate;
	_hour = d._hour;
	_minute = d._minute;
	_second = d._second;
	_strTime = d._strTime;
}

CDate::CDate(int year, int month, int day, int hour, int minute, int second)
{
	_year = year;
	_month = month;
	_day = day;
	_hour = hour;
	_minute = minute;
	_second = second;
}

//赋值重载
CDate& CDate::operator=(const CDate& d)
{
	if (this != &d)
	{
		this->_year = d._year;
		this->_month = d._month;
		this->_day = d._day;
		this->_strdate = d._strdate;
		this->_hour = d._hour;
		this->_minute = d._minute;
		this->_second = d._second;
		this->_strTime = d._strTime;
	}
	return *this;
}

bool CDate::operator ==(const CDate& d)const//判断两个日期相等
{
	return _year == d._year
		&&_month == d._month
		&&_day == d._day
		&&_hour == d._hour
		&&_minute == d._minute
		&&_second == d._second;
}
bool CDate::operator !=(const CDate& d)const//判断两个日期不相等
{
	return !(*this == d);
}
bool CDate::operator>(const CDate& d) const// > 
{
	if (	(_year > d._year)
		|| (_year == d._year && _month > d._month)
		|| (_year == d._year && _month == d._month && _day > d._day)
		|| (_year == d._year && _month == d._month && _day == d._day && _hour > d._hour)
		|| (_year == d._year && _month == d._month && _day == d._day && _hour == d._hour && _minute > d._minute)
		|| (_year == d._year && _month == d._month && _day == d._day && _hour == d._hour && _minute == d._minute && _second > d._second)
		)
	{
		return true;
	}
	return false;
}
bool CDate::operator>=(const CDate& d)const// >= 
{
	return ((*this == d) || *this > d);
}
bool CDate::operator<(const CDate& d) const// <
{
	if ((_year < d._year)
		|| (_year == d._year && _month < d._month)
		|| (_year == d._year && _month == d._month && _day < d._day)
		|| (_year == d._year && _month == d._month && _day == d._day && _hour < d._hour)
		|| (_year == d._year && _month == d._month && _day == d._day && _hour == d._hour && _minute < d._minute)
		|| (_year == d._year && _month == d._month && _day == d._day && _hour == d._hour && _minute == d._minute && _second < d._second)
		)
	{
		return true;
	}
	return false;
}
bool CDate::operator<=(const CDate& d) const//<=
{
	return ((*this == d) || *this < d);
}

//是否是润年
bool CDate::IsLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int CDate::GetDayByYearAndMonth(int year, int month)//判断返回的月份的天数
{
	int MonthDay[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (IsLeapYear(year) && month == 2)
	{
		return 29;
	}
	else{
		return MonthDay[month];
	}
}


//日期加上天数，并不会改变原来的日期值
CDate CDate::operator+(int dayNum)
{
	if (_day < 0)
	{
		return *this - (-_day);
	}
	CDate tmpDate(*this);//拷贝this指针
	tmpDate._day += dayNum;
	while (tmpDate._day > GetDayByYearAndMonth(_year, _month))
	{
		tmpDate._day = tmpDate._day - GetDayByYearAndMonth(tmpDate._year, tmpDate._month);
		tmpDate._month += 1;
		if (tmpDate._month > 12)
		{
			++tmpDate._year;
			tmpDate._month = 1;
		}
	}
	return tmpDate;
}

CDate CDate::operator+(const CDate& d)
{
	CDate tmpDate(*this);//拷贝this指针
	if (	d._year != 0 || d._month != 0
		|| d._hour > 23 || d._hour <0 
		|| tmpDate._minute>59 || tmpDate._minute < 0
		|| tmpDate._second > 59 || tmpDate._second<0
		)
	{
		return tmpDate;
	}
	tmpDate._hour += d._hour;
	tmpDate._minute += d._minute;
	tmpDate._second += d._second;
	int DeltDay = 0;
	while (tmpDate._second >= 60)
	{
		tmpDate._second -= 60;
		tmpDate._minute++;
	}
	while (tmpDate._minute >= 60)
	{
		tmpDate._minute -= 60;
		tmpDate._hour++;
	}
	while (tmpDate._hour >= 24)
	{
		tmpDate._hour -= 24;
		DeltDay++;
	}
	DeltDay += d._day;
	tmpDate += DeltDay;
	return tmpDate;
}

CDate& CDate::operator+=(int days)// +=,会改变原来的值，进而在原来的值上面继续加指定的天数
{
	_day += days;
	while (_day > GetDayByYearAndMonth(_year, _month))
	{
		_day = _day - GetDayByYearAndMonth(_year, _month);
		_month += 1;
		if (_month > 12)
		{
			++_year;
			_month = 1;
		}
	}
	return *this;
}
CDate& CDate::operator++() //前置++
{
	*this += 1;
	return *this;
}
CDate CDate::operator++(int)//后置++ 
{
	CDate tmpDate(*this);
	*this += 1;
	return *this;
}
//日期减去天数，并不会改变原来的日期值
CDate CDate::operator-(int dayNum)
{
	CDate tmpDate(*this);
	tmpDate._day -= dayNum;
	while (tmpDate._day <= 0)
	{
		tmpDate._month -= 1;
		if (tmpDate._month <= 0)
		{
			--tmpDate._year;
			tmpDate._month = 12;
		}
		tmpDate._day += GetDayByYearAndMonth(tmpDate._year, tmpDate._month);
	}
	return tmpDate;
}
CDate& CDate::operator-=(int dayNum)//-=,会改变原来的值，进而在原来的值上面继续减指定的天数
{
	_day -= dayNum;
	while (_day <= 0)
	{
		_month -= 1;
		if (_month <= 0)
		{
			--_year;
			_month = 12;
		}
		_day += GetDayByYearAndMonth(_year, _month);
	}
	return *this;
}
CDate& CDate::operator--()//前置– 
{
	*this -= 1;
	return *this;
}
CDate CDate::operator--(int)//后置 –
{
	CDate tmpDate(*this);//先对*this进行拷贝
	*this -= 1;
	return *this;
}
int CDate::operator-(const CDate& d) // 两个日期相隔天数 
{
	CDate bigday(*this);
	CDate smallday(d);
	int day = 0;
	int flags = 1;
	if (bigday < smallday)
	{
		smallday = *this;
		bigday = d;
		flags *= -1;
	}

	while (smallday != bigday)
	{
		smallday = smallday + 1;
		++day;
	}

	return day*flags;
}

bool CDate::SetDay(const string& _date)
{
	StringList vstringlist;
	vstringlist.clear();
	string _time;
	string tempdate = _date;
	if (tempdate.size() < 8)
	{
		return false;
	}
	//处理有小时和分钟的情况
	if (tempdate.find(" ") > 0 && tempdate.find(" ") < 20)
	{
		CutString(tempdate, " ", vstringlist);
		if (vstringlist.size()!=2)
		{
			cout << "Time string error"<<endl;
			system("pause");
			exit(-1);
			return false;
		}
		SetTime(vstringlist[1]);
		tempdate = vstringlist[0];
		vstringlist.clear();
	}

	if (tempdate.find("-") > 0 && tempdate.find("-") < 20)
	{
		CutString(tempdate, "-", vstringlist);
		if (vstringlist.size() != 3)
		{
			return false;
		}
	}
	else if (tempdate.find("/") > 0 && tempdate.find("/") < 20)
	{
		CutString(tempdate, "/", vstringlist);
		if (vstringlist.size() != 3)
		{
			return false;
		}
	}
	else if (tempdate.size() == 8)
	{
		vstringlist.push_back(tempdate.substr(0, 4));
		vstringlist.push_back(tempdate.substr(4, 2));
		vstringlist.push_back(tempdate.substr(6, 4));
	}
	_year = atoi(vstringlist[0].c_str());
	_month = atoi(vstringlist[1].c_str());
	_day = atoi(vstringlist[2].c_str());
	_strdate = tempdate;
	return true;
}


std::string CDate::GetDay() const
{
	stringstream ss;

	ss << _year << "-" << setw(2) << setfill('0') << _month << "-" << setw(2) << setfill('0') << _day;
	if (_hour != 0)
		ss << "-" << setw(2) << setfill('0') << _hour << "-" << setw(2) << setfill('0') << _minute;
	return ss.str();
}

std::string CDate::GetDateTime() const
{
	stringstream ss;

	ss << _year 
		<< "-" << setw(2) << setfill('0') << _month
		<< "-" << setw(2) << setfill('0') << _day
		<< " " << setw(2) << setfill('0') << _hour 
		<< ":" << setw(2) << setfill('0') << _minute 
		<< ":" << setw(2) << setfill('0') << _second;
	return ss.str();

}

std::string CDate::GetTime() const
{
	stringstream ss;
	ss  << setw(2) << setfill('0') << _hour
		<< ":" << setw(2) << setfill('0') << _minute
		<< ":" << setw(2) << setfill('0') << _second;
	return ss.str();

}

std::string CDate::GetDay(const string& splitter) const
{
	stringstream ss;
	ss << _year << splitter << setw(2) << setfill('0') << _month << splitter << setw(2) << setfill('0') << _day;
	if (_hour != 0)
		ss << "-" << setw(2) << setfill('0') << _hour << "-" << setw(2) << setfill('0') << _minute;
	return ss.str();
}

int CDate::GetWeekDay()
{
	float Doy = floor(_month * 275 / 9) - floor((_month + 9) / 12)*(floor((_year - 4 * floor(_year / 4) + 2) / 3) + 1) + _day - 30;
	int allDay = (_year - 1) * 365 + floor((_year - 1) / 4) - floor((_year - 1) / 100) + floor((_year - 1) / 400) + Doy;
	int week = allDay % 7;
	return week;
}

CDate CDate::GetFrontWeekDay(int day, int week)
{
	CDate tempday = *this;
	int diffDay = week * 7 + GetWeekDay() - day;
	tempday -= diffDay;

	return tempday;
}

int CDate::GetMonth()
{
	return _month;
}

bool CDate::SetTime(const string& _time)
{
	string temptime = _time;
	if (temptime.size()<3){
		cout << "time string error.";
		system("pause");
		exit(-1);
		return false;
	}
	StringList timelist;
	if (temptime.find(":") > 0 && temptime.find(":") < 20){
		CutString(temptime, ":", timelist);
		if (timelist.size() == 2){
			_hour = atoi(timelist[0].c_str());
			_minute = atoi(timelist[1].c_str());
		}
		else if (timelist.size() == 3){
			_hour = atoi(timelist[0].c_str());
			_minute = atoi(timelist[1].c_str());
			_second = atoi(timelist[2].c_str());
		}
		else
			return false;
	}
	else
		return false;
	_strTime = _time;
	return true;
}

bool CDate::IsOnTheSameDay(const CDate& d) const
{
	return _year == d._year && _month == d._month && _day == d._day;
}


