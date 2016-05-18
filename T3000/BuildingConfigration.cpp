// BuildingConfigration.cpp : implementation file
//
#include "stdafx.h"
#include "T3000.h"
#include "BuildingConfigration.h"
#include "afxdialogex.h"
#include "ado/ADO.h"
#include "bado/BADO.h"
#include <iostream>
#include <windows.h>
#include <io.h>
#include "FileOperations.h"
#include "BuildingConfigEditDlg.h"
#include "SqliteLib/CppSQLite3.h"
#include "globle_function.h"

#include <ctime>
#include <iostream>
using namespace std;
// CBuildingConfigration dialog
#define WM_FRESH_DB  WM_USER + 1014
#define PATH_MAX 256
IMPLEMENT_DYNAMIC(CBuildingConfigration, CDialogEx)


const int INDEX_MODBUS_485 = 0;
const int INDEX_MODBUS_TCP = 1;
const int INDEX_BACNET_MSTP = 2;
const int INDEX_REMOTE_DEVICE = 3;
const int INDEX_AUTO = 4;



CBuildingConfigration::CBuildingConfigration(CWnd* pParent /*=NULL*/)
    : CDialogEx(CBuildingConfigration::IDD, pParent)
{
    m_bChanged=FALSE;
	CString temp = GetExePath(true) + L"Psychrometry\\db_psychrometric_project.s3db";
	strcpy(m_sqlitepath,(CStringA)temp);
}

CBuildingConfigration::~CBuildingConfigration()
{
}

void CBuildingConfigration::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_BUILDING_CONFIG, m_building_config_list);
    DDX_Control(pDX, IDC_BUILDING_BUTTON_ADD, m_addBuildingButton);
    DDX_Control(pDX, IDC_BUILDING_BUTTON_DELETE, m_deleteBuildingButton);
}

BEGIN_MESSAGE_MAP(CBuildingConfigration, CDialogEx)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Building_Config_Item)
    ON_BN_CLICKED(IDC_BUILDING_BUTTON_ADD, &CBuildingConfigration::OnBnClickedBuildingButtonAdd)
    ON_BN_CLICKED(IDC_BUILDING_BUTTON_DELETE, &CBuildingConfigration::OnBnClickedBuildingButtonDelete)
    ON_WM_SIZE()
    ON_NOTIFY(NM_CLICK, IDC_LIST_BUILDING_CONFIG, &CBuildingConfigration::OnNMClickListBuildingConfig)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_BUILDING_CONFIG, &CBuildingConfigration::OnNMRClickListBuildingConfig)
    ON_COMMAND(ID_BUILDINGCONFIG_SELECT, &CBuildingConfigration::OnBuildingconfigSelect)
    ON_COMMAND(ID_BUILDINGCONFIG_UNSELECT, &CBuildingConfigration::OnBuildingconfigUnselect)
    ON_COMMAND(ID_BUILDINGCONFIG_DELETE, &CBuildingConfigration::OnBuildingconfigDelete)
    ON_BN_CLICKED(IDOK, &CBuildingConfigration::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BUILDING_CONFIG, &CBuildingConfigration::OnNMDblclkListBuildingConfig)
END_MESSAGE_MAP()


// CBuildingConfigration message handlers


BOOL CBuildingConfigration::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    Deal_BuildingPath();
    m_building_pCon.CreateInstance(_T("ADODB.Connection"));
    m_building_pRs.CreateInstance(_T("ADODB.Recordset"));
    m_building_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
    GetSerialComPortNumber1(m_szBuildingComs);

    m_building_config_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    //m_building_config_list.SetExtendedStyle(m_building_config_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    m_building_config_list.SetExtendedStyle(m_building_config_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_building_config_list.InsertColumn(BC_ITEM, _T(""), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_building_config_list.InsertColumn(BC_MAINNAME, _T("Building"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_PROTOCOL, _T("Protocol"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_IPADDRESS, _T("IP / Domain / Tel#"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_IPPORT, _T("IP Port"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_COMPORT, _T("COM Port"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_BAUDRATE, _T("Baud Rate"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_BUILDINGPATH, _T("Building Path"), 600, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

    m_building_config_hwnd = this->m_hWnd;
    g_hwnd_now = m_building_config_hwnd;


    CRect list_rect,win_rect;
    m_building_config_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_input_dlg_hwnd,win_rect);
    m_building_config_list.Set_My_WindowRect(win_rect);
    m_building_config_list.Set_My_ListRect(list_rect);


    Initial_Building_List();
    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBuildingConfigration::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
    {
        CRect list_rect,win_rect;
        m_building_config_list.GetWindowRect(list_rect);
        ScreenToClient(&list_rect);
        ::GetWindowRect(m_pragram_dlg_hwnd,win_rect);
        m_building_config_list.Set_My_WindowRect(win_rect);
        m_building_config_list.Set_My_ListRect(list_rect);

        m_building_config_list.Get_clicked_mouse_position();
        return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CBuildingConfigration::Initial_Building_List()
{

    CBADO bado;
    LoadBuildingConfigDB();
    TraverseFolder(g_strBuildingFolder,m_vecdbfile);
    for (int i=0; i<(int)m_vecdbfile.size(); i++)
    {
        int index=m_vecdbfile.at(i).Find(_T("Database"));
        m_vecdbfile.at(i).Delete(0,index);

    }
    Building_Config BCTemp;
    CStringArray ArrayFileName;
    BOOL Is_The_Same=TRUE;
    for (int i=0; i<m_vecdbfile.size(); i++)
    {
        for (int j=0; j<m_BuildNameLst.size(); j++)
        {
            if (m_vecdbfile.at(i).CompareNoCase(m_BuildNameLst.at(j).BuildingPath)==0)
            {
                Is_The_Same=TRUE;
                break;
            }
            else
            {
                Is_The_Same=FALSE;
            }
        }
        if (Is_The_Same)
        {
            continue;
        }
        //1.��������ǰĿ¼�£��µ�Building ���뵽���ݿ���
        //2.�����ݿ�����ָĵ��ˣ������ݿ��·�����ĵ�
        //3.��Building��Ŀ¼���ָĵ��ˣ�����һ���µ�Building
        CString sql;
        _variant_t temp_variant;
        CString Building_Name=L"";
        CString DB_Building_Name=L"";
        if (!Is_The_Same)
        {
            SplitCStringA(ArrayFileName,m_vecdbfile.at(i),L".");
            if (ArrayFileName[ArrayFileName.GetSize()-1].CompareNoCase(L"mdb")!=0)
            {
                continue;
            }
#if 1 //У�һ�¿����Ƿ���Building��Database
            bado.SetDBPath(GetExePath(true)+m_vecdbfile.at(i));
            bado.OnInitADOConn();
            if (!bado.IsHaveTable(bado,_T("ALL_NODE")))
            {

                bado.CloseConn();
                continue;
            }
            else
            {

                sql.Format(_T("Select  distinct mainbuilding_name  from  ALL_NODE"));
                bado.m_pRecordset=bado.OpenRecordset(sql);
                if(!bado.m_pRecordset->EndOfFile)
                {
                    temp_variant=bado.m_pRecordset->GetCollect(_T("MainBuilding_Name"));
                    if (temp_variant.vt!=VT_NULL)
                    {
                        DB_Building_Name=temp_variant;
                    }


                }
                bado.CloseRecordset();

            }
#endif
            SplitCStringA(ArrayFileName,m_vecdbfile.at(i),L"\\");
            if (ArrayFileName.GetSize()<2)
            {
                break;
            }

            Building_Name=ArrayFileName[ArrayFileName.GetSize()-2];
            if(DB_Building_Name.CompareNoCase(Building_Name)!=0)
            {
                sql.Format(_T("Select * from ALL_NODE"));
                bado.m_pRecordset=bado.OpenRecordset(sql);
                while(!bado.m_pRecordset->EndOfFile)
                {

                    sql.Format(_T("update ALL_NODE set MainBuilding_Name  = '%s' ,Building_Name = '%s'  where  MainBuilding_Name  = '%s'"),Building_Name,Building_Name,DB_Building_Name);
                    //	bado.OpenRecordset(sql);
                    bado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
                    bado.m_pRecordset->MoveNext();

                }
                bado.CloseRecordset();
            }
            bado.CloseConn();


            //}

            BCTemp.MainBuildingName=Building_Name;
            BCTemp.Sub_NetName=Building_Name;
            BCTemp.BuildingPath=m_vecdbfile.at(i);
            BCTemp.b_selected=FALSE;

            //����ALL_NODE ���������


            CADO ado;
            ado.OnInitADOConn();

            sql.Format(_T("Select *  from  Building  where  Main_BuildingName='%s' "),BCTemp.MainBuildingName);
            ado.m_pRecordset=ado.OpenRecordset(sql);
            if (ado.m_pRecordset->EndOfFile)//�б���û�ж�Ӧ���кŵ�ֵ
            {
                ado.CloseRecordset();
                for (int j=0; j<m_BuildNameLst.size(); j++)
                {
                    if (BCTemp.MainBuildingName.CompareNoCase(m_BuildNameLst.at(j).MainBuildingName)==0)
                    {
                        Is_The_Same=TRUE;
                        break;

                    }
                    else
                    {
                        Is_The_Same=FALSE;
                    }
                }
                if (!Is_The_Same)
                {
                    sql.Format(_T("delete * from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
                    ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                    sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
                    ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                    sql.Format(_T("Insert into Building(Main_BuildingName,Building_Name,Default_SubBuilding,Building_Path) values('%s','%s','%d','%s')"),BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),0,BCTemp.BuildingPath.GetBuffer());
                    ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
                }
                else
                {
                    sql.Format(_T("delete * from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
                    ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                    sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
                    ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                    sql.Format(_T("update Building set Building_Path = '%s' ,Building_Name = '%s'  where  Main_BuildingName = '%s' "),BCTemp.BuildingPath.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer());

                    ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                }

            }
            else
            {
// 				sql.Format(_T("delete * from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
// 				ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
//
// 				sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
// 				ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
//
// 				sql.Format(_T("Insert into Building(Main_BuildingName,Building_Name,Default_SubBuilding,Building_Path) values('%s','%s','%d','%s')"),BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),0,BCTemp.BuildingPath.GetBuffer());
// 				ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
            }
            ado.CloseConn();
            //���µ����ݿ�֮�󣬼��뵽Vector�б�
            m_BuildNameLst.push_back(BCTemp);

        }

    }
    //���¼�������
    LoadBuildingConfigDB();
    m_building_config_list.DeleteAllItems();
    for (int i=0; i<(int)m_BuildNameLst.size(); i++)
    {
        CString temp_item;
        temp_item.Format(_T("%d"),i+1);
        m_building_config_list.InsertItem(i,_T(""));

        m_building_config_list.SetItemText(i,BC_MAINNAME,m_BuildNameLst.at(i).MainBuildingName);
        //m_building_config_list.SetItemText(i,BC_SUBNAME,m_BuildNameLst.at(i).Sub_NetName);


        for (int z=0; z<(sizeof(Building_Protocol)/sizeof(Building_Protocol[0])); z++)
        {
            if(m_BuildNameLst.at(i).Protocol.CompareNoCase(Building_Protocol[z]) == 0)
            {
                m_BuildNameLst.at(i).protocol_index = z;
                break;
            }
            else
            {
                m_BuildNameLst.at(i).protocol_index = P_AUTO;
            }
        }

        if(m_BuildNameLst.at(i).protocol_index == INDEX_AUTO)
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,0);
        }
        //if( (m_BuildNameLst.at(i).protocol_index == P_MODBUS_TCP) ||
        //	(m_BuildNameLst.at(i).protocol_index == P_BACNET_IP) ||
        //	(m_BuildNameLst.at(i).protocol_index == P_REMOTE_DEVICE))
        if(m_BuildNameLst.at(i).protocol_index == INDEX_REMOTE_DEVICE)
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,1);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,1);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,0);
        }

        if( (m_BuildNameLst.at(i).protocol_index == INDEX_MODBUS_485) ||
                (m_BuildNameLst.at(i).protocol_index == INDEX_BACNET_MSTP))
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,1);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,1);
        }
        if(m_BuildNameLst.at(i).b_selected)
            m_building_config_list.SetItemText(i,BC_ITEM,_T("Selected"));
        else
            m_building_config_list.SetItemText(i,BC_ITEM,_T(""));


        CString m_show_protocol;
        if(m_BuildNameLst.at(i).protocol_index == INDEX_MODBUS_485)
            m_show_protocol = _T("Modbus 485");
        else if(m_BuildNameLst.at(i).protocol_index == INDEX_MODBUS_TCP)
            m_show_protocol = _T("Modbus TCP");
        else if(m_BuildNameLst.at(i).protocol_index == INDEX_BACNET_MSTP)
            m_show_protocol = _T("Bacnet MSTP");
        else if(m_BuildNameLst.at(i).protocol_index == INDEX_REMOTE_DEVICE)
            m_show_protocol = _T("Remote Device");
        else
            m_show_protocol = _T("Auto");

        m_building_config_list.SetItemText(i,BC_PROTOCOL,m_show_protocol);
        m_building_config_list.SetItemText(i,BC_IPADDRESS,m_BuildNameLst.at(i).IPAddress_Domain);
        m_building_config_list.SetItemText(i,BC_IPPORT,m_BuildNameLst.at(i).IP_Port);
        m_building_config_list.SetItemText(i,BC_COMPORT,m_BuildNameLst.at(i).Comport);
        m_building_config_list.SetItemText(i,BC_BAUDRATE,m_BuildNameLst.at(i).BaudRate);
        m_building_config_list.SetItemText(i,BC_BUILDINGPATH,m_BuildNameLst.at(i).BuildingPath);

        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
            {
                strlist.push_back(Building_Protocol[j]);
            }
            m_building_config_list.SetCellStringList(i, BC_PROTOCOL, strlist);
        }

        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
            {
                strlist.push_back(Building_Baudrate[j]);
            }
            m_building_config_list.SetCellStringList(i, BC_BAUDRATE, strlist);
        }


        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<m_szBuildingComs.size(); j++)
            {
                strlist.push_back(m_szBuildingComs.at(j));
            }
            m_building_config_list.SetCellStringList(i, BC_COMPORT, strlist);
        }

