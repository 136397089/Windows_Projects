#include "stdafx.h"
#include "CommonFunction.h"


vector<string> CutString(string inputstring,string CutIndex)
{
	vector<string> vTempString;
	string strLeft,strRight;
	strRight = inputstring;

	int index = strRight.find(CutIndex);
	while(index > 0)
	{
		strLeft = strRight.substr(0,strRight.find(CutIndex));
		strRight = strRight.substr(strRight.find(CutIndex)+1);
		vTempString.push_back(strLeft);
		index = strRight.find(CutIndex);
	}
	return vTempString;
}

std::string AssembleString( vector<string>inputstring,string Index )
{
	string ReturnString="";
	for (vector<string>::iterator ite = inputstring.begin();ite!=inputstring.end();ite++)
	{
		ReturnString = ReturnString+*ite+Index;
	}
	//删除最后一个index
	ReturnString = ReturnString.substr(0,ReturnString.size()-1-Index.size());
	return ReturnString;
}
