#pragma once
#include <iostream>
#include <complex>
#include <vector>
#include "number/commonfunction.h"
#include "glog/logging.h"

#define MAX_VECTOR_SIZE 2048 // ԭʼ�ź����������
#define PI 3.141592653 // Բ���ʦеĽ���ֵ

using namespace std;
typedef double DFTBaseData;
typedef vector<complex<double>> DFTDataList;
class CDFT
{
public:
	CDFT();
	~CDFT();
	DFTDataList DFT(DFTDataList& theData);

	DFTDataList DFT(const VStockData& theData);

};

