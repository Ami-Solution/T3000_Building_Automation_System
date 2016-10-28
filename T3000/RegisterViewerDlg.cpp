// RegisterViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RegisterViewerDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
 #include "../SQLiteDriver/CppSQLite3.h"
#include "globle_function.h"
#include "TemcoDataGrid.h"

#define BACK_CELL RGB(139,137,137)
#define DIMGREY RGB(176,196,222)
#define WRITE_COLOR RGB(255,255,255)


#define SECTION _T("CRegisterViewerDlg_REGISTERLISTGIRD")
UINT BackRegisterViewerFreshProc(LPVOID pParam);
// CRegisterViewerDlg dialog
#define ROWS 50
#define COLS 7
IMPLEMENT_DYNAMIC(CRegisterViewerDlg, CDialog)

CRegisterViewerDlg::CRegisterViewerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CRegisterViewerDlg::IDD, pParent)
{
    m_recordcount=0;
    m_Datapages=1;
    m_curPage=1;
    m_CanEdit=FALSE;
}

CRegisterViewerDlg::~CRegisterViewerDlg()
{


}

void CRegisterViewerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_registerlistGrid);
    DDX_Control(pDX, IDC_COLNAME, m_ColName);
    DDX_Control(pDX, IDC_CELLEDIT, m_inNameEdt);
}


BEGIN_MESSAGE_MAP(CRegisterViewerDlg, CDialog)
//	ON_BN_CLICKED(IDC_LEFT, &CRegisterViewerDlg::OnBnClickedLeft)
//	ON_BN_CLICKED(IDC_RIGHT, &CRegisterViewerDlg::OnBnClickedRight)
//	ON_BN_CLICKED(IDC_BEGIN, &CRegisterViewerDlg::OnBnClickedBegin)
//	ON_BN_CLICKED(IDC_END, &CRegisterViewerDlg::OnBnClickedEnd)
    ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_ADDNEWPRODUCT, &CRegisterViewerDlg::OnBnClickedAddnewproduct)
    ON_COMMAND(ID_MAIN_NEW, &CRegisterViewerDlg::OnMainNew)
//ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSEWHEEL()
    ON_EN_KILLFOCUS(IDC_CELLEDIT, &CRegisterViewerDlg::OnEnKillfocusCelledit)
//ON_BN_CLICKED(IDC_SAVETODB, &CRegisterViewerDlg::OnBnClickedSavetodb)
    ON_BN_CLICKED(IDOK, &CRegisterViewerDlg::OnBnClickedOk)
    ON_WM_DESTROY()
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CRegisterViewerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    LoadDataFromDB();
    Fresh_GridCol();
    if (!is_connect())
    {
        AfxMessageBox(_T("Please Connect to your device,firstly!"));
        return FALSE;
    }

    m_curPage=1;
    Show_ColorData();
// 	GetDlgItem(IDC_STATIC_PRODUCTNAME)->ShowWindow(FALSE);
// 	GetDlgItem(IDC_EDIT_PRODUCTNAME)->ShowWindow(FALSE);



    pParamBackFresh=AfxBeginThread(BackRegisterViewerFreshProc,this);
    pParamBackFresh->m_bAutoDelete =FALSE;
    return TRUE;
}

