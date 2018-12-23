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
//���ļ��ж�ȡĿ��������
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
//�������ݱ��浽�ļ���
//
//
//////////////////////////////////////////////////////////////////////////
void CNumberManager::ReSavefileColumn(string FilePath, vector<tySData> vNumValue, string tittle)
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
		vector<string> vAllTittle = CutString(buffer[0],",");
		if (vAllTittle.size()<4)
		{
			return;
		}
		vector<string>::size_type TageTittle = 0;
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
	}//��bufferȫ��д�뵽�ļ�
	outfile.close();  
	return;	
}

//////////////////////////////////////////////////////////////////////////
//��CSV�ļ��ж�Ӧ�е�����
//filepath:�ļ�·��
//strTittle����Ҫ����������
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

	vector<string> TittleValues = CutString(linestring, ",");//ÿ�ж�����','�ָ��������
	if (TittleValues.size()<3)
	{
		return Tagevalue;//�ҵ���������̫��
	}

	vector<string>::iterator beginite = TittleValues.begin();//��һ�������ݵĵ����ƣ��ڶ�����ʼ������������
	beginite++;
	for (vector<string>::iterator ite = beginite; ite != TittleValues.end();ite++)
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
void CNumberManager::ReSavefileRanks(string FilePath, vector<tySData> vNumValue, string tittle)
{
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
			for (vector<tySData>::iterator ite = vNumValue.begin(); ite != vNumValue.end(); ite++)
			{
				outfile << "," << *ite;
			}
			outfile << endl;
		}
	}

	if (addNewLine)//��Ҫ�����µ���
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
//�½��̣߳������ļ�����
//filename���ļ�·��
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
			//����1���̰߳�ȫ��
			//����2���̵߳�ַ�ռ�
			//����3���̺߳�����ַ
			//����4���̺߳����������
			//����5����־�����Ƿ�����
			//����6���߳�ID��һ��NULL
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
	getline(file, linestring);//���ļ��е�һ�������ڣ����Բ��ò���
	while (getline(file, linestring))//�ڶ��п�ʼ��������
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
//�̺߳���
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