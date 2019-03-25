#pragma pack(1)
#include<iostream>
#include<math.h>
#include"BMPStruct.h"
#define Double_PI 6.283185307//2*PI
#define NUM_ZERO 1e-10//������0
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
	inline double length(){return sqrt(real*real+imag*imag);}//������ģ��
	//friend istream& operator >>(istream &in,complex x);
public:
	double real;//������ʵ��
	double imag;//�������鲿
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
{//��λ���ú���������������ֱ�Ӱ��ն�������Ϊ����
	int len=1<<r;//�������ݵĳ���
	int i,j,k,t;
	complex b;//��Ϊ����Դ���ݵ��м����
	for(i=0;i<len;i++)
	{
		k=i;//��ŵ�ǰ���
		t=0;//��ŵ�ǰ��ŵĶ�����������
		for(j=0;j<r;j++)//��ǰ��ŵĶ�����������
		{
			t<<=1;
			t|=(k&1);
			k>>=1;
		}
		if(i<t)//��ֹ�ظ�����
		{
			b=a[i];
			a[i]=a[t];
			a[t]=b;
		}
	}
}
void FFT(complex *result,complex *x,int r)
{
	//����˵����result��Ÿ���Ҷ�任��Ľ����x��Դ���ݣ�r��Դ���ݵĳ�����2Ϊ�׵Ķ���
	int N=1<<r;//N��Դ���ݵ��ܳ��ȣ�Ҳ���ǲ�������
	int wLen=N>>1;//��Ȩϵ���ĳ���
	complex *w=new complex[wLen];//w�Ǽ�Ȩϵ��
	int i,k;
	double angle;
	for(i=0;i<wLen;i++)//�����Ȩϵ��
	{
		angle=-i*Double_PI/N;
		w[i].real=cos(angle);
		w[i].imag=sin(angle);
	}
	memcpy_s(result,N*sizeof(complex),x,N*sizeof(complex));//��ʱ���źŸ��Ƶ�Ƶ���ź���
	bitReverse(result,r);//�Ƚ�����λ���ã���֤�������˳�����
	complex x1,x2;//ȡ��һ�εļ�����
	int level;//�����㷨��ǰ�ļ���
	int groups;//��ǰ���ϵĵ�������
	int count;//��ǰ���ϵ����ڵ�����Ŀ
	int p;//ָʾ��ǰ������������
	for(level=0;level<r;level++)//����ǰ���ĵ�������
	{
		groups=1<<(r-level-1);
		for(i=0;i<groups;i++)//�Ե�ǰ���ϵ���������ν��д���
		{
			count=1<<level;
			p=(i<<1)*(1<<level);
			for(k=0;k<count;k++)//��ǰ������ڵ��ν��д���
			{
				x1=result[p+k];
				x2=result[p+k+count];//count=1<<levelͬʱҲ��ʾ��ż�ڵ�ļ��
				result[p+k]=x1+x2*w[k*(1<<(r-level-1))];
				result[p+k+count]=x1-x2*w[k*(1<<(r-level-1))];
			}
		}
	}
	delete []w;
}
void AnalyzeBmpWithFFT(const char *bmpfilename,char*resultfile)//����ɢ����Ҷ�任����һ��λͼ
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
	//��ȡλͼ�ļ�ͷ����Ϣͷ����ɫ��
	fread(&fileHead,sizeof(fileHead),1,fin);
	if(fileHead.bfType!=BM)
	{
		cout<<"It's not a bitmap!\n"<<endl;
		return;
	}
	fread(&infoHead,sizeof(infoHead),1,fin);
	if(infoHead.biBitCount==8)//256ɫλͼ���е�ɫ��
	{
		tab=new RGBQUAD[256];
		fread(tab,4,256,fin);
	}
	int height=infoHead.biHeight,width=infoHead.biWidth;//ͼ��Ŀ�Ⱥ͸߶�
	int lineBytes=((width*infoHead.biBitCount+31)>>5)<<2;//ͼ���ÿһ���ֽ�����4��������
	int srcLen=height*lineBytes;
	BYTE *src=new BYTE[srcLen];//����ͼ�����ݿռ�
	fread(src,1,srcLen,fin);
	//Ϊ�˱�֤��ɢ����Ҷ�任��������2������������ȡnearW,nearHΪ��λͼ�Ŀ�͸ߵ������
	//2��������ֵ
	int nearW=1,nearH=1;
	int wLevel=0,hLevel=0;//wLevel,hLevel�ֱ��ӦnearW,nearHΪ2����������ʱ��ָ��
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
	//�����ʱ���Ƶ�����洢�ռ�
	int destLen=nearW*nearH;
	BYTE *dest=new BYTE[destLen];//����FFT�任���ͼ�����ݿռ�
	complex *F=new complex[destLen];//Ƶ������
	complex *f=new complex[destLen];//ʱ������
	int i,j;
	for(i=0;i<nearH;i++)//����λͼ���ݵ�ʱ����
	{
		for(j=0;j<nearW;j++){
			f[i*nearW+j].real=src[i*lineBytes+j];
			f[i*nearW+j].imag=0.0;
		}
	}
	//�������FFT.��Ϊͼ���Ƕ�ά�ģ�����Ӧ�ý��ж�ά����Ҷ�任���ֶ�ά����Ҷ�任���Կ�������һάDFT
	//�ȶ�ͼ�����y�����ϵ�FFT
	for(i=0;i<nearH;i++)
		FFT(&F[i*nearW],&f[i*nearW],wLevel);//�����������F(x,v)
	//����任���,���ڽ���x�����ϵı任��
	for(i=0;i<nearH;i++)
		for(j=0;j<nearW;j++)
			f[i+nearH*j]=F[j+nearW*i];//��F(x,v)����ת���Ա���F(u,v)
	for(j=0;j<nearW;j++)
		FFT(&F[j*nearH],&f[j*nearH],hLevel);//�����������F(u,v)
	//������㸵��Ҷ�ף���Ƶ��
	double temp;//������������Ƶ�ף��Ա�ת��Ϊͼ��ĻҶȼ�
	for(i=0;i<nearH;i++)
	{
		for(j=0;j<nearW;j++){
			temp=F[i*nearW+j].length()/100;
			if(temp>255.0)//�Ҷȼ���0~255�����ܴ���255
				temp=255.0;
			dest[i*nearW+j]=(BYTE)temp;
		}
	}
	//��󴴽�FFT�任���ͼ��
	//����FFT�任���ͼ���һЩ�����Ϣ
	infoHead.biWidth=nearW;
	infoHead.biHeight=nearH;
	infoHead.biSizeImage=destLen;
	fileHead.bfSize=destLen+fileHead.bfOffBits;
	//���洴��ͼ��
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
	AnalyzeBmpWithFFT("Դͼ��.bmp","Ч��ͼ.bmp");
	system("Ч��ͼ.bmp");
	return 0;
}