// 		for (int x=0;x<BC_COL_NUMBER;x++)
// 		{
// 			if((i%2)==0)
// 				m_building_config_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
// 			else
// 				m_building_config_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
// 		}
    }

    int last_new_item = (int)m_BuildNameLst.size();
    CString temp_cs;
    temp_cs.Format(_T("%d"),last_new_item + 1);
    m_building_config_list.InsertItem(last_new_item,_T(""));
// 	for (int x=0;x<BC_COL_NUMBER;x++)
// 	{
// 		if((last_new_item%2)==0)
// 			m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR);
// 		else
// 			m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
// 	}

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
        {
            strlist.push_back(Building_Protocol[j]);
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_PROTOCOL, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
        {
            strlist.push_back(Building_Baudrate[j]);
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_BAUDRATE, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<m_szBuildingComs.size(); j++)
        {
            strlist.push_back(m_szBuildingComs.at(j));
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_COMPORT, strlist);
    }



}

void CBuildingConfigration::Fresh_List_Row()
{
    LoadBuildingConfigDB();

    m_building_config_list.ShowWindow(SW_HIDE);

    m_building_config_list.DeleteAllItems();

    //while ( m_building_config_list.DeleteColumn (0)) ;

    //m_building_config_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    ////m_building_config_list.SetExtendedStyle(m_building_config_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    //m_building_config_list.SetExtendedStyle(m_building_config_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    //m_building_config_list.InsertColumn(BC_ITEM, _T(""), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    //m_building_config_list.InsertColumn(BC_MAINNAME, _T("Main Building"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    ////m_building_config_list.InsertColumn(BC_SUBNAME, _T("Sub Net"), 0, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_PROTOCOL, _T("Protocol"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_IPADDRESS, _T("IP Address / Domain"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_IPPORT, _T("IP Port"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_COMPORT, _T("COM Port"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_BAUDRATE, _T("Baud Rate"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_BUILDINGPATH, _T("Building Path"), 600, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

    for (int i=0; i<(int)m_BuildNameLst.size(); i++)
    {
        CString temp_item;
        temp_item.Format(_T("%d"),i+1);
        m_building_config_list.InsertItem(i,_T(""));

        m_building_config_list.SetItemText(i,BC_MAINNAME,m_BuildNameLst.at(i).MainBuildingName);
        //m_building_config_list.SetItemText(i,BC_SUBNAME,m_BuildNameLst.at(i).Sub_NetName);


        for (int z=0; z<(sizeof(Building_Protocol)/sizeof(Building_Protocol[0])); z++)
        {
            if(m_BuildNameLst.at(i).Protocol.CompareNoCase(Building_Protocol[z]) == 0)
            {
                m_BuildNameLst.at(i).protocol_index = z;
                break;
            }
            else
            {
                m_BuildNameLst.at(i).protocol_index = P_AUTO;
            }
        }
        if(m_BuildNameLst.at(i).protocol_index == P_AUTO)
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,0);
        }
        if( (m_BuildNameLst.at(i).protocol_index == P_MODBUS_TCP) ||
                (m_BuildNameLst.at(i).protocol_index == P_BACNET_IP))
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,1);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,1);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,0);
        }

        if( (m_BuildNameLst.at(i).protocol_index == P_MODBUS_485) ||
                (m_BuildNameLst.at(i).protocol_index == P_BACNET_MSTP))
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,1);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,1);
        }
        if(m_BuildNameLst.at(i).b_selected)
            m_building_config_list.SetItemText(i,BC_ITEM,_T("Selected"));
        else
            m_building_config_list.SetItemText(i,BC_ITEM,_T(""));
        m_building_config_list.SetItemText(i,BC_PROTOCOL,Building_Protocol[m_BuildNameLst.at(i).protocol_index]);
        m_building_config_list.SetItemText(i,BC_IPADDRESS,m_BuildNameLst.at(i).IPAddress_Domain);
        m_building_config_list.SetItemText(i,BC_IPPORT,m_BuildNameLst.at(i).IP_Port);
        m_building_config_list.SetItemText(i,BC_COMPORT,m_BuildNameLst.at(i).Comport);
        m_building_config_list.SetItemText(i,BC_BAUDRATE,m_BuildNameLst.at(i).BaudRate);
        m_building_config_list.SetItemText(i,BC_BUILDINGPATH,m_BuildNameLst.at(i).BuildingPath);

        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
            {
                strlist.push_back(Building_Protocol[j]);
            }
            m_building_config_list.SetCellStringList(i, BC_PROTOCOL, strlist);
        }

        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
            {
                strlist.push_back(Building_Baudrate[j]);
            }
            m_building_config_list.SetCellStringList(i, BC_BAUDRATE, strlist);
        }


        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<m_szBuildingComs.size(); j++)
            {
                strlist.push_back(m_szBuildingComs.at(j));
            }
            m_building_config_list.SetCellStringList(i, BC_COMPORT, strlist);
        }

// 		for (int x=0;x<BC_COL_NUMBER;x++)
// 		{
// 			if((i%2)==0)
// 				m_building_config_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
// 			else
// 				m_building_config_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
// 		}
    }

    int last_new_item = (int)m_BuildNameLst.size();
    CString temp_cs;
    temp_cs.Format(_T("%d"),last_new_item + 1);
    m_building_config_list.InsertItem(last_new_item,_T(""));
