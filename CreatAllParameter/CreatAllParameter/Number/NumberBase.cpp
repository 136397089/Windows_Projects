#include "StdAfx.h"
#include <iomanip>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "../glog/logging.h"
#include "NumberBase.h"
#include "../Locker.h"


static string logPath("D:\\StockFile\\Log");
static string plotPath("D:\\StockFile\\PlotFile");
const static string Separator(",");


CNumberBase::CNumberBase(void)
{
	_StockCSVFileMutex = CreateMutex(nullptr, true, "StockCSVFileMutex");
	_TreadNum = 0;
}

CNumberBase::~CNumberBase(void)
{
	CloseHandle(_StockCSVFileMutex);
}
//////////////////////////////////////////////////////////////////////////
//从文件中读取目标列数据
//
//
//////////////////////////////////////////////////////////////////////////
VStockData CNumberBase::ReadColumnStringFormFile(string filepath, string strTittle)
{
	VStockData Tagevalue;
	string linestring;
	ifstream file;
	file.open(filepath.c_str());
	if(!file)
	{
		cerr<<"file open fail"<<endl;
		exit(-1);
	}

	getline(file,linestring);
	if (linestring.find(File_Open_INDEX) > linestring.size())
	{
		return Tagevalue;
	}
	StringList TittleValues;
	CutString(linestring, Separator, TittleValues);

	unsigned int index = 0;
	for (StringList::iterator ite = TittleValues.begin(); ite != TittleValues.end(); ite++)
	{
		if (ite->find(strTittle) >= 0 && ite->find(strTittle) < ite->length())
		{
			break;
		}
		index++;
	}


	while(getline(file,linestring))
	{
		StringList StockValues;
		CutString(linestring, Separator, StockValues);
		if (StockValues.size() < index)
		{
			break;
		}
		Tagevalue.push_back((StockDataType)atof(StockValues[index].c_str()));
	}

	file.close();

	return Tagevalue;
}

//////////////////////////////////////////////////////////////////////////
//将列数据保存到文件中
//
//有Bug，无调用，暂不修改
//////////////////////////////////////////////////////////////////////////
void CNumberBase::AddColumnToCsvFile(string FilePath, VStockData vNumValue, string tittle)
{
	fstream outfile(FilePath.c_str(),ios::out|ios::in);
	string ch;
	string buffer[MAX_FILE_LINE_READFROM_COLUMN];

	//将文件全部内容读出到buffer
	int i=0; 
	while (getline(outfile,buffer[i]))
	{ 
		i++;  
	}
	//如果读出数据的行数与写入数据的行数不对应，则不做操作
	//如果行数太少，则不做操作
	if (vNumValue.size()+1 != i || i<10)
	{
		return;
	}
	string MACDValue;
	int k= 0;
	//如果没有找到写入的目标列，则在后面添加
	if (buffer[0].find(tittle) > buffer[0].size())
	{
		buffer[0] += Separator;
		buffer[0] +=tittle;
		for (k=1; k<i;k++)
		{
			ostringstream Macd_buffer;
			Macd_buffer<<Separator<<vNumValue[k-1];
			MACDValue = Macd_buffer.str();
			buffer[k] += MACDValue;
		}
	}
	//如果找到写入的目标列，则修改原数据
	else
	{
		StringList vAllTittle;
		CutString(buffer[0], Separator, vAllTittle);
		if (vAllTittle.size()<4)
		{
			return;
		}
		StringList::size_type TageTittle = 0;
		//找到目标tittle的列
		for (TageTittle = 0;TageTittle<vAllTittle.size();TageTittle++)
		{
			if (vAllTittle[TageTittle] == tittle)
			{
				break;
			}
		}
		//如果没有找到目标列
		if (TageTittle >= vAllTittle.size())
		{
			return;
		}
		//修改对应的列数据
		for (k=1; k<i;k++)
		{
			StringList vIndexData;
			CutString(buffer[k], Separator, vIndexData);
			if (TageTittle >= vIndexData.size())
			{
				return;
			}
			vIndexData[TageTittle] = vNumValue[k-1];
			buffer[k] = AssembleString(vIndexData,Separator);
		}
	}
	outfile.close();
	ofstream infile(FilePath.c_str()); 
	k=0;
	while(k!=i)
	{
		infile<<buffer[k]<<endl;
		k++;
	}//将buffer全部写入到文件
	outfile.close();  
	return;	
}