void CRegisterViewerDlg::LoadDataFromDB()
{
    CString m_cur_TableName,m_cur_Col_RegName,m_cur_col_RegAddress,m_modelname;
    m_VecregisterData.clear();
    CString ProductModelName =Get_ProductModel();
    _variant_t temp_var;
    if (product_type==T3000_5ABCDFG_LED_ADDRESS||product_type==T3000_5EH_LCD_ADDRESS||product_type==T3000_6_ADDRESS)//Tstat serial
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
        if (SqliteDBT3000.tableExists("T3000_Register_Address_By_ID"))
        {
            CString sql,temp;
            DBRegister tempstruct;
            sql.Format(_T("Select * from T3000_Register_Address_By_ID order by Register_Address"));
            q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);



            while (!q.eof())//�б���û�ж�Ӧ���кŵ�ֵ
            {
                if (product_type==T3000_5ABCDFG_LED_ADDRESS)
                {
                    tempstruct.Register_Address=q.getIntField("Register_Address");
                    tempstruct.AddressName=q.getValuebyName(_T("TSTAT5_LED_AddressName"));
					if (tempstruct.AddressName.CompareNoCase(L"MODBUS_INFO_BYTE") == 0)
					{
						tempstruct.AddressName = L"MODBUS_OCC_COMDELAY";
					}
                    tempstruct.DataType=q.getValuebyName(_T("TSTAT5_LED_DATATYPE"));
                    tempstruct.length=q.getIntField("TSTAT5_LED_LEN");
                    tempstruct.Description=q.getValuebyName(_T("TSTAT5_LED_INSTRUCTION"));
                    
                    tempstruct.Operation=q.getValuebyName(_T("TSTAT5_LED_Operation"));
                    
                   
                }
                else if (product_type==T3000_5EH_LCD_ADDRESS)
                {
                    _variant_t vartemp;
                    tempstruct.Register_Address=q.getIntField("Register_Address");
                    tempstruct.AddressName=q.getValuebyName(_T("TSTAT5_LCD_AddressName"));
                    tempstruct.DataType=q.getValuebyName(_T("TSTAT5_LCD_DATATYPE"));
                    tempstruct.length=q.getIntField("TSTAT5_LCD_LEN");
                    //tempstruct.Description=(CString)ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_DESCRIPTION"));
                    tempstruct.Description=q.getValuebyName(_T("TSTAT5_LCD_INSTRUCTION"));
                  tempstruct.Operation =  q.getValuebyName(_T("TSTAT5_LCD_Operation"));
                                          
                }
                else if (product_type==T3000_6_ADDRESS)
				{
					 

					 
						tempstruct.Register_Address=q.getIntField("Register_Address");
					 

                    
                        tempstruct.AddressName=q.getValuebyName(_T("TSTAT6_AddressName"));
                    

                  
                        tempstruct.DataType=q.getValuebyName(_T("TSTAT6_DATATYPE"));
                  


                  
                        tempstruct.length=q.getIntField("TSTAT6_LEN");
                   

                     
                        tempstruct.Description=q.getValuebyName(_T("TSTAT6_INSTRUCTION"));
                     
				 
                        tempstruct.Operation=q.getValuebyName(_T("TSTAT6_Operation"));
                   
                }


                if (tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)
                {
                    q.nextRow();
                    continue;
                }
                m_VecregisterData.push_back(tempstruct);
                m_recordcount++;
                q.nextRow();
            }
        }
        SqliteDBT3000.closedb();
    }
    else if (product_type==T3000_T3_MODULES)//T3 Serial
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
		
        if (SqliteDBT3000.tableExists("T3_RegisterList"))
        {
            CString sql,temp;
            DBRegister tempstruct;
            sql.Format(_T("Select * from T3_RegisterList order by RegID"));
            q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
            if (m_modelno==PM_T3PT10)
            {
                while(!q.eof())
                {
                    tempstruct.Register_Address=q.getIntField("RegID");
                    tempstruct.AddressName=q.getValuebyName(_T("T3-RTD"));
                   
                    if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                    {
                        q.nextRow();
                        continue;
                    }
                    tempstruct.DataType=q.getValuebyName(_T("T3-8AI8AO_DATATYPE"));
                    tempstruct.length=q.getIntField("T3-8AI8AO_LEN");
                    tempstruct.Description=q.getValuebyName(L"T3-8AI8AO_DESCRIPTION");
                  
                    
                        tempstruct.Operation=q.getValuebyName(_T("T3-8AI8AO_OPERATION"));
                    
                    m_VecregisterData.push_back(tempstruct);
                    m_recordcount++;
                    q.nextRow();
                }
            }
            else if (m_modelno==PM_T38AI16O)
            {
                while(!q.eof())
                {
                    tempstruct.Register_Address=q.getIntField("RegID");
                   
                    tempstruct.AddressName=q.getValuebyName(L"T3-8AI16O");
                    
                    if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                    {
                       q.nextRow();
                        continue;
                    }
                    tempstruct.DataType=q.getValuebyName(_T("T3-8AI16O_DATATYPE"));
                    tempstruct.length=q.getIntField("T3-8AI16O_LEN");
                    
                        tempstruct.Description=q.getValuebyName(_T("T3-8AI16O_DESCRIPTION"));;
                   
                  
                        tempstruct.Operation=q.getValuebyName(_T("T3-8AI16O_OPERATION"));;
                   
                    m_VecregisterData.push_back(tempstruct);
                    m_recordcount++;
                    q.nextRow();
                }
            }
            else if (m_modelno==PM_T3IOA)
            {
                while(!q.eof())
                {
                    tempstruct.Register_Address=q.getIntField("RegID");
                    
                        tempstruct.AddressName=q.getValuebyName(_T("T3-8AI8AO"));
                   
                    if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                    {
                       q.nextRow();
                        continue;
                    }
                    tempstruct.DataType=q.getValuebyName(_T("T3-8AI8AO_DATATYPE"));
                    tempstruct.length=q.getIntField("T3-8AI8AO_LEN");
                     
                        tempstruct.Description=q.getValuebyName(_T("T3-8AI8AO_DESCRIPTION"));
                     
                   
                        tempstruct.Operation=q.getValuebyName(_T("T3-8AI8AO_OPERATION"));
                    
                    m_VecregisterData.push_back(tempstruct);
                    m_recordcount++; 
                   q.nextRow();
                }
            }
            else if (m_modelno==PM_T332AI)
            {
                while(!q.eof())
                {
                    tempstruct.Register_Address=q.getIntField("RegID");
                    tempstruct.AddressName=q.getValuebyName(_T("T3-32AI"));
                    if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                    {
                       q.nextRow();
                        continue;
                    }
                    tempstruct.DataType=q.getValuebyName(_T("T3-32AI_DATATYPE"));
                    tempstruct.length=q.getIntField("T3-32AI_LEN");
                    tempstruct.Description=q.getValuebyName(_T("T3-32AI_DESCRIPTION"));
                    
                    tempstruct.Operation=q.getValuebyName(_T("T3-32AI_OPERATION"));
                     
                    m_VecregisterData.push_back(tempstruct);
                    m_recordcount++;
                    q.nextRow();
                }
            }
            else if (m_modelno==PM_T34AO)
            {
                while(!q.eof())
                {
                    tempstruct.Register_Address=q.getIntField("RegID");
                    tempstruct.AddressName=q.getValuebyName(_T("T3-4AO"));
                   
                    if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                    {
                        q.nextRow();
                        continue;
                    }
                    tempstruct.DataType=q.getValuebyName(_T("T3-4AO_DATATYPE"));
                    tempstruct.length=q.getIntField("T3-4AO_LEN");
                     tempstruct.Description=q.getValuebyName(_T("T3-4AO_DESCRIPTION"));
                     
                    tempstruct.Operation=q.getValuebyName(_T("T3-4AO_OPERATION"));
                  
                    m_VecregisterData.push_back(tempstruct);
                    m_recordcount++;
                    q.nextRow();

                }
            }
            else if (m_modelno==PM_T36CT)
            {
                while(!q.eof())
                {
                    tempstruct.Register_Address=q.getIntField("RegID");
                    tempstruct.AddressName=q.getValuebyName(_T("T3-6CT"));
                    
                    if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                    {
                       q.nextRow();
                        continue;
                    }
                    tempstruct.DataType=q.getValuebyName(_T("T3-6CT_DATATYPE"));
                    tempstruct.length=q.getIntField("T3-6CT_LEN");
                    tempstruct.Description=q.getValuebyName(_T("T3-6CT_DESCRIPTION"));
                    
                     tempstruct.Operation=q.getValuebyName(_T("T3-6CT_OPERATION"));
                     
                    m_VecregisterData.push_back(tempstruct);
                    m_recordcount++;
                    q.nextRow();
                }
            }

        }
        SqliteDBT3000.closedb();
    }
    else if (product_type == PM_AirQuality||product_type == PM_HUM_R)
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

        if (SqliteDBT3000.tableExists("AirQuanlity_Reglist"))
        {
            CString sql,temp;
            DBRegister tempstruct;
            sql.Format(_T("Select * from AirQuanlity_Reglist order by RegID"));
           q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);

            while(!q.eof())
            {
                tempstruct.Register_Address=q.getIntField("RegID");
                tempstruct.AddressName=q.getValuebyName(_T("RegName"));
              
                if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                {
                    q.nextRow();
                    continue;
                }
                tempstruct.DataType=L"Unsigned";
                tempstruct.length=1;
                tempstruct.Description=q.getValuebyName(_T("RegDescription"));
               

                tempstruct.Operation=L"R";

                m_VecregisterData.push_back(tempstruct);
                m_recordcount++;
               q.nextRow();
            }


        }
        SqliteDBT3000.closedb();

    }
    else if (product_type == PM_CM5)
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

        if (SqliteDBT3000.tableExists("CM5"))
        {
            CString sql,temp;
            DBRegister tempstruct;
            sql.Format(_T("Select * from CM5 order by RegID"));
          q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);

            while(!q.eof())
            {
                tempstruct.Register_Address=q.getIntField("RegID");
                tempstruct.AddressName=q.getValuebyName(_T("T3000_ID"));
              
                if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                {
                   q.nextRow();
                    continue;
                }
                tempstruct.DataType=L"Unsigned";

                tempstruct.length=q.getIntField("Length");


                tempstruct.Description=q.getValuebyName(_T("Description"));
                

                tempstruct.Operation=L"R";

                m_VecregisterData.push_back(tempstruct);
                m_recordcount++;
                q.nextRow();
            }


        }
        SqliteDBT3000.closedb();
    }
    else if (product_type == PM_MINIPANEL)
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

        if (SqliteDBT3000.tableExists("MiniPanel_Registerlist"))
        {
            CString sql,temp;
            DBRegister tempstruct;
            sql.Format(_T("Select * from MiniPanel_Registerlist order by RegID"));
           SqliteDBT3000.execDML((UTF8MBSTR)sql);

            while(!q.eof())
            {
                tempstruct.Register_Address=q.getIntField("RegID");
                tempstruct.AddressName=q.getValuebyName(_T("Reg_Name"));
               
                if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                {
                    q.nextRow();
                    continue;
                }
                tempstruct.DataType=L"Unsigned";
                tempstruct.length=1;
                temp_var=q.getValuebyName(_T("Register_Description"));
                if (temp_var.vt==VT_NULL)
                {
                    tempstruct.Description=_T("");
                }
                else
                {
                    tempstruct.Description=temp_var;
                }

                tempstruct.Operation=L"R";

                m_VecregisterData.push_back(tempstruct);
                m_recordcount++;
                q.nextRow();
            }


        }
        SqliteDBT3000.closedb();
    }
    else if (product_type == CS3000)
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

        if (SqliteDBT3000.tableExists("CS3000"))
        {
            CString sql,temp;
            DBRegister tempstruct;
            sql.Format(_T("Select * from CS3000 order by RegID"));
            
			q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
            while(!q.eof())
            {
                tempstruct.Register_Address=q.getIntField("RegID");
                tempstruct.AddressName=q.getValuebyName(_T("RegName"));
                
                if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                {
                   q.nextRow();
                    continue;
                }
                tempstruct.DataType=L"Unsigned";
                tempstruct.length=1;
                tempstruct.Description=q.getValuebyName(_T("RegDiscription"));
             

                tempstruct.Operation=L"R";

                m_VecregisterData.push_back(tempstruct);
                m_recordcount++;
                q.nextRow();
            }


        }
        SqliteDBT3000.closedb();
    }
    else if (product_type == PM_PRESSURE_SENSOR)
    {
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
        if (SqliteDBT3000.tableExists("PS_Registerlist"))
        {
            CString sql,temp;
            DBRegister tempstruct;
            sql.Format(_T("Select * from PS_Registerlist order by RegID"));
            q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);

            while(!q.eof())
            {
                tempstruct.Register_Address=q.getIntField("RegID");
                tempstruct.AddressName=q.getValuebyName(_T("Reg_Name"));
               
                if ((tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)||tempstruct.AddressName.IsEmpty())
                {
                    q.nextRow();
                    continue;
                }
                tempstruct.DataType=L"Unsigned";
                tempstruct.length=1;
                 tempstruct.Description=q.getValuebyName(_T("Reg_Description"));
                

                tempstruct.Operation=L"R";

                m_VecregisterData.push_back(tempstruct);
                m_recordcount++;
               q.nextRow();
            }


        }
        SqliteDBT3000.closedb();
    }
    /* else if (product_type == PM_TSTAT6_HUM_Chamber)
     {

     }*/
    else	//other products ,there are no the property of data type ,data length and so on .
    {
// 		CADO ado;
// 		ado.OnInitADOConn();
// 		if(ado.IsHaveTable(ado,_T("ProductsTypeRegisterTables")))
// 		{
// 			CString sql,temp;
// 			sql.Format(_T("Select * from ProductsTypeRegisterTables where ProductType=%d"),product_type);
//
// 			ado.m_pRecordset=ado.OpenRecordset(sql);
// 			if (!ado.m_pRecordset->EndOfFile)
// 			{
// 				temp_var=ado.m_pRecordset->GetCollect(_T("TableName"));
// 				if (temp_var.vt!=VT_NULL)
// 				{
// 					temp=temp_var;
// 				}
// 				else
// 				{
// 					temp=_T("");
// 				}
// 				m_cur_TableName=temp;
// 				temp_var=ado.m_pRecordset->GetCollect(_T("Col_RegName"));
// 				if (temp_var.vt!=VT_NULL)
// 				{
// 					temp=temp_var;
// 				}
// 				else
// 				{
// 					temp=_T("");
// 				}
// 				m_cur_Col_RegName=temp;
// 				temp_var=ado.m_pRecordset->GetCollect(_T("Col_RegAddress"));
// 				if (temp_var.vt!=VT_NULL)
// 				{
// 					temp=temp_var;
// 				}
// 				else
// 				{
// 					temp=_T("");
// 				}
// 				m_cur_col_RegAddress=temp;
// 				temp_var=ado.m_pRecordset->GetCollect(_T("ProductName"));
// 				if (temp_var.vt!=VT_NULL)
// 				{
// 					m_modelname=temp_var;
// 				}
// 				else
// 				{
// 					m_modelname=_T("");
// 				}
//
// 			}
// 		}
//
// 		if (!m_cur_TableName.IsEmpty())
// 		{
// 			CString sql,temp;
// 			DBRegister tempstruct;
// 			sql.Format(_T("Select [%s] , [%s]  from [%s] order by [%s]"),m_cur_Col_RegName,m_cur_col_RegAddress,m_cur_TableName,m_cur_col_RegAddress);
// 			ado.m_pRecordset=ado.OpenRecordset(sql);
//
// 			while (!ado.m_pRecordset->EndOfFile)
// 			{
//
// 				temp_var=ado.m_pRecordset->GetCollect(m_cur_Col_RegName.GetBuffer());
// 				if (temp_var.vt!=VT_NULL)
// 				{
// 					tempstruct.AddressName=temp_var;
// 				}
// 				temp_var=ado.m_pRecordset->GetCollect(RegName.GetBuffer());
//
// 				if (temp_var.vt!=VT_NULL)
// 				{
// 					tempstruct.RegName=temp_var;
// 				}
// 				m_VecregisterData.push_back(tempstruct);
// 				ado.m_pRecordset->MoveNext();
// 			}
// 		}
    }


    CString temp;
    m_Datapages=(m_recordcount/ROWS)+1;
    temp.Format(_T("Product Name"));
    GetDlgItem(IDC_STATIC_PRODUCTNAME)->SetWindowText(temp);

    GetDlgItem(IDC_EDIT_PRODUCTNAME)->SetWindowText(ProductModelName);
