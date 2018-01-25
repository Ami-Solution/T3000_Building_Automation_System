// ParameterExtDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "T3000.h"
#include "ParameterExtDlg.h"
#include "afxdialogex.h"
#include "global_function.h"

// CParameterExtDlg �Ի���

IMPLEMENT_DYNAMIC(CParameterExtDlg, CDialogEx)

CParameterExtDlg::CParameterExtDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PARAMETERDIALOG_EXT, pParent)
{

}

CParameterExtDlg::~CParameterExtDlg()
{
}

void CParameterExtDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PARA_EXT_DELAY, m_edit_delay_time);
    DDX_Control(pDX, IDC_EDIT_PARA_EXT_TIME_REMAINING, m_edit_time_remaining);
    DDX_Control(pDX, IDC_EDIT_PARA_EXT_MIN_PWM, m_edit_min_pwm);
}


BEGIN_MESSAGE_MAP(CParameterExtDlg, CDialogEx)
    ON_EN_KILLFOCUS(IDC_EDIT_PARA_EXT_DELAY, &CParameterExtDlg::OnEnKillfocusEditParaExtDelay)
    ON_EN_KILLFOCUS(IDC_EDIT_PARA_EXT_MIN_PWM, &CParameterExtDlg::OnEnKillfocusEditParaExtMinPwm)
    ON_BN_CLICKED(IDC_BUTTON_PARA_EXT_OK, &CParameterExtDlg::OnBnClickedButtonParaExtOk)
END_MESSAGE_MAP()


// CParameterExtDlg ��Ϣ�������


BOOL CParameterExtDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CString temp_delay_time;
    CString temp_time_left;
    CString temp_min_pwm;

    temp_delay_time.Format(_T("%d"), product_register_value[801]);
    temp_time_left.Format(_T("%d"),  product_register_value[802]);
    temp_min_pwm.Format(_T("%d"),    product_register_value[803]);

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_edit_delay_time.SetWindowTextW(temp_delay_time);
    m_edit_delay_time.textColor(RGB(0, 0, 0));
    //m_edit_delay_time.bkColor(RGB(0,255,255));
    m_edit_delay_time.setFont(18, 12, NULL, _T("Arial"));

    m_edit_time_remaining.SetWindowTextW(temp_time_left);
    m_edit_time_remaining.textColor(RGB(0, 0, 0));
    //m_edit_time_remaining.bkColor(RGB(0,255,255));
    m_edit_time_remaining.setFont(18, 12, NULL, _T("Arial"));

    m_edit_min_pwm.SetWindowTextW(temp_min_pwm);
    m_edit_min_pwm.textColor(RGB(0, 0, 0));
    //m_edit_min_pwm.bkColor(RGB(0,255,255));
    m_edit_min_pwm.setFont(18, 12, NULL, _T("Arial"));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CParameterExtDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���

    return CDialogEx::PreTranslateMessage(pMsg);
}


void CParameterExtDlg::OnEnKillfocusEditParaExtDelay()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (g_ParamLevel == 1)
        return;

    CString strText;
    m_edit_delay_time.GetWindowText(strText);
    int nValue = _wtoi(strText);
    if (nValue < 0 || nValue > 255)
    {
        AfxMessageBox(_T("Heating Cooling  To  Coasting  Delay Time Must between 0 and 255"));
        return;
    }

    if ((short)product_register_value[801] == nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, 801, nValue,
        (short)product_register_value[801], this->m_hWnd, IDC_ESETPOINTHI, _T("Heating Cooling  To  Coasting  Delay Time"));

}





void CParameterExtDlg::OnEnKillfocusEditParaExtMinPwm()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (g_ParamLevel == 1)
        return;

    CString strText;
    m_edit_min_pwm.GetWindowText(strText);
    int nValue = _wtoi(strText);
    if (nValue < 0 || nValue > 100)
    {
        AfxMessageBox(_T("Heating Cooling  To  Coasting   Minimum PWM Must between 0 and 100"));
        return;
    }

    if ((short)product_register_value[803] == nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, 803, nValue,
        (short)product_register_value[803], this->m_hWnd, IDC_ESETPOINTHI, _T("Heating Cooling  To  Coasting  Delay Time"));

}


void CParameterExtDlg::OnBnClickedButtonParaExtOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    PostMessage(WM_CLOSE, NULL, NULL);
}