// 	for (int x=0;x<BC_COL_NUMBER;x++)
// 	{
// 		if((last_new_item%2)==0)
// 			m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR);
// 		else
// 			m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
// 	}

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
        {
            strlist.push_back(Building_Protocol[j]);
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_PROTOCOL, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
        {
            strlist.push_back(Building_Baudrate[j]);
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_BAUDRATE, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<m_szBuildingComs.size(); j++)
        {
            strlist.push_back(m_szBuildingComs.at(j));
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_COMPORT, strlist);
    }

    m_building_config_list.ShowWindow(SW_SHOW);
//// for (int i=0;i<(int)m_BuildNameLst.size();i++)
//// {
//	CString temp_item;
//	temp_item.Format(_T("%d"),m_changedRow+1);
////	m_building_config_list.InsertItem(m_changedRow,_T(""));
//
//	m_building_config_list.SetItemText(m_changedRow,BC_MAINNAME,m_BuildNameLst.at(m_changedRow).MainBuildingName);
//	//m_building_config_list.SetItemText(m_changedRow,BC_SUBNAME,m_BuildNameLst.at(m_changedRow).Sub_NetName);
//
//
//	for (int z=0;z<(sizeof(Building_Protocol)/sizeof(Building_Protocol[0]));z++)
//	{
//		if(m_BuildNameLst.at(m_changedRow).Protocol.CompareNoCase(Building_Protocol[z]) == 0)
//		{
//			m_BuildNameLst.at(m_changedRow).protocol_index = z;
//			break;
//		}
//		else
//		{
//			m_BuildNameLst.at(m_changedRow).protocol_index = P_AUTO;
//		}
//	}
//	if(m_BuildNameLst.at(m_changedRow).protocol_index == P_AUTO)
//	{
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_IPADDRESS,0);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_IPPORT,0);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_COMPORT,0);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_BAUDRATE,0);
//
//	}
//	if( (m_BuildNameLst.at(m_changedRow).protocol_index == P_MODBUS_TCP) ||
//		(m_BuildNameLst.at(m_changedRow).protocol_index == P_BACNET_IP))
//	{
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_IPADDRESS,1);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_IPPORT,1);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_COMPORT,0);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_BAUDRATE,0);
//	}
//
//	if( (m_BuildNameLst.at(m_changedRow).protocol_index == P_MODBUS_485) ||
//		(m_BuildNameLst.at(m_changedRow).protocol_index == P_BACNET_MSTP))
//	{
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_IPADDRESS,0);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_IPPORT,0);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_COMPORT,1);
//		m_building_config_list.SetCellEnabled(m_changedRow,BC_BAUDRATE,1);
//	}
//
//	if(m_BuildNameLst.at(m_changedRow).b_selected)
//		m_building_config_list.SetItemText(m_changedRow,BC_ITEM,_T("Selected"));
//	else
//		m_building_config_list.SetItemText(m_changedRow,BC_ITEM,_T(""));
//
//	m_building_config_list.SetItemText(m_changedRow,BC_PROTOCOL,Building_Protocol[m_BuildNameLst.at(m_changedRow).protocol_index]);
//	m_building_config_list.SetItemText(m_changedRow,BC_IPADDRESS,m_BuildNameLst.at(m_changedRow).IPAddress_Domain);
//	m_building_config_list.SetItemText(m_changedRow,BC_IPPORT,m_BuildNameLst.at(m_changedRow).IP_Port);
//	m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,m_BuildNameLst.at(m_changedRow).Comport);
//	m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,m_BuildNameLst.at(m_changedRow).BaudRate);
//	m_building_config_list.SetItemText(m_changedRow,BC_BUILDINGPATH,m_BuildNameLst.at(m_changedRow).BuildingPath);