//GetProductName(product_register_value[7])

}
void CRegisterViewerDlg::ShowCurPage()
{
    CString temp;

//GetDlgItem(IDC_PAGES)->SetWindowText(temp);

    int startAdd=0;
    int endAdd=m_VecregisterData.size();

    for (int i=startAdd; i<endAdd; i++)
    {

        temp.Format(_T("%d"),m_VecregisterData[i].Register_Address);
        m_registerlistGrid.put_TextMatrix(i+1,m_regno,temp);
        if (product_register_value[m_VecregisterData[i].Register_Address]==65535)
        {
            temp=_T("0");
        }
        else
        {
            temp.Format(_T("%d"),product_register_value[m_VecregisterData[i].Register_Address]);
        }

        m_registerlistGrid.put_TextMatrix(i+1,m_value,temp);
        m_registerlistGrid.put_TextMatrix(i+1,m_name,m_VecregisterData[i].AddressName);
        m_registerlistGrid.put_TextMatrix(i+1,m_type,m_VecregisterData[i].DataType);
        temp.Format(_T("%d"),m_VecregisterData[i].length);
        m_registerlistGrid.put_TextMatrix(i+1,m_length,temp);
        m_registerlistGrid.put_TextMatrix(i+1,m_description,m_VecregisterData[i].Description);
        m_registerlistGrid.put_TextMatrix(i+1,m_Operation,m_VecregisterData[i].Operation);
    }
}

