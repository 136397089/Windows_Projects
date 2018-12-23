#include "StdAfx.h"
#include "IndexNumber.h"

CNumberManager::CNumberManager(void)
{
	TreadNum = 0;
}

CNumberManager::~CNumberManager(void)
{
}
//////////////////////////////////////////////////////////////////////////
//从文件中读取目标列数据
//
//
//////////////////////////////////////////////////////////////////////////
vector<tySData> CNumberManager::ReadColumnStringFormFile(string filepath, string strTittle)
{
	vector<tySData> Tagevalue;
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
	vector<string> TittleValues = CutString(linestring,",");

	unsigned int index = 0;
	for (vector<string>::iterator ite = TittleValues.begin(); ite !=TittleValues.end(); ite++)
	{
		if (ite->find(strTittle) >= 0 && ite->find(strTittle) < ite->length())
		{
			break;
		}
		index++;
	}


	while(getline(file,linestring))
	{
		vector<string> StockValues = CutString(linestring,",");
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
//将列数据保存到文件中
//
//
//////////////////////////////////////////////////////////////////////////
void CNumberManager::ReSavefileColumn(string FilePath, vector<tySData> vNumValue, string tittle)
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
	//如果找到写入的目标列，则修改原数据
	else
	{
		vector<string> vAllTittle = CutString(buffer[0],",");
		if (vAllTittle.size()<4)
		{
			return;
		}
		vector<string>::size_type TageTittle = 0;
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
			vector<string> vIndexData = CutString(buffer[k],",");
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
	}//将buffer全部写入到文件
	outfile.close();  
	return;	
}

//////////////////////////////////////////////////////////////////////////
//读CSV文件中对应行的数据
//filepath:文件路径
//strTittle：需要读出的行名
//////////////////////////////////////////////////////////////////////////
vector<tySData> CNumberManager::ReadRanksStringFormFile(string filepath, string strTittle)
{
	vector<tySData> Tagevalue;
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

	vector<string> TittleValues = CutString(linestring, ",");//每行都是用','分割各个数据
	if (TittleValues.size()<3)
	{
		return Tagevalue;//找到的行数据太少
	}

	vector<string>::iterator beginite = TittleValues.begin();//第一个是数据的的名称，第二个开始才是真正数据
	beginite++;
	for (vector<string>::iterator ite = beginite; ite != TittleValues.end();ite++)
	{
		Tagevalue.push_back((tySData)atof(ite->c_str()));
	}
	return Tagevalue;
}

//////////////////////////////////////////////////////////////////////////
//增加对应的行保存到CSV文件中
//FilePath：文件路径
//vMACDValue:要加入的行数据
//tittle:列名
//////////////////////////////////////////////////////////////////////////
void CNumberManager::ReSavefileRanks(string FilePath, vector<tySData> vNumValue, string tittle)
{
	fstream infile(FilePath.c_str(),ios::in);
	string ch;
	string buffer[MAX_FILE_LINE_READFROM_RANK];
	//数据太少，不做操作
	if (vNumValue.size() < 3)
	{
		cout << "Size is too small.";
		return;
	}

	//将文件全部内容读出到buffer[]数据中
	int lineIndex = 0;
	while (getline(infile, buffer[lineIndex]))
	{
		lineIndex++;
	}
	infile.close();

	fstream outfile(FilePath.c_str(), ios::out);
	bool addNewLine = true;//有时要保存的数据在原文件已经存在，用来标记是不是要增加新的行
	for (int lineNumber = 0; lineNumber < lineIndex;lineNumber++)
	{
		if (buffer[lineNumber].find(tittle) >= buffer[lineNumber].size())//不是要保存目标行按照原数据放回
		{
			outfile << buffer[lineNumber]<<endl;
		}
		else//是目标行则把输入的数据保存
		{
			outfile << tittle;
			addNewLine = false;
			for (vector<tySData>::iterator ite = vNumValue.begin(); ite != vNumValue.end(); ite++)
			{
				outfile << "," << *ite;
			}
			outfile << endl;
		}
	}

	if (addNewLine)//需要增加新的行
	{
		outfile << tittle;
		for (vector<tySData>::iterator ite = vNumValue.begin(); ite != vNumValue.end(); ite++)
		{
			outfile << "," << *ite;
		}
		outfile << endl;
	}

	outfile.close();
}



vector<string> CNumberManager::ReadDataFromClickBoardRanks()
{
	ShowDebugInformationInCon("in ReadDataFromClickBoard. ");

	vector<string> resivedata;
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
			resivedata = CutString(tempstring, "\n");
		}
		CloseClipboard();
	}
	return resivedata;
	ShowDebugInformationInCon("end ReadDataFromClickBoard. ");
}