//////////////////////////////////////////////////////////////////////////
//读CSV文件中对应行的数据
//filepath:文件路径
//strTittle：需要读出的行名
//////////////////////////////////////////////////////////////////////////
VStockData CNumberBase::ReadRanksStringFormFile(string filepath, string strTittle)
{
	VStockData Tagevalue;
	string linestring;
	ifstream file;
	file.open(filepath.c_str());
	if (!file)
	{
		cerr << "file open fail" << endl;
		exit(-1);
	}
	getline(file, linestring);//在文件中第一行是日期，可以不用查找
	while (getline(file, linestring))//第二行开始才是数据
	{
		if (linestring.find(strTittle) >= 0 && linestring.find(strTittle)<linestring.size())
		{
			break;
		}
	}
	if (linestring.find(strTittle) > linestring.size())
	{
		return Tagevalue;//说明找不到对应的目标行数据
	}

	StringList TittleValues;
	CutString(linestring, Separator, TittleValues);//每行都是用','分割各个数据
	if (TittleValues.size()<3)
	{
		return Tagevalue;//找到的行数据太少
	}

	StringList::iterator beginite = TittleValues.begin();//第一个是数据的的名称，第二个开始才是真正数据
	beginite++;
	for (StringList::iterator ite = beginite; ite != TittleValues.end(); ite++)
	{
		Tagevalue.push_back((StockDataType)atof(ite->c_str()));
	}
	return Tagevalue;
}
//////////////////////////////////////////////////////////////////////////
//调用ReSavefileRanks之前运行
//////////////////////////////////////////////////////////////////////////
void CNumberBase::AddRanksToCsvFileBegin(string FilePath)
{
	CMutexLocker(_StockCSVFileMutex, INFINITE);
	fstream infile(FilePath.c_str(), ios::in);
	string fileLineStringg;
	//将文件全部内容读出到buffer[]数据中
	buffer.clear();
	newBuffer.clear();
	while (getline(infile, fileLineStringg))
	{
		buffer.push_back(fileLineStringg);
	}
	infile.close();
}

//////////////////////////////////////////////////////////////////////////
//调用ReSavefileRanks之后运行
//////////////////////////////////////////////////////////////////////////
void CNumberBase::AddRanksToCsvFileEnd(string FilePath)
{
	CMutexLocker(_StockCSVFileMutex, INFINITE);
	fstream outfile(FilePath.c_str(), ios::out | ios::app);
	for (unsigned int lineNumber = 0; lineNumber < newBuffer.size(); lineNumber++)
	{
		outfile << newBuffer[lineNumber] << endl;
	}
	buffer.clear();
	newBuffer.clear();
	outfile.close();
}

//////////////////////////////////////////////////////////////////////////
//增加对应的行保存到CSV文件中
//FilePath：文件路径
//vNumValue:要加入的行数据
//tittle:列名
//////////////////////////////////////////////////////////////////////////
void CNumberBase::AddRanksToCsvFile(string FilePath, const  VStockData& vNewValue, string tittle)
{
	CMutexLocker(_StockCSVFileMutex, INFINITE);
	for (unsigned int lineNumber = 0; lineNumber < buffer.size(); lineNumber++)
	{//如果tittle已经存在于数据当中，则不将当前数据加入到buffer当中，返回
		if (buffer[lineNumber].find(tittle) != string::npos && buffer[lineNumber].find(Separator) == tittle.size())
			return;
	}
	ostringstream  oss;
	oss << tittle;
	for (unsigned int lineNumber = 0; lineNumber < vNewValue.size(); lineNumber++)
	{
		oss << Separator << vNewValue[lineNumber];
	}
	newBuffer.push_back(oss.str());
}

