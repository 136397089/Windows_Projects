
// CodeChangelateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CodeChangelate.h"
#include "CodeChangelateDlg.h"
#include "afxdialogex.h"
#include <map>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCodeChangelateDlg �Ի���



CCodeChangelateDlg::CCodeChangelateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCodeChangelateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCodeChangelateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITINPUT, m_editInput);
	DDX_Control(pDX, IDC_EDITOUTPUT, m_Output);
}

BEGIN_MESSAGE_MAP(CCodeChangelateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHANGELATE, &CCodeChangelateDlg::OnBnClickedChangelate)
END_MESSAGE_MAP()


// CCodeChangelateDlg ��Ϣ�������

BOOL CCodeChangelateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCodeChangelateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCodeChangelateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCodeChangelateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



enum BasisType
{
	_eDPUnknow = 0,

	_eBasisDayDIFF = (1<<0),
	_eBasisDeDayDIFF = (1 << 1),
	_eBasisDayCR = (1 << 2),//
	_eBasisDayKDJ_D = (1 << 3),
	_eBasisDeDayKDJ_K = (1 << 4),
 	_eBasisDeDayDEA = (1 << 5),

	_eBasisShDayDIFF = (1 << 6),
	_eBasisShDeDayDIFF = (1 << 7),
 	_eBasisShDeDayDEA = (1 << 8),
	_eBasisShDeDayKDJ_K = (1 << 9),
	_eBasisDeDayKDJ_D = (1 << 10),
	_eBasisDayMean = (1 << 11),

	_eBasisMonthDIFF = (1 << 12),
	_eBasisDeMonthDIFF = (1 << 13),
	_eBasisShDeDayKDJ_D = (1 << 14),
	_eBasisMonthKDJ_K = (1 << 15),
	_eBasisDeMonthKDJ_K = (1 << 16),
	_eBasisDeMa = (1 << 17),

	_eBasisDeDayCRMA = (1 << 18),
	_eBasisShDeDayCRMA = (1 << 19),

	_eBasisDeDayMA1 = (1 << 20),
	_eBasisDeDayMA2 = (1 << 21),
	_eBasisDeDayMA3 = (1 << 22),
	_eBasisDeDayMA4 = (1 << 23),

	_eBasisDeDayCR = (1 << 24),
	_eBasisDayRSI1 = (1 << 25),
	_eBasisDeDayRSI1 = (1 << 26),
	_eBasisDeDayRate = (1 << 27),
	_eBasisDayVR = (1 << 28),
	_eBasisDeDayVR = (1 << 29)

};



void CCodeChangelateDlg::OnBnClickedChangelate()
{
	map <BasisType, string> typemap =
	{
		{ _eDPUnknow, "DPUnknow" },
		{ _eBasisDayDIFF, "DayDIFF" },
		{ _eBasisDeDayDIFF, "DeDayDIFF" },
		{ _eBasisDayCR, "DayCR" },
		{ _eBasisDayKDJ_D, "DayKDJ_D" },
		{ _eBasisDeDayKDJ_K, "DeDayKDJ_K" },
		{ _eBasisDeDayDEA, "DeDayDEA" },
		{ _eBasisShDayDIFF, "ShDayDIFF" },
		{ _eBasisShDeDayDIFF, "ShDeDayDIFF" },
		{ _eBasisShDeDayDEA, "ShDeDayDEA" },
		{ _eBasisShDeDayKDJ_K, "ShDeDayKDJ_K" },
		{ _eBasisDeDayKDJ_D, "DeDayKDJ_D"},
		{ _eBasisDayMean, "DayMean" },
		{ _eBasisMonthDIFF, "MonthDIFF" },
		{ _eBasisDeMonthDIFF, "DeMonthDIFF" },
		{ _eBasisShDeDayKDJ_D ,"ShDeDayKDJ_D"},
		{ _eBasisMonthKDJ_K, "MonthKDJ_K" },
		{ _eBasisDeMonthKDJ_K, "DeMonthKDJ_K" },
		{ _eBasisDeMa, "DeMa" },
		{ _eBasisDeDayCRMA, "DeDayCRMA" },
		{ _eBasisShDeDayCRMA, "ShDeDayCRMA" },
		{ _eBasisDeDayMA1, "DeDayMA1" },
		{ _eBasisDeDayMA2, "DeDayMA2" },
		{ _eBasisDeDayMA3, "DeDayMA3" },
		{ _eBasisDeDayMA4, "DeDayMA4" },
		{ _eBasisDeDayCR, "DeDayCR" },
		{ _eBasisDayRSI1, "DayRSI1" },
		{ _eBasisDeDayRSI1, "DeDayRSI1" },
		{ _eBasisDeDayRate, "DeDayRate" },
		{ _eBasisDayVR, "DayVR" },
		{ _eBasisDeDayVR, "DeDayVR" }
	};
	CString winTest;
	m_editInput.GetWindowTextW(winTest);
	unsigned int Code = _ttoi(winTest);
	CString OutPutString;
	for (unsigned int i = 0; i < 32;i++)
	{
		if ((Code & (1 << i)) == (1 << i))
		{
			OutPutString.Format(OutPutString + _T(" ") + typemap[(BasisType)(1 << i)].c_str()+"\r\n");
		}
	}
	m_Output.SetWindowTextW(OutPutString);
	return;
}
