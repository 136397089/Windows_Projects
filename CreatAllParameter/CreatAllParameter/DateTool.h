#pragma once
#include "stdafx.h"
//#include <Windows.h>
// #include <iostream>
// #include <fstream>
#include <string>
#include <vector>
// #include <list>
//#include <sstream>
//#include <algorithm>
#include <iomanip>
#include "number/commonfunction.h"


using namespace std;

#ifndef DATETOOL_H
#define DATETOOL_H


//////////////////////////////////////////////////////////////////////////
//ʱ�� �� �� ��
//
//////////////////////////////////////////////////////////////////////////
class CDate
{
public:

	//���캯��
	CDate();
	CDate(int year, int month, int day);
	//��������
	~CDate();
	//�������캯��
	CDate(const CDate& d);//���봫���ã���ֵ����������ݹ����

	//��ֵ����
	CDate& operator=(const CDate& d);

	bool operator ==(const CDate& d)const; //�ж������������
	bool operator !=(const CDate& d)const;//�ж��������ڲ����
	bool operator>(const CDate& d)const; // > 
	bool operator>=(const CDate& d)const;// >= 
	bool operator<(const CDate& d)const; // <
	bool operator<=(const CDate& d)const; //<=


	bool SetDay(string date);//��������

	CDate GetFrontWeekDay(int day, int week);//���ǰ�������ڵ��ܼ�����

	int GetWeekDay();//��õ��յ�������

	string GetDay()const;//��������ַ���
	string GetDay(const string& splitter)const;//��������ַ���

	bool IsLeapYear(int year);//�Ƿ�������

	int GetDayByYearAndMonth(int year, int month);//�жϷ��ص��·ݵ�����
	//���ڼ���������������ı�ԭ��������ֵ
	CDate operator+(int dayNum);
	CDate& operator+=(int days);// +=,��ı�ԭ����ֵ��������ԭ����ֵ���������ָ��������
	CDate& operator++(); //ǰ��++
	CDate operator++(int);//����++ 
	//���ڼ�ȥ������������ı�ԭ��������ֵ
	CDate operator-(int dayNum);
	CDate& operator-=(int dayNum);//-=,��ı�ԭ����ֵ��������ԭ����ֵ���������ָ��������
	CDate& operator--();//ǰ�èC 
	CDate operator--(int);//���� �C
	int operator-(const CDate& d); // ��������������� 

	int _year;
	int _month;
	int _day;
private:
	string dateTime;
};


#endif