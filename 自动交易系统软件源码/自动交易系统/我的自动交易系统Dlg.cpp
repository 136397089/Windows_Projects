// �ҵ��Զ�����ϵͳDlg.cpp : implementation file
//

#include "stdafx.h"
#include "�ҵ��Զ�����ϵͳ.h"
#include "�ҵ��Զ�����ϵͳDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
	m_edit_jb = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
	DDX_Text(pDX, IDC_EDIT1, m_edit_jb);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
HWND m_mbhwnd,mainhwnd,m_yjchwnd;
CString str_dtj,str_zqdm,str_ztj,str_mm,str_dqj;
CString str_mmsl1;
BOOL mm_false,jd_false;
double dqj;
int tempnum;
/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers
void FileWrite(CString gpdm);
void FileRead();
void CreateFile();//����һ���ļ�
CString GetModulePath();//��ȡ�����������ڵ�·��

BOOL CALLBACK EnumChildProc(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
);
BOOL CALLBACK EnumChildProc1(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
);
BOOL CALLBACK EnumChildProc2(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
);
BOOL CALLBACK EnumChildProc4(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
);
BOOL CALLBACK EnumChildProc2(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
)
{
	if(!hwnd)
		return false;
	char buff[1024];
	CString str;
	::GetWindowText(hwnd,buff,sizeof(buff));
	str=buff;
	/*if(str=="ˢ ��")
	{
		::PostMessage(hwnd,WM_LBUTTONDOWN,NULL,NULL);
		::PostMessage(hwnd,WM_LBUTTONUP,NULL,NULL);
	}*/

	if(str=="��  ��")
	{
		if(IsWindowEnabled(hwnd))
		{
			//MessageBox(NULL,"ok","ok",NULL);
			::SetForegroundWindow(m_mbhwnd);
			SetTimer(mainhwnd,6,500,NULL);
		}
		else
		{
			//MessageBox(NULL,"nook","nook",NULL);
			SetTimer(mainhwnd,1,500,NULL);
		}
		return false;
	}
	return true;
}
BOOL CALLBACK EnumChildProc4(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
)
{
	if(!hwnd)
		return false;
	char buff[1024];
	CString str;
	::GetWindowText(hwnd,buff,sizeof(buff));
	str=buff;
	/*if(str=="ˢ ��")
	{
		::PostMessage(hwnd,WM_LBUTTONDOWN,NULL,NULL);
		::PostMessage(hwnd,WM_LBUTTONUP,NULL,NULL);
	}*/

	if(str=="��  ��")
	{
		if(IsWindowEnabled(hwnd))
		{
			//MessageBox(NULL,"ok","ok",NULL);
			::SetForegroundWindow(m_mbhwnd);
			SetTimer(mainhwnd,7,500,NULL);
		}
		else
		{
			//MessageBox(NULL,"nook","nook",NULL);
			SetTimer(mainhwnd,8,500,NULL);
		}
		return false;
	}
	return true;
}
BOOL CALLBACK EnumChildProcSet(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
);
void KeySend(CString str,bool Enter);
BOOL CALLBACK EnumChildProcSet(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
)

{

	if(lParam)
		return false;
		char buff[1024];
	double f;
	double hqf;
	int idhwnd;
	HWND HD;
	CString str;
	idhwnd=::GetDlgCtrlID(hwnd);
	str=buff;
	if(idhwnd==1593)//��ͣ��
		::SetWindowText(hwnd,str_dtj);
	if(idhwnd==12006)//֤ȯ����
	{
		
		::GetWindowText(::GetDlgItem(::GetActiveWindow(),IDC_EDIT1),buff,sizeof(buff));
		str=buff;
		::SetWindowText(hwnd,str);
	}
	return true;
}
BOOL CALLBACK EnumChildProcGet(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
);
BOOL CALLBACK EnumChildProcGet(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
)

{
	if(lParam)
		return false;
		char buff[1024];
	double f;
	double hqf;
	int idhwnd;
	CString str;
	idhwnd=::GetDlgCtrlID(hwnd);
	str=buff;
		if(idhwnd==1593)//��ͣ��
	{
		::GetWindowText(hwnd,buff,sizeof(buff));
		str=buff;
		if (str!="")
		str_ztj=buff;
		return true;
	}
		if(idhwnd==1193)//��ͣ��
	{
		::GetWindowText(hwnd,buff,sizeof(buff));
		str=buff;
		if (str!="")
		str_dtj=buff;
		return true;
	}
	
	return true;
}
BOOL CALLBACK EnumChildProc3(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
);
BOOL CALLBACK EnumChildProc3(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
  //char str_MyItemText[12]={0};
  //GetWindowText(NULL,tempStr,256);
)