void CNumberBase::AddRanksToCsvFile(string FilePath, const vector<string>& vNewValue, string tittle)
{
	CMutexLocker(_StockCSVFileMutex, INFINITE);
	for (unsigned int lineNumber = 0; lineNumber < buffer.size(); lineNumber++)
	{//如果tittle已经存在于数据当中，则不将当前数据加入到buffer当中，返回
		if (buffer[lineNumber].find(tittle) != string::npos && buffer[lineNumber].find(Separator) == tittle.size())
			return;
	}
	ostringstream  oss;
	oss << tittle;
	for (unsigned int lineNumber = 0; lineNumber < vNewValue.size(); lineNumber++)
	{
		oss << Separator << vNewValue[lineNumber];
	}
	newBuffer.push_back(oss.str());
}

void CNumberBase::AddRanksToCsvFile(string FilePath, const map<string, StockDataType>& vNewValue)
{
	CMutexLocker(_StockCSVFileMutex, INFINITE);
	ostringstream  tittleoss;
	ostringstream  dataoss;
	for (map<string, StockDataType>::const_iterator ite = vNewValue.begin(); ite != vNewValue.end(); ite++)
	{
		tittleoss << ite->first<<",";
		dataoss << ite->second<<",";
	}
	newBuffer.push_back(tittleoss.str());
	newBuffer.push_back(dataoss.str());
}

void CNumberBase::AddRanksToCsvFile(string FilePath, const FreqListType& vNewValue)
{
	CMutexLocker(_StockCSVFileMutex, INFINITE);
	ostringstream  dataoss;
	for (FreqListType::const_iterator ite = vNewValue.begin(); ite != vNewValue.end(); ite++)
	{
		dataoss << ite->second << ",";
	}
	newBuffer.push_back(dataoss.str());

}



StringList CNumberBase::ReadDataFromClickBoardRanks()
{
	ShowDebugInformationInCon("in ReadDataFromClickBoard. ");

	StringList resivedata;
	resivedata.clear();
	if (OpenClipboard(NULL))
	{
		//获得剪贴板数据
		HGLOBAL hMem = GetClipboardData(CF_TEXT);
		ShowDebugInformationInCon("GetClipboardData finished.");
		if (NULL != hMem)
		{
			char* lpStr = (char*)GlobalLock(hMem);
			if (NULL == lpStr)
			{
				//MessageBox(0, lpStr, "", 0);
				GlobalUnlock(hMem);
				return resivedata;
			}
			string tempstring(lpStr);
			CutString(tempstring, "\n", resivedata);
		}
		CloseClipboard();
	}
	return resivedata;
	ShowDebugInformationInCon("end ReadDataFromClickBoard. ");
}

StringBlock CNumberBase::ReadDataFromClickBoardAndCutRanks()
{
	ShowDebugInformationInCon("in ReadDataFromClickBoardAndCut");
	StringList Tempdata = ReadDataFromClickBoardRanks();

	StringList LineData;
	StringBlock ReturnData;
	for (StringList::iterator ite = Tempdata.begin(); ite != Tempdata.end(); ite++)
	{
		LineData.clear();
		CutString(*ite, "\t", LineData);
		ReturnData.push_back(LineData);
	}
	ShowDebugInformationInCon("endof ReadDataFromClickBoardAndCut");
	return ReturnData;
}

bool CNumberBase::SaveDataToClickBoard(StringBlock& savedata)
{
	int nDataLen = 5;
	string saveToClickBoard = "";
	//char pszData[] = "heheh";
	ShowDebugInformationInCon("Try to get save data.");
	for (StringBlock::iterator ite = savedata.begin(); ite != savedata.end(); ite++)
	{
		for (StringList::iterator strite = ite->begin(); strite != ite->end(); strite++)
		{
			saveToClickBoard = saveToClickBoard + '\t' + *strite;
		}
		saveToClickBoard += "\n";
	}
	ShowDebugInformationInCon("Savedata to clickboard is ready.");
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		HGLOBAL clipbuffer;
		char *buffer;
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, saveToClickBoard.length() + 1);
		buffer = (char *)GlobalLock(clipbuffer);
		strcpy_s(buffer, saveToClickBoard.length() + 1, saveToClickBoard.c_str());
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
		return true;
	}
	return false;
}

















