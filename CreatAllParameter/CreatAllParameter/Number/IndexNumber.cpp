#include "StdAfx.h"
#include <iomanip>
#include <string>
#include <map>
#include "IndexNumber.h"
#include "../Locker.h"


static string logPath("D:\\StockFile\\Log");
static string plotPath("D:\\StockFile\\PlotFile");



CNumberManager::CNumberManager(void)
{
	_StockCSVFileMutex = CreateMutex(nullptr, true, "StockCSVFileMutex");
	_TreadNum = 0;
}

CNumberManager::~CNumberManager(void)
{
	CloseHandle(_StockCSVFileMutex);
}
//////////////////////////////////////////////////////////////////////////
//���ļ��ж�ȡĿ��������
//
//
//////////////////////////////////////////////////////////////////////////
VStockData CNumberManager::ReadColumnStringFormFile(string filepath, string strTittle)
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
	StringList TittleValues = CutString(linestring, ",");

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
		StringList StockValues = CutString(linestring, ",");
		if (StockValues.size() < index)
		{
			break;
		}
		Tagevalue.push_back((tySData)atof(StockValues[index].c_str()));
	}

	file.close();

	return Tagevalue;
}

//////////////////////////////////////////////////////////////////////////
//�������ݱ��浽�ļ���
//
//��Bug���޵��ã��ݲ��޸�
//////////////////////////////////////////////////////////////////////////
void CNumberManager::ReSavefileColumn(string FilePath, VStockData vNumValue, string tittle)
{
	fstream outfile(FilePath.c_str(),ios::out|ios::in);
	string ch;
	string buffer[MAX_FILE_LINE_READFROM_COLUMN];

	//���ļ�ȫ�����ݶ�����buffer
	int i=0; 
	while (getline(outfile,buffer[i]))
	{ 
		i++;  
	}
	//����������ݵ�������д�����ݵ���������Ӧ����������
	//�������̫�٣���������
	if (vNumValue.size()+1 != i || i<10)
	{
		return;
	}
	string MACDValue;
	int k= 0;
	//���û���ҵ�д���Ŀ���У����ں������
	if (buffer[0].find(tittle) > buffer[0].size())
	{
		buffer[0] += ",";
		buffer[0] +=tittle;
		for (k=1; k<i;k++)
		{
			ostringstream Macd_buffer;
			Macd_buffer<<","<<vNumValue[k-1];
			MACDValue = Macd_buffer.str();
			buffer[k] += MACDValue;
		}
	}
	//����ҵ�д���Ŀ���У����޸�ԭ����
	else
	{
		StringList vAllTittle = CutString(buffer[0], ",");
		if (vAllTittle.size()<4)
		{
			return;
		}
		StringList::size_type TageTittle = 0;
		//�ҵ�Ŀ��tittle����
		for (TageTittle = 0;TageTittle<vAllTittle.size();TageTittle++)
		{
			if (vAllTittle[TageTittle] == tittle)
			{
				break;
			}
		}
		//���û���ҵ�Ŀ����
		if (TageTittle >= vAllTittle.size())
		{
			return;
		}
		//�޸Ķ�Ӧ��������
		for (k=1; k<i;k++)
		{
			StringList vIndexData = CutString(buffer[k], ",");
			if (TageTittle >= vIndexData.size())
			{
				return;
			}
			vIndexData[TageTittle] = vNumValue[k-1];
			buffer[k] = AssembleString(vIndexData,",");
		}
	}
	outfile.close();
	ofstream infile(FilePath.c_str()); 
	k=0;
	while(k!=i)
	{
		infile<<buffer[k]<<endl;
		k++;
	}//��bufferȫ��д�뵽�ļ�
	outfile.close();  
	return;	
}

//////////////////////////////////////////////////////////////////////////
//��CSV�ļ��ж�Ӧ�е�����
//filepath:�ļ�·��
//strTittle����Ҫ����������
//////////////////////////////////////////////////////////////////////////
VStockData CNumberManager::ReadRanksStringFormFile(string filepath, string strTittle)
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
	getline(file, linestring);//���ļ��е�һ�������ڣ����Բ��ò���
	while (getline(file, linestring))//�ڶ��п�ʼ��������
	{
		if (linestring.find(strTittle) >= 0 && linestring.find(strTittle)<linestring.size())
		{
			break;
		}
	}
	if (linestring.find(strTittle) > linestring.size())
	{
		return Tagevalue;//˵���Ҳ�����Ӧ��Ŀ��������
	}

	StringList TittleValues = CutString(linestring, ",");//ÿ�ж�����','�ָ��������
	if (TittleValues.size()<3)
	{
		return Tagevalue;//�ҵ���������̫��
	}

	StringList::iterator beginite = TittleValues.begin();//��һ�������ݵĵ����ƣ��ڶ�����ʼ������������
	beginite++;
	for (StringList::iterator ite = beginite; ite != TittleValues.end(); ite++)
	{
		Tagevalue.push_back((tySData)atof(ite->c_str()));
	}
	return Tagevalue;
}

