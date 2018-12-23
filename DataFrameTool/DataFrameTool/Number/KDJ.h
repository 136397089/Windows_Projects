#pragma once
#include "indexnumber.h"

////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
struct KDJ
{
public:
	KDJ():K_OF_KDJ(0.0f),D_OF_KDJ(0.0f),J_OF_KDJ(0.0f){}
	float K_OF_KDJ;
	float D_OF_KDJ;
	float J_OF_KDJ;
};
///////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
class CKDJIndexNumber :
	public CNumberManager
{
public:
	CKDJIndexNumber(void);
	~CKDJIndexNumber(void);
	//////////////////////////////////////////////////////////////////////////
	//更新目录下所有CSV文件里的KDJ值(数据按列排)
	//strPath:目录路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileColumn(string strFolderPath);
	//////////////////////////////////////////////////////////////////////////
	//更新目录下所有CSV文件里的KDJ值(数据按行排)
	//strPath:目录路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdatePathFilesRanks(string strPath);
	//////////////////////////////////////////////////////////////////////////
	//更新CSV文件文件里的KDJ值(数据按行排)
	//strPath:文件路径
	//////////////////////////////////////////////////////////////////////////
	virtual void UpdateFileRanks(string strfilename);
private:
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////
	KDJ GetNextKDJ(float closeData, vector<float> High, vector<float> Low, KDJ mFrontKDJ);
};