vector<vector<string>> CNumberManager::ReadDataFromClickBoardAndCutRanks()
{
	ShowDebugInformationInCon("in ReadDataFromClickBoardAndCut");
	vector <string> Tempdata = ReadDataFromClickBoardRanks();

	vector<string> LineData;
	vector<vector<string>> ReturnData;
	for (vector <string>::iterator ite = Tempdata.begin(); ite != Tempdata.end(); ite++)
	{
		LineData.clear();
		LineData = CutString(*ite, "\t");
		ReturnData.push_back(LineData);
	}
	ShowDebugInformationInCon("endof ReadDataFromClickBoardAndCut");
	return ReturnData;
}

bool CNumberManager::SaveDataToClickBoard(vector<vector<string>>& savedata)
{
	int nDataLen = 5;
	string saveToClickBoard = "";
	//char pszData[] = "heheh";
	ShowDebugInformationInCon("Try to get save data.");
	for (vector<vector<string>>::iterator ite = savedata.begin(); ite != savedata.end(); ite++)
	{
		for (vector<string>::iterator strite = ite->begin(); strite != ite->end(); strite++)
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
void CNumberManager::RunTread(string filePath)
{
	while (true)
	{
		if (TreadNum >= 0 && TreadNum < 8)
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
			TreadNum++;
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

void CNumberManager::UpdatePathFilesRanks(string strPath)
{
	ShowDebugInformationInCon("Error:In BaseClass UpdatePathFilesRanks.");
}

void CNumberManager::UpdateFileRanks(string strfilename)
{
	ShowDebugInformationInCon("Error:In BaseClass UpdatePathFilesRanks.");
}

void CNumberManager::UpdateFileColumn(string strPath)
{
	ShowDebugInformationInCon("Error:In BaseClass UpdatePathFilesRanks.");
}


vector<string> CNumberManager::ReadDataFromCSVFileRanks(const string& fullFilePath)
{
	vector<tySData> Tagevalue;
	string linestring;
	ifstream file;
	file.open(fullFilePath.c_str());
	vector<string> strAllData;
	strAllData.clear();
	if (!file)
	{
		cerr << "file open fail" << endl;
		exit(-1);
	}
	getline(file, linestring);//在文件中第一行是日期，可以不用查找
	while (getline(file, linestring))//第二行开始才是数据
	{
		strAllData.push_back(linestring);
	}
	return strAllData;
}

vector<vector<string>> CNumberManager::ReadDataFromCSVFileAndCutRanks(const string& fullFilePath/*,const vector<string>& StringToCut*/, string strCutIndex /*= ","*/)
{
	vector<string> vLineData = ReadDataFromCSVFileRanks(fullFilePath);
	vector<vector<string>> ReturnData;
	if (vLineData.size() < 3)
	{
		return ReturnData;
	}
	vector<string> vOneLinedata;
	for (vector<string>::iterator ite = vLineData.begin(); ite != vLineData.end();ite++)
	{
		vOneLinedata = CutString(*ite, strCutIndex);
		ReturnData.push_back(vOneLinedata);
		vOneLinedata.clear();
	}
	return ReturnData;
}

bool CNumberManager::SaveDataToCSVFile(const string& fullFilePath, const vector<vector<string>>& savedata)
{
	fstream outfile(fullFilePath.c_str(), ios::out);

	for (vector<vector<string>>::const_iterator lineNumber = savedata.begin(); lineNumber != savedata.end(); lineNumber++)
	{
		for (vector<string>::const_iterator ite = lineNumber->begin(); ite != lineNumber->end(); ite++)
		{
			outfile << "," << *ite;
		}
		outfile << endl;
	}

	outfile.close();
}


//////////////////////////////////////////////////////////////////////////
//线程函数
//
//
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadToUpdatefile(PVOID pvParam)
{
	NumberTreadPara* para = (NumberTreadPara*)pvParam;
	CNumberManager& macd = *para->pmacd_;

	macd.UpdateFileRanks(para->filename_);
	delete pvParam;
	macd.TreadNum--;
	return 0;
}