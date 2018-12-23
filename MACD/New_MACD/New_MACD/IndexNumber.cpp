#include "StdAfx.h"
#include "IndexNumber.h"

CIndexNumber::CIndexNumber(void)
{
}

CIndexNumber::~CIndexNumber(void)
{
}
//从文件中读取目标列数据
vector<double> CIndexNumber::ReadStringFormFile(string filepath,string strTittle)
{
	vector<double> Tagevalue;
	string linestring;
	ifstream file;
	file.open(filepath.c_str());
	if(!file)
	{
		cerr<<"file open fail"<<endl;
		exit(-1);
	}

	getline(file,linestring);
	if (linestring.find("open") < 0)
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
		Tagevalue.push_back((double)atof(StockValues[index].c_str()));
	}

	file.close();

	return Tagevalue;
}

void CIndexNumber::ReSavefileColumn(string FilePath,vector<double> vMACDValue,string tittle)
{
	fstream outfile(FilePath.c_str(),ios::out|ios::in);
	string ch;
	string buffer[MAX_FILE_LINE];

	//将文件全部内容读出到buffer
	int i=0; 
	while (getline(outfile,buffer[i]))
	{ 
		i++;  
	}
	//如果读出数据的行数与写入数据的行数不对应，则不做操作
	//如果行数太少，则不做操作
	if (vMACDValue.size()+1 != i || i<10)
	{
		return;
	}
	string MACDValue;
	int k= 0;
	//如果没有找到写入的目标列，则在后面添加
	if (buffer[0].find(tittle)>buffer[0].size())
	{
		buffer[0] += ",";
		buffer[0] +=tittle;
		for (k=1; k<i;k++)
		{
			ostringstream Macd_buffer;
			Macd_buffer<<","<<vMACDValue[k-1];
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
			vIndexData[TageTittle] = vMACDValue[k-1];
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

float CDateData::operator[]( string index )
{
	if ("Open" == index)
		return fOpen;
	else if ("Clode" == index)
		return fClode;
	else if ("High" == index)
		return fHigh;
	else if ("Low" == index)
		return fLow;
	else
		return 0.0;
}