void CRegisterViewerDlg::Show_ColorData()
{
    CString temp;
    int startAdd=0;
    int endAdd=m_VecregisterData.size();
    for (int i=startAdd; i<endAdd; i++)
    {
        for(int j=0; j<m_registerlistGrid.get_Cols(); j++)
        {
            m_registerlistGrid.put_Row(i+1);
            m_registerlistGrid.put_Col(j);
            m_registerlistGrid.put_CellBackColor(WRITE_COLOR);
        }
    }
    for (int i=startAdd; i<endAdd; i++)
    {
        if ((m_VecregisterData[i].Operation.CompareNoCase(_T("R"))==0)&&(m_VecregisterData[i].Operation.CompareNoCase(_T("R/W"))!=0))
        {
            for (int j=0; j<m_registerlistGrid.get_Cols(); j++)
            {
                m_registerlistGrid.put_Row(i+1);
                m_registerlistGrid.put_Col(j);
                m_registerlistGrid.put_CellBackColor(DIMGREY);
            }
        }

        temp.Format(_T("%d"),m_VecregisterData[i].Register_Address);
        m_registerlistGrid.put_TextMatrix(i+1,m_regno,temp);

        if (product_register_value[m_VecregisterData[i].Register_Address]==65535)
        {
            temp=_T("0");
        }
        else
        {
            temp.Format(_T("%d"),product_register_value[m_VecregisterData[i].Register_Address]);
        }

        m_registerlistGrid.put_TextMatrix(i+1,m_value,temp);
        m_registerlistGrid.put_TextMatrix(i+1,m_name,m_VecregisterData[i].AddressName);
        m_registerlistGrid.put_TextMatrix(i+1,m_type,m_VecregisterData[i].DataType);
        temp.Format(_T("%d"),m_VecregisterData[i].length);
        m_registerlistGrid.put_TextMatrix(i+1,m_length,temp);
        m_registerlistGrid.put_TextMatrix(i+1,m_description,m_VecregisterData[i].Description);
        m_registerlistGrid.put_TextMatrix(i+1,m_Operation,m_VecregisterData[i].Operation);
    }
}

