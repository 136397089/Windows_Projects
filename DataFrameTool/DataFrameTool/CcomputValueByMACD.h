#pragma once
#ifndef _COMPUTEVALUEBTMACE_H
#define _COMPUTEVALUEBTMACE_H

//һ��Ĺ�Ʊ����
struct OneDayVariable
{
	float closePrice;
	float changeRate;
	float MACD_EDA;
	float MACD_Diff;
	float MACD_BAR;
};
class CCcomputValueByMACD
{
public:
	CCcomputValueByMACD();
	~CCcomputValueByMACD();
	float GetValue(const OneDayVariable& dayVar);
private:

};



#endif