{
	if(!hwnd)
	{
		if(mm_false)
			
		return false;
	}
	CMyDlg dlg;
	char buff[1024];
	double f;
	double hqf,hqf1;
	int idhwnd;
	CString str;
	idhwnd=::GetDlgCtrlID(hwnd);
	str=buff;


				
				::KillTimer(mainhwnd,0);
				if(jd_false)
				{
					jd_false=false;
					::PostMessage(m_mbhwnd,WM_KEYDOWN,112,NULL);//f1��
					SetForegroundWindow(m_mbhwnd);
					Sleep(150);
					KeySend(str_zqdm,false);
					SetForegroundWindow(m_mbhwnd);
					Sleep(1500);
					KeySend(str_ztj,true);//����ͣ����
					::GetWindowText(::GetDlgItem(mainhwnd,IDC_EDIT3),buff,sizeof(buff));
					str_mmsl1="100";
						Sleep (1500);
					SetForegroundWindow(m_mbhwnd);
				
					
					KeySend(str_mmsl1,true);
					Sleep (150);
					::SetTimer(mainhwnd,2,1500,NULL);
				}

	return true;
}
BOOL CALLBACK EnumChildProc(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value

)
{
	SYSTEMTIME   t; 
	if (!hwnd)
	{
		return 0;
	}
	char buff[1024];
	CString str;
        GetWindowText(hwnd, buff, sizeof buff);
	str=buff;
	str=str.Mid(0,14);
//if (str=="*5485 - �㷢ͨ���Ž���-֤���V4.38")
//if (str=="�㷢ͨ���Ž���V5.66 �����ؽ���· ������")
//	if (str=="����֤ȯ����֤")//V5.66 �����ؽ���· ������")
		if (str=="�㷢ͨ���Ž���")//V5.66 �����ؽ���· ������")
{
	m_mbhwnd=hwnd;
//	EnumChildWindows(hwnd,EnumChildProc1,NULL);
	return true;

}
	if(str.GetLength()>8)
	//str=str.Mid(0,6);
	if (str.Mid(0,6)=="Ԥ�� -")
{
	m_yjchwnd=hwnd;
//	PostMessage(hwnd,WM_CLOSE,NULL,NULL);
//	EnumChildWindows(hwnd,EnumChildProc1,NULL);
	return true;
}
	if (str.Mid(0,19)=="�ɺ�����ʦ - �����")
{
		//MessageBox(NULL,"OK","OK",NULL);
		::GetLocalTime(&t);   
	   int hour,minute,second;
	   
	   //str.Format("%d:%d:%d",t.wHour,t.wMinute,t.wSecond);
	   str.Format("%d",t.wHour);
	   hour=atoi(str);
	   if(hour>=15)
	   {
		PostMessage(hwnd,WM_CLOSE,NULL,NULL);
		SetTimer(mainhwnd,9,60000,NULL);
		
	   }
	return true;
}

//if (str=="Windows ���������")
{
//	MessageBox(NULL,"ok","ok",NULL);
	

	//	GetClassName(hwnd,buff,sizeof(buff));
	//	str=buff;
	//hwnd=FindWindow("#32770","Windows ���������");
		
	hwnd=FindWindowEx(hwnd,0,"#32771",0);
	hwnd=FindWindowEx(hwnd,0,"SysListView32",0);//SysListView32
    //ע�⣺��������www.ccrun.com��by ccrun(����)��ת����ע��������
    //Ϊ��ֹĳЩ�������ε�ת���ߣ��ʳ����²ߣ��ڴ����м�������������ԭ�¡�

    //GetWindowThreadProcessId(hWindow,&dwProcessID);
	
}
	return true;
}

  BOOL CALLBACK EnumChildProc1(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value

)
{
	if (!hwnd)
	{
		return 0;
	}
	
		
	const nMaxLen=1023;
    char szBuf[nMaxLen+1];
	CString str;
	
	int iItem=0;
	LVITEM lvitem, *plvitem;
	DWORD PID;
	char *p_MyItemText;//Ŀ��������������TEXT�ĵ�ַ
	char str_MyItemText[100]={0};



    int          nLVItemCount;
    DWORD        dwProcessID;
    HANDLE       hProcess;
	HWND	Hwnd1;
    LVITEM       lvItemLocal;
    DWORD        dwBytesRead, dwBytesWrite;
    bool         bSuccess,bWriteOK;
	char bubb[1024];
	GetClassName(hwnd,bubb,sizeof(bubb));
	str=bubb;

	if(str!="SysListView32")
		return true;
	str="";
	GetWindowText(hwnd,bubb, sizeof bubb);
	str=bubb;
	if(str!="List2")
		return true;
	GetWindowThreadProcessId(hwnd, &dwProcessID);
    hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);
    if(!hProcess)  //�ò���ָ�����̵ľ��
	{
		::MessageBox(NULL,"δ�ҵ�ָ���Ĵ���","����",NULL);
        return false;
	}
    //��ָ�������ڷ���洢�ռ�
	
    LPVOID lpTextRemote=VirtualAllocEx(hProcess,NULL,nMaxLen+1,MEM_COMMIT,PAGE_READWRITE);
  
	LPVOID lpListItemRemote=VirtualAllocEx(hProcess,NULL,sizeof(LVITEM),MEM_COMMIT,PAGE_READWRITE);
    if((!lpTextRemote) || (!lpListItemRemote)) //������ָ�������ڷ���洢�ռ�
	{
       // ::MessageBox(NULL,"[Windows ���������] ��δ������","����",NULL);
		return false;
	}
    //nLVItemCount=ListView_GetItemCount(hWindow);
	nLVItemCount=ListView_GetItemCount(hwnd);
	
    //strlist->Add("Welcome to www.ccrun.com");    
   // strlist->Add("ListView��Item����: " + String(nLVItemCount));
   // strlist->Add("---------------------------");
	if(nLVItemCount<1) return false;
	bool okflase=false;
    for(int i=0;i<nLVItemCount;i++)
    {
		
		for(int j=0;j<3;j++)
		{
        ZeroMemory(szBuf,nMaxLen+1);
        bWriteOK= WriteProcessMemory(hProcess,lpTextRemote,(LPVOID)szBuf,nMaxLen+1,(LPDWORD)&dwBytesWrite);
// ����ת�� C++Builder�о� - http://www.ccrun.com/article.asp?i=583&d=eahk4z
        if(!bWriteOK) //д�ڴ����
            return false;
        lvItemLocal.iItem=i;
        lvItemLocal.iSubItem=j;
        lvItemLocal.mask=LVIF_TEXT;
        lvItemLocal.cchTextMax=nMaxLen;
        lvItemLocal.pszText=(LPTSTR)lpTextRemote;
        dwBytesWrite=0;
        bWriteOK=WriteProcessMemory(hProcess,lpListItemRemote,(LPVOID)&lvItemLocal,sizeof(LVITEM),(LPDWORD)&dwBytesWrite);
        if(!bWriteOK) //д�ڴ����
            return false;

        //SendMessage(hWindow,LVM_GETITEMTEXT,(WPARAM)i,(LPARAM)lpListItemRemote);
		::SendMessage(hwnd,LVM_GETITEMTEXT,(WPARAM)i,(LPARAM)lpListItemRemote);
	//	::SendMessage(hwnd, LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)lvItemLocal);

        bSuccess=ReadProcessMemory(hProcess,lpTextRemote,szBuf,nMaxLen+1,&dwBytesRead);
		
        //��ָ�����̴洢�ռ��ȡ�ı�
        if(!bSuccess) //������ָ�������ڶ�ȡ�ı�
            return false;
		str=szBuf;
			if(str != "")
			{
				switch (j)
				{
				case 0://Ʒ�ִ���
					{
						GetDlgItem(GetActiveWindow(),IDC_EDIT1);
						//::SetWindowText(GetDlgItem(GetActiveWindow(),IDC_EDIT1),"123");
						::SetWindowText(GetDlgItem(GetForegroundWindow(),IDC_EDIT1),str);
						str_zqdm=str;
						break;
					}
				case 1://Ԥ������
					{
						GetDlgItem(GetActiveWindow(),IDC_EDIT2);
						::SetWindowText(GetDlgItem(GetForegroundWindow(),IDC_EDIT2),str);
						str_mm=str.Mid(0,2);
						str_mmsl1=str.Mid(str.Find(" "),5);//�ӵ�һ���ո�ʼ�ص��ڶ����ո�λ�õ��ַ�
						break;
					}
				case 2://Ԥ��ʱ��/�۸�
					{
						SYSTEMTIME   t;   
						char buff[1204];
						  ::GetLocalTime(&t);   
						 CString strHour,strMinute;
						 
						  //str.Format("%d:%d:%d",t.wHour,t.wMinute,t.wSecond);
						  //str.Format("%d:%d:%d:%d:%d:%d:",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond);
						 // MessageBox(str);
						 
						 strHour="�㶫 ������ " +str;
						
							 sprintf(str_MyItemText,strHour);   
					str=str.Mid(str.Find(" ")+1);
					str_dqj=str.Mid(str.Find(" ")+1);
					
					
						str=str.Mid(0,str.Find(" "));
						strHour=str.Mid(0,2);
						strMinute=str.Mid(str.Find(":")+1,2);
						if (atoi(strHour)==t.wHour)
						{
							
							if(atoi(strMinute)==t.wMinute || atoi(strMinute)+1==t.wMinute  )
							{
							//::MessageBox(NULL,"OK","��ʿ",NULL);
								::KillTimer(mainhwnd,4);
								//::GetWindowText(::GetDlgItem(mainhwnd,IDC_EDIT1),buff,sizeof(buff));
								//str_zqdm=buff;
							/*	KeySend(str_zqdm,false);
								Sleep(500);
								jd_false=true;
								EnumChildWindows(m_mbhwnd,EnumChildProcGet,NULL);
								EnumChildWindows(m_mbhwnd,EnumChildProc3,NULL);*/
								okflase=true;
								str_zqdm=str_zqdm+" "+str_mmsl1+" "+str_dqj+" "+str_mm;
								FileWrite(str_zqdm);
								Sleep(1000);
								plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
								p_MyItemText=(char*)VirtualAllocEx(hProcess, NULL, 100, MEM_COMMIT, PAGE_READWRITE);
								if ((!plvitem)||(!p_MyItemText))
								{
								//MessageBox(NULL,"�޷������ڴ棡","����",NULL);
								}
								else
								{
								//MessageBox(NULL,"����ʾ���򽫸���TaskManager�е�6����Ŀ�е�1�е����ݡ�","��ʾ",NULL);

								iItem=i;//5�������ǵ����������㿪ʼ��
								lvitem.iSubItem=j;//ͬ��
								lvitem.pszText=p_MyItemText;

								WriteProcessMemory(hProcess, p_MyItemText, &str_MyItemText, 100, NULL);
								WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);

								//��Ŀ�������LVM_SETITEMTEXT��Ϣ
								
								SendMessage(hwnd, LVM_SETITEMTEXT, (WPARAM)iItem, (LPARAM)plvitem);
								}


							}
						}
						::SetWindowText(GetDlgItem(GetForegroundWindow(),IDC_EDIT3),str);
						break;
					}
				}
				
			} 
		if(okflase) 	continue;
		CreateFile();//����б�
		}

					
    }//end of for(i)
    //��ָ���������ͷŴ洢�ռ�
    VirtualFreeEx(hProcess,lpListItemRemote,0,MEM_RELEASE);
    VirtualFreeEx(hProcess,lpTextRemote,0,MEM_RELEASE);
    //�ر�ָ�����̾��
    CloseHandle(hProcess);
	FileRead();
	return true;
}
BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	EnumChildWindows(NULL,EnumChildProc,NULL);
	if(!m_mbhwnd)
		MessageBox("��ָ���Ľ������û�д�","��ʾ",NULL);
	::PostMessage(m_mbhwnd,WM_KEYDOWN,113,NULL);
	str_zqdm="123456";
	KeySend(str_zqdm,true);