//void CRegisterViewerDlg::OnBnClickedLeft()
//{
//	if (m_curPage>1)
//	{
//		--m_curPage;
//		Show_ColorData();
//		m_registerlistGrid.put_TopRow(0);
//	}
//}



//void CRegisterViewerDlg::OnBnClickedRight()
//{
//	// TODO: Add your control notification handler code here
//	if (m_curPage<m_Datapages)
//	{
//	  ++m_curPage;
//	  Show_ColorData();
//	  m_registerlistGrid.put_TopRow(0);
//	}
//}


//void CRegisterViewerDlg::OnBnClickedBegin()
//{
//	// TODO: Add your control notification handler code here
//	m_curPage=1;
//	Show_ColorData();
//	m_registerlistGrid.put_TopRow(0);
//}


//void CRegisterViewerDlg::OnBnClickedEnd()
//{
//	// TODO: Add your control notification handler code here
//	m_curPage=m_Datapages;
//	Show_ColorData();
//	m_registerlistGrid.put_TopRow(0);
//}
BEGIN_EVENTSINK_MAP(CRegisterViewerDlg, CDialog)
//	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_MOUSEDOWN, CRegisterViewerDlg::MouseDownInputMsflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_MOUSEMOVE, CRegisterViewerDlg::MouseMoveInputMsflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_MOUSEUP, CRegisterViewerDlg::MouseUpInputMsflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_CLICK, CRegisterViewerDlg::ClickInputMsflexgrid, VTS_NONE)
//	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, 73, CRegisterViewerDlg::ScrollInputMsflexgrid, VTS_NONE)
//ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, 73, CRegisterViewerDlg::ScrollInputMsflexgrid, VTS_NONE)
ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_DBLCLICK, CRegisterViewerDlg::DblClickInputMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()


//void CRegisterViewerDlg::MouseDownInputMsflexgrid(short Button, short Shift, long x, long y)
//{
//	// TODO: Add your message handler code here
//	if (Button&0x1)
//	{
//		m_SelCurCol=m_registerlistGrid.get_ColSel();
//		m_SelCurRow=m_registerlistGrid.get_RowSel();
//
//		if (m_SelCurRow!=0)
//		{
//			return;
//		}
//		m_CurColName=m_registerlistGrid.get_TextMatrix(0,m_SelCurCol);
//		m_ColName.SetWindowText(m_CurColName);
//		m_ColName.ShowWindow(SW_SHOW);
//
//		m_registerlistGrid.put_BackColorSel(m_registerlistGrid.get_GridColorFixed());
//	}
//
//}


