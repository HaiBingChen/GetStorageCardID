// GetStorageCardIDDlg.h : 头文件
//

#pragma once

// CGetStorageCardIDDlg 对话框
class CGetStorageCardIDDlg : public CDialog
{
// 构造
public:
	CGetStorageCardIDDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GETSTORAGECARDID_DIALOG };
	DWORD MonitorThreadProDeal();
	DWORD ActMonitorThreadProDeal();
	BOOL IsDirectorExist(const LPCTSTR lpFilePath); 
	DWORD FindSDCardID(CString id, const LPCTSTR FindPath);
	DWORD CopyLincese(CString id, const LPCTSTR FromDir, const LPCTSTR ToDir);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()

	static DWORD MonitorThreadPro(LPVOID lpParam);
	static DWORD ActMonitorThreadPro(LPVOID lpParam);
public:
	afx_msg void OnBnClickedButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	afx_msg void OnBnClickedButton1();

private:
	HANDLE	m_hThreadMonitor;
	HANDLE	m_hThreadActMonitor;
	CEdit	*m_EditInfo1;
	CButton *m_BtnGetID;
	CButton *m_BtnActivate;
	CButton *m_BtnQuit;
public:
	afx_msg void OnBnClickedButton2();
};