//////////////////////////////////////////////////////////////////////////
//���Ӷ�Ӧ���б��浽CSV�ļ���
//FilePath���ļ�·��
//vMACDValue:Ҫ�����������
//tittle:����
//////////////////////////////////////////////////////////////////////////
void CNumberManager::ReSavefileRanks(string FilePath,const  VStockData vNumValue, string tittle)
{
	CLocker(_StockCSVFileMutex, INFINITE);
	fstream infile(FilePath.c_str(),ios::in);
	string ch;
	string buffer[MAX_FILE_LINE_READFROM_RANK];
	//����̫�٣���������
	if (vNumValue.size() < 3)
	{
		cout << "Size is too small.";
		return;
	}

	//���ļ�ȫ�����ݶ�����buffer[]������
	int lineIndex = 0;
	while (getline(infile, buffer[lineIndex]))
	{
		lineIndex++;
	}
	infile.close();

	fstream outfile(FilePath.c_str(), ios::out);
	bool addNewLine = true;//��ʱҪ�����������ԭ�ļ��Ѿ����ڣ���������ǲ���Ҫ�����µ���
	for (int lineNumber = 0; lineNumber < lineIndex;lineNumber++)
	{
		if (buffer[lineNumber].find(tittle) >= buffer[lineNumber].size())//����Ҫ����Ŀ���а���ԭ���ݷŻ�
		{
			outfile << buffer[lineNumber]<<endl;
		}
		else//��Ŀ���������������ݱ���
		{
			outfile << tittle;
			addNewLine = false;
			for (VStockData::const_iterator ite = vNumValue.cbegin(); ite != vNumValue.cend(); ite++)
			{
				outfile << "," << *ite;
			}
			outfile << endl;
		}
	}

	if (addNewLine)//��Ҫ�����µ���
	{
		outfile << tittle;
		for (VStockData::const_iterator ite = vNumValue.cbegin(); ite != vNumValue.cend(); ite++)
		{
			outfile << "," << *ite;
		}
		outfile << endl;
	}

	outfile.close();
}

void CNumberManager::ReSavefileRanks(string FilePath, const vector<string> vNewValue, string tittle)
{
	CLocker(_StockCSVFileMutex, INFINITE);
	fstream infile(FilePath.c_str(), ios::in);
	string ch;
	string buffer[MAX_FILE_LINE_READFROM_RANK];
	//����̫�٣���������
	if (vNewValue.size() < 3)
	{
		cout << "Size is too small.";
		return;
	}

	//���ļ�ȫ�����ݶ�����buffer[]������
	int lineIndex = 0;
	while (getline(infile, buffer[lineIndex]))
	{
		lineIndex++;
	}
	infile.close();

	fstream outfile(FilePath.c_str(), ios::out);
	bool addNewLine = true;//��ʱҪ�����������ԭ�ļ��Ѿ����ڣ���������ǲ���Ҫ�����µ���
	for (int lineNumber = 0; lineNumber < lineIndex; lineNumber++)
	{
		if (buffer[lineNumber].find(tittle) >= buffer[lineNumber].size())//����Ҫ�����Ŀ���У�ԭ���ݷŻ�
		{
			outfile << buffer[lineNumber] << endl;
		}
		else//��Ŀ���������������ݱ���
		{
			outfile << tittle;
			addNewLine = false;
			for (vector<string>::const_iterator ite = vNewValue.cbegin(); ite != vNewValue.cend(); ite++)
			{
				outfile << "," << *ite;
			}
			outfile << endl;
		}
	}

	if (addNewLine)//��Ҫ�����µ���
	{
		outfile << tittle;
		for (vector<string>::const_iterator ite = vNewValue.cbegin(); ite != vNewValue.cend(); ite++)
		{
			outfile << "," << *ite;
		}
		outfile << endl;
	}

	outfile.close();

}



StringList CNumberManager::ReadDataFromClickBoardRanks()
{
	ShowDebugInformationInCon("in ReadDataFromClickBoard. ");

	StringList resivedata;
	resivedata.clear();
	if (OpenClipboard(NULL))
	{
		//��ü���������
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
			resivedata = CutString(tempstring, "\n");
		}
		CloseClipboard();
	}
	return resivedata;
	ShowDebugInformationInCon("end ReadDataFromClickBoard. ");
}

StringBlock CNumberManager::ReadDataFromClickBoardAndCutRanks()
{
	ShowDebugInformationInCon("in ReadDataFromClickBoardAndCut");
	StringList Tempdata = ReadDataFromClickBoardRanks();

	StringList LineData;
	StringBlock ReturnData;
	for (StringList::iterator ite = Tempdata.begin(); ite != Tempdata.end(); ite++)
	{
		LineData.clear();
		LineData = CutString(*ite, "\t");
		ReturnData.push_back(LineData);
	}
	ShowDebugInformationInCon("endof ReadDataFromClickBoardAndCut");
	return ReturnData;
}

