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

class CComplexNumber
{
public:
	CComplexNumber(double rl, double im);
	CComplexNumber(void);
	~CComplexNumber(void);
public:
	// ���������������
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
	double     m_rl;    // ʵ��, real part
	double     m_im;    // �鲿, imagery part
};



// class CDft1
// {
// public:
// 	CDft1(void);
// 	~CDft1(void);
// public:
// 	bool dft(double IN const vec[], int IN const len); // һά��ɢ����Ҷ�任
// 	bool idft(LPVOID OUT *pVec, int OUT *ilen); // һά��ɢ����Ҷ��任
// 
// 	bool has_dft_vector();// �Ƿ��Ѵ��б任���
// 
// 	void clear_dft_vector(); // ������еı任���
// 	void print();// ��ӡ�任���
// public:
// 	CComplexNumber *m_dft_vector;// ����任���������
// private:
// 	bool      m_has_dft_vector;
// 	int         m_dft_vector_size;// �任����ĳ���
// };

