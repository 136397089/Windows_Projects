#pragma once
#include "indexnumber.h"

struct KDJ
{
public:
	KDJ():K_OF_KDJ(0.0f),D_OF_KDJ(0.0f),J_OF_KDJ(0.0f){}
	float K_OF_KDJ;
	float D_OF_KDJ;
	float J_OF_KDJ;
};
class CKDJIndexNumber :
	public CIndexNumber
{
public:
	CKDJIndexNumber(void);
	~CKDJIndexNumber(void);
	virtual void UpdateFile(string strPath);
	KDJ GetNextKDJ(float closeData, vector<float> High,vector<float> Low,KDJ mFrontKDJ);
};
