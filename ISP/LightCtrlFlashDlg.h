#pragma once
#include "afxcmn.h"
#include "TFTPServer.h"
#include "ConfigFileHandler.h"
#include "ComWriter.h"
#include "FlashBase.h"
// CLightCtrlFlashDlg dialog

class CTCPFlasher;

class CLightCtrlFlashDlg : public CDialog, public CFlashBase
{
	DECLARE_DYNAMIC(CLightCtrlFlashDlg)

public:
	CLightCtrlFlashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightCtrlFlashDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LIGHTCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioMain();
	afx_msg void OnBnClickedRadioInput();
	afx_msg void OnBnClickedOutput();
	afx_msg LRESULT OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFlashFinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddStatusInfo(WPARAM wParam, LPARAM lParam);
	
	virtual BOOL OnInitDialog();
public:
	int GetFlashType();
	void UpdateStatusInfo(const CString& strInfo, BOOL bReplace);
	void InitFromConfigFile();	
	void FlashByEthernet();
	DWORD GetIPAddress();
	CString GetIPAddressStr();

	void SaveToConfigFile(CConfigFileHandler& cfgFileHandler);
	BOOL Flash();
	


	CIPAddressCtrl		m_IPAddrLC;
protected:
	CString GetFlashTypeStr();
	BOOL FileValidation(const CString& strFileName);

	void FlashMainBoard();
	void OnFlashInputBoard();
	void OnFlashOutputBoard();


	BOOL ValidMdbIDString();
	int GetModbusID(vector<int>& szMdbIDs);

protected:

	int						m_nFlashLCType;			// 0=main board, 1=input board, 2=output board
	//CString				m_strFlashFileName;		// 


//	CTCPFlasher*		m_pTCPFlasher;
	TFTPServer*		m_pTFTPServer;				// ʹ�����磬TFTPЭ��flash��ʹ��ʱʵ����

	char*					m_pFileBuf;						// �ļ���������������Ŷ�ȡ���ļ������ݣ�flash���Ӧ��delete��ʹ��ʱ��new

	CComWriter*		m_pTCPFlasher;

	vector<int>			m_szMdbIDs;
};