//////////////////////////////////////////////////////////////////////////
//新建线程，更新文件参数
//filename：文件路径
//////////////////////////////////////////////////////////////////////////
void CNumberBase::RunTread(string filePath)
{
	while (true)
	{
		if (_TreadNum >= 0 && _TreadNum < 8)
		{
			SECURITY_ATTRIBUTES sa;
			NumberTreadPara &para = *(new NumberTreadPara);
			para.pmacd_ = this;
			para.filename_ = filePath;
			//参数1：线程安全性
			//参数2：线程地址空间
			//参数3：线程函数地址
			//参数4：线程函数输入参数
			//参数5：标志创建是否运行
			//参数6：线程ID，一般NULL
			_TreadNum++;
			sa.nLength = sizeof(sa);
			sa.bInheritHandle = FALSE;
			DWORD dwTreadID;
			HANDLE iThread = CreateThread(NULL, 0, ThreadToUpdatefile, (PVOID)&para, 0, &dwTreadID);
			break;
		}
		else
		{
			Sleep(200);
		}
	}
}




bool CNumberBase::ReadDataFromCSVFileRanks(const string& fullFilePath, StringList& strAllData) const
{
	string linestring;
// 	StringList strAllData;
	strAllData.clear();
	FILE *pfile;
	fopen_s(&pfile, fullFilePath.c_str(), "r");
	if (NULL == pfile){
		LOG(ERROR) << "Open data file error.";
		return false;
		exit(-1);
	}
	char v;
	char buffer[1000000];
	int i = 0;
	while (1)
	{
		if (feof(pfile))
			break;
		if (1 == fscanf_s(pfile, "%c", &v)){
			if (10 == v)//如果是换行符
			{
				buffer[i] = 0;
				linestring = buffer;
				strAllData.push_back(linestring);
				i = 0;
				continue;
			}
			buffer[i] = v;
			i++;
			if (i >= 100000)
				break;
		}
		else
			fscanf_s(pfile, "%*c", buffer);
	}
	fclose(pfile);
	return true;
}

StringBlock CNumberBase::ReadDataFromCSVFileAndCutRanks(const string& fullFilePath, string strCutIndex )
{
	StringList vLineData;
	if (!ReadDataFromCSVFileRanks(fullFilePath, vLineData))
	{
		LOG(ERROR) << "Error:Read Data From:" << fullFilePath;
	}
	StringBlock ReturnData;
	if (vLineData.size() < 3)
	{
		return ReturnData;
	}
	StringList vOneLinedata;
	for (StringList::iterator ite = vLineData.begin(); ite != vLineData.end(); ite++)
	{
		CutString(*ite, strCutIndex, vOneLinedata);
		ReturnData.push_back(vOneLinedata);
		vOneLinedata.clear();
	}
	return ReturnData;
}

bool CNumberBase::SaveDataToCSVFile(const string& fullFilePath, const StringBlock& savedata)
{
	fstream outfile(fullFilePath.c_str(), ios::out);

	for (StringBlock::const_iterator lineNumber = savedata.begin(); lineNumber != savedata.end(); lineNumber++)
	{
		for (StringList::const_iterator ite = lineNumber->begin(); ite != lineNumber->end(); ite++)
		{
			outfile << Separator << *ite;
		}
		outfile << endl;
	}

	outfile.close();
	return true;
}