//void CRegisterViewerDlg::MouseMoveInputMsflexgrid(short Button, short Shift, long x, long y)
//{
//
//
//	 if (Button&0x1) // 1 - vbLeftButton
//	 {
//		 if (m_SelCurRow!=0)
//		 {
//			 return;
//		 }
//			CRect rect;
//			m_registerlistGrid.GetWindowRect(&rect);                 //��ȡFlexGrid�ؼ��Ĵ��ھ���
//			ScreenToClient(&rect);                                    //ת��Ϊ�ͻ�������
//			CDC* pDC=GetDC();
//			//MSFlexGrid �ؼ��ĺ����ĳ��ȵ�λ�ǡ��(twips)������Ҫ����ת��Ϊ���أ�1440 � = 1 Ӣ��
//			//�������ص��羵�ת������
//			int nTwipsPerDotX=1440/pDC->GetDeviceCaps(LOGPIXELSX);
//			int nTwipsPerDotY=1440/pDC->GetDeviceCaps(LOGPIXELSY);
//
//			//x = m_registerlistGrid.get_ColPos(lCol)/nTwipsPerDotX;
//			y = m_registerlistGrid.get_RowPos(0)/nTwipsPerDotY;
//			//long x = m_outputname.get_ColPos(lCol)/nTwipsPerDotX;
//			int left=m_registerlistGrid.get_CellLeft()/nTwipsPerDotY-1;
//			int top=m_registerlistGrid.get_CellTop()/nTwipsPerDotX-1;
//
//			long width = m_registerlistGrid.get_ColWidth(m_SelCurCol)/nTwipsPerDotX+1;
//			long height = m_registerlistGrid.get_RowHeight(m_SelCurRow)/nTwipsPerDotY+1;
//			//�γ�ѡ�и����ڵľ�������
//			CRect rc(x,y,x+width,y+height);
//			//ת������ԶԻ��������
//			rc.OffsetRect(rect.left,rect.top);
//			m_ColName.MoveWindow(rc);
//			m_registerlistGrid.put_BackColorSel(m_registerlistGrid.get_GridColor());
//
//
//	 }
//	                     //�ı��С���Ƶ�ѡ�и�λ��
//}


//void CRegisterViewerDlg::MouseUpInputMsflexgrid(short Button, short Shift, long x, long y)
//{
//	// TODO: Add your message handler code here
//	if (Button&0x1)
//	{  if (m_SelCurRow!=0)
//	{
//		return;
//	}
//		int row = m_registerlistGrid.get_MouseRow();
//		int col = m_registerlistGrid.get_MouseCol();
//
//		m_registerlistGrid.put_ColPosition(m_SelCurCol,col);
//		m_registerlistGrid.put_BackColorSel(m_registerlistGrid.get_GridColorFixed());
//		m_ColName.ShowWindow(SW_HIDE);
//		//Write_GridCol();
//
//	}
//
//}



void CRegisterViewerDlg::Fresh_GridCol()
{

    m_registerlistGrid.Clear();
    /* m_VecregisterData.clear();*/
    CString inifilepath=g_strExePth;
    inifilepath+=_T("\\Config\\GRIDCONFIG.ini");
    m_Inifile.SetFileName(inifilepath);
    m_regno=m_Inifile.GetProfileInt(SECTION,_T("RegNo"),0);
    m_value=m_Inifile.GetProfileInt(SECTION,_T("Value"),1);
    m_name=m_Inifile.GetProfileInt(SECTION,_T("Name"),2);
    m_type=m_Inifile.GetProfileInt(SECTION,_T("Data Type"),3);
    m_length=m_Inifile.GetProfileInt(SECTION,_T("Data Length"),4);
    m_description=m_Inifile.GetProfileInt(SECTION,_T("Description"),5);
    m_Operation=m_Inifile.GetProfileInt(SECTION,_T("Operation"),6);

    m_registerlistGrid.put_Rows(m_VecregisterData.size()+1);
    m_registerlistGrid.put_Cols(COLS);
    m_registerlistGrid.put_TextMatrix(0,m_regno,_T("RegNo"));
    m_registerlistGrid.put_Row(0);
    m_registerlistGrid.put_Col(m_regno);
    m_registerlistGrid.put_CellBackColor(BACK_CELL);

    m_registerlistGrid.put_TextMatrix(0,m_value,_T("Value"));
    if (m_CanEdit)
    {
        m_registerlistGrid.put_ColWidth(m_value,0);
    }

    m_registerlistGrid.put_Row(0);
    m_registerlistGrid.put_Col(m_value);
    m_registerlistGrid.put_CellBackColor(BACK_CELL);


    m_registerlistGrid.put_TextMatrix(0,m_name,_T("Name"));
    m_registerlistGrid.put_ColWidth(m_name,3500);
    m_registerlistGrid.put_Row(0);
    m_registerlistGrid.put_Col(m_name);
    m_registerlistGrid.put_CellBackColor(BACK_CELL);

    m_registerlistGrid.put_TextMatrix(0,m_type,_T("Data Type"));
    m_registerlistGrid.put_Row(0);
    m_registerlistGrid.put_Col(m_type);
    m_registerlistGrid.put_CellBackColor(BACK_CELL);

    m_registerlistGrid.put_TextMatrix(0,m_length,_T("Data Length"));
    m_registerlistGrid.put_Row(0);
    m_registerlistGrid.put_Col(m_length);
    m_registerlistGrid.put_CellBackColor(BACK_CELL);

    m_registerlistGrid.put_TextMatrix(0,m_description,_T("Description"));
    m_registerlistGrid.put_Row(0);
    m_registerlistGrid.put_Col(m_description);
    m_registerlistGrid.put_CellBackColor(BACK_CELL);

    m_registerlistGrid.put_TextMatrix(0,m_Operation,_T("Operation"));
    m_registerlistGrid.put_Row(0);
    m_registerlistGrid.put_Col(m_Operation);
    m_registerlistGrid.put_CellBackColor(BACK_CELL);

}

