#pragma once
#include <iostream>
#include <complex>
#include <vector>
#include "number/commonfunction.h"
#include "glog/logging.h"

#define MAX_VECTOR_SIZE 2048 // 原始信号最大允许长度
#define PI 3.141592653 // 圆周率π的近似值

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

class CComplexNumber
{
public:
	CComplexNumber(double rl, double im);
	CComplexNumber(void);
	~CComplexNumber(void);
public:
	// 重载四则运算符号
	inline CComplexNumber CComplexNumber::operator +(const CComplexNumber &c) {
		return CComplexNumber(m_rl + c.m_rl, m_im + c.m_im);
	}
	inline CComplexNumber CComplexNumber::operator -(const CComplexNumber &c) {
		return CComplexNumber(m_rl - c.m_rl, m_im - c.m_im);
	}
	inline CComplexNumber CComplexNumber::operator *(const CComplexNumber &c) {
		return CComplexNumber(m_rl*c.m_rl - m_im*c.m_im, m_im*c.m_rl + m_rl*c.m_im);
	}

	inline CComplexNumber CComplexNumber::operator /(const CComplexNumber &c) {
		if ((0 == c.m_rl) && (0 == c.m_im)) {
			//LOG(ERROR)("11111 ComplexNumber    ERROR: divider is 0!");
			return CComplexNumber(m_rl, m_im);
		}
		return CComplexNumber((m_rl*c.m_rl + m_im*c.m_im) / (c.m_rl*c.m_rl + c.m_im*c.m_im),
			(m_im*c.m_rl - m_rl*c.m_im) / (c.m_rl*c.m_rl + c.m_im*c.m_im));
	}
	void   SetValue(double rl, double im);
public:
	double     m_rl;    // 实部, real part
	double     m_im;    // 虚部, imagery part
};



// class CDft1
// {
// public:
// 	CDft1(void);
// 	~CDft1(void);
// public:
// 	bool dft(double IN const vec[], int IN const len); // 一维离散傅里叶变换
// 	bool idft(LPVOID OUT *pVec, int OUT *ilen); // 一维离散傅里叶逆变换
// 
// 	bool has_dft_vector();// 是否已存有变换结果
// 
// 	void clear_dft_vector(); // 清除已有的变换结果
// 	void print();// 打印变换结果
// public:
// 	CComplexNumber *m_dft_vector;// 保存变换结果的容器
// private:
// 	bool      m_has_dft_vector;
// 	int         m_dft_vector_size;// 变换结果的长度
// };