bool CNumberManager::SaveDataToClickBoard(StringBlock& savedata)
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
//�½��̣߳������ļ�����
//filename���ļ�·��
//////////////////////////////////////////////////////////////////////////
void CNumberManager::RunTread(string filePath)
{
	while (true)
	{
		if (_TreadNum >= 0 && _TreadNum < 8)
		{
			SECURITY_ATTRIBUTES sa;
			NumberTreadPara &para = *(new NumberTreadPara);
			para.pmacd_ = this;
			para.filename_ = filePath;
			//����1���̰߳�ȫ��
			//����2���̵߳�ַ�ռ�
			//����3���̺߳�����ַ
			//����4���̺߳����������
			//����5����־�����Ƿ�����
			//����6���߳�ID��һ��NULL
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




StringList CNumberManager::ReadDataFromCSVFileRanks(const string& fullFilePath)
{
	VStockData Tagevalue;
	string linestring;
	ifstream file;
	file.open(fullFilePath.c_str());
	StringList strAllData;
	strAllData.clear();
	if (!file)
	{
		cerr << "file open fail" << endl;
		exit(-1);
	}
	//getline(file, linestring);//���ļ��е�һ�������ڣ����Բ��ò���
	while (getline(file, linestring))//�ڶ��п�ʼ��������
	{
		strAllData.push_back(linestring);
	}
	return strAllData;
}

StringBlock CNumberManager::ReadDataFromCSVFileAndCutRanks(const string& fullFilePath, string strCutIndex )
{
	StringList vLineData = ReadDataFromCSVFileRanks(fullFilePath);
	StringBlock ReturnData;
	if (vLineData.size() < 3)
	{
		return ReturnData;
	}
	StringList vOneLinedata;
	for (StringList::iterator ite = vLineData.begin(); ite != vLineData.end(); ite++)
	{
		vOneLinedata = CutString(*ite, strCutIndex);
		ReturnData.push_back(vOneLinedata);
		vOneLinedata.clear();
	}
	return ReturnData;
}

bool CNumberManager::SaveDataToCSVFile(const string& fullFilePath, const StringBlock& savedata)
{
	fstream outfile(fullFilePath.c_str(), ios::out);

	for (StringBlock::const_iterator lineNumber = savedata.begin(); lineNumber != savedata.end(); lineNumber++)
	{
		for (StringList::const_iterator ite = lineNumber->begin(); ite != lineNumber->end(); ite++)
		{
			outfile << "," << *ite;
		}
		outfile << endl;
	}

	outfile.close();
	return true;
}

//////////////////////////////////////////////////////////////////////////
//�̺߳���
//
//
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadToUpdatefile(PVOID pvParam)
{
	NumberTreadPara* para = (NumberTreadPara*)pvParam;
	CNumberManager& macd = *para->pmacd_;

	delete pvParam;
	macd._TreadNum--;
	return 0;
}
map<IndexType, string> IndexName = {
	{_eFile_Close_INDEX ,  File_Close_INDEX } ,
	{_eFile_Close_INDEX , File_Close_INDEX },
	{_eFile_Date_INDEX , File_Date_INDEX },
	{_eFile_Open_INDEX , File_Open_INDEX },
	{_eFile_High_INDEX , File_High_INDEX },
	{_eFile_Low_INDEX , File_Low_INDEX },
	{_eFile_Volume_INDEX ,File_Volume_INDEX },
	{ _eMACD_MA12, MACD_MA12 },
	{ _eMACD_MA26, MACD_MA26 },
	{ _eMACD_DIFF, MACD_DIFF },
	{_eMACD_DEA , MACD_DEA },
	{_eMACD_BAR , MACD_BAR },
	{_eKDJ_K , KDJ_K },
	{_eKDJ_D , KDJ_D },
	{_eKDJ_J , KDJ_J },
	{_eDMA_D , DMA_D },
	{_eDMA_A , DMA_A },
	{_eTRIX_TRIX , TRIX_TRIX },
	{_eTRIX_MA , TRIX_MA },
	{ _eTRIX_VTR, TRIX_VTR },
	{ _eTRIX_VTB, TRIX_VTB },
	{ _eTRIX_VTA, TRIX_VTA },
	{_eMA_MA5 , MA_MA5 },
	{ _eMA_MA10, MA_MA10 },
	{ _eMA_MA20, MA_MA20 },
	{ _eMA_MA40, MA_MA40 },
	{_ePRICECHANGE , PRICECHANGE },
	{_eVOLHANGE , VOLHANGE },
	{_eASI_I , ASI_I },
	{_eASI_T , ASI_T }
};
std::string GetIndexNameByIndexType(IndexType _indextype)
{
	return IndexName[_indextype];
}






