void CRegisterViewerDlg::Write_GridCol()
{
    CString name;
    for (int i=0; i<6; i++)
    {
        name=m_registerlistGrid.get_TextMatrix(0,i);
        m_Inifile.WriteProfileInt(SECTION,name,i);
    }

    CString inifilepath=g_strExePth;
    inifilepath+=_T("\\Config\\GRIDCONFIG.ini");
    m_Inifile.SetFileName(inifilepath);
    m_regno=m_Inifile.GetProfileInt(SECTION,_T("RegNo"),0);
    m_value=m_Inifile.GetProfileInt(SECTION,_T("Value"),1);
    m_name=m_Inifile.GetProfileInt(SECTION,_T("Name"),2);
    m_type=m_Inifile.GetProfileInt(SECTION,_T("Data Type"),3);
    m_length=m_Inifile.GetProfileInt(SECTION,_T("Data Length"),4);
    m_description=m_Inifile.GetProfileInt(SECTION,_T("Description"),5);
    m_Operation=m_Inifile.GetProfileInt(SECTION,_T("Operation"),6);
}

HBRUSH CRegisterViewerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here
    switch(pWnd->GetDlgCtrlID())
    {
    case IDC_COLNAME:
    {
        pDC->SetTextColor(RGB(255,255,255));
        pDC->SetBkColor(RGB(0,255,255));
        pDC->SetBkMode(TRANSPARENT);
        HBRUSH B = CreateSolidBrush(RGB(0,0,0));
        return (HBRUSH)B;
    }
    }
    // TODO:  Return a different brush if the default is not desired
    return hbr;
}


//void CRegisterViewerDlg::OnBnClickedAddnewproduct()
//{
//
//}


void CRegisterViewerDlg::OnMainNew()
{
    m_CanEdit=TRUE;

    Fresh_GridCol();
}


void CRegisterViewerDlg::ClickInputMsflexgrid()
{

    m_inNameEdt.ShowWindow(SW_HIDE);
    long lRow,lCol;
    lRow = m_registerlistGrid.get_RowSel();//��ȡ������к�
    lCol = m_registerlistGrid.get_ColSel(); //��ȡ������к�
    TRACE(_T("Click input grid!\n"));

    CRect rect;
    m_registerlistGrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
    ScreenToClient(rect); //ת��Ϊ�ͻ�������
    CDC* pDC =GetDC();

    int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
    int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
    //����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
    long y = m_registerlistGrid.get_RowPos(lRow)/nTwipsPerDotY;
    long x = m_registerlistGrid.get_ColPos(lCol)/nTwipsPerDotX;
    //����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
    long width = m_registerlistGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
    long height = m_registerlistGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
    //�γ�ѡ�и����ڵľ�������
    CRect rcCell(x,y,x+width,y+height);
    //ת������ԶԻ��������
    rcCell.OffsetRect(rect.left+1,rect.top+1);
    ReleaseDC(pDC);
    CString strValue = m_registerlistGrid.get_TextMatrix(lRow,lCol);
    m_oldname=strValue;
    m_SelCurCol=lCol;
    m_SelCurRow=lRow;

    if (!m_CanEdit)
    {
        /* m_registerlistGrid.put_Row(m_SelCurRow);
        m_registerlistGrid.put_Col(m_SelCurCol);
        unsigned long color=m_registerlistGrid.get_CellBackColor();
        if (BACK_CELL==color)
        {
        m_registerlistGrid.put_Row(m_SelCurRow);
        m_registerlistGrid.put_Col(m_SelCurCol);
        m_registerlistGrid.put_CellBackColor(WRITE_COLOR);

        }
        else
        {
        m_registerlistGrid.put_Row(m_SelCurRow);
        m_registerlistGrid.put_Col(m_SelCurCol);
        m_registerlistGrid.put_CellBackColor(BACK_CELL);
        }*/

        return;
    }
    m_inNameEdt.MoveWindow(&rcCell,1);
    m_inNameEdt.ShowWindow(SW_SHOW);
    m_inNameEdt.SetWindowText(strValue);
    m_inNameEdt.SetFocus();
    m_inNameEdt.SetCapture();//LSC
    int nLenth=strValue.GetLength();
    m_inNameEdt.SetSel(nLenth,nLenth); //ȫѡ//
}


//void CRegisterViewerDlg::ScrollInputMsflexgrid()
//{
//
//}


//void CRegisterViewerDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// This feature requires Windows Vista or greater.
//	// The symbol _WIN32_WINNT must be >= 0x0600.
//	// TODO: Add your message handler code here and/or call default
//	RECT rc;
//	m_registerlistGrid.GetWindowRect(&rc);
//	CRect rect(rc);
//	if(rect.PtInRect(pt))
//	{
//		long lTopRow = m_registerlistGrid.get_TopRow();
//		lTopRow = (zDelta<0) ? (lTopRow+1) : (lTopRow-1);
//		if(lTopRow<1 || lTopRow>m_registerlistGrid.get_Rows())
//		{
//			 return  ;
//		}
//		m_registerlistGrid.put_TopRow(lTopRow);
//	}
//	CDialog::OnMouseHWheel(nFlags, zDelta, pt);
//}


//void CRegisterViewerDlg::ScrollInputMsflexgrid()
//{
//
//}


BOOL CRegisterViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    RECT rc;
    m_registerlistGrid.GetWindowRect(&rc);
    CRect rect(rc);
    if(rect.PtInRect(pt))
    {
        long lTopRow = m_registerlistGrid.get_TopRow();
        lTopRow = (zDelta<0) ? (lTopRow+1) : (lTopRow-1);
        if(lTopRow<0 || lTopRow>m_registerlistGrid.get_Rows())
        {
            return TRUE ;
        }
        m_registerlistGrid.put_TopRow(lTopRow);
    }
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CRegisterViewerDlg::OnEnKillfocusCelledit()
{

}