//}
}
void CBuildingConfigration::Update_Building()
{
    Building_Config BuildingTemp;
    CADO ado;
    ado.OnInitADOConn();
    BuildingTemp.BuildingPath=m_building_config_list.GetItemText(m_changedRow,BC_BUILDINGPATH);
    BuildingTemp.Protocol=m_building_config_list.GetItemText(m_changedRow,BC_PROTOCOL);
    BuildingTemp.IPAddress_Domain=m_building_config_list.GetItemText(m_changedRow,BC_IPADDRESS);
    BuildingTemp.IP_Port=m_building_config_list.GetItemText(m_changedRow,BC_IPPORT);
    BuildingTemp.Comport=m_building_config_list.GetItemText(m_changedRow,BC_COMPORT);
    BuildingTemp.BaudRate=m_building_config_list.GetItemText(m_changedRow,BC_BAUDRATE);
    try
    {
        CString strSql;
        strSql.Format(_T("update Building set Protocal='%s',Ip_Address='%s',Ip_Port='%s',Com_Port='%s',Braudrate='%s' where Building_Path='%s'  "),
                      BuildingTemp.Protocol,BuildingTemp.IPAddress_Domain,BuildingTemp.IP_Port,BuildingTemp.Comport,BuildingTemp.BaudRate,BuildingTemp.BuildingPath);
        ado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);

    }
    catch (...)
    {
        AfxMessageBox(_T("Failed"));
    }
    ado.CloseConn();



    //Fresh_List();
    //PostMessage(WM_FRESH_DB,0,0);
}
void CBuildingConfigration::Deal_BuildingPath()
{
    CADO ado;
    ado.OnInitADOConn();
    CString strSql;
    strSql=_T("select * from Building");
    ado.m_pRecordset = ado.OpenRecordset(strSql);
    _variant_t temp_variant;
    CString pathfull;
    CString pathnew;
    try
    {
        while(VARIANT_FALSE==ado.m_pRecordset->EndOfFile)
        {
            temp_variant=ado.m_pRecordset->GetCollect("Building_Path");//
            if(temp_variant.vt!=VT_NULL)
                pathfull=temp_variant;
            else
                pathfull.Empty();
            pathnew=pathfull;
            int index=pathnew.Find(_T("Database"));
            pathnew.Delete(0,index);

            strSql.Format(_T("update Building set Building_Path='%s' where Building_Path='%s'"),pathnew,pathfull);
            ado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
            ado.m_pRecordset->MoveNext();
        }
    }
    catch (...)
    {
        AfxMessageBox(_T("Failed"));
    }
    ado.CloseConn();
}
LRESULT CBuildingConfigration::Fresh_Building_Config_Item(WPARAM wParam,LPARAM lParam)
{
    CADO ado;
    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;
	int sqlID;
    m_changedRow=Changed_Item;
    m_changedCol=Changed_SubItem;
    CString strTips;
    CString cs_temp = m_building_config_list.GetItemText(Changed_Item,Changed_SubItem);
    CString dbpath = m_building_config_list.GetItemText(Changed_Item,BC_BUILDINGPATH);
	if (m_changedRow<m_BuildNameLst.size())
	{
	   sqlID = m_BuildNameLst.at(m_changedRow).ID;
	}
	 
    Building_Config BCTemp;
    BOOL Is_The_Same=TRUE;
    if (cs_temp.IsEmpty())
    {
        m_building_config_list.Set_Edit(FALSE);
        return 0;
    }
    if(Changed_SubItem == BC_MAINNAME)
    {
        CString IP = m_building_config_list.GetItemText(m_changedRow,BC_IPADDRESS);
        CString Port = m_building_config_list.GetItemText(m_changedRow,BC_IPPORT);
        CString ComPort = m_building_config_list.GetItemText(m_changedRow,BC_COMPORT);
        CString Baudrate = m_building_config_list.GetItemText(m_changedRow,BC_BAUDRATE);
        CString Protocol = m_building_config_list.GetItemText(m_curRow,BC_PROTOCOL);

        LoadBuildingConfigDB();
        BOOL Is_Changed=FALSE;
        CString path=m_building_config_list.GetItemText(Changed_Item,BC_BUILDINGPATH);
        if (path.IsEmpty())
        {
            Is_Changed=FALSE;
        }
        else
        {
            m_select_text=m_BuildNameLst.at(Changed_Item).MainBuildingName;
            if (!path.IsEmpty())
            {

                Is_Changed=TRUE;
            }
        }
        if (m_select_text.CompareNoCase(cs_temp)==0)
        {
            return 0;
        }

        if (Is_Changed)
        {


            if (m_select_text.CompareNoCase(cs_temp)==0)
            {
                return 0;
            }
            for (int i=0; i<m_BuildNameLst.size(); i++)
            {
                if (cs_temp.CompareNoCase(m_BuildNameLst.at(i).MainBuildingName)==0)
                {
                    strTips.Format(_T("%s can't create,it existed!"),cs_temp);
                    MessageBox(strTips);

                    m_building_config_list.SetItemText(Changed_Item,BC_MAINNAME,m_select_text);
                    // Initial_Building_List();
                    return 0;
                }
            }
            CString filefoldSource,fileSource;
            CString NewfilefoldSource,NewfileSource;

            fileSource = g_strExePth + path;
            filefoldSource = fileSource;
            PathRemoveFileSpec(filefoldSource.GetBuffer (256));
            filefoldSource.ReleaseBuffer ();
            NewfileSource=filefoldSource+L"\\"+cs_temp+L".mdb";
            filefoldSource= filefoldSource+L"\\";

            NewfilefoldSource = g_strExePth+CString("Database\\Buildings\\")+cs_temp+CString("\\");
            CFileOperation      fp;
            if (fp.CheckPath (fileSource)>0)
            {
                fp.Rename (fileSource,NewfileSource);
                fp.Rename (filefoldSource,NewfilefoldSource) ;
                
            }

            NewfileSource=NewfilefoldSource+cs_temp+L".mdb";
            NewfileSource.Delete (0,g_strExePth.GetLength ());
            CString sql;
            try
            {
                CADO ado;
                ado.OnInitADOConn();

                sql.Format(_T("Select * from Building where Building_Path='%s'"),path);
                ado.m_pRecordset=ado.OpenRecordset(sql);
                if (!ado.m_pRecordset->EndOfFile)//�б���û�ж�Ӧ���кŵ�ֵ
                {
                    ado.CloseRecordset();

                    sql.Format(_T("Select * from Building "));
                    ado.m_pRecordset=ado.OpenRecordset(sql);
                    if (!ado.m_pRecordset->EndOfFile)
                    {
                      /*  ado.m_pRecordset->MoveLast();*/

                        sql.Format(_T("update Building_ALL set Building_Name = '%s' where Building_Name = '%s' "),cs_temp,m_select_text);
                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                        sql.Format(_T("update Building set Main_BuildingName = '%s' ,Building_Name = '%s' , Building_Path = '%s'  where  Building_Path = '%s' "),cs_temp,cs_temp,NewfileSource,path);

                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
                    }





                }
                ado.CloseConn();

				m_BuildNameLst.at(m_curRow).MainBuildingName = cs_temp;
				m_BuildNameLst.at(m_curRow).Sub_NetName = cs_temp;

				CppSQLite3DB SqliteDB;
				SqliteDB.open(m_sqlitepath);

				CString SqlTemp;
				SqlTemp.Format(_T("Update tbl_building_location set BuildingName = '%s' Where ID = %d "),cs_temp,sqlID);
				
				char charqltext[1024];
				 
				memset(charqltext,0,1024);
				WideCharToMultiByte( CP_ACP, 0, SqlTemp.GetBuffer(), -1, charqltext, 1024, NULL, NULL );

				SqliteDB.execDML(charqltext);

				 SqliteDB.close();
			
				
            }
            catch(_com_error *e)
            {
                AfxMessageBox(_T("Operator Failed"));

            }
            m_bChanged=TRUE;
            m_building_config_list.SetItemText (Changed_Item,BC_BUILDINGPATH,NewfileSource);
            _variant_t temp_variant;

            CString DB_Building_Name=L"";
            CBADO bado;
            //�޸�Building�����ݿ�,ҲҪ����,�����������..����....�군
#if 1 //У�һ�¿����Ƿ���Building��Database
            CString BuildingPath = GetExePath(true)+ NewfileSource;
            bado.SetDBPath(BuildingPath);
            bado.OnInitADOConn();
            if (!bado.IsHaveTable(bado,_T("ALL_NODE")))
            {

                bado.CloseConn();
                AfxMessageBox(_T("The Database of the building is error."));
                return 0;
            }
            else
            {

                sql.Format(_T("Select  distinct mainbuilding_name  from  ALL_NODE"));
                bado.m_pRecordset=bado.OpenRecordset(sql);
                if(!bado.m_pRecordset->EndOfFile)
                {
                    temp_variant=bado.m_pRecordset->GetCollect(_T("MainBuilding_Name"));
                    if (temp_variant.vt!=VT_NULL)
                    {
                        DB_Building_Name=temp_variant;
                    }


                }
                bado.CloseRecordset();

            }
#endif




            sql.Format(_T("Select * from ALL_NODE"));
            bado.m_pRecordset=bado.OpenRecordset(sql);
            while(!bado.m_pRecordset->EndOfFile)
            {

                sql.Format(_T("update ALL_NODE set MainBuilding_Name  = '%s' ,Building_Name = '%s'  where  MainBuilding_Name  = '%s'"),cs_temp,cs_temp,DB_Building_Name);
                //	bado.OpenRecordset(sql);
                bado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
                bado.m_pRecordset->MoveNext();

            }
            bado.CloseRecordset();

            bado.CloseConn();


        }
        else
        {
            for (int i=0; i<m_BuildNameLst.size(); i++)
            {
                if (cs_temp.CompareNoCase(m_BuildNameLst.at(i).MainBuildingName)==0)
                {
                    strTips.Format(_T("%s can't create,it existed!"),cs_temp);
                    MessageBox(strTips);
                    m_building_config_list.SetItemText(Changed_Item,Changed_SubItem,L"");
                    return 0;
                }
            }

            m_building_config_list.SetItemText(Changed_Item,Changed_SubItem,cs_temp);
            // m_building_config_list.SetItemText(Changed_Item,BC_SUBNAME,cs_temp);


            CString filebuildingPath=g_strBuildingFolder+cs_temp+_T("\\");//+_T(".mdb");
            // CString strDestFileName=g_strBuildingFolder+BCTemp.MainBuildingName+_T("\\");
            CreateDirectory(filebuildingPath,NULL);
            filebuildingPath+=cs_temp+_T(".mdb");

            DeleteFile(filebuildingPath);
            HANDLE hFind;//
            WIN32_FIND_DATA wfd;//
            //create building db file

            hFind = FindFirstFile(filebuildingPath, &wfd);//
            if (hFind==INVALID_HANDLE_VALUE)//˵����ǰĿ¼����t3000.mdb
            {

                HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_BUILDING_DB2), _T("BUILDING_DB"));
                HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);


                LPVOID lpExe = LockResource(hGlobal);
                CFile file;
                if(file.Open(filebuildingPath, CFile::modeCreate | CFile::modeWrite))
                    file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));
                file.Close();
                ::UnlockResource(hGlobal);
                ::FreeResource(hGlobal);
            }//
            int index=filebuildingPath.Find(_T("Database"));
            filebuildingPath.Delete(0,index);

            BCTemp.b_selected=FALSE;
            BCTemp.Sub_NetName=cs_temp;
            BCTemp.MainBuildingName=cs_temp;
            BCTemp.BuildingPath=filebuildingPath;
            if(Protocol.CompareNoCase(_T("Auto"))==0||Protocol.IsEmpty())
            {
                BCTemp.Protocol=_T("Auto");
                BCTemp.IPAddress_Domain = NO_APPLICATION;
                BCTemp.IP_Port = NO_APPLICATION;
                BCTemp.Comport = NO_APPLICATION;
                BCTemp.BaudRate = NO_APPLICATION;
            }
            else
            {
                BCTemp.Protocol=Protocol;
                BCTemp.IPAddress_Domain = IP;
                BCTemp.IP_Port = Port;
                BCTemp.Comport =ComPort ;
                BCTemp.BaudRate = Baudrate;
            }

            Is_The_Same=FALSE;
#if 1
            try
            {
				 
                CADO ado;
                ado.OnInitADOConn();
                CString sql;
                sql.Format(_T("Select * from Building where Building_Path='%s'"),BCTemp.BuildingPath);
                ado.m_pRecordset=ado.OpenRecordset(sql);
                if (ado.m_pRecordset->EndOfFile)//�б���û�ж�Ӧ���кŵ�ֵ
                {
                    ado.CloseRecordset();
                    for (int j=0; j<m_BuildNameLst.size(); j++)
                    {
                        if (BCTemp.MainBuildingName.CompareNoCase(m_BuildNameLst.at(j).MainBuildingName)==0)
                        {
                            Is_The_Same=TRUE;
                            break;

                        }
                        else
                        {
                            Is_The_Same=FALSE;
                        }
                    }
                    if (!Is_The_Same)
                    {
                        sql.Format(_T("delete * from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                        sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                        sql.Format(_T("Insert into Building(Main_BuildingName,Building_Name,Protocal,Default_SubBuilding,Building_Path,Com_Port,Ip_Address,IP_Port,Braudrate) values('%s','%s','%s','%d','%s' ,'%s','%s','%s','%s')"),
                                   BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),BCTemp.Protocol.GetBuffer(),0,BCTemp.BuildingPath.GetBuffer(),
                                   BCTemp.Comport,
                                   BCTemp.IPAddress_Domain,
                                   BCTemp.IP_Port,
                                   BCTemp.BaudRate);
                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                    }
                    else
                    {
                        sql.Format(_T("delete * from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);


                        sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);


                        sql.Format(_T("update Building set Building_Path = '%s' ,Building_Name = '%s' ,Protocal = '%s' where  Main_BuildingName = '%s' "),BCTemp.BuildingPath.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),BCTemp.Protocol,BCTemp.MainBuildingName.GetBuffer());

                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                    }

                }
                ado.CloseConn();

				ado.OnInitADOConn();
				sql.Format(_T("Select * from Building where Building_Path='%s'"),BCTemp.BuildingPath);
				ado.m_pRecordset=ado.OpenRecordset(sql);
				if (!ado.m_pRecordset->EndOfFile)
				{
				  sqlID = ado.m_pRecordset->GetCollect("ID");
				}
				ado.CloseRecordset();
				ado.CloseConn();

				CppSQLite3DB SqliteDB;
				SqliteDB.open(m_sqlitepath);

				CString SqlTemp;
				SqlTemp.Format(_T("insert into tbl_building_location(ID,BuildingName) Values(%d,'%s') "),sqlID,BCTemp.MainBuildingName);

				char charqltext[1024];
				 
				memset(charqltext,0,1024);
				WideCharToMultiByte( CP_ACP, 0, SqlTemp.GetBuffer(), -1, charqltext, 1024, NULL, NULL );
				SqliteDB.execDML(charqltext);
				SqliteDB.close();
				 
			 
				BCTemp.ID = sqlID;

                m_BuildNameLst.push_back(BCTemp);
                /*	 int last_new_item = (int)m_BuildNameLst.size();
                	 CString temp_cs;
                	 temp_cs.Format(_T("%d"),last_new_item + 1);
                	 m_building_config_list.InsertItem(last_new_item,_T(""));*/



                int last_new_item = (int)m_BuildNameLst.size();
                CString temp_cs;
                temp_cs.Format(_T("%d"),last_new_item + 1);
                m_building_config_list.InsertItem(last_new_item,_T(""));
// 				 for (int x=0;x<BC_COL_NUMBER;x++)
// 				 {
// 					 if((last_new_item%2)==0)
// 						 m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR);
// 					 else
// 						 m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
// 				 }

                if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
                {
                    ListCtrlEx::CStrList strlist;
                    for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
                    {
                        strlist.push_back(Building_Protocol[j]);
                    }
                    m_building_config_list.SetCellStringList(last_new_item, BC_PROTOCOL, strlist);
                }

                if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
                {
                    ListCtrlEx::CStrList strlist;
                    for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
                    {
                        strlist.push_back(Building_Baudrate[j]);
                    }
                    m_building_config_list.SetCellStringList(last_new_item, BC_BAUDRATE, strlist);
                }

                if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
                {
                    ListCtrlEx::CStrList strlist;
                    for (int j=0; j<m_szBuildingComs.size(); j++)
                    {
                        strlist.push_back(m_szBuildingComs.at(j));
                    }
                    m_building_config_list.SetCellStringList(last_new_item, BC_COMPORT, strlist);
                }


            }
            catch(_com_error *e)
            {
                AfxMessageBox(_T("Operator Failed"));
            }

            //���µ����ݿ�֮�󣬼��뵽Vector�б�


