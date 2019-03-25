#pragma pack(1)
#include<iostream>
#include<math.h>
#include"BMPStruct.h"
#define Double_PI 6.283185307//2*PI
#define NUM_ZERO 1e-10//浮点数0
using namespace std;
class complex
{
public:
	complex(){real=0;imag=0;}
	complex(double r,double i):real(r),imag(i){}
	complex(double x){real=x;imag=0.0;}
	friend complex operator +(complex x,complex y);
	friend complex operator -(complex x,complex y);
	friend complex operator *(complex x,complex y);
	friend complex operator /(complex x,complex y);
	friend ostream& operator <<(ostream &out,complex &x);
	void operator=(complex x){real=x.real;imag=x.imag;}
	inline double length(){return sqrt(real*real+imag*imag);}//复数的模长
	//friend istream& operator >>(istream &in,complex x);
public:
	double real;//复数的实部
	double imag;//复数的虚部
};
complex operator+(complex x,complex y)
{
	return complex(x.real+y.real,x.imag+y.imag);
}
complex operator-(complex x,complex y)
{
	return complex(x.real-y.real,x.imag-y.imag);
}
complex operator*(complex x,complex y)
{
	return complex(x.real*y.real-x.imag*y.imag,x.real*y.imag+x.imag*y.real);
}
complex operator/(complex x,complex y)
{
	return complex((x.real*y.real+x.imag*y.imag)/(y.real*y.real+y.imag*y.imag),
		(x.imag*y.real-x.real*y.imag)/(y.real*y.real+y.imag*y.imag));
}
ostream& operator<<(ostream &out,complex &x)
{
	out<<x.real;
	if(x.imag>0)
		out<<"+"<<x.imag<<"i";
	else if(abs(x.imag)<=NUM_ZERO)
	     return out;
	else
		out<<x.imag<<"i";
	return out;
}
void bitReverse(complex *a,int r)
{//码位倒置函数，将输入的序号直接按照二进制码为倒置
	int len=1<<r;//输入数据的长度
	int i,j,k,t;
	complex b;//作为交换源数据的中间变量
	for(i=0;i<len;i++)
	{
		k=i;//存放当前序号
		t=0;//存放当前序号的二进制逆序数
		for(j=0;j<r;j++)//求当前序号的二进制逆序数
		{
			t<<=1;
			t|=(k&1);
			k>>=1;
		}
		if(i<t)//防止重复交换
		{
			b=a[i];
			a[i]=a[t];
			a[t]=b;
		}
	}
}
void FFT(complex *result,complex *x,int r)
{
	//参数说明：result存放傅里叶变换后的结果，x是源数据，r是源数据的长度以2为底的对数
	int N=1<<r;//N是源数据的总长度，也就是采样点数
	int wLen=N>>1;//加权系数的长度
	complex *w=new complex[wLen];//w是加权系数
	int i,k;
	double angle;
	for(i=0;i<wLen;i++)//计算加权系数
	{
		angle=-i*Double_PI/N;
		w[i].real=cos(angle);
		w[i].imag=sin(angle);
	}
	memcpy_s(result,N*sizeof(complex),x,N*sizeof(complex));//将时域信号复制到频域信号里
	bitReverse(result,r);//先进行码位倒置，保证输出按照顺序输出
	complex x1,x2;//取上一次的计算结果
	int level;//蝶形算法当前的级数
	int groups;//当前级上的蝶形组数
	int count;//当前组上的相邻蝶形数目
	int p;//指示当前计算的组的首行
	for(level=0;level<r;level++)//处理当前级的蝶形运算
	{
		groups=1<<(r-level-1);
		for(i=0;i<groups;i++)//对当前级上的所有组蝶形进行处理
		{
			count=1<<level;
			p=(i<<1)*(1<<level);
			for(k=0;k<count;k++)//当前组的相邻蝶形进行处理
			{
				x1=result[p+k];
				x2=result[p+k+count];//count=1<<level同时也表示对偶节点的间距
				result[p+k]=x1+x2*w[k*(1<<(r-level-1))];
				result[p+k+count]=x1-x2*w[k*(1<<(r-level-1))];
			}
		}
	}
	delete []w;
}
void AnalyzeBmpWithFFT(const char *bmpfilename,char*resultfile)//用离散傅里叶变换分析一幅位图
{
	FILE *fin;
	if(fopen_s(&fin,bmpfilename,"r")!=0)
	{
		cout<<"Cannot open the file!\n"<<endl;
		return;
	}
	BITMAPFILEHEADER fileHead;
	BITMAPINFOHEADER infoHead;
	RGBQUAD *tab=NULL;
	//读取位图文件头，信息头，调色板
	fread(&fileHead,sizeof(fileHead),1,fin);
	if(fileHead.bfType!=BM)
	{
		cout<<"It's not a bitmap!\n"<<endl;
		return;
	}
	fread(&infoHead,sizeof(infoHead),1,fin);
	if(infoHead.biBitCount==8)//256色位图，有调色板
	{
		tab=new RGBQUAD[256];
		fread(tab,4,256,fin);
	}
	int height=infoHead.biHeight,width=infoHead.biWidth;//图像的宽度和高度
	int lineBytes=((width*infoHead.biBitCount+31)>>5)<<2;//图像的每一行字节数是4的整数倍
	int srcLen=height*lineBytes;
	BYTE *src=new BYTE[srcLen];//分配图像数据空间
	fread(src,1,srcLen,fin);
	//为了保证离散傅里叶变换数据量是2的整数倍，故取nearW,nearH为距位图的宽和高的最近的
	//2的整数倍值
	int nearW=1,nearH=1;
	int wLevel=0,hLevel=0;//wLevel,hLevel分别对应nearW,nearH为2的整数次幂时的指数
	while((nearW<<1)<=width)
	{
		nearW<<=1;
		wLevel++;
	}
	while((nearH<<1)<=height)
	{
		nearH<<=1;
		hLevel++;
	}
	//下面对时域和频域分配存储空间
	int destLen=nearW*nearH;
	BYTE *dest=new BYTE[destLen];//分配FFT变换后的图形数据空间
	complex *F=new complex[destLen];//频域数据
	complex *f=new complex[destLen];//时域数据
	int i,j;
	for(i=0;i<nearH;i++)//复制位图数据到时域里
	{
		for(j=0;j<nearW;j++){
			f[i*nearW+j].real=src[i*lineBytes+j];
			f[i*nearW+j].imag=0.0;
		}
	}
	//下面进行FFT.因为图像是二维的，所以应该进行二维傅里叶变换，又二维傅里叶变换可以看成两次一维DFT
	//先对图像进行y方向上的FFT
	for(i=0;i<nearH;i++)
		FFT(&F[i*nearW],&f[i*nearW],wLevel);//这里求出的是F(x,v)
	//保存变换结果,便于进行x方向上的变换用
	for(i=0;i<nearH;i++)
		for(j=0;j<nearW;j++)
			f[i+nearH*j]=F[j+nearW*i];//对F(x,v)进行转置以便求F(u,v)
	for(j=0;j<nearW;j++)
		FFT(&F[j*nearH],&f[j*nearH],hLevel);//这里求出的是F(u,v)
	//下面计算傅里叶谱，即频谱
	double temp;//保存计算出来的频谱，以便转化为图像的灰度级
	for(i=0;i<nearH;i++)
	{
		for(j=0;j<nearW;j++){
			temp=F[i*nearW+j].length()/100;
			if(temp>255.0)//灰度级（0~255）不能大于255
				temp=255.0;
			dest[i*nearW+j]=(BYTE)temp;
		}
	}
	//最后创建FFT变换后的图像
	//设置FFT变换后的图像的一些相关信息
	infoHead.biWidth=nearW;
	infoHead.biHeight=nearH;
	infoHead.biSizeImage=destLen;
	fileHead.bfSize=destLen+fileHead.bfOffBits;
	//下面创建图像
	FILE *fout;
	if(fopen_s(&fout,resultfile,"w")!=0)
	{
		cout<<"Cannot create the file !"<<endl;
		return ;
	}
	fwrite(&fileHead,sizeof(fileHead),1,fout);
	fwrite(&infoHead,sizeof(infoHead),1,fout);
	if(tab)
	{
		for(i=0;i<256;i++)
			fwrite(&tab[i],4,1,fout);
	}

	fwrite(dest,1,infoHead.biSizeImage,fout);
	fclose(fin);
	fclose(fout);
	delete[]dest;
	if(tab)
		delete[]tab;
	delete[]src;
	delete[]F;
	delete[]f;
}
int main()
{
	AnalyzeBmpWithFFT("源图像.bmp","效果图.bmp");
	system("效果图.bmp");
	return 0;
}