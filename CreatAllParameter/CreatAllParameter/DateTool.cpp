#include "stdafx.h"
#include "DateTool.h"

CDate::CDate()
{

}
//构造函数
CDate::CDate(int year, int month, int day)
{
	_year = year;
	_month = month;
	_day = day;
}
//析构函数
CDate::~CDate()
{

}
//拷贝构造函数
CDate::CDate(const CDate& d)//必须传引用，传值会引发无穷递归调用
{
	_year = d._year;
	_month = d._month;
	_day = d._day;
	dateTime = d.dateTime;
}
//赋值重载
CDate& CDate::operator=(const CDate& d)
{
	if (this != &d)
	{
		this->_year = d._year;
		this->_month = d._month;
		this->_day = d._day;
	}
	return *this;
}


bool CDate::operator ==(const CDate& d)const//判断两个日期相等
{
	return _year == d._year
		&&_month == d._month
		&&_day == d._day;
}
bool CDate::operator !=(const CDate& d)const//判断两个日期不相等
{
	return !(*this == d);
}
bool CDate::operator>(const CDate& d) const// > 
{
	if ((_year > d._year) || (_year == d._year&&_month > d._month) || (_year == d._year&&_month == d._month&&_day > d._day))
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
	if ((_year < d._year) || (_year == d._year&&_month < d._month) || (_year == d._year&&_month == d._month&&_day < d._day))
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
	while (tmpDate._day < 0)
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
	while (_day < 0)
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

bool CDate::SetDay(string date)
{
	StringList vtimelist;
	vtimelist.clear();
	dateTime = date;
	if (date.size() < 8)
	{
		return false;
	}
	if (date.find("-") > 0 && date.find("-") < 20)
	{
		vtimelist = CutString(date, "-");
		if (vtimelist.size() != 3)
		{
			return false;
		}
	}
	else if (date.find("/") > 0 && date.find("/") < 20)
	{
		vtimelist = CutString(date, "/");
		if (vtimelist.size() != 3)
		{
			return false;
		}
	}
	else if (date.size() == 8)
	{
		vtimelist.push_back(date.substr(0, 4));
		vtimelist.push_back(date.substr(4, 2));
		vtimelist.push_back(date.substr(6, 4));
	}
	_year = atoi(vtimelist[0].c_str());
	_month = atoi(vtimelist[1].c_str());
	_day = atoi(vtimelist[2].c_str());

	int week = GetWeekDay();
	return true;
}


std::string CDate::GetDay() const
{
	stringstream ss;

	ss << _year << "-" << setw(2) << setfill('0') << _month << "-" << setw(2) << setfill('0') << _day;

	return ss.str();
}

std::string CDate::GetDay(const string& splitter) const
{
	stringstream ss;
	ss << _year << splitter << setw(2) << setfill('0') << _month << splitter << setw(2) << setfill('0') << _day;
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
	tempday = tempday - diffDay;
	return tempday;
}