#endif


            m_building_config_list.SetItemText(Changed_Item,BC_BUILDINGPATH,filebuildingPath);
            m_building_config_list.SetItemText(Changed_Item,BC_PROTOCOL,BCTemp.Protocol);


            m_building_config_list.SetItemText(Changed_Item,BC_IPADDRESS,BCTemp.IPAddress_Domain);
            m_building_config_list.SetItemText(Changed_Item,BC_IPPORT,BCTemp.IP_Port);
            m_building_config_list.SetItemText(Changed_Item,BC_COMPORT,BCTemp.Comport);
            m_building_config_list.SetItemText(Changed_Item,BC_BAUDRATE,BCTemp.BaudRate);

            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_BAUDRATE,0);

        }
        //m_changedRow=m_curRow;
        //PostMessage(WM_FRESH_DB,0,0);


    }
    else if(Changed_SubItem == BC_PROTOCOL)
    {
        CString IP = m_building_config_list.GetItemText(m_changedRow,BC_IPADDRESS);
        CString Port = m_building_config_list.GetItemText(m_changedRow,BC_IPPORT);
        CString ComPort = m_building_config_list.GetItemText(m_changedRow,BC_COMPORT);
        CString Baudrate = m_building_config_list.GetItemText(m_changedRow,BC_BAUDRATE);


        int protocol_index=-1;
        for (int z=0; z<(sizeof(Building_Protocol)/sizeof(Building_Protocol[0])); z++)
        {
            if(cs_temp.CompareNoCase(Building_Protocol[z]) == 0)
            {
                protocol_index = z;
                break;
            }
            else
            {
                protocol_index = P_AUTO;
            }
        }


        if(protocol_index == INDEX_AUTO)	//ֻ����дԶ�̵�ʱ����Ҫ��дIP��ַ�Ͷ˿�;
        {
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_BAUDRATE,0);

            m_building_config_list.SetItemText(m_changedRow,BC_IPADDRESS,NO_APPLICATION);
            m_building_config_list.SetItemText(m_changedRow,BC_IPPORT,NO_APPLICATION);
            m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,NO_APPLICATION);
            m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,NO_APPLICATION);

        }
        else if( (protocol_index == P_MODBUS_TCP) ||(protocol_index == P_BACNET_IP) ||	(protocol_index == INDEX_REMOTE_DEVICE))
            //	else if( protocol_index == INDEX_REMOTE_DEVICE)
        {
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPADDRESS,1);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPPORT,1);

            m_building_config_list.SetCellEnabled(Changed_Item,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_BAUDRATE,0);
            if (!dbpath.IsEmpty())
            {
                GetPrivateProfileString(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_IP"),_T("192.168.0.3"),IP.GetBuffer(MAX_PATH),MAX_PATH,g_achive_device_name_path);
                GetPrivateProfileString(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_Port"),_T("10000"),Port.GetBuffer(MAX_PATH),MAX_PATH,g_achive_device_name_path);
            }
            else
            {
                IP = _T("192.168.0.3");
                Port = _T("10000");
            }

            m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,NO_APPLICATION);
            m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,NO_APPLICATION);

            m_building_config_list.SetItemText(m_changedRow,BC_IPADDRESS,IP);
            m_building_config_list.SetItemText(m_changedRow,BC_IPPORT,Port);

        }

        if( (protocol_index == INDEX_MODBUS_485) ||
                (protocol_index == INDEX_BACNET_MSTP))
        {
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPPORT,0);

            m_building_config_list.SetCellEnabled(Changed_Item,BC_COMPORT,1);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_BAUDRATE,1);


            if (ComPort.CompareNoCase(NO_APPLICATION)==0)
            {
                ComPort  = _T("COM1");
                Baudrate = _T("19200");
                m_building_config_list.SetItemText(m_changedRow,BC_IPADDRESS,NO_APPLICATION);
                m_building_config_list.SetItemText(m_changedRow,BC_IPPORT,NO_APPLICATION);
                m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,NO_APPLICATION);
                m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,NO_APPLICATION);
                m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,ComPort);
                m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,Baudrate);
            }
        }

        if (!dbpath.IsEmpty())
        {

            Update_Building();
            // return 0;
        }
        else
        {
            return 0;
        }




    }
    else if(Changed_SubItem == BC_IPADDRESS)
    {
        if (dbpath.IsEmpty())
        {
            return 0;
        }
        CString temp_ip;
        temp_ip = m_building_config_list.GetItemText(m_changedRow,BC_IPADDRESS);

        WritePrivateProfileStringW(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_IP"),temp_ip,g_achive_device_name_path);


        Update_Building();
    }
    else if(Changed_SubItem == BC_IPPORT)
    {
        if (dbpath.IsEmpty())
        {
            return 0;
        }
        CString temp_port;
        temp_port=m_building_config_list.GetItemText(m_changedRow,BC_IPPORT);
        WritePrivateProfileStringW(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_Port"),temp_port,g_achive_device_name_path);
        Update_Building();
    }
    else if(Changed_SubItem == BC_COMPORT)
    {
        if (dbpath.IsEmpty())
        {
            return 0;
        }
        Update_Building();
    }
    else if(Changed_SubItem == BC_BAUDRATE)
    {
        if (dbpath.IsEmpty())
        {
            return 0;
        }
        Update_Building();
    }
    else if (Changed_SubItem == BC_BUILDINGPATH)
    {
        cs_temp = m_building_config_list.GetItemText(Changed_Item,Changed_SubItem);
    }
    return 0;
}


void CBuildingConfigration::LoadBuildingConfigDB()
{

    //m_buildNameLst.clear();
    m_BuildNameLst.clear();
    CString strSql;
    //strSql.Format(_T("select * from Building where Main_BuildingName = '%s'"),m_strMainBuildingName); order by Main_BuildingName
    strSql.Format(_T("select * from Building "));
    m_building_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_building_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

    CString str_temp;
    str_temp.Empty();

	int id =1;
    _variant_t temp_variant;
    while(VARIANT_FALSE==m_building_pRs->EndOfFile)
    {
        Building_Config temp_building; //������ʱbuilding ������ֵ;
        int temp_value=0;
        temp_value=m_building_pRs->GetCollect(_T("Default_SubBuilding"));
        if(temp_value==-1)//def building;
        {
            temp_building.b_selected = true;
        }
        else
        {
            temp_building.b_selected = false;
        }
        _variant_t temp_variant;

		


        temp_variant=m_building_pRs->GetCollect("Main_BuildingName");//
        if(temp_variant.vt!=VT_NULL)
            temp_building.MainBuildingName=temp_variant;
        else
            temp_building.MainBuildingName.Empty();

		if (!temp_building.MainBuildingName.IsEmpty())
		{
			temp_variant=m_building_pRs->GetCollect("ID");//
			if(temp_variant.vt!=VT_NULL)
				temp_building.ID=temp_variant;
			else
				temp_building.ID=-1;
		}

        temp_variant=m_building_pRs->GetCollect("Building_Name");//
        if(temp_variant.vt!=VT_NULL)
            temp_building.Sub_NetName=temp_variant;
        else
            temp_building.Sub_NetName.Empty();

        temp_variant=m_building_pRs->GetCollect("Protocal");//
        if(temp_variant.vt!=VT_NULL)
        {
            temp_building.Protocol=temp_variant;
        }
        else
        {
            temp_building.Protocol.Empty();
        }



        temp_variant=m_building_pRs->GetCollect("Ip_Address");//
        if(temp_variant.vt!=VT_NULL)
            temp_building.IPAddress_Domain=temp_variant;
        else
            temp_building.IPAddress_Domain.Empty();

        temp_variant=m_building_pRs->GetCollect("Ip_Port");//
        if(temp_variant.vt!=VT_NULL)
            temp_building.IP_Port=temp_variant;
        else
            temp_building.IP_Port.Empty();

        temp_variant=m_building_pRs->GetCollect("Com_Port");//
        if(temp_variant.vt!=VT_NULL)
            temp_building.Comport=temp_variant;
        else
            temp_building.Comport.Empty();

        temp_variant=m_building_pRs->GetCollect("Braudrate");//
        if(temp_variant.vt!=VT_NULL)
            temp_building.BaudRate=temp_variant;
        else
            temp_building.BaudRate.Empty();
        //Building_Path
        temp_variant=m_building_pRs->GetCollect("Building_Path");//
        if(temp_variant.vt!=VT_NULL)
            temp_building.BuildingPath=temp_variant;
        else
            temp_building.BuildingPath.Empty();
        int index=temp_building.BuildingPath.Find(_T("Database"));
        temp_building.BuildingPath.Delete(0,index);


		temp_variant=m_building_pRs->GetCollect("country");//
		if(temp_variant.vt!=VT_NULL)
			temp_building.country=temp_variant;
		else
			temp_building.country.Empty();

		temp_variant=m_building_pRs->GetCollect("state");//
		if(temp_variant.vt!=VT_NULL)
			temp_building.state=temp_variant;
		else
			temp_building.state.Empty();

		temp_variant=m_building_pRs->GetCollect("city");//
		if(temp_variant.vt!=VT_NULL)
			temp_building.city=temp_variant;
		else
			temp_building.city.Empty();


		temp_variant=m_building_pRs->GetCollect("street");//
		if(temp_variant.vt!=VT_NULL)
			temp_building.street=temp_variant;
		else
			temp_building.street.Empty();

		temp_variant=m_building_pRs->GetCollect("ZIP");//
		if(temp_variant.vt!=VT_NULL)
			temp_building.Zip=temp_variant;
		else
			temp_building.Zip=0;

	 

	 

		 
        m_BuildNameLst.push_back(temp_building);

        m_building_pRs->MoveNext();//
    }

    if(m_building_pRs->State)
        m_building_pRs->Close();
}

BOOL IsNum(CString str)
{
    int n=str.GetLength();
    for(int i=0; i<n; i++)
        if (str[i]<'0'||str[i]>'9')
            return FALSE;
    return TRUE;
}


//BOOL ValidAddress(CString sAddress)
//{
//	int nPos;
//	UINT n1,n2,n3,n4;
//	CString sTemp=sAddress;
//	n1=_wtoi(sTemp);
//	nPos=sTemp.Find(_T("."));
//	if(nPos==-1) return false;
//	sTemp=sTemp.Mid(nPos+1);
//
//	n2=_wtoi(sTemp);
//	nPos=sTemp.Find(_T("."));
//	if(nPos==-1) return false;
//	sTemp=sTemp.Mid(nPos+1);
//	n3=_wtoi(sTemp);
//	nPos=sTemp.Find(_T("."));
//	if(nPos==-1) return false;
//	sTemp=sTemp.Mid(nPos+1);
//	n4=_wtoi(sTemp);
//	if(n1<0 ||n1>255) return false;
//	if(n2<0 ||n2>255) return false;
//	if(n3<0 ||n3>255) return false;
//	if(n4<0 ||n4>255) return false;
//	return TRUE;
//}

void CBuildingConfigration::OnBnClickedBuildingButtonAdd()
{
    // TODO: Add your control notification handler code here
    bool is_domain = false;
    int last_count = m_building_config_list.GetItemCount() - 1;
    CString strMainBuildName;
    CString strSubBuildingName;
    CString strProtocol;
    CString strIP;
    CString strIpPort;
    CString strCOMPort;
    CString strCOMPortBraud;
    strMainBuildName =  m_building_config_list.GetItemText(last_count,BC_MAINNAME);
//	strSubBuildingName = m_building_config_list.GetItemText(last_count,BC_SUBNAME);
    strProtocol =  m_building_config_list.GetItemText(last_count,BC_PROTOCOL);
    strIP = m_building_config_list.GetItemText(last_count,BC_IPADDRESS);
    strIpPort = m_building_config_list.GetItemText(last_count,BC_IPPORT);
    strCOMPort = m_building_config_list.GetItemText(last_count,BC_COMPORT);
    strCOMPortBraud = m_building_config_list.GetItemText(last_count,BC_BAUDRATE);
    if(strMainBuildName.IsEmpty())
    {
        MessageBox(_T("Warning!Main Building is empty!"));
        return;
    }
    else if(strSubBuildingName.IsEmpty())
    {
        MessageBox(_T("Warning!Sub Net is empty!"));
        return;
    }
    else if(strProtocol.IsEmpty())
    {
        MessageBox(_T("Warning!Protocol is empty!"));
        return;
    }

    if(strProtocol.CompareNoCase(_T("Auto")) == 0)
    {

    }
    else if((strProtocol.CompareNoCase(_T("Modbus TCP")) == 0) ||
            (strProtocol.CompareNoCase(_T("Remote Device")) == 0))
    {
        if((strIP.IsEmpty()) || (strIpPort.IsEmpty()))
        {
            MessageBox(_T("Warning!IP or domain is empty!"));
            return;
        }
        CStringArray temparray;
        SplitCStringA(temparray,strIP,_T("."));
        if((temparray.GetSize()==4))	//��3��  . 4��
        {
            if((IsNum(temparray.GetAt(0))) && (IsNum(temparray.GetAt(1))) && (IsNum(temparray.GetAt(2))))
            {
                if(ValidAddress(strIP) == false)
                {
                    MessageBox(_T("Warning!IP address error!"));
                    return;
                }

            }
            else	//�����ж�Ϊ ����;
            {
                is_domain = true;
            }
        }
        else	//�ж�Ϊ ����;
        {
            is_domain = true;
        }

        if(is_domain)
        {
            char temp_domain[200];
            WideCharToMultiByte( CP_ACP, 0, strIP.GetBuffer(), -1, temp_domain, 255, NULL, NULL );
            strIP.ReleaseBuffer();
            hostent* host = gethostbyname(temp_domain);
            if(host == NULL)
            {
                if(IDYES == MessageBox(_T("The domain is unreachable .Continue?"),_T("Warning"),MB_YESNO))
                {

                }
                else
                {
                    return;
                }
            }
        }

    }
    else
    {
        if(strCOMPort.IsEmpty())
        {
            MessageBox(_T("Warning!Com port is empty!"));
            return;
        }
        else if(strCOMPortBraud.IsEmpty())
        {
            MessageBox(_T("Warning!Baudrate is empty!"));
            return;
        }
    }



    CString strSql;
    strSql.Format(_T("Select * from Building_ALL where Building_Name = '%s'"),strMainBuildName);
    m_building_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_building_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
    _variant_t temp_variant_name;
    bool findMainBuilding=false;
    if(VARIANT_FALSE==m_building_pRs->EndOfFile)		//If it's not empty , means the input Building has exist in Main Building.
    {
        findMainBuilding=true;
    }

    if(m_building_pRs->State)
        m_building_pRs->Close();


    if(!findMainBuilding)
    {
        strSql.Format(_T("insert into Building_ALL (Building_Name,Telephone,Address) values('"+strMainBuildName+"','"+ _T("") +"','"+_T("") +"')"));
        m_building_pCon->Execute(strSql.GetString(),NULL,adCmdText);
    }
    //---------------------------------------------------------------------------------------------



    try
    {
        BOOL bDefault =FALSE;
        strSql.Format(_T("insert into Building (Main_BuildingName,Building_Name,Protocal,Com_Port,Ip_Address,Ip_Port,Braudrate) values('"+strMainBuildName+"','"+strSubBuildingName+"','"+strProtocol+"','"+strCOMPort+"','"+strIP+"','"+strIpPort+"','"+strCOMPortBraud+"')"));
        m_building_pCon->Execute(strSql.GetString(),NULL,adCmdText);
    }
    catch(_com_error *e)
    {
        AfxMessageBox(e->ErrorMessage());
    }
    //Update_Recorder();




}


void CBuildingConfigration::OnBnClickedBuildingButtonDelete()
{
    if (m_building_pRs->State)
    {
        m_building_pRs->Close();
    }
    if (m_building_pCon->State)
    {
        m_building_pCon->Close();
    }

    CDialogEx::OnCancel();
}


void CBuildingConfigration::OnSize(UINT nType, int cx, int cy)
{


    if (nType==SIZE_RESTORED)
    {

        CRect ViewRect;
        GetClientRect(&ViewRect);
        //TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
        // m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
        if (m_building_config_list.GetSafeHwnd())
        {
            m_building_config_list.MoveWindow(CRect(ViewRect.left,ViewRect.top+70,ViewRect.Width(),ViewRect.Height()),TRUE);
        }


    }

    CDialogEx::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
}


void CBuildingConfigration::OnNMClickListBuildingConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    CString temp_cstring;
    long lRow,lCol;
    m_building_config_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_building_config_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_building_config_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;
    m_curCol=lCol;
    m_curRow=lRow;
    CString Strprotcol=m_building_config_list.GetItemText(lRow,BC_PROTOCOL);
    BOOL Is_The_Same=TRUE;
    m_select_text=m_building_config_list.GetItemText(lRow,lCol);
    if(lRow>m_building_config_list.GetItemCount()) //����������������кţ���������Ч��
        return;
    if(lRow<0)
        return;
    CString New_CString;
    CString temp_task_info;

    CBADO bado;

    CString temp1;
    CStringArray temparray;
    Building_Config BCTemp;

    CString sql;
    _variant_t temp_variant;
    CString Building_Name=L"";
    CString DB_Building_Name=L"";

    //if (BC_ITEM==lCol)
    //{
    //    m_changedCol=m_curCol;
    //    m_changedRow=m_curRow;
    //    CMenu menu;
    //    menu.LoadMenu(IDR_BUILDING_CONFIG);
    //    CMenu *pmenu=menu.GetSubMenu(0);
    //    CPoint point;
    //    GetCursorPos(&point);
    //    pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);
    //}
     if (BC_BUILDINGPATH==lCol)//�����ѡ��һ�����ݿ��ʱ�򣬰�Building�������Ѿ����ݿ����Ϣ����ȡ����
    {
        m_building_config_list.Set_Edit(FALSE);

        CString path=m_building_config_list.GetItemText(lRow,lCol);
        if (!path.IsEmpty())
        {

            return;
        }

        CStringArray  ArrayFileName;
        CString strFilter = _T("Access Database file|*.mdb|all file|*.*||");
        CFileDialog dlg(true,_T("Open image file"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
        if(IDOK==dlg.DoModal())
        {
            CString strdatabasefile=dlg.GetPathName();
            CString databasefilename=dlg.GetFileName();
            SplitCStringA(ArrayFileName,strdatabasefile,L".");
            if (ArrayFileName[ArrayFileName.GetSize()-1].CompareNoCase(L"mdb")!=0)
            {
                MessageBox(_T("Please select a access file"));

                return;
            }
#if 1 //У�һ�¿����Ƿ���Building��Database
            bado.SetDBPath(strdatabasefile);
            bado.OnInitADOConn();
            if (!bado.IsHaveTable(bado,_T("ALL_NODE")))
            {

                bado.CloseConn();
                AfxMessageBox(_T("The file is not a correct building database."));
                return;
                //continue;
            }
            else
            {

                sql.Format(_T("Select  distinct mainbuilding_name  from  ALL_NODE"));
                bado.m_pRecordset=bado.OpenRecordset(sql);
                if(!bado.m_pRecordset->EndOfFile)
                {
                    temp_variant=bado.m_pRecordset->GetCollect(_T("MainBuilding_Name"));
                    if (temp_variant.vt!=VT_NULL)
                    {
                        DB_Building_Name=temp_variant;
                    }


                }
                bado.CloseRecordset();

            }
#endif
            for (int j=0; j<m_BuildNameLst.size(); j++)
            {
                if (DB_Building_Name.CompareNoCase(m_BuildNameLst.at(j).MainBuildingName)==0)
                {
                    Is_The_Same=TRUE;
                    break;

                }
                else
                {
                    Is_The_Same=FALSE;
                }
            }
            if (Is_The_Same)
            {
                CString strTips;
                strTips.Format(_T("The building:%s,Exists"),DB_Building_Name);
                AfxMessageBox(strTips);
                return;
            }
            SplitCStringA(ArrayFileName,strdatabasefile,L"\\");
            if (ArrayFileName.GetSize()<2)
            {
                MessageBox(_T("file path error"));
                return;
            }
// 			//
// 			if(DB_Building_Name.CompareNoCase(Building_Name)!=0)
// 			{
// 				sql.Format(_T("Select * from ALL_NODE"));
// 				bado.m_pRecordset=bado.OpenRecordset(sql);
// 				while(!bado.m_pRecordset->EndOfFile)
// 				{
//
// 					sql.Format(_T("update ALL_NODE set MainBuilding_Name  = '%s' ,Building_Name = '%s'  where  MainBuilding_Name  = '%s'"),Building_Name,Building_Name,DB_Building_Name);
// 					//	bado.OpenRecordset(sql);
// 					bado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
// 					bado.m_pRecordset->MoveNext();
//
// 				}
// 				bado.CloseRecordset();
// 			}
// 			bado.CloseConn();

            Building_Name=ArrayFileName[ArrayFileName.GetSize()-2];
            BCTemp.MainBuildingName=DB_Building_Name;
            BCTemp.Sub_NetName=DB_Building_Name;
            CString strDestFileName=g_strBuildingFolder+BCTemp.MainBuildingName+_T("\\");

            if (!CreateDirectory(strDestFileName,NULL))
            {
                CString strTips;
                strTips.Format(_T("%s  Create Failed"),strDestFileName);
                AfxMessageBox(strTips);
                return;
            }

            strDestFileName=g_strBuildingFolder+BCTemp.MainBuildingName+_T("\\")+databasefilename;

            if(!CopyFile(strdatabasefile,strDestFileName,FALSE))
            {
                CString strTips;
                strTips.Format(_T("Copy file form %s to %s Failed"),strdatabasefile,strDestFileName);
                AfxMessageBox(strTips);
                return;
            }



            BCTemp.BuildingPath=strDestFileName;
            int index= BCTemp.BuildingPath.Find(_T("Database"));
            BCTemp.BuildingPath.Delete(0,index);

            BCTemp.b_selected=FALSE;

            m_building_config_list.SetItemText(lRow,lCol,strDestFileName);
//			 m_building_config_list.SetItemText(lRow,BC_SUBNAME,BCTemp.MainBuildingName);
            m_building_config_list.SetItemText(lRow,BC_MAINNAME,BCTemp.MainBuildingName);

            //�������ݿ�
#if 1
            try
            {
                CADO ado;
                ado.OnInitADOConn();
                CString sql;
                sql.Format(_T("Select * from Building where Building_Path='%s'"),BCTemp.BuildingPath);
                ado.m_pRecordset=ado.OpenRecordset(sql);
                if (ado.m_pRecordset->EndOfFile)//�б���û�ж�Ӧ���кŵ�ֵ
                {
                    ado.CloseRecordset();
                    for (int j=0; j<m_BuildNameLst.size(); j++)
                    {
                        if (BCTemp.MainBuildingName.CompareNoCase(m_BuildNameLst.at(j).MainBuildingName)==0)
                        {
                            Is_The_Same=TRUE;
                            break;

                        }
                        else
                        {
                            Is_The_Same=FALSE;
                        }
                    }
                    if (!Is_The_Same)
                    {
                        sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),
                                   BCTemp.MainBuildingName.GetBuffer(),0);
                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                        sql.Format(_T("Insert into Building(Main_BuildingName,Building_Name,Default_SubBuilding,Building_Path) values('%s','%s','%d','%s')"),
                                   BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),0,BCTemp.BuildingPath.GetBuffer());
                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);
                    }
                    else
                    {
                        sql.Format(_T("update Building set Building_Path = '%s' ,Building_Name = '%s'  where  Main_BuildingName = '%s' "),
                                   BCTemp.BuildingPath.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer());

                        ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

                    }

                }
                ado.CloseConn();
                m_BuildNameLst.push_back(BCTemp);
                int last_new_item = (int)m_BuildNameLst.size();
                CString temp_cs;
                temp_cs.Format(_T("%d"),last_new_item + 1);
                m_building_config_list.InsertItem(last_new_item,_T(""));
            }
            catch(_com_error *e)
            {
                AfxMessageBox(_T("Operator Failed"));
            }
            //���µ����ݿ�֮�󣬼��뵽Vector�б�
#endif



        }


    }