//	::SetWindowPos (m_mbhwnd, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOSIZE || SWP_NOMOVE);
	mainhwnd=this->m_hWnd;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetTimer(0,2000,NULL);
	//EnumChildWindows(m_mbhwnd,EnumChildProc1,NULL);
	
	/*hwnd=::FindWindow("#32770","�㷢ͨ���Ž���-֤���V4.38 - yzt[15805485]");
	hwnd=::FindWindow("#32770","�㷢ͨ���Ž���-֤���V4.38");
	hwnd=::FindWindow("#32770","�㷢ͨ���Ž���V5.66 �����ؽ���· ������");
	//hwnd=::FindWindow("#32770","Windows ���������");
	hwnd=::FindWindowEx(hwnd,0,"#32770",0);*/

//	CDialog::OnOK();
}

void CMyDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	//SetTimer(0,1000,NULL);
	SetTimer(1,1000,NULL);


}

void CMyDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString   str; 
	SYSTEMTIME   t;   
	CMyDlg dlg;
	char buff[1024];
	switch (nIDEvent)
	{
	case 0:
			
	   ::GetLocalTime(&t);   
	    
	   str.Format("%d:%d:%d",t.wHour,t.wMinute,t.wSecond);
	   if(t.wHour>=9  )
		   if(true )//
	   {
		   /*str.Format("%d",t.wHour);
		   MessageBox(str);*/
		   ::SetForegroundWindow(m_mbhwnd);
		::PostMessage(m_mbhwnd,WM_KEYDOWN,113,NULL);//f2
	//	str_zqdm=GetHq(IDC_EDIT1);
		::GetWindowText(::GetDlgItem(mainhwnd,IDC_EDIT1),buff,sizeof(buff));
		str_zqdm=buff;
	//	str_mmsl1=GetHq(IDC_EDIT3);
		
		KeySend(str_zqdm,false);
		Sleep(500);
		jd_false=true;
		EnumChildWindows(m_mbhwnd,EnumChildProcGet,NULL);
		EnumChildWindows(m_mbhwnd,EnumChildProc3,NULL);
		/*	char strClassName[255],strCaption[255];
			int strTextLength;
			char buff[1024];
			CString str;
			HWND hWnd;
				POINT   pt;   
			  GetCursorPos(&pt);   
			 hWnd = (HWND) WindowFromPoint(pt);   
					  GetClassName(   (HWND)hWnd,   buff,   sizeof(buff)   );   
			 if   (   strcmp(buff,   "Button")   ) 
			 {
					  str=buff;
					  if (str=="SysListView32")
							MessageBox(str);
					//	m_edit_jb="hWnd";
					//	m_edit_jb.UpdateDate(TRUE);
					   str=buff;
					  GetDlgItem(IDC_EDIT1)->SetWindowText(str);
			 }*/
	   }
						break;
					
	case 1:
		KillTimer(1);
		EnumChildWindows(m_mbhwnd,EnumChildProc2,NULL);
		break;
	case 2:
		if(FindWindow(NULL,"����ȷ��"))
		{
			::SetForegroundWindow(::FindWindow(NULL,"����ȷ��"));
			Sleep(500);
			::keybd_event(13,NULL,0,NULL);
			::keybd_event(13,NULL,2,NULL);
			
		}
		else
		{
			if(FindWindow(NULL,"��ʾ"))
			{
				SetTimer(3,1200,NULL);
			}
			else
			{
				tempnum=tempnum+1;
			}
			if(tempnum>=6)
			{
				tempnum=0;
				SetTimer(3,1200,NULL);
			}


			
			//
		}
		break;
	case 3:
		if(FindWindow(NULL,"��ʾ"))
		{
			
			::SetForegroundWindow(::FindWindow(NULL,"��ʾ"));
			::keybd_event(13,NULL,0,NULL);
			::keybd_event(13,NULL,2,NULL);
			SetTimer(3,1200,NULL);
		}
		else
		{
			
			KillTimer(3);
				KillTimer(2);
				
				FileRead();
		}
		break;
	case 4:
		
	   ::GetLocalTime(&t);   
	   int hour,minute,second;
	   
	   //str.Format("%d:%d:%d",t.wHour,t.wMinute,t.wSecond);
	   str.Format("%d",t.wHour);
	   hour=atoi(str);
	   str=str+" ʱ";
	   ::SetWindowText(::GetDlgItem(mainhwnd,IDC_STATIC1),str);
	   str.Format("%d",t.wMinute);
	   minute=atoi(str);
	   str=str+" ��";
	   ::SetWindowText(::GetDlgItem(mainhwnd,IDC_STATIC2),str);
	   str.Format("%d",t.wSecond);
	   second=atoi(str);
	   str=str+" ��";
	   ::SetWindowText(::GetDlgItem(mainhwnd,IDC_STATIC3),str);
		EnumChildWindows(NULL,EnumChildProc,NULL);
		EnumChildWindows(m_yjchwnd,EnumChildProc1,NULL);
		break;
	case 5:
		KillTimer(5);
		if(mm_false)
		{

		::PostMessage(m_mbhwnd,WM_KEYDOWN,112,NULL);//f1��
		
			//
			//::SetForegroundWindow(m_mbhwnd);
			//::MessageBox(NULL,str_zqdm,str_zqdm,NULL);
			::SetWindowText(::GetDlgItem(mainhwnd,IDC_STATIC_jb),str_zqdm);
		KeySend(str_zqdm,false);
		Sleep(1500);
		EnumChildWindows(m_mbhwnd,EnumChildProc2,NULL);
		//EnumChildWindows(m_mbhwnd,EnumChildProcGet,NULL);//�õ���ͣ��
		
		}
		else
		{
			::PostMessage(m_mbhwnd,WM_KEYDOWN,113,NULL);//f2��

	//		::SetForegroundWindow(m_mbhwnd);
			//	Sleep(500);
				::SetWindowText(::GetDlgItem(mainhwnd,IDC_STATIC_jb),str_zqdm);
			KeySend(str_zqdm,false);
			Sleep(1500);
			EnumChildWindows(m_mbhwnd,EnumChildProc2,NULL);
		}
		break;
	case 6:
		KillTimer(6);
		if(mm_false)//Ϊtrue��  Ϊfalse��
		{
			dqj=atof(str_dqj);

		/*	if(dqj>=22) str_mmsl1="100";
			if(dqj<22 && dqj>=12) str_mmsl1="200";
			if(dqj<12 && dqj>=8) str_mmsl1="300";
			if(dqj<8 && dqj>=6) str_mmsl1="400";
			if(dqj<6) str_mmsl1="500";*/

			dqj=dqj*1.015;
			str_dqj.Format("%f",dqj);
			str_dqj=str_dqj;
			::SetForegroundWindow(m_mbhwnd);
			Sleep(500);
			//KeySend(str_ztj,true);//����ͣ����
			KeySend(str_dqj,true);//����ڵ�ǰ��%2��

			::GetWindowText(::GetDlgItem(mainhwnd,IDC_EDIT3),buff,sizeof(buff));
			Sleep(1500);
			EnumChildWindows(m_mbhwnd,EnumChildProc4,NULL);
		
		}
		else
		{
			dqj=atof(str_dqj);

			/*if(dqj>=22) str_mmsl1="100";
			if(dqj<22 && dqj>=12) str_mmsl1="200";
			if(dqj<12 && dqj>=8) str_mmsl1="300";
			if(dqj<8 && dqj>=6) str_mmsl1="400";
			if(dqj<6) str_mmsl1="500";*/
		//	str_mmsl1="600";

			dqj=dqj*0.985;
			str_dqj.Format("%f",dqj);
			str_dqj=str_dqj;
			::SetForegroundWindow(m_mbhwnd);
			Sleep(1500);
			KeySend(str_dqj,true);//����ڵ�ǰ��%2��
			::GetWindowText(::GetDlgItem(mainhwnd,IDC_EDIT3),buff,sizeof(buff));
			Sleep(1500);
			EnumChildWindows(m_mbhwnd,EnumChildProc4,NULL);
			
		}
	case 7:
		KillTimer(7);
		if(mm_false)
		{
			
			
			::SetForegroundWindow(m_mbhwnd);
			KeySend(str_mmsl1,true);
			Sleep (200);
			SetTimer(2,1200,NULL);
		}
		else
		{
			::SetForegroundWindow(m_mbhwnd);
			KeySend(str_mmsl1,true);
			Sleep (200);
			SetTimer(2,1200,NULL);
		}
	/*	::PostMessage(m_mbhwnd,WM_KEYDOWN,113,NULL);//f2��
		KeySend(str_zqdm,false);
		Sleep(2500);
		//EnumChildWindows(m_mbhwnd,EnumChildProcGet,NULL);//�õ���ͣ��
		dqj=atof(str_dqj);
					dqj=dqj*0.985;
					str_dqj.Format("%f",dqj);
					str_dqj=str_dqj;


		::PostMessage(m_mbhwnd,WM_KEYDOWN,113,NULL);//f2��
		::SetForegroundWindow(m_mbhwnd);
		Sleep(2500);
		KeySend(str_zqdm,false);
		::SetForegroundWindow(m_mbhwnd);
		Sleep(2500);
		KeySend(str_dtj,true);//���ͣ����
		::GetWindowText(::GetDlgItem(mainhwnd,IDC_EDIT3),buff,sizeof(buff));
		str_mmsl1="100";
		Sleep (1500);
		::SetForegroundWindow(m_mbhwnd);
		KeySend(str_mmsl1,true);
		Sleep (200);
		SetTimer(2,2200,NULL);*/
		break;
	case 8:
		KillTimer(8);
		EnumChildWindows(m_mbhwnd,EnumChildProc2,NULL);
		break;
	case 9:
		KillTimer(9);
		system("shutdown /s /t 0"); 
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMyDlg::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CMyDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CMyDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
/*	::PostMessage(m_mbhwnd,WM_KEYDOWN,113,NULL);//f2
	str_zqdm=GetHq(IDC_EDIT1);
	str_mmsl1=GetHq(IDC_EDIT3);
	KeySend(str_zqdm,false);
	Sleep(500);
	jd_false=true;
	EnumChildWindows(m_mbhwnd,EnumChildProcGet,NULL);
	EnumChildWindows(m_mbhwnd,EnumChildProc3,NULL);*/
	/*  SYSTEMTIME   t;   
	  ::GetLocalTime(&t);   
	  CString   str;   
	  str.Format("%d:%d:%d",t.wHour,t.wMinute,t.wSecond);
	  //str.Format("%d:%d:%d:%d:%d:%d:",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond);
	  MessageBox(str);*/
	
	SetTimer(4,1000,NULL);
//	FileWrite();
}