void CRegisterViewerDlg::DblClickInputMsflexgrid()
{

    m_inNameEdt.ShowWindow(SW_HIDE);
    long lRow,lCol;
    lRow = m_registerlistGrid.get_RowSel();//��ȡ������к�
    lCol = m_registerlistGrid.get_ColSel(); //��ȡ������к�
    TRACE(_T("Click input grid!\n"));

    CRect rect;
    m_registerlistGrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
    ScreenToClient(rect); //ת��Ϊ�ͻ�������
    CDC* pDC =GetDC();

    int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
    int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
    //����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
    long y = m_registerlistGrid.get_RowPos(lRow)/nTwipsPerDotY;
    long x = m_registerlistGrid.get_ColPos(lCol)/nTwipsPerDotX;
    //����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
    long width = m_registerlistGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
    long height = m_registerlistGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
    //�γ�ѡ�и����ڵľ�������
    CRect rcCell(x,y,x+width,y+height);
    //ת������ԶԻ��������
    rcCell.OffsetRect(rect.left+1,rect.top+1);
    ReleaseDC(pDC);
    CString strValue = m_registerlistGrid.get_TextMatrix(lRow,lCol);
    m_oldname=strValue;
    m_SelCurCol=lCol;
    m_SelCurRow=lRow;

    m_regno=m_Inifile.GetProfileInt(SECTION,_T("RegNo"),0);
    m_value=m_Inifile.GetProfileInt(SECTION,_T("Value"),1);
    m_name=m_Inifile.GetProfileInt(SECTION,_T("Name"),2);
    m_type=m_Inifile.GetProfileInt(SECTION,_T("Data Type"),3);
    //m_length=m_Inifile.GetProfileInt(SECTION,_T("Data Length"),4);
    m_description=m_Inifile.GetProfileInt(SECTION,_T("Description"),5);
    m_Operation=m_Inifile.GetProfileInt(SECTION,_T("Operation"),6);

// 		m_registerlistGrid.put_Row(m_SelCurRow);
// 		m_registerlistGrid.put_Col(m_SelCurCol);
// 		m_registerlistGrid.put_CellBackColor(WRITE_COLOR);
    if (m_CanEdit)
    {
        return;
    }

    if ((m_SelCurCol==m_value)&&m_SelCurRow!=0)
    {
        CString reg_no=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_regno);
        CString reg_value=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_value);
        CString reg_name=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_name);
        CString reg_description=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_description);
        CString reg_Type=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_type);
        CString reg_Operation=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_Operation);
        m_WriteValueDlg.Set(0,reg_Type,reg_name,reg_no,reg_value,reg_description,reg_Operation);
        m_WriteValueDlg.DoModal();
    }
    if ((m_SelCurCol==m_description)&&m_SelCurRow!=0)
    {
        CString reg_no=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_regno);
        CString reg_value=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_value);
        CString reg_name=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_name);
        CString reg_description=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_description);
        CString reg_Type=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_type);
        CString reg_Operation=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_Operation);
        m_WriteValueDlg.Set(1,reg_Type,reg_name,reg_no,reg_value,reg_description,reg_Operation);
        m_WriteValueDlg.DoModal();
    }
}


//void CRegisterViewerDlg::OnBnClickedSavetodb()
//{
//
//}


void CRegisterViewerDlg::OnBnClickedOk()
{

}

UINT BackRegisterViewerFreshProc(LPVOID pParam)
{
    CRegisterViewerDlg* pdlg = (CRegisterViewerDlg*)pParam;

    //if ((pdlg->strparamode.CompareNoCase(_T("Tstat6")) == 0)||(pdlg->strparamode.CompareNoCase(_T("Tstat7")) == 0))
    //	pdlg->Refresh6();
    //else
    //	pdlg->Refresh();
    while(1)
    {
        Sleep(1000);

        pdlg->ShowCurPage();
    }

    return 0;
}

void CRegisterViewerDlg::OnDestroy()
{


    DWORD dwCode;
    if (pParamBackFresh!=NULL)
    {
        GetExitCodeThread(pParamBackFresh->m_hThread,&dwCode);
        if (dwCode==STILL_ACTIVE)
        {
            Sleep(200);

            TerminateThread(pParamBackFresh->m_hThread,dwCode);
            delete pParamBackFresh;
        }
    }
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
}

CString CRegisterViewerDlg::Get_ProductModel()
{
    CString ModelName=_T("");
    if (!is_connect())
    {
        AfxMessageBox(_T("Please Connect to your device,firstly!"));
    }
    else
    {
        m_modelno=read_one(g_tstat_id,7,20);
        if (m_modelno>0)
        {
            ModelName=GetProductName(m_modelno);
        }
        else
        {
            AfxMessageBox(_T("Can't read your device"));
        }
    }
    return ModelName;
}

void CRegisterViewerDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    if (nType==SIZE_RESTORED)
    {

        CRect ViewRect;
        GetClientRect(&ViewRect);
        //TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
        // m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
        if (m_registerlistGrid.GetSafeHwnd())
        {
            m_registerlistGrid.MoveWindow(CRect(ViewRect.left+10,ViewRect.top+40,ViewRect.Width(),ViewRect.Height()),TRUE);
        }

    }
    // TODO: Add your message handler code here
}