//	else if(BC_SUBNAME==lCol)
// 	{
// 	m_building_config_list.Set_Edit(FALSE);
// 	return;
// 	}
    else if(lCol== BC_MAINNAME)
    {

    }
    else if(lCol== BC_PROTOCOL)
    {
// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 		    m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }
    else if(lCol == BC_IPADDRESS)
    {


// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 			m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }
    else if(lCol == BC_IPPORT)
    {
// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 			m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }
    else if(lCol == BC_COMPORT)
    {
// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 			m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }
    else if(lCol == BC_BAUDRATE)
    {
// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 			m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }



    *pResult = 0;
}


void CBuildingConfigration::OnNMRClickListBuildingConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    CString temp_cstring;
    long lRow,lCol;
    m_building_config_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_building_config_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_building_config_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;

    m_curRow=lRow;
    m_curCol=lCol;
    if(lRow>m_building_config_list.GetItemCount()) //����������������кţ���������Ч��
        return;
    if(lRow<0)
        return;
    CString New_CString;
    CString temp_task_info;

    CString temp1;
    CStringArray temparray;
    m_select_text=m_building_config_list.GetItemText(lRow,lCol);

    if (BC_ITEM==lCol)
    {
        m_changedCol=m_curCol;
        m_changedRow=m_curRow;
        CMenu menu;
        menu.LoadMenu(IDR_BUILDING_CONFIG);
        CMenu *pmenu=menu.GetSubMenu(0);
        CPoint point;
        GetCursorPos(&point);
        pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);

    }
    else
    {
        return;
    }
    *pResult = 0;
}


void CBuildingConfigration::OnBuildingconfigSelect()
{
    int count=m_building_config_list.GetRowCount();
	
    if (m_curRow==count-1)
    {
        return;
    }

	int ID = m_BuildNameLst.at(m_curRow).ID;
    CString m_select,m_strMainBuildingName;
    m_strMainBuildingName=m_building_config_list.GetItemText(m_curRow,BC_MAINNAME);
    m_select=m_building_config_list.GetItemText(m_curRow,m_curCol);
    if (!m_select.IsEmpty())
    {
        AfxMessageBox(_T("you have already Selected.\ndon't need to select ,again!"));
        return;
	}
	
    CADO ado;
    ado.OnInitADOConn();
    try
    {

        CString execute_str=_T("update Building set Default_SubBuilding = 0 where Default_SubBuilding = -1");
        ado.m_pConnection->Execute(execute_str.GetString(),NULL,adCmdText);
        execute_str.Format(_T("update Building set Default_SubBuilding = -1 where  Main_BuildingName= '%s'"),m_strMainBuildingName);
        ado.m_pConnection->Execute(execute_str.GetString(),NULL,adCmdText);
	    
		CppSQLite3DB SqliteDB;
		SqliteDB.open(m_sqlitepath);
	 
		execute_str = _T("update tbl_building_location set Selection = 0 where Selection = 1");

		char charqltext[1024];
		memset(charqltext,0,1024);
		WideCharToMultiByte( CP_ACP, 0, execute_str.GetBuffer(), -1, charqltext, 1024, NULL, NULL );
		SqliteDB.execDML(charqltext);

		execute_str.Format(_T("update tbl_building_location set Selection = 1 where  ID = %d "),ID);

		memset(charqltext,0,1024);
		WideCharToMultiByte( CP_ACP, 0, execute_str.GetBuffer(), -1, charqltext, 1024, NULL, NULL );
		SqliteDB.execDML(charqltext);

		SqliteDB.close();

    }
    catch (_com_error *e)
    {
        AfxMessageBox(_T("Operator Failed"));
    }
    ado.CloseConn();
    //int count=m_building_config_list.GetRowCount();
    for (int row=0; row<count-1; row++)
    {
        if (row!=m_curRow)
        {
            m_building_config_list.SetItemText(row,m_curCol,L"");
        }
        else
        {
            m_building_config_list.SetItemText(m_curRow,m_curCol,L"Selected");
        }

    }

    //Fresh_List();
    //Initial_Building_List();
    m_bChanged=TRUE;
}


void CBuildingConfigration::OnBuildingconfigUnselect()
{


    CString m_select,m_strMainBuildingName;

    m_strMainBuildingName=m_building_config_list.GetItemText(m_curRow,BC_MAINNAME);
    m_select=m_building_config_list.GetItemText(m_curRow,m_curCol);
    if (m_select.IsEmpty())
    {
        AfxMessageBox(_T("you have already Unselected.\ndon't need to unselect ,again!"));
        return;
    }
    CADO ado;
    ado.OnInitADOConn();
    try
    {

        CString execute_str=_T("update Building set Default_SubBuilding = 0 where Default_SubBuilding = -1");
        ado.m_pConnection->Execute(execute_str.GetString(),NULL,adCmdText);
    }
    catch (_com_error *e)
    {
        AfxMessageBox(_T("Operator Failed"));
    }
    ado.CloseConn();
    m_bChanged=TRUE;
    //m_building_config_list.SetItemText(m_curRow,m_curCol,L"");
//	Fresh_List();
    Initial_Building_List();
}


void CBuildingConfigration::OnBuildingconfigDelete()
{

    CString m_select,m_strMainBuildingName,filename;

    m_strMainBuildingName=m_building_config_list.GetItemText(m_curRow,BC_MAINNAME);
    m_select=m_building_config_list.GetItemText(m_curRow,m_curCol);
	int sqlID  = m_BuildNameLst.at(m_curRow).ID;
    CString buildingPath=m_building_config_list.GetItemText(m_curRow,BC_BUILDINGPATH);
    if(m_curRow>=(m_building_config_list.GetRowCount()-1) || m_curRow <0 )
    {
        return;
    }
    if (!m_select.IsEmpty())
    {
        AfxMessageBox(_T("The current building is using,Can't delete it !"));
        return;
    }

    CADO ado;
    ado.OnInitADOConn();
    CString strSql;
    strSql.Format(_T("delete * from Building where  Main_BuildingName='%s'"),m_strMainBuildingName);
    try
    {


        CString strTemp;
        strTemp.Format(_T("Are you sure to delete the building:'%s'"),m_strMainBuildingName);
        if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
        {
            ado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
        }
    }
    catch (_com_error *e)
    {
        AfxMessageBox(_T("Operator Failed"));
    }
    ado.CloseConn();

	int ID = m_BuildNameLst.at(m_curRow).ID;

	CppSQLite3DB SqliteDB;
	SqliteDB.open(m_sqlitepath);
	CString execute_str;
	execute_str.Format(_T("delete   from tbl_building_location where ID = %d"),ID);

	char charqltext[1024];
	memset(charqltext,0,1024);
	WideCharToMultiByte( CP_ACP, 0, execute_str.GetBuffer(), -1, charqltext, 1024, NULL, NULL );
	SqliteDB.execDML(charqltext);

	SqliteDB.close();
	 

    CString PathTemp;
    PathTemp=GetExePath(true)+buildingPath;
    buildingPath=PathTemp;
    CStringArray  ArrayFileName;
    SplitCStringA(ArrayFileName,buildingPath,L"\\");
    m_BuildNameLst.clear();
    filename=L"";
    for (int i=0; i<ArrayFileName.GetSize()-1; i++)
    {
        filename+=ArrayFileName[i];
        filename+=L"\\";
    }
    //DeleteFile(buildingPath);
    DeleteDirectory(filename);
    m_building_config_list.DeleteItem(m_curRow);
    /*LoadBuildingConfigDB();
    Initial_Building_List();*/
    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
        {
            strlist.push_back(Building_Protocol[j]);
        }
        m_building_config_list.SetCellStringList(m_curRow, BC_PROTOCOL, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
        {
            strlist.push_back(Building_Baudrate[j]);
        }
        m_building_config_list.SetCellStringList(m_curRow, BC_BAUDRATE, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<m_szBuildingComs.size(); j++)
        {
            strlist.push_back(m_szBuildingComs.at(j));
        }
        m_building_config_list.SetCellStringList(m_curRow, BC_COMPORT, strlist);
    }



	


    m_bChanged=TRUE;
    // Fresh_List();
    //Initial_Building_List();
    //PostMessage(WM_FRESH_DB,0,0);
}


LRESULT CBuildingConfigration::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if (message==WM_FRESH_DB)
    {
        Fresh_List_Row();
        // Initial_Building_List();
    }
    return CDialogEx::WindowProc(message, wParam, lParam);
}


void CBuildingConfigration::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    //CDialogEx::OnOK();
}


