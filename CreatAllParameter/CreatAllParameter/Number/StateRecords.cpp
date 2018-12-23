#include "stdafx.h"
#include "StateRecords.h"
#include "../glog/logging.h"
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
StateRecords::StateRecords()
{
	Inition();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
StateRecords::~StateRecords()
{
}

//////////////////////////////////////////////////////////////////////////
//输入数据，进行单日静态分析
//////////////////////////////////////////////////////////////////////////
void StateRecords::DailyStaticStateRecord(const SigDayTechIndex& theIndex, const DatePriceData& OneDayData)
{
	//LOG(INFO) << "Static Record Begin." << endl;
	//静态分析
	StaticStateRecordSigPoint(_eMACD,
		theIndex._MacdData.diff,
		theIndex._MacdData.dea,
		OneDayData.mDate.dateTime);
	StaticStateRecordSigPoint(_eTRIX,
		theIndex._TrixData._TRIX,
		theIndex._TrixData._TRMA,
		OneDayData.mDate.dateTime);
	StaticStateRecordSigPoint(_eDMA,
		theIndex._DMAData._DDD,
		theIndex._DMAData._AMA,
		OneDayData.mDate.dateTime);
	StaticStateRecordSigPoint(_eKDJ,
		theIndex._Kdj.J_OF_KDJ,
		theIndex._Kdj.D_OF_KDJ,
		OneDayData.mDate.dateTime);
	StaticStateRecordSigPoint(_eASI,
		theIndex._Asi._asi,
		theIndex._Asi._asit,
		OneDayData.mDate.dateTime);
	//LOG(INFO) << "Static Record Finish." << endl;
}
///////////////////////////////////////////////////////////////////////////
//输入数据，进行单日局部分析
//////////////////////////////////////////////////////////////////////////
void StateRecords::DailyLocalStateRecord(const SigDayTechIndex& dayIndex, const DatePriceData& OneDayData)
{
	//LOG(INFO) << "Local Record Begin." << endl;
	//局部分析
	if (_CurrentMark < 60)
		return;
	LocalStateRecordSingleStep(_eLowPrice, OneDayData._Low, OneDayData.mDate.dateTime);
	LocalStateRecordSingleStep(_eMACD, dayIndex._MacdData.diff, OneDayData.mDate.dateTime);
	LocalStateRecordSingleStep(_eTRIX, dayIndex._TrixData._TRIX, OneDayData.mDate.dateTime);
	LocalStateRecordSingleStep(_eDMA, dayIndex._DMAData._DDD, OneDayData.mDate.dateTime);
	LocalStateRecordSingleStep(_eASI, dayIndex._Asi._asi, OneDayData.mDate.dateTime);
	LocalStateRecordSingleStep(_eKDJ, dayIndex._Kdj.D_OF_KDJ, OneDayData.mDate.dateTime);
	return;
	//LOG(INFO) << "Local Record Finish." << endl;
}

//////////////////////////////////////////////////////////////////////////
//返回静态分析结果
//////////////////////////////////////////////////////////////////////////
AllSigParaState_Static StateRecords::GetNowStaticState()
{
	AllSigParaState_Static temp;
	temp._ASIType = _NowState_Static[ASIIndex];
	temp._MACDType = _NowState_Static[MACDIndex];
	temp._DMAType = _NowState_Static[DMAIndex];
	temp._TRIXType = _NowState_Static[TRIXIndex];
	temp._KDJType = _NowState_Static[KDJIndex];
	return temp;
}


std::string StateRecords::GetLastError()
{
	return _LastError;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void StateRecords::StaticStateRecordSigPoint(
	const IndexGenre& valueType,
	const tySData& FrontData,
	const tySData& BackData,
	const string& Day)
{
	//unsigned int& mFrontIndex = _FrontCloseMark_Static[valueType];
	FeatPoint_Static& mfrontStatic = _NowState_Static[valueType];

	//上穿判断
	if (mfrontStatic._Value < 0 && (FrontData - BackData) > 0 )
	{
		_NowStateChange = true;
		mfrontStatic._Value = FrontData - BackData;
		mfrontStatic._TimeIndex = _CurrentMark;
		mfrontStatic._IndexGenre = valueType;
		mfrontStatic._Day = Day;
		if (FrontData >= 0)
			mfrontStatic._StaticFeat = _ePositiveUpClose;
		else
			mfrontStatic._StaticFeat = _eNegaTiveUpClose;
		_vIndexRecord_State.push_back(mfrontStatic);
	}
	//下穿判断
	else if (mfrontStatic._Value >= 0 && (FrontData - BackData) < 0)
	{
		_NowStateChange = true;
		mfrontStatic._Value = FrontData - BackData;
		mfrontStatic._TimeIndex = _CurrentMark;
		mfrontStatic._IndexGenre = valueType;
		mfrontStatic._Day = Day;
		if (FrontData >= 0)
			mfrontStatic._StaticFeat = _ePositiveDownClose;
		else
			mfrontStatic._StaticFeat = _eNegaTiveDownClose;
		_vIndexRecord_State.push_back(mfrontStatic);
	}
	return;
}


//////////////////////////////////////////////////////////////////////////
//对最终的数据进行趋势判断
//////////////////////////////////////////////////////////////////////////
bool StateRecords::GetFinalTrend(const StockDataPointer& pointers)
{
	if (!ChckeAllPointer(pointers))
		return false;

	VStockData priceLowValue;
	VStockData DiffLowValue;
	vector<unsigned int > priceLowIndex;
	vector<unsigned int > diffLowIndex;
	priceLowValue.clear();
	DiffLowValue.clear();
	if (!GetDownIndex(*pointers._pLow, 50, priceLowIndex, priceLowValue))
	{
		LOG(ERROR) << "GetDownIndex Fail.";
		return false;
	}
	if (!GetUpIndex(*pointers._pDiff, 20, diffLowIndex, DiffLowValue))
	{
		LOG(ERROR) << "GetUpIndex Fail.";
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool StateRecords::ChckeAllPointer(const StockDataPointer& pointers)
{
	if ( 
		pointers._pCloseData == nullptr
		|| pointers._pOpenData == nullptr
		|| pointers._pHigh == nullptr
		|| pointers._pLow == nullptr
		|| pointers._pAMAValue == nullptr
		|| pointers._pMa12 == nullptr
		|| pointers._pMa26 == nullptr
		|| pointers._pDiff == nullptr
		|| pointers._pDEA == nullptr
		|| pointers._pDMAValue == nullptr
		|| pointers._pAMAValue == nullptr
		|| pointers._pTRIX == nullptr
		|| pointers._pTRMA == nullptr
		|| pointers._pK == nullptr
		|| pointers._pD == nullptr
		|| pointers._pJ == nullptr
		|| pointers._pAsi == nullptr
		|| pointers._pAsit == nullptr)
	{
		LOG(ERROR) << "Has empty pointer.";
		return false;
	}

	if (
		   pointers._pCloseData->size() != pointers._pOpenData->size()
		|| pointers._pHigh->size() != pointers._pCloseData->size()
		|| pointers._pLow->size() != pointers._pCloseData->size()
		|| pointers._pAMAValue->size() != pointers._pCloseData->size()
		|| pointers._pMa12->size() != pointers._pCloseData->size()
		|| pointers._pMa26->size() != pointers._pCloseData->size()
		|| pointers._pDiff->size() != pointers._pCloseData->size()
		|| pointers._pDEA->size() != pointers._pCloseData->size()
		|| pointers._pDMAValue->size() != pointers._pCloseData->size()
		|| pointers._pAMAValue->size() != pointers._pCloseData->size()
		|| pointers._pTRIX->size() != pointers._pCloseData->size()
		|| pointers._pTRMA->size() != pointers._pCloseData->size()
		|| pointers._pK->size() != pointers._pCloseData->size()
		|| pointers._pD->size() != pointers._pCloseData->size()
		|| pointers._pJ->size() != pointers._pCloseData->size()
		|| pointers._pAsi->size() != pointers._pCloseData->size()
		|| pointers._pAsit->size() != pointers._pCloseData->size())
	{
		LOG(ERROR) << "Data size is Unequal.";
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool StateRecords::GetDownIndex(
	const VStockData& pointers, 
	const unsigned int parameter, 
	vector<unsigned int>& indexVectorindexVector,
	VStockData& LowValue)
{
	//输入参数检查
	indexVectorindexVector.clear();
	unsigned int PermitMinParameter = 2;
	if (nullptr == &pointers
		|| pointers.size() < 2 * parameter
		|| parameter < PermitMinParameter)
	{
		LOG(ERROR) << "StockData Size is too small.";
		return false;
	}

	//遍历查找局部最低点
	unsigned int frontIndex = 0;//
	for (unsigned int index = 0; index < pointers.size(); index++)
	{
		if (index - frontIndex > parameter + 1)
		{
			indexVectorindexVector.push_back(frontIndex);
			LowValue.push_back(pointers[frontIndex]);
			frontIndex = index;
		}
		if (pointers[frontIndex] > pointers[index])
			frontIndex = index;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool StateRecords::GetUpIndex(
	const VStockData& pointers, 
	const unsigned int parameter, 
	vector<unsigned int>& indexVector,
	VStockData& HighValue)
{
	indexVector.clear();
	unsigned int PermitMinParameter = 2;
	assert(!(nullptr == &pointers
			|| pointers.size() < 2 * parameter
			|| parameter < PermitMinParameter));
	if (nullptr == &pointers
		|| pointers.size() < 2 * parameter
		|| parameter < PermitMinParameter)
	{
		return false;
	}

	unsigned int frontIndex = 0;//
	for (unsigned int index = 0; index < pointers.size(); index++)
	{
		if (index - frontIndex > parameter + 1)
		{
			indexVector.push_back(frontIndex);
			HighValue.push_back(pointers[frontIndex]);
			frontIndex = index;
		}
		if (pointers[frontIndex] < pointers[index])
			frontIndex = index;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//输入数据，进行单日趋势分析
//////////////////////////////////////////////////////////////////////////
void StateRecords::DailyTrendStateRecord(
	const SigDayTechIndex& dayIndex,
	const DatePriceData& OneDayData)
{
	//LOG(INFO) << "Trend Record Begin." << endl;
	TrendStateRecordSingleStep(_eLowPrice, OneDayData._Low, OneDayData.mDate.dateTime);
	TrendStateRecordSingleStep(_eMACD, dayIndex._MacdData.diff, OneDayData.mDate.dateTime);
	TrendStateRecordSingleStep(_eDMA, dayIndex._DMAData._DDD, OneDayData.mDate.dateTime);
	TrendStateRecordSingleStep(_eTRIX, dayIndex._TrixData._TRIX, OneDayData.mDate.dateTime);
	TrendStateRecordSingleStep(_eASI, dayIndex._Asi._asi, OneDayData.mDate.dateTime);
	TrendStateRecordSingleStep(_eKDJ, dayIndex._Kdj.K_OF_KDJ, OneDayData.mDate.dateTime);
	//LOG(INFO) << "Trend Record Finish." << endl;
}
///////////////////////////////////////////////////////////////////////////
//单日单指标趋势分析
//////////////////////////////////////////////////////////////////////////
void StateRecords::TrendStateRecordSingleStep(
	const IndexGenre& valueType,
	const tySData& dayValue,
	const string& day)
{
	vector<FeatPoint_Local> _vGenreMinIndex;
	vector<FeatPoint_Local> _vGenreMaxIndex;
	for (unsigned int i = 0; i < _vIndexRecord_Local.size(); i++)
	{
		if (_vIndexRecord_Local[i]._IndexGenre == valueType && _vIndexRecord_Local[i]._IndexSig == _eLocalMin)
		{
			_vGenreMinIndex.push_back(_vIndexRecord_Local[i]);
		}
		if (_vIndexRecord_Local[i]._IndexGenre == valueType && _vIndexRecord_Local[i]._IndexSig == _eLocalMax)
		{
			_vGenreMaxIndex.push_back(_vIndexRecord_Local[i]);
		}

	}
	if (_vGenreMinIndex.size() < _TrAnaBeginSize || _vGenreMaxIndex.size() < _TrAnaBeginSize)
		return;

	//计算之前的最高点和最高点与最近产生的最高点与最低点的斜率
	vector<FeatPoint_Local>::reverse_iterator closelyMinIte = _vGenreMinIndex.rbegin();
	vector<FeatPoint_Local>::reverse_iterator closelyMaxIte = _vGenreMaxIndex.rbegin();

	vector<tySData> vMindiff;
	vector<tySData> vMaxdiff;
	for (vector<FeatPoint_Local>::reverse_iterator ite = closelyMinIte + 1; ite != _vGenreMinIndex.rbegin(); ++ite)
	{
		vMindiff.push_back(closelyMinIte->_Value - ite->_Value);
		if (vMindiff.size() == _TrAnaBeginSize / 2)//只需要对最近的四个局部特征点做分析
			break;
	}
	for (vector<FeatPoint_Local>::reverse_iterator ite = closelyMaxIte + 1; ite != _vGenreMaxIndex.rbegin(); ++ite)
	{
		vMaxdiff.push_back(closelyMaxIte->_Value - ite->_Value);
		if (vMaxdiff.size() == _TrAnaBeginSize / 2)//只对最近的四个局部特征点做分析
			break;
	}
	if (vMaxdiff.size() != _TrAnaBeginSize / 2 || vMindiff.size() != _TrAnaBeginSize/2)
		return;
	bool MaxUp = true;
	bool MaxDown = true;
	bool MinUp = true;
	bool MinDown = true;
	for (unsigned int i = 0; i < _TrAnaBeginSize / 2; ++i)
	{
		if (vMaxdiff[i] > 0 && MaxUp)
			MaxUp = true;
		else
			MaxUp = false;
		if (vMindiff[i] > 0 && MinUp)
			MinUp = true;
		else
			MinUp = false;
		if (vMaxdiff[i] < 0 && MaxDown)
			MaxDown = true;
		else
			MaxDown = false;
		if (vMindiff[i] < 0 && MinDown)
			MinDown = true;
		else
			MinDown = false;
	}

	if (MaxUp && MinUp && !MaxDown && !MinDown)
		_NowState_Trend._AllType[valueType] = _eTopRise_BottomRise;
	else if (!MaxUp && !MinUp && MaxDown && MinDown)
		_NowState_Trend._AllType[valueType] = _eTopFall_BottomFall;
	else if (!MaxUp && MinUp && MaxDown && !MinDown)
		_NowState_Trend._AllType[valueType] = _eRangeShrink;
	else if (MaxUp && !MinUp && !MaxDown && MinDown)
		_NowState_Trend._AllType[valueType] = _eRangeEnlarge;
	else if (MaxUp && !MaxDown)
		_NowState_Trend._AllType[valueType] = _eTopRise;
	else if (!MaxUp && MaxDown)
		_NowState_Trend._AllType[valueType] = _eTopFall;
	else if (MinUp && !MinDown)
		_NowState_Trend._AllType[valueType] = _eBottomRise;
	else if (!MinUp && MinDown)
		_NowState_Trend._AllType[valueType] = _eBottomFall;
	else
		_NowState_Trend._AllType[valueType] = _eNoRule;

	//return;
}

//////////////////////////////////////////////////////////////////////////
//初始化，清除所有内部动态结果
//////////////////////////////////////////////////////////////////////////
void StateRecords::Inition()
{
	//静态记录中使用的参数
	_NowStateChange = false;
	//清除所有Index的记录
	_vIndexRecord_Local.clear();
	_vIndexRecord_State.clear();
	for (unsigned int i = 0; i < MaxSizeOfIndex; ++i)
	{
		//上次的最低点价格
		_frontMarkValue_Local[i] = 0;
		_frontMark_Local[i] = 0;
		_frontMarkValue_Local[i] = true;
	}
	//趋势记录过程中，记录当前的index
	_CurrentMark = 0;
	//只有在出现6个局部特征点之后才开始趋势分析
	_TrAnaBeginSize = 6;
	//最后一次出错信息
	_LastError.clear();

	//趋势分析时间长度参数
	_Parameter_Local[LowPriceIndex] = 10;
	_Parameter_Local[HighPriceIndex] = 5;
	_Parameter_Local[MACDIndex] = 5;
	_Parameter_Local[TRIXIndex] = 5;
	_Parameter_Local[DMAIndex] = 5;
	_Parameter_Local[KDJIndex] = 5;
	_Parameter_Local[ASIIndex] = 5;

	//记录当前的index
	_CurrentMark = 0;
	//最后一次出错信息
	_LastError.clear();
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void StateRecords::LocalStateRecordSingleStep(
	const IndexGenre& valueType,
	const tySData& dayValue,
	const string& day)
{
	LocalStateRecordSingleStep_High(valueType, dayValue, day);
	LocalStateRecordSingleStep_Low(valueType, dayValue, day);
}
void StateRecords::LocalStateRecordSingleStep_Low(
	const IndexGenre& valueType,
	const tySData& dayValue,
	const string& day)
{
	if (_forntMarkIsLow_Local[valueType])
		return;
	if (_frontMarkValue_Local[valueType] > dayValue)
	{
		_frontMark_Local[valueType] = _CurrentMark;
		_frontMarkValue_Local[valueType] = dayValue;
		_frontMarkDay_Local[valueType] = day;
	}
	//判断有没有形成局部低点
	if (_CurrentMark - _frontMark_Local[valueType] > _Parameter_Local[valueType])
	{
		FeatPoint_Local lowIndex;
		lowIndex._TimeIndex = _frontMark_Local[valueType];
		lowIndex._Value = _frontMarkValue_Local[valueType];
		lowIndex._Day = _frontMarkDay_Local[valueType];
		lowIndex._IndexSig = _eLocalMin;//
		lowIndex._IndexGenre = valueType;
		_forntMarkIsLow_Local[valueType] = true;
		//_frontMark_Local[valueType] = _CurrentMark;
		_vIndexRecord_Local.push_back(lowIndex);
	}
}
void StateRecords::LocalStateRecordSingleStep_High(
	const IndexGenre& valueType,
	const tySData& dayValue,
	const string& day)
{
	if (!_forntMarkIsLow_Local[valueType])
		return;
	if (_frontMarkValue_Local[valueType] < dayValue)
	{
		_frontMark_Local[valueType] = _CurrentMark;
		_frontMarkValue_Local[valueType] = dayValue;
		_frontMarkDay_Local[valueType] = day;
	}
	//判断有没有形成局部高点
	if (_CurrentMark - _frontMark_Local[valueType] > _Parameter_Local[valueType])
	{
		FeatPoint_Local highIndex;
		highIndex._TimeIndex = _frontMark_Local[valueType];
		highIndex._Value = _frontMarkValue_Local[valueType];
		highIndex._Day = _frontMarkDay_Local[valueType];
		highIndex._IndexSig = _eLocalMax;//
		highIndex._IndexGenre = valueType;
		_forntMarkIsLow_Local[valueType] = false;
		//_frontMark_Local[valueType] = _CurrentMark;
		_vIndexRecord_Local.push_back(highIndex);
	}
}
//////////////////////////////////////////////////////////////////////////
//返回趋势分析结果
//////////////////////////////////////////////////////////////////////////
TrendState StateRecords::GetNowTrendnamicState()
{
	TrendState temp;
	return temp;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void StateRecords::DailyStateRecord(const SigDayTechIndex& theIndex, const DatePriceData& OneDayData)
{
	DailyStaticStateRecord(theIndex,OneDayData);
	//log.OutIndexTime("DailyStaticStateRecord:");
	DailyLocalStateRecord(theIndex, OneDayData);
	//log.OutIndexTime("DailyLocalStateRecord:");
	DailyTrendStateRecord(theIndex, OneDayData);
	//log.OutIndexTime("DailyTrendStateRecord:");
	++_CurrentMark;
}

void StateRecords::StaticResultPrint()
{
	string strStaticFeat[] = {
		"NegaTiveUpClose",
		"NegaTiveDownClose",
		"PositiveUpClose",
		"PositiveDownClose",
		"UndefindType_S"};
	string strGenre[] = {
		"LowPrice",
		"HighPrice",
		"MACD",
		"TRIX",
		"ASI",
		"DMA",
		"KDJ"};

	for (unsigned int i = 0; i < _vIndexRecord_State.size();++i)
	{
		LOG(INFO)
			<< "\tday:" << _vIndexRecord_State[i]._Day << "  "
			<< "\tGenre:" << strGenre[_vIndexRecord_State[i]._IndexGenre] << "  "
			<< "\tValue:" << _vIndexRecord_State[i]._Value << "  "
			<< "\tStaticFeat:" << strStaticFeat[_vIndexRecord_State[i]._StaticFeat] << endl;
		LOG(INFO) << "\t*************************************************************" << endl;
	}
	LOG(INFO);
}

void StateRecords::ResultPrint()
{
	//LocalResultPrint();
	TrendResultPrint();
}

void StateRecords::LocalResultPrint()
{
	string IndexSig[]{
			"_eLocalMin",
			"_eLocalMax",
			"_eUpCloseZero",
			"_eDownCloseZero",
			"_eUndefindType_L"};

	string strGenre[] = {
		"LowPrice",
		"HighPrice",
		"MACD",
		"TRIX",
		"ASI",
		"DMA",
		"KDJ",
		"" }; 

	for (unsigned int i = 0; i < _vIndexRecord_Local.size(); ++i)
	{
		if (_vIndexRecord_Local[i]._IndexGenre != _eDMA)
			continue;
		LOG(INFO)
			<< "\tday:" << _vIndexRecord_Local[i]._Day << "  "
			<< "\tGenre:" << strGenre[_vIndexRecord_Local[i]._IndexGenre] << "  "
			<< "\tValue:" << _vIndexRecord_Local[i]._Value << "  "
			<< "\tIndexSig:" << IndexSig[_vIndexRecord_Local[i]._IndexSig] << endl;
	}
}

void StateRecords::TrendResultPrint()
{
	string IndexSig[]{
		"TopRise_BottomRise",//出现顶上升底上升
		"TopFall_BottomFall",//出现顶下降底下降
		"RangeShrink",//范围收缩－－顶越来越低，底越来越高
		"RangeEnlarge",//范围扩大－－顶越来越高，同时底越来越低
		"TopRise",
		"BottomRise",
		"TopFall",
		"BottomFall",
		"NoRule",//无规律
		"UndefindType_D"//没有被定义
	};
	string strGenre[] = {
		"LowPrice",
		"HighPrice",
		"MACD",
		"TRIX",
		"ASI",
		"DMA",
		"KDJ",
		"" };
	vector<FeatPoint_Local> tempIndex;
	string LocalPointsType[]
	{
		"LocalMin",
		"LocalMax",
		"UpCloseZero",
		"DownCloseZero",
		"UndefindType_L"
	};
	for (unsigned int index = 0; index < MaxSizeOfIndex; ++index)
	{
		LOG(INFO)
			<< strGenre[index] <<":"<< IndexSig[_NowState_Trend._AllType[index]] << endl;
		GetLocalIndex((IndexGenre)index, tempIndex);
		if (tempIndex.size() >= _TrAnaBeginSize)
		{
			string info;
			vector<FeatPoint_Local>::const_reverse_iterator endIte = tempIndex.rbegin();
			unsigned int i = 0;
			while (i < _TrAnaBeginSize)
			{
				info = info + endIte->_Day + ":" + LocalPointsType[endIte->_IndexSig] + " ";
				++i;
				++endIte;
			}
			LOG(INFO) << info;
		}
	}
}

bool StateRecords::GetLocalIndex(const IndexGenre& valueType, vector<FeatPoint_Local>& _ReturnIndex)
{
	_ReturnIndex.clear();
	for (vector<FeatPoint_Local>::const_iterator ite = _vIndexRecord_Local.begin();
		ite != _vIndexRecord_Local.end();
		++ite)
	{ 
		if (valueType == ite->_IndexGenre)
		{
			_ReturnIndex.push_back(*ite);
		}
	}
	return true;
}

