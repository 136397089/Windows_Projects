#include "StdAfx.h"
#include "KDJ.h"

using namespace std;
CKDJIndexNumber::CKDJIndexNumber(void)
{
}

CKDJIndexNumber::~CKDJIndexNumber(void)
{
}

void CKDJIndexNumber::UpdateFile( string strPath )
{
	
}

KDJ CKDJIndexNumber::GetNextKDJ( float closeData, vector<float> High,vector<float> Low ,KDJ mFrontKDJ)
{
	float H9 = *max_element(High.begin(),High.end());
	float L9 = *min_element(Low.begin(),Low.end());
	KDJ m_kdj;
	if (L9 == H9) 
	{
		return m_kdj;
	}

	float RSV = (closeData - L9)*100/(H9-L9);
	m_kdj.K_OF_KDJ = RSV/3+2*mFrontKDJ.K_OF_KDJ/3;
	m_kdj.D_OF_KDJ = m_kdj.K_OF_KDJ/3+2*mFrontKDJ.D_OF_KDJ/3;
	m_kdj.J_OF_KDJ = 3*m_kdj.D_OF_KDJ-2*m_kdj.K_OF_KDJ;

	return m_kdj;
}
