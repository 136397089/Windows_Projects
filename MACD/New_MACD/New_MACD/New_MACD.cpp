// New_MACD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Macd.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CMacdCalculater mMacd;
	Macd temp,resultmacd;
	
	temp.m12=22.3;
	temp.m26=22;
	temp.bar=-0.30;
	for (int i=-100;i<100;i++)
	{
		resultmacd = mMacd.GetNextMacd(i*0.01+19,temp);
		cout<<resultmacd.bar<<endl;
	}
// 	mMacd.UpdateFile("D:\\StockData\\*.csv");
//	vector<float> tempdata=mMacd.ReadStringFormFile("D:\\StockData\\000002.csv","Close");
	system("pause");
	return 0;
}
