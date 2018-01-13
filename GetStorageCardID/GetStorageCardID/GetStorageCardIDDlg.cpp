// GetStorageCardIDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GetStorageCardID.h"
#include "GetStorageCardIDDlg.h"
#include "CeGetStorageCardID.h"
#include "afx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGetStorageCardIDDlg 对话框

CGetStorageCardIDDlg::CGetStorageCardIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetStorageCardIDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetStorageCardIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGetStorageCardIDDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON, &CGetStorageCardIDDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON1, &CGetStorageCardIDDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CGetStorageCardIDDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CGetStorageCardIDDlg 消息处理程序

BOOL CGetStorageCardIDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetWindowPos(NULL,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),0);

	// TODO: 在此添加额外的初始化代码
	CFont *m_Font_Tip;
    m_Font_Tip = new CFont; 
    m_Font_Tip->CreateFont(30,12,0,0,100,
    FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("宋体"));

	m_EditInfo1 = (CEdit *)GetDlgItem(IDC_EDIT1);
    m_EditInfo1->SetFont(m_Font_Tip,FALSE);
    m_EditInfo1->SetFont(m_Font_Tip);
	m_EditInfo1->EnableWindow(FALSE);

	CFont *m_Font_Btn;
    m_Font_Btn = new CFont; 
    m_Font_Btn->CreateFont(25,10,0,0,100,
    FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("宋体"));

	m_BtnGetID = (CButton *)GetDlgItem(IDC_BUTTON);
	m_BtnGetID->SetFont(m_Font_Btn);

	m_BtnQuit = (CButton *)GetDlgItem(IDC_BUTTON1);
	m_BtnQuit->SetFont(m_Font_Btn);

	m_BtnActivate = (CButton *)GetDlgItem(IDC_BUTTON2);
	m_BtnActivate->SetFont(m_Font_Btn);

	SetTimer(IDTIMER1,200,0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CGetStorageCardIDDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_GETSTORAGECARDID_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_GETSTORAGECARDID_DIALOG));
	}
}
#endif

void CGetStorageCardIDDlg::OnBnClickedButton()
{
	// TODO: Add your control notification handler code here
	m_hThreadMonitor = CreateThread(NULL,NULL,MonitorThreadPro,this,NULL,NULL);
}

DWORD CGetStorageCardIDDlg::MonitorThreadPro(LPVOID lpParam)
{
	DWORD ret;

	CGetStorageCardIDDlg* GSDDeal = (CGetStorageCardIDDlg*)lpParam;
	
	ret= GSDDeal->MonitorThreadProDeal();

	return ret;
}

DWORD CGetStorageCardIDDlg::MonitorThreadProDeal()
{
	if(!IsDirectorExist(SDCARD_PATH))
	{
	#if(LANGUAGE == ENGLISH)
			m_EditInfo1->SetWindowText(_T("No SD card detected, insert SD card and click again to get ID"));
	#elif(LANGUAGE == SIM_CHINESE)
			m_EditInfo1->SetWindowText(_T("未检测到SD卡，请插入SD卡,重新点击获取ID"));
	#endif
		return -1;
	}

	BYTE *write_str = GetStroageID(_T("DSK8:")); //First StorageCard
	CString show_str(write_str);

	if(write_str == NULL || show_str.GetLength() != 10)
	{
		#if(LANGUAGE == ENGLISH)
			m_EditInfo1->SetWindowText(_T("Read SD card ID failed, please read again"));
		#elif(LANGUAGE == SIM_CHINESE)
			m_EditInfo1->SetWindowText(_T("读取SD卡ID失败，请重新读取"));
		#endif

		return -1;
	}

	if(!IsDirectorExist(USB_PATH))
	{
		#if(LANGUAGE == ENGLISH)
			m_EditInfo1->SetWindowText(_T("No USB detected, please insert USB, click again to get ID"));
		#elif(LANGUAGE == SIM_CHINESE)
			m_EditInfo1->SetWindowText(_T("未检测到USB，请插入USB，重新点击获取ID"));
		#endif

		return -1;
	}
//===================================================================//
	CFile file_id_local;

	file_id_local.Open(SDCARD_ID_FILE, CFile::modeCreate | CFile::modeReadWrite, NULL);
	
	file_id_local.Write(write_str, show_str.GetLength());  
	RETAILMSG(1, (_T("write_str=%s\r\n"), show_str));

	file_id_local.Close();

//=====================================================================//
	CFile file_id_all;

	//打开文件，如果不存在则创建，存在则打开不清除
	file_id_all.Open(SDCARD_ID_ALL, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite, NULL);

	//定位到文件尾
	file_id_all.SeekToEnd();

	//写入ID
	file_id_all.Write(write_str, show_str.GetLength());  

	//换行
	file_id_all.Write(_T("\r"), 1); 

	file_id_all.Close();

	CString str=_T("SD卡ID:" + show_str);
	
	str += _T("\r\nSDCARD ID保存成功,请换下一张卡读取");

#if(LANGUAGE == ENGLISH)
	str += _T("\r\nSDCARD ID Save success, please replace a card to read");
#elif(LANGUAGE == SIM_CHINESE)
	str += _T("\r\nSDCARD ID保存成功,请换下一张卡读取");
#endif

	m_EditInfo1->SetWindowText(str);


	return 0;
}

void CGetStorageCardIDDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CloseHandle(m_hThreadMonitor);
	m_hThreadMonitor = INVALID_HANDLE_VALUE;

	CloseHandle(m_hThreadActMonitor);
	m_hThreadActMonitor = INVALID_HANDLE_VALUE;


	KillTimer(IDTIMER1);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

void CGetStorageCardIDDlg::OnTimer(UINT_PTR nIDEvent)
{
	static bool card_exist_tips = TRUE;
	static bool card_not_exist_tips = TRUE;

	switch(nIDEvent) {  
    case IDTIMER1:  
        {     
			if(!IsDirectorExist(SDCARD_PATH))
			{
				if(card_exist_tips)
				{
					card_exist_tips = FALSE;
					card_not_exist_tips = TRUE;

				#if(LANGUAGE == ENGLISH)
					m_EditInfo1->SetWindowText(_T("No SD card detected, insert SD card"));
				#elif(LANGUAGE == SIM_CHINESE)
					m_EditInfo1->SetWindowText(_T("未检测到SD卡，请插入SD卡"));
				#endif
					m_BtnGetID->EnableWindow(FALSE);
					m_BtnActivate->EnableWindow(FALSE);
				}
			}
			else
			{
				if(card_not_exist_tips)
				{
					card_exist_tips = TRUE;
					card_not_exist_tips = FALSE;
				#if(LANGUAGE == ENGLISH)
					m_EditInfo1->SetWindowText(_T("SD card detected"));
				#elif(LANGUAGE == SIM_CHINESE)
					m_EditInfo1->SetWindowText(_T("检测到SD卡"));
				#endif
					m_BtnGetID->EnableWindow(TRUE);
					m_BtnActivate->EnableWindow(TRUE);
				}
			}
			
            break;  
        }
    default:  
        ;  
    }  
}

void CGetStorageCardIDDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	m_hThreadActMonitor = CreateThread(NULL,NULL,ActMonitorThreadPro,this,NULL,NULL);

}

DWORD CGetStorageCardIDDlg::ActMonitorThreadPro(LPVOID lpParam)
{
	DWORD ret;

	CGetStorageCardIDDlg* ActDeal = (CGetStorageCardIDDlg*)lpParam;
	
	ret= ActDeal->ActMonitorThreadProDeal();

	return ret;
}

DWORD CGetStorageCardIDDlg::ActMonitorThreadProDeal()
{
	//检测是否插入SD卡
	if(!IsDirectorExist(SDCARD_PATH))
	{
	#if(LANGUAGE == ENGLISH)
		m_EditInfo1->SetWindowText(_T("No SD card detected, insert SD card and click again to activate the map"));
	#elif(LANGUAGE == SIM_CHINESE)
		m_EditInfo1->SetWindowText(_T("未检测到SD卡，请插入SD卡,重新点击激活地图"));
	#endif
		return -1;
	}

	//获取SD卡ID
	BYTE *id = GetStroageID(_T("DSK8:")); //First StorageCard
	CString id_str(id);

	//检测是否有地图文件
	if(!IsDirectorExist(SDCARD_MAP_PATH))
	{
	#if(LANGUAGE == ENGLISH)
		m_EditInfo1->SetWindowText(_T("Not detected on the SD card\r\nPlease confirm whether the SD card has a map\r\nand then click again to activate the map"));
	#elif(LANGUAGE == SIM_CHINESE)
		m_EditInfo1->SetWindowText(_T("未在SD卡检测到地图\r\n请确认SD卡是否有地图\r\n然后重新点击激活地图"));
	#endif
		return -1;
	}

	//检测地图是否激活
	if(FindSDCardID(id_str, SDCARD_MAP_LICENSE))
	{
	#if(LANGUAGE == ENGLISH)
		m_EditInfo1->SetWindowText(_T("The map is activated, do not activate it repeatedly"));
	#elif(LANGUAGE == SIM_CHINESE)
		m_EditInfo1->SetWindowText(_T("地图已激活，请勿重复激活"));
	#endif
		return -1;
	}

	//检测是否插入USB
	if(!IsDirectorExist(USB_PATH))
	{
	#if(LANGUAGE == ENGLISH)
		m_EditInfo1->SetWindowText(_T("No USB detected, please insert USB and click again to activate the map"));
	#elif(LANGUAGE == SIM_CHINESE)
		m_EditInfo1->SetWindowText(_T("未检测到USB，请插入USB，重新点击激活地图"));
	#endif
		
		return -1;
	}

	//检测USB是否有对应的license,并拷贝
	if(!CopyLincese(id_str, USB_MAP_LICENSE, SDCARD_MAP_LICENSE))
	{	
		return -1;
	}

#if(LANGUAGE == ENGLISH)
	m_EditInfo1->SetWindowText(_T("Certificate copy is successful ID："+id_str));
#elif(LANGUAGE == SIM_CHINESE)
	m_EditInfo1->SetWindowText(_T("证书拷贝成功 ID："+id_str));
#endif

	return 0;
}

