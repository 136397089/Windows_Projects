#ifndef __COMMONSTRUCT_H
#define __COMMONSTRUCT_H

#include <string>
#include <vector>
using namespace std;
struct OneDayVariable;

//
enum State
{
	Normal,
	DefaultState
};
enum DayVariableType
{
	MACDData
};
#define G_IndexClose  "close"
#define G_IndexChangeRate  "changeRate"
#define G_IndexMACD_EDA  "MACD_EDA"
#define G_IndexMACD_Diff  "MACD_Diff"
#define G_IndexMACD_BAR  "MACD_BAR"
// #define G_IndexChengeRate "changeRare"
//一天的股票数据
struct OneDayVariable
{
	OneDayVariable() :
		closePrice(0.0), changeRate(0.0), fMACD_EDA(0.0),
		fMACD_Diff(0.0), fMACD_BAR(0.0), valueType(MACDData),
		todayState(DefaultState), _Date(""){}

	float closePrice;
	float changeRate;
	float fMACD_EDA;
	float fMACD_Diff;
	float fMACD_BAR;
	string _Date;
	State todayState;
	DayVariableType valueType;
};


//状态机
class StateMachine
{
	StateMachine(){
		_state = DefaultState;
	}

	~StateMachine(){}
	State GetState(){
		return _state;
	}

	bool Setvalue(OneDayVariable value){
		return false;
	}

private:
	State _state;
};

//
struct ValueParameter
{
	int maxDataSize;
	float alf;
	float beta;
	float gama;
	ValueParameter() :maxDataSize(0), alf(0), beta(0), gama(0){}
};

//PID系数
// struct PID
// {
// 	double value_P;
// 	double value_I;
// 	double value_D;
// 
// };
//PID输出量
struct PID
{
	string dataDescribe;
	double result_P;
	double result_I;
	double result_D;
	PID(){
		result_I = 0;
		result_P = 0;
		result_D = 0;
	}
	void SetMaxData(PID Inpudata){
		if (Inpudata.result_P > result_P) result_P = Inpudata.result_P;
		if (Inpudata.result_I > result_I) result_I = Inpudata.result_I;
		if (Inpudata.result_D > result_D) result_D = Inpudata.result_D;
	}
	void SetTo(double InpuData){ 
		result_P = InpuData; 
		result_I = InpuData;
		result_D = InpuData;
	};
	void SetMinData(PID Inpudata){
		if (Inpudata.result_P < result_P) result_P = Inpudata.result_P;
		if (Inpudata.result_I < result_I) result_I = Inpudata.result_I;
		if (Inpudata.result_D < result_D) result_D = Inpudata.result_D;
	}
	inline double GetSum(){
		return result_P + result_I + result_D;
	}
	PID operator * (const PID piddata)  const {
		PID reData;
		reData.result_P = this->result_P * piddata.result_P;
		reData.result_I = this->result_I * piddata.result_I;
		reData.result_D = this->result_D * piddata.result_D;
		return reData;
	}
	PID operator/(PID piddata) const {
		PID reData;
		reData.result_P = this->result_P / piddata.result_P;
		reData.result_I = this->result_I / piddata.result_I;
		reData.result_D = this->result_D / piddata.result_D;
		return reData;
	}
	PID operator-(PID piddata) const {
		PID reData;
		reData.result_P = this->result_P - piddata.result_P;
		reData.result_I = this->result_I - piddata.result_I;
		reData.result_D = this->result_D - piddata.result_D;
		return reData;
	}
};

typedef vector<float> ParameterDatas;

//用于进化的系数，
typedef vector<float> Coefficients;


struct PIDLimits
{
	PID closeMax;
	PID closeMin;
	PID ChangeRateMax;
	PID ChangeRateMin;
	PID MACD_EDAMax;
	PID MACD_EDAMin;
	PID MACD_DiffMax;
	PID MACD_DiffMin;
	PID MACD_BARMax;
	PID MACD_BARMin;

};


#endif