//////////////////////////////////////////////////////////////////////////
//线程函数
//
//
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadToUpdatefile(PVOID pvParam)
{
	NumberTreadPara* para = (NumberTreadPara*)pvParam;
	CNumberBase& macd = *para->pmacd_;

	delete pvParam;
	macd._TreadNum--;
	return 0;
}
map<StockNumberType, string> IndexName = {
	{_eFile_Close_INDEX , File_Close_INDEX },
 	{_eFile_Date_INDEX , File_Date_INDEX },
	{_eFile_Open_INDEX , File_Open_INDEX },
	{_eFile_High_INDEX , File_High_INDEX },
	{_eFile_Low_INDEX , File_Low_INDEX },
	{ _eFile_Volume_INDEX, File_Volume_INDEX },
	{ _ePriceChangeRate, "PriceChangeRate" }, { _eVolChangeRate, "VolChangeRate" }, { _eDataEnable, "DataEnable" },
	{ _eMACD_MA12, "MACD_Ma12" }, { _eMACD_MA26, "MACD_Ma26" },
	{ _eMACD_DIFF, "MACD_Diff" },{_eMACD_DEA , "MACD_DEA" },{_eMACD_BAR , "MACD_BAR" },
	{_eKDJ_K , "KDJ_K" },{_eKDJ_D , "KDJ_D" },{_eKDJ_J , "KDJ_J" },
	{_eDMA_D , "DMA_D" },{_eDMA_A , "DMA_D" },
	{_eTRIX_TRIX , "TRIX_TRIX" },{_eTRIX_MA , "TRIX_MA" },{ _eTRIX_VTR, "TRIX_VTR" },{ _eTRIX_VTB, "TRIX_VTB" },{ _eTRIX_VTA, "TRIX_VTA" },
	{_eMA_MA1 , "MA_MA5" },{ _eMA_MA2, "MA_MA10" },{ _eMA_MA3, "MA_MA20" },{ _eMA_MA4, "MA_MA40" },
	{ _eMA_UPRATE, "MA_UPRATE" },{ _ePRICECHANGE, "PRICECHANGE" },{ _eVOLHANGE, "VOLHANGE" },
	{ _eASI_I, "ASI_I" },{ _eASI_T, "ASI_T" },
	{ _eCDP_CDP, "CDP_CDP" }, { _eCDP_AH, "CDP_AH" }, { _eCDP_NH, "CDP_NH" }, 
	{ _eCDP_NL, "CDP_NL" }, { _eCDP_AL, "CDP_AL" }, { _eCDP_LOWEST ,"CDP_LOWEST"},
	{ _eAR, "ABVP_AR" }, { _eBR, "ABVP_BR" }, { _eCR, "ABVP_CR" },
	{ _ePSY, "ABVP_PSY" }, { _eLimitUpTime, "LimitUpTime" }, { _eLimitDownTime, "LimitDownTime" },
	{ _eCRMA1, "ABVP_CRMA1" }, { _eCRMA2, "ABVP_CRMA2" }, { _eCRMA3, "ABVP_CRMA3" }, { _eCRMA4, "ABVP_CRMA4" },
	{ _eVR, "ABVP_VR" }, { _eVRMA, "ABVP_VRMA" },
	{ _eEMV, "EMV_EMV" },{ _eEMVMA, "EMV_EMVMA" },
	{ _eBOOLUp, "BOOLUp" }, { _eBOOLMid, "BOOLMid" }, { _eBOOLDown, "BOOLDown" }, { _eBOOLSTD, "BOOLSTD" },
	{ _eBOOLCDP, "BOOLCDP" }, { _eBOOLCDPAUP, "BOOLCDPAUP" }, { _eBOOLCDPNUP, "BOOLCDPNUP" }, 
	{ _eBOOLCDPNDown, "BOOLCDPNDown" }, { _eBOOLCDPADown, "BOOLCDPADown" },
	{ _eDMI_DIP, "DMI_DIP" }, { _eDMI_DIN, "DMI_DIN" }, { _eDMI_ADX, "DMI_ADX" }, { _eDMI_ADXR, "DMI_ADXR" },
	{ _eRSI1, "RSI1" },{ _eRSI2, "RSI2" },{ _eRSI3, "RSI3" }
};


std::string GetNumberNameByIndexType(StockNumberType _indextype)
{
	return IndexName[_indextype];
}

map<StockNumberType, string> GetAllNumberype()
{
	return IndexName;
}




void SinCyclePriceData::Inition()
{
	mDate.SetDay("1991-1-1");
	_Open = 0.0f;
	_Close = 0.0f;
	_High = 0.0f;
	_Low = 0.0f;
	_Volume = 0.0f;
}

void SinCyclePriceData::Inition(StockDataType _dOpen, StockDataType _dClose, StockDataType _dHigh, StockDataType _dLow, StockDataType _dVol, string _strdate)
{
	mDate.SetDay(_strdate);
	_Open = _dOpen;
	_Close = _dClose;
	_High = _dHigh;
	_Low = _dLow;
	_Volume = _dVol;
}

bool SinCyclePriceData::operator==(const SinCyclePriceData& d) const
{
	return (_Open == d._Open
		&& _Close == d._Close
		&& _High == d._High
		&& _Low == d._Low
		&& _Volume == d._Volume);
}
