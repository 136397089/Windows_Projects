#include "stdafx.h"
#include "DFT.h"


CDFT::CDFT()
{
}


CDFT::~CDFT()
{
}

// vector< complex<double> > CDFT::DFT(vector< complex<double> >& theData)
// {
// 
// }

// CComplexNumber::CComplexNumber(void)
// {
// 	m_rl = 0;
// 	m_im = 0;
// }
// 
// CComplexNumber::CComplexNumber(double rl, double im)
// {
// 	m_rl = rl;
// 	m_im = im;
// }
// 
// CComplexNumber::~CComplexNumber(void)
// {
// }
// 
// void CComplexNumber::SetValue(double rl, double im) {
// 	m_rl = rl;
// 	m_im = im;
// }


// CDft1::CDft1(void)
// {
// 	m_dft_vector = NULL;
// 	m_has_dft_vector = false;
// 	m_dft_vector_size = 0;
// }
// 
// CDft1::~CDft1(void)
// {
// 	if (m_has_dft_vector && (NULL != m_dft_vector) && (m_dft_vector_size > 0))
// 		delete[] m_dft_vector;
// }
// 
// 
// bool CDft1::has_dft_vector()
// {
// 	return m_has_dft_vector;
// }
// 
// 
// void CDft1::clear_dft_vector()
// {
// 	if (m_has_dft_vector && (NULL != m_dft_vector) && (m_dft_vector_size > 0)) {
// 		delete[] m_dft_vector;
// 		m_has_dft_vector = false;
// 		m_dft_vector_size = 0;
// 	}
// }
// 
// void CDft1::print()
// {
// 	char msg[256] = "11111 ";
// 
// 	if ((!m_has_dft_vector) || (NULL == m_dft_vector) || (m_dft_vector_size <= 0))
// 		return;
// 
// 	for (int i = 0; i < m_dft_vector_size; i++) {
// 		sprintf(msg + 6, "%d: %lf + %lfi", i + 1, m_dft_vector[i].m_rl, m_dft_vector[i].m_im);
// 		LOG(INFO)<<msg;
// 	}
// }
// 
// 
// bool CDft1::dft(double IN const vec[], int IN const len)
// {
// 	if ((len <= 0) || (NULL == vec))
// 		return false;
// 
// 	clear_dft_vector();
// 
// 	m_dft_vector = new CComplexNumber[len];
// 	for (int u = 0; u < len; u++) {
// 		m_dft_vector[u].SetValue(0, 0);
// 	}
// 	CComplexNumber   cplTemp(0, 0);
// 	double fixed_factor = (-2 * PI) / len;
// 	// 这是最关键的两个嵌套for循环，是离散傅里叶变换公式的代码实现，其中的运算操作和公式定义是完全一致的
// 	for (int u = 0; u < len; u++) {
// 		for (int x = 0; x < len; x++) {
// 			double power = u * x * fixed_factor;
// 			cplTemp.m_rl = vec[x] * cos(power);
// 			cplTemp.m_im = vec[x] * sin(power);
// 			m_dft_vector[u] = m_dft_vector[u] + cplTemp;
// 		}
// 	}
// 
// 	m_has_dft_vector = true;
// 	m_dft_vector_size = len;
// 	return true;
// }
// 
// 
// bool CDft1::idft(LPVOID OUT  *pVec, int OUT *ilen)
// {
// 	if ((!m_has_dft_vector) || (NULL == m_dft_vector) || (m_dft_vector_size <= 0))
// 		return false;
// 
// 	if (*pVec)
// 		delete[] * pVec;
// 	*pVec = (LPVOID)new double[m_dft_vector_size];
// 
// 	CComplexNumber   cplTemp(0, 0);
// 	CComplexNumber   cplResult(0, 0);
// 	double fixed_factor = (2 * PI) / m_dft_vector_size;
// 	// 这是最关键的两个嵌套for循环，是离散傅里叶逆变换公式的代码实现，其中的运算操作和公式定义是完全一致的
// 	for (int x = 0; x < m_dft_vector_size; x++) {
// 		for (int u = 0; u < m_dft_vector_size; u++) {
// 			double power = u * x * fixed_factor;
// 			cplTemp.SetValue(cos(power), sin(power));
// 			cplResult = cplResult + m_dft_vector[u] * cplTemp;
// 		}
// 		((double*)*pVec)[x] = cplResult.m_rl / m_dft_vector_size; cplResult.SetValue(0, 0);
// 	}
// 
// 	*ilen = m_dft_vector_size;
// 	return true;
// }


DFTDataList CDFT::DFT(DFTDataList& theData)
{
	// Define the Size of the read in vector
	const int S = theData.size();

	// Initalise new vector with size of S
	DFTDataList out(S, 0);
	for (unsigned i = 0; (i < S); i++)
	{
		out[i] = complex<DFTBaseData>(0.0, 0.0);
		for (unsigned j = 0; (j < S); j++)
		{
			out[i] += theData[j] * polar<DFTBaseData>(1.0, -2 * PI * i * j / S);
		}
	}

	return out;
}

DFTDataList CDFT::DFT(const VStockData& theData)
{
	DFTDataList numbers;

	for (int i = 0; i < theData.size(); i++)
	{
		numbers.push_back(theData[i]);
	}

	return DFT(numbers);
}