DWORD CGetStorageCardIDDlg::FindSDCardID(CString id, const LPCTSTR FindPath)
{
	CString szFindDir=FindPath;
	CString szExistDir;

	if(szFindDir.Right(1)!="\\")
	{
		szFindDir += "\\";
	}

	szFindDir+="*.*"; //搜索所有文件
	
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	hFind=FindFirstFile(szFindDir,&fd); //寻找第一个文件

	if(hFind!=INVALID_HANDLE_VALUE)
	{
		do
		{   	
			CString id_tmp = fd.cFileName;
			//截取license前面10个字符的ID

			id_tmp = id_tmp.Left(10);

			RETAILMSG(1, (_T("%s finding file :%s, id=%s\r\n"), _T(__FUNCTION__), fd.cFileName, id_tmp));
			
			if(!wcscmp(id , id_tmp))
			{
				RETAILMSG(1, (_T("%s, find license %s\r\n"), _T(__FUNCTION__), fd.cFileName));
				return TRUE;
			}
		}
		while(FindNextFile(hFind,&fd)); //查找是否存在下一个文件
	}
	else
	{
		//源文件夹为空,返回
		RETAILMSG(1, (_T("%s no find license\r\n"), _T(__FUNCTION__)));
		return FALSE;
	}
	
	RETAILMSG(1, (_T("%s no find license\r\n"), _T(__FUNCTION__)));
	return FALSE;
}

DWORD CGetStorageCardIDDlg::CopyLincese(CString id, const LPCTSTR FromDir, const LPCTSTR ToDir)
{
	CString szExistDir;
	
	CString szAimDir = ToDir; //保存目标文件夹路径

	CString szFindDir = FromDir; //保存源文件夹路径

	if(szFindDir.Right(1)!="\\")
	{
		szFindDir+="\\";
		szExistDir=szFindDir;
	}
	szFindDir+="*.*"; //搜索所有文件
	
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	hFind=FindFirstFile(szFindDir,&fd); //寻找第一个文件

	if(hFind!=INVALID_HANDLE_VALUE)
	{
		do
		{   	
			if(szAimDir.Right(1)!="\\")
			{
				szAimDir+="\\";
			}

			CString id_tmp = fd.cFileName;
			//截取license前面10个字符的ID

			id_tmp = id_tmp.Left(10);

			RETAILMSG(1, (_T("%s finding file :%s, id=%s\r\n"), _T(__FUNCTION__), fd.cFileName, id_tmp));
			
			if(!wcscmp(id , id_tmp))
			{
				RETAILMSG(1, (_T("%s find license %s\r\n"), _T(__FUNCTION__), fd.cFileName));

				if(CopyFile(szExistDir+fd.cFileName, szAimDir+fd.cFileName, FALSE)==FALSE) //拷贝文件到目标文件夹
				{
					RETAILMSG(1, (_T("copy license fail %s\r\n"), fd.cFileName));
				#if(LANGUAGE == ENGLISH)
					m_EditInfo1->SetWindowText(_T("Copy the certificate error, click again to activate the map"));
				#elif(LANGUAGE == SIM_CHINESE)
					m_EditInfo1->SetWindowText(_T("拷贝证书出错,重新点击激活地图"));
				#endif
			
					return FALSE;
				}

				RETAILMSG(1, (_T("copy license success %s\r\n"), fd.cFileName));
				return TRUE;
			}
		}
		while(FindNextFile(hFind,&fd)); //查找是否存在下一个文件
	}
	else
	{
		//源文件夹为空,返回
		CString str = _T("SDCARD ID:" + id);
	#if(LANGUAGE == ENGLISH)
		str += _T("\r\nNot found in the USB ID corresponding to the certificate \r\nPlease confirm the USB license folder has a corresponding certificate");
	#elif(LANGUAGE == SIM_CHINESE)
		str += _T("\r\n未在USB中找到对应ID的证书\r\n请确认USB license文件夹中有对应的证书");
	#endif
		m_EditInfo1->SetWindowText(str);
		RETAILMSG(1, (_T("%s no find license\r\n"), _T(__FUNCTION__)));
		return FALSE;
	}

	CString str = _T("SDCARD ID:" + id);
#if(LANGUAGE == ENGLISH)
	str += _T("\r\nNot found in the USB ID corresponding to the certificate \r\nPlease confirm the USB license folder has a corresponding certificate");
#elif(LANGUAGE == SIM_CHINESE)
	str += _T("\r\n未在USB中找到对应ID的证书\r\n请确认USB license文件夹中有对应的证书");
#endif
	m_EditInfo1->SetWindowText(str);
	RETAILMSG(1, (_T("%s no find license\r\n"), _T(__FUNCTION__)));

	return FALSE;
}

BOOL CGetStorageCardIDDlg::IsDirectorExist(const LPCTSTR lpFilePath)   
{  
	if( GetFileAttributes(lpFilePath) == 0xFFFFFFFF)
		return FALSE;

	return TRUE;
}