void CBuildingConfigration::OnNMDblclkListBuildingConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString temp_cstring;
	long lRow,lCol;
	m_building_config_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_building_config_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_building_config_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;
	m_curCol=lCol;
	m_curRow=lRow;
	CString Strprotcol=m_building_config_list.GetItemText(lRow,BC_PROTOCOL);
	BOOL Is_The_Same=TRUE;
	m_select_text=m_building_config_list.GetItemText(lRow,lCol);
	if(lRow>m_building_config_list.GetItemCount()) //����������������кţ���������Ч��
		return;
	if(lRow<0)
		return;
     CBuildingConfigEditDlg dlg;
	 dlg.m_currentBuilding = m_BuildNameLst.at(m_curRow);
	 if (dlg.DoModal() == IDOK)
	 {
		 
		m_BuildNameLst.at(m_curRow) = dlg.m_currentBuilding;
		CADO m_database_operator;
		 

		try
		{
			_variant_t temp_var;
			m_database_operator.OnInitADOConn();
			CString strSql;
			strSql.Format(_T("Select * from Building Where ID = %d "), m_BuildNameLst.at(m_curRow).ID);
			m_database_operator.m_pRecordset = m_database_operator.OpenRecordset(strSql);

			if(!m_database_operator.m_pRecordset->EndOfFile)
			{


				if(!m_BuildNameLst.at(m_curRow).city.IsEmpty())
				{
					m_database_operator.m_pRecordset->PutCollect("city", (_variant_t)m_BuildNameLst.at(m_curRow).city);
				}
				if(!m_BuildNameLst.at(m_curRow).country.IsEmpty())
				{
					m_database_operator.m_pRecordset->PutCollect("country", (_variant_t)m_BuildNameLst.at(m_curRow).country);
				} 
				if(!m_BuildNameLst.at(m_curRow).Elevation.IsEmpty())
				{
					m_database_operator.m_pRecordset->PutCollect("Elevation", (_variant_t)m_BuildNameLst.at(m_curRow).Elevation);
				}
				if(!m_BuildNameLst.at(m_curRow).Latitude.IsEmpty())
				{
					m_database_operator.m_pRecordset->PutCollect("Latitude", (_variant_t)m_BuildNameLst.at(m_curRow).Latitude);
				}
				if(!m_BuildNameLst.at(m_curRow).Longitude.IsEmpty())
				{
					m_database_operator.m_pRecordset->PutCollect("Longitude", (_variant_t)m_BuildNameLst.at(m_curRow).Longitude);
				}
				if(!m_BuildNameLst.at(m_curRow).state.IsEmpty())
				{
					m_database_operator.m_pRecordset->PutCollect("state", (_variant_t)m_BuildNameLst.at(m_curRow).state);
				}
				if(!m_BuildNameLst.at(m_curRow).street.IsEmpty())
				{
					m_database_operator.m_pRecordset->PutCollect("street", (_variant_t)m_BuildNameLst.at(m_curRow).street);
				}
				if(m_BuildNameLst.at(m_curRow).Zip!=-1)
				{
					m_database_operator.m_pRecordset->PutCollect("Zip", (_variant_t)m_BuildNameLst.at(m_curRow).Zip);
				}
				m_database_operator.m_pRecordset->Update();
			}

			m_database_operator.CloseRecordset();
			m_database_operator.CloseConn();

			 
			CppSQLite3DB SqliteDB;
			

			SqliteDB.open(m_sqlitepath);
			CString SqlText;
			SqlText.Format(_T("Update tbl_building_location Set \
			 country = '%s' ,\
			 state = '%s' ,\
			 city = '%s' ,\
			 street = '%s' ,\
			 longitude = '%s' ,\
			 latitude = '%s' ,\
			 elevation = '%s' \
			 where ID = %d \
			  "),
			  m_BuildNameLst.at(m_curRow).country,
			  m_BuildNameLst.at(m_curRow).state,
			  m_BuildNameLst.at(m_curRow).city,
			  m_BuildNameLst.at(m_curRow).street,
			  m_BuildNameLst.at(m_curRow).Longitude,
			  m_BuildNameLst.at(m_curRow).Latitude,
			  m_BuildNameLst.at(m_curRow).Elevation,
			  m_BuildNameLst.at(m_curRow).ID
			  );
			  char charqltext[1024];
			 
			  memset(charqltext,0,1024);
			  WideCharToMultiByte( CP_ACP, 0, SqlText.GetBuffer(), -1, charqltext, 1024, NULL, NULL );

			  SqliteDB.execDML(charqltext);
       SqliteDB.close();
		 
			AfxMessageBox(_T("Update Successfully"));

		}
		catch(CException* e)
		{

			e->GetErrorMessage(_T("Exception"), 0);
		}
	 }
	  
	  
	*pResult = 0;
}
