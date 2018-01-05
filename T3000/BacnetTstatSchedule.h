#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"

// CBacnetTstatSchedule �Ի���

class CBacnetTstatSchedule : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetTstatSchedule)

public:
	CBacnetTstatSchedule(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBacnetTstatSchedule();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_TSTAT_SCHEDULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ListCtrlEx::CListCtrlEx m_bac_tstat_sch_list;

	void Initial_List();
	afx_msg LRESULT Fresh_TSTAT_Schedule_List(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_TSTAT_Schedule_Item(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMClickListBacTstatSchedule(NMHDR *pNMHDR, LRESULT *pResult);
};