CString CMyDlg::GetHq(int idc)
{
	CString str1;
	char buff[1024];
//	

		double f;
	//GetDlgItem(GetActiveWindow(),IDC_EDIT2);mainhwnd
	//::GetWindowText(::GetDlgItem(::GetActiveWindow(),idc),buff,sizeof(buff));
	::GetWindowText(::GetDlgItem(mainhwnd,idc),buff,sizeof(buff));
	str1=buff;
	
//	f=atof(buff);
	return str1;
}
void KeySend(CString str,  bool Enter )
{
	int len,i;
	CString tempstr;
	len=str.GetLength();
	SetForegroundWindow(m_mbhwnd);
	Sleep(500);
	for(i=0;i<len;i++)
	{
		tempstr=str.Mid(i,1);
		if(tempstr==".")
			tempstr="10";
		switch(atoi(tempstr))
		{
		case 0:
			::keybd_event(48,NULL,0,NULL);
			::keybd_event(48,NULL,2,NULL);
			break;
		case 1:
			::keybd_event(49,NULL,0,NULL);
			::keybd_event(49,NULL,2,NULL);
			break;
		case 2:
			::keybd_event(50,NULL,0,NULL);
			::keybd_event(50,NULL,2,NULL);
			break;
		case 3:
			::keybd_event(51,NULL,0,NULL);
			::keybd_event(51,NULL,2,NULL);
			break;
		case 4:
			::keybd_event(52,NULL,0,NULL);
			::keybd_event(52,NULL,2,NULL);
			break;
		case 5:
			::keybd_event(53,NULL,0,NULL);
			::keybd_event(53,NULL,2,NULL);
			break;
		case 6:
			::keybd_event(54,NULL,0,NULL);
			::keybd_event(54,NULL,2,NULL);
			break;
		case 7:
			::keybd_event(55,NULL,0,NULL);
			::keybd_event(55,NULL,2,NULL);
			break;
		case 8:
			::keybd_event(56,NULL,0,NULL);
			::keybd_event(56,NULL,2,NULL);
			break;
		case 9:
			::keybd_event(57,NULL,0,NULL);
			::keybd_event(57,NULL,2,NULL);
			break;
		case 10:
			::keybd_event(110,NULL,0,NULL);
			::keybd_event(110,NULL,2,NULL);
			break;
		}
		
	}
	if(Enter)//�س�
		{
			::keybd_event(13,NULL,0,NULL);
			::keybd_event(13,NULL,2,NULL);
		}
}
void FileWrite(CString gpdm)
{
	CString gpdmTemp;
	gpdmTemp=gpdm+"\n";
	CString strtemp=GetModulePath();
	strtemp=strtemp+"\\ok.txt";
	
 FILE   *   fp   ;   
  fp   =   fopen(strtemp,"a");   
  fputs((LPCTSTR)gpdmTemp,fp);   
  fclose(fp);   


}
void FileRead()
{
		CString strtemp=GetModulePath();
	strtemp=strtemp+"\\ok.txt";
	CString   csline,csall;   
  CStdioFile   file;   
  file.Open(strtemp,   CFile::modeRead,NULL);   
 // while(file.ReadString(csline)!=NULL)   
 // {   
  if(file.ReadString(csline)!=NULL)
  {
	  
        csall   =   csline   +"\r\n";   //
		csline.TrimRight(csline);       //�õ�֤ȯ����
		csline.TrimLeft(csline);		//
		str_zqdm=csall.Mid(0,6);		//


	str_dqj=csall.Mid(csall.Find(" "));
		str_dqj.TrimRight(str_dqj);
		str_dqj.TrimLeft(str_dqj);
		str_dqj=str_dqj.Mid(str_dqj.Find(" ")+2);
		str_dqj.TrimRight(str_dqj);
		//str_dqj.TrimLeft(str_dqj);
		str_mmsl1=str_dqj.Mid(0,str_dqj.Find(" "));//�õ�����
		//str_dqj.TrimRight(str_dqj);
		//str_dqj.TrimLeft(str_dqj);
		str_dqj.TrimRight(str_dqj);
	//	str_dqj.TrimLeft(str_dqj);
		str_dqj=str_dqj.Mid(str_dqj.Find(" "));
		while(!str_dqj.Find(" "))
		{
				//
	
			str_dqj=str_dqj.Mid(1);
		}
	
str_dqj=str_dqj.Mid(0,str_dqj.Find(" "));//�õ���ǰ��
		csline =csall.Mid(1,1);
		 file.Close(); 
		if (csline!="")

		{
			CFile filetemp(strtemp,CFile::modeRead);//����CFile����
			char FileContent[1024];
			  memset(FileContent,0,1024);//��ʼ��FileContent
			  filetemp.Read(FileContent,1024);//��������
			  filetemp.Close();//�ر��ļ�����
			  csline=FileContent;
			  csline=csline.Mid(csline.Find("\n")+1);
			  CFile file((LPCTSTR)strtemp,CFile::modeCreate| CFile::modeWrite);//����CFile����
			  file.Write(csline,strlen(csline));//д�����ݵ��ļ�
			  file.Close();//�ر�CFile����
			csline=csall.Right(4);
			csline.TrimRight(csline);
			csline.TrimLeft(csline);
			//::MessageBox(NULL,csline,"",NULL);
			if(csline=="��")
			{
				
				SetForegroundWindow(m_mbhwnd);
				
			  SetTimer(mainhwnd,5,1500,NULL);
				mm_false=true;
			//::MessageBox(NULL,FileContent,"ok",NULL);//��ʾ  
			}
			else
			{
				if(csline=="��")
				{
					
					SetForegroundWindow(m_mbhwnd);
				//	Sleep(500);
				  SetTimer(mainhwnd,5,1500,NULL);
				  mm_false=false;
				//::MessageBox(NULL,FileContent,"ok",NULL);//��ʾ  
				}
				else 
				{
					
						FileRead();
						
				}
			}

		}
	}  
  else
  {
	SetTimer(mainhwnd,4,1000,NULL);
  }
    
  
}
void CreateFile()//����һ���ļ�
{
	CString strtemp=GetModulePath();
	strtemp=strtemp+"\\ok.txt";
/*CFile file((LPCTSTR)strtemp,CFile::modeCreate| CFile::modeWrite);//����CFile����
 // file.Write("Welcome to VCFans !",strlen("Welcome to VCFans !"));//д�����ݵ��ļ�
  file.Close();//�ر�CFile����*/
	HANDLE file=CreateFile((LPCTSTR)strtemp, GENERIC_WRITE,FILE_SHARE_READ ,NULL,CREATE_ALWAYS,NULL,NULL);
	::CloseHandle(file); 


}
CString GetModulePath()
{
  CString strModulePath;
  GetModuleFileName(NULL, strModulePath.GetBuffer(MAX_PATH), MAX_PATH);
 //ע��ʹ�ø�API�����õ����ǳ����ļ�����·���ļ�����

 //ȥ���ļ��������·����
 strModulePath.ReleaseBuffer();
 int intpoint = strModulePath.ReverseFind('\\');
 strModulePath = strModulePath.Left(intpoint);
 TCHAR ch = strModulePath.GetAt(intpoint - 1);
 if(ch != '\\') // '\\'����ǰ���\Ϊת���ַ�������Ч��
 {
  strModulePath += "\\";// '\\'����ǰ���\Ϊת���ַ�������Ч��
 }
  

return strModulePath;
}


void CMyDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	//FileRead();
	//CreateFile();
	EnumChildWindows(m_mbhwnd,EnumChildProc2,NULL);
}
