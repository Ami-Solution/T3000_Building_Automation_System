 
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "T3000.h"

#include "MainFrm.h"
#include "T3000Doc.h"
#include "T3000TableView.h"
#include "T3000view.h"
#include "WorkspaceBar.h"

#include "global_variable_extern.h"
#include "globle_function.h"
#include "AddBuilding.h"
#include "ScanSelectDlg.h"

#include "MannageBuidingDlg.h"
#include "AllNodesDiaolg.h"
#include "GridFlashDlg.h"
#include "ScanDbWaitDlg.h"
#include "ChangePasswordDlg.h"
#include "ManageAccountDlg.h"
#include "LoginDlg.h"
#include "AfxMessageDialog.h"
#include "ImportDatabaseDlg.h"
#include "ToolCalibrateDlg.h"
#include "EreaseDlg.h"
#include "ToolCalibrateDlg.h"
#include "hangeIDDlg.h"
#include "Dialog_Progess.h"
 
#include "ScanDlg.h"
#pragma comment(lib, "Version.lib")

#ifdef _DEBUG
#define new DEBUG_NEW

#endif
// CMainFrame
#define REFRESH_TIMER	1
#define REFRESH_TIMER_VALUE	 35000
#define ID_NODEFAULTMAINBUILDING_MSG 8000
#define SCAN_TIMER 2

CEvent g_killMultiReadEvent;
//CEvent g_pauseMultiReadEvent;
extern CT3000TableView* pTableView;
//BOOL g_bPauseMultiRead=FALSE;

CString g_strExePath;

#define TVINSERV_BUILDING 		{tvInsert.item.iImage=0;tvInsert.item.iSelectedImage=0;}
#define TVINSERV_FLOOR	 			{tvInsert.item.iImage=0;tvInsert.item.iSelectedImage=0;}
#define TVINSERV_ROOM				{tvInsert.item.iImage=2;tvInsert.item.iSelectedImage=2;}
#define TVINSERV_TSTAT 			{tvInsert.item.iImage=6;tvInsert.item.iSelectedImage=6;}
#define TVINSERV_LED 				{tvInsert.item.iImage=8;tvInsert.item.iSelectedImage=8;}
#define TVINSERV_NET_WORK		{tvInsert.item.iImage=12;tvInsert.item.iSelectedImage=12;}
#define TVINSERV_SOLAR				{tvInsert.item.iImage=12;tvInsert.item.iSelectedImage=12;}
#define TVINSERV_CMFIVE			{tvInsert.item.iImage=10;tvInsert.item.iSelectedImage=10;} 
#define TVINSERV_MINIPANEL		{tvInsert.item.iImage=14;tvInsert.item.iSelectedImage=14;}
#define TVINSERV_LC				{tvInsert.item.iImage=16;tvInsert.item.iSelectedImage=16;} 


#define ITEM_MASK				TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset) // 
	ON_COMMAND(ID_BUILDINGBAR,OnCheckBuildingBar)
	ON_UPDATE_COMMAND_UI(ID_BUILDINGBAR, OnUpdateCheckBuildingBar)
	ON_COMMAND(ID_INPUTOUTPUTPANE,OnCheckIOPane)
	ON_UPDATE_COMMAND_UI(ID_INPUTOUTPUTPANE, OnUpdateCheckIOPane)

	ON_COMMAND(ID_FILE_SAVE_CONFIG, OnFileOpen)
	ON_COMMAND(ID_CONFIGFILE_SAVE_AS, SaveConfigFile)
	ON_COMMAND(ID_LOAD_CONFIG,OnLoadConfigFile)
	ON_COMMAND(ID_CONNECT2,OnConnect)
	ON_COMMAND(ID_DISCONNECT2,OnDisconnect)
	ON_COMMAND(ID_SCAN,OnScanDevice)
	//ON_COMMAND(ID_TOOL_SCAN,OnScanDevice)
	ON_COMMAND(ID_BUILDINGCONFIGDB,OnAddBuildingConfig)
	ON_COMMAND(ID_ALLNODESDATABASE,OnAllNodeDatabase)
	ON_COMMAND( ID_FILE_LOADCONFIGFILE,OnLoadConfigFile)

	ON_COMMAND(ID_FILE_IMPORTDATAFROMDATABASEFILE,OnImportDatase)

	ON_COMMAND(ID_RW_INFO, OnLabel)
	ON_COMMAND(ID_BUILDING_INFO, OnLabe2)
	ON_COMMAND(ID_PROTOCOL_INFO, OnLabe3)
	ON_MESSAGE(WM_USER_FRESHSTATUSBAR,OnFreshStatusBar)
	ON_MESSAGE(WM_ADDTREENODE, OnAddTreeNode)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_USERACCOUNT_CHANGEPASSWORD,OnChangeAdminPassword)
	ON_COMMAND(ID_USERACCOUNT_MANNAGEUSER,OnUserMannageMent)
	ON_UPDATE_COMMAND_UI(ID_USERACCOUNT_MANNAGEUSER,OnUserMannageMentUpdate)
	ON_COMMAND(ID_USERACCOUNT_LOGIN,OnUserLogin)
	ON_COMMAND(ID_HELP,OnHelp)
	ON_COMMAND(ID_TOOL_CALIBRATE,OnCaliBrate)
	ON_COMMAND(ID_TOOL_GLOABALOVERRIDE,OnGloabalOverrides)
	ON_COMMAND(ID_TOOL_EREASE,OnToolErease)
	ON_COMMAND(ID_TOOL_FRESH,OnToolFresh)
	ON_COMMAND(ID_TOOL_FRESHLEFTTREEVIEW,OnToolRefreshLeftTreee)

	ON_COMMAND(ID_DATABASE_TEMCOPRODUCTS, &CMainFrame::OnDatabaseTemcoproducts)
	ON_COMMAND(ID_GRID_FLASH, &CMainFrame::OnGridFlashhex)
	ON_COMMAND(ID_ISPTOOLForONE, &CMainFrame::OnIspToolForOne)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	//ID_SEPARATOR,           // status line indicator
	ID_RW_INFO,
	ID_BUILDING_INFO,
	ID_PROTOCOL_INFO,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

unsigned short tempchange[512];
UINT _ScanproductFun(LPVOID pParam)
{
	CMainFrame* pFrame=(CMainFrame*)(pParam);
	CString strMainBuildingName=pFrame->m_strCurMainBuildingName;
	CString strSubScanNet=pFrame->m_strsubNetSelectedScan;


	pFrame->m_bScanFinished=TRUE;
	return 1;
}

UINT __cdecl  _FreshTreeView(LPVOID pParam );

// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
	m_strCurSubBuldingName.Empty();
	m_strCurMainBuildingName.Empty();
	m_nBaudrate=19200;
	m_pThreadScan=NULL;
	m_pFreshMultiRegisters=NULL;
	m_hCurCommunication=NULL;
	m_bScanFinished=FALSE;
	m_nCurSubBuildingIndex=-1;
	m_bScanALL=FALSE;
	m_strIP=_T("");

	//////////////////////////////////////////////////////////////////////////
	m_bEnableRefreshTreeView = TRUE;
	m_pRefreshThread = NULL;

	m_pWaitScanDlg = NULL;
	m_pScanner = NULL;
	FistWdb = TRUE;
}


CMainFrame::~CMainFrame()
{
	if (m_pScanner != NULL)
	{
		delete m_pScanner;
		m_pScanner = NULL;
	}
}
void CMainFrame::InitViews()
{
	m_nCurView = 0;
    CView* pActiveView = GetActiveView();
	if(pActiveView==NULL)
		return;
	m_pViews[0] = pActiveView;
	CDocument* pCurrentDoc = GetActiveDocument();
    CCreateContext newContext;
    newContext.m_pNewViewClass = NULL;
    newContext.m_pNewDocTemplate = NULL;
    newContext.m_pLastView = NULL;
    newContext.m_pCurrentFrame = NULL;
    newContext.m_pCurrentDoc = pCurrentDoc;
	CRect rect(0, 0, 0, 0);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	OnApplicationLook(theApp.m_nAppLook);
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
	CMFCToolBar::SetSizes(CSize(34,34),CSize(34,34));
	CMFCToolBar::SetMenuSizes(CSize(30,30),CSize(24,23));



	UINT uiToolbarHotID = theApp.m_bHiColorIcons ? IDB_BITMAP_V25049 : 0;
	UINT uiToolbarColdID = theApp.m_bHiColorIcons ? IDB_BITMAP_V25049 : 0;
	UINT uiMenuID = theApp.m_bHiColorIcons ? IDB_MENUBAROWN_BITMAP : IDB_MENUBAROWN_BITMAP;


	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	 		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_TOOLBAR_VER25049 : IDR_TOOLBAR_VER25049,uiToolbarColdID, uiMenuID, 
	 			FALSE /* Not locked */, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;//fail to create
	}



	///////////////////////////////////////////////////////////////////////////////////

	if (!m_wndWorkSpace.Create (_T("Building View"), this, CRect (0, 0, 300, 800),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN  | CBRS_LEFT ))//| CBRS_FLOAT_MULTI
	{
		TRACE0("Failed to create Workspace bar\n");
		return FALSE;// fail to create
	}


	//////////////////////////////////////////////////////////////////////////////////
	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);


	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);


	if (!m_wndStatusBar.Create(this)||!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
		
	
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_LEFT);

	
	EnableDocking(CBRS_ALIGN_ANY);	
	
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane (&m_wndWorkSpace);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);


	
	m_wndStatusBar.SetPaneInfo(0,ID_RW_INFO,SBPS_NOBORDERS,   300);

	m_wndStatusBar.SetPaneInfo(1,ID_BUILDING_INFO,SBPS_NOBORDERS, 600);

	m_wndStatusBar.SetPaneInfo(2,ID_PROTOCOL_INFO,SBPS_STRETCH | SBPS_NOBORDERS , 300);

	m_wndStatusBar.SetPaneText(1, _T("No Connection"), TRUE);
	m_wndStatusBar.SetPaneTextColor(0, RGB(224,0,0));

	m_wndStatusBar.EnablePaneDoubleClick ();



	m_pTreeViewCrl=&m_wndWorkSpace.m_TreeCtrl;	
	m_pTreeViewCrl->SetExtendedStyle(TVS_EDITLABELS, TVS_EDITLABELS);




	ScanTstatInDB();





	SetTimer(REFRESH_TIMER, REFRESH_TIMER_VALUE, NULL);






	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp, LPARAM)
{

	UINT uiToolBarId = (UINT) wp;	

	if (IDR_TOOLBAR_VER25049)
	{	
		//int n;
		m_wndToolBar.EnableTextLabels(true);
		//CMFCToolBarMenuButton btnEditor(ID_FILE_SAVE_CONFIG, NULL,GetCmdMgr ()->GetCmdImage (ID_FILE_SAVE_CONFIG), _T("Save"),FALSE);
		//CMFCToolBarMenuButton btnEditor(0, NULL,NULL, _T(""),FALSE);
		//n=GetCmdMgr ()->GetCmdImage (ID_CONFIGFILE_SAVE_AS);
		//m_wndToolBar.ReplaceButton (ID_FILE_OPEN, btnEditor);
		//m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_CONFIGFILE_SAVE_AS),_T("Save"));

		//CMFCToolBarMenuButton btnLoadConfig(ID_LOAD_CONFIG, NULL,GetCmdMgr ()->GetCmdImage (ID_LOAD_CONFIG), _T("Load"),FALSE);
		//CMFCToolBarMenuButton btnLoadConfig(ID_LOAD_CONFIG, NULL,GetCmdMgr ()->GetCmdImage (ID_LOAD_CONFIG), _T(""),FALSE);
		//n=GetCmdMgr ()->GetCmdImage (ID_LOAD_CONFIG);
		//m_wndToolBar.ReplaceButton (ID_LOAD_CONFIG, btnLoadConfig);
		//m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_LOAD_CONFIG),_T("Load"));

		//CMFCToolBarMenuButton btnBurn(ID_FILE_BATCHBURNHEX, NULL,GetCmdMgr ()->GetCmdImage (ID_FILE_BATCHBURNHEX), _T("Burn"),FALSE);
		//CMFCToolBarMenuButton btnBurn(ID_FILE_BATCHBURNHEX, NULL,GetCmdMgr ()->GetCmdImage (ID_FILE_BATCHBURNHEX), _T(""),FALSE);
		//m_wndToolBar.ReplaceButton (ID_FILE_BATCHBURNHEX, btnBurn);
		//n=m_wndToolBar.CommandToIndex (ID_FILE_BATCHBURNHEX);
		//m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_FILE_BATCHBURNHEX),_T("Burn"));
		
		CMFCToolBarMenuButton btnConnect(ID_CONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_CONNECT2), _T("Connect"),FALSE);
		//CMFCToolBarMenuButton btnConnect(ID_CONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_CONNECT2), _T(""),FALSE);
		m_wndToolBar.ReplaceButton (ID_CONNECT2, btnConnect);
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_CONNECT2),_T("Connect"));

		CMFCToolBarMenuButton btndiDisConnect(ID_DISCONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_DISCONNECT2), _T("DisConn"),FALSE);
		//CMFCToolBarMenuButton btndiDisConnect(ID_DISCONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_DISCONNECT2), _T(""),FALSE);
		m_wndToolBar.ReplaceButton (ID_DISCONNECT2, btndiDisConnect);
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DISCONNECT2),_T("DisConn"));

		CMFCToolBarMenuButton btnScan(ID_SCAN, NULL,GetCmdMgr ()->GetCmdImage(ID_SCAN), _T("Scan"),FALSE);
		//CMFCToolBarMenuButton btnScan(ID_SCAN, NULL,GetCmdMgr ()->GetCmdImage(ID_SCAN), _T(""),FALSE);
		m_wndToolBar.ReplaceButton (ID_SCAN, btnScan);
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_SCAN),_T("Scan"));

		CMFCToolBarMenuButton btnConfig(ID_BUILDINGCONFIGDB, NULL, GetCmdMgr ()->GetCmdImage(ID_BUILDINGCONFIGDB), _T("Config"),FALSE);
		//CMFCToolBarMenuButton btnConnect(ID_CONNECT2, NULL, GetCmdMgr ()->GetCmdImage(ID_CONNECT2), _T(""),FALSE);
		m_wndToolBar.ReplaceButton (ID_BUILDINGCONFIGDB, btnConfig);
		m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_BUILDINGCONFIGDB),_T("Config"));
	}
	return 0;
}

void CMainFrame::OnCheckBuildingBar()
{
	ShowPane (&m_wndWorkSpace, (m_wndWorkSpace.GetStyle () & WS_VISIBLE) == 0, FALSE, TRUE);

}
void CMainFrame::OnUpdateCheckBuildingBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndWorkSpace.IsWindowVisible ());
}
void CMainFrame::OnCheckIOPane()
{
}
void CMainFrame::OnUpdateCheckIOPane(CCmdUI* pCmdUI)
{

}

void CMainFrame::OnHTreeItemSeletedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{	
	
	 
	HTREEITEM hSelItem=m_pTreeViewCrl->GetSelectedItem();
 

	//////////////////////////////////////////////////////////////////////////
	//获取当前节点的字符串的值
	m_strCurSelNodeName = m_pTreeViewCrl->GetItemText(hSelItem);



	//CM5
	//连接到该节点
     DoConnectToANode(hSelItem); 


}




void CMainFrame::OnHTreeItemEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{


}

LRESULT CMainFrame::OnHTreeItemBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{

	return 0;
}

void  CMainFrame::OnHTreeItemClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnHTreeItemSeletedChanged(pNMHDR, pResult);
}



BOOL CMainFrame::ValidAddress(CString sAddress)
{
	int nPos;
	UINT n1,n2,n3,n4;
	CString sTemp=sAddress;
	n1=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n2=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n3=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n4=_wtoi(sTemp);
	if(n1<0 ||n1>255) return false;
	if(n2<0 ||n2>255) return false;
	if(n3<0 ||n3>255) return false;
	if(n4<0 ||n4>255) return false;
	return TRUE;
}
BOOL CMainFrame::ValidAddress(CString sAddress,UINT& n1,UINT& n2,UINT& n3,UINT& n4)
{
	int nPos;
	CString sTemp=sAddress;
	n1=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n2=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n3=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n4=_wtoi(sTemp);
	if(n1<0 ||n1>255) return false;
	if(n2<0 ||n2>255) return false;
	if(n3<0 ||n3>255) return false;
	if(n4<0 ||n4>255) return false;
	return true;
}

// scan Tstats in database
void CMainFrame::ScanTstatInDB(void)
{

	TRACE(_T("Scan TStat In DB!!  \n"));
#if 1
	ClearBuilding();
	m_product.clear();
	m_pCon.CreateInstance("ADODB.Connection");
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	m_pRs.CreateInstance("ADODB.Recordset");


	_variant_t temp_variant;
	CString strTemp;

	CString temp_str=_T("select * from Building where Default_SubBuilding=-1");
	m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	if(m_pRs->GetRecordCount()<=0)
	{
		if(m_pRs->State)
		m_pRs->Close(); 
		if(m_pCon->State)
		m_pCon->Close();
	//	PostMessage(ID_NODEFAULTMAINBUILDING_MSG,0,0);
		AfxMessageBox(_T("There is no default building, please select a building First！"));

		return;
	}
	
	m_strCurMainBuildingName=m_pRs->GetCollect("Main_BuildingName");
	if(m_pRs->State)
		m_pRs->Close();
	m_subNetLst.clear();
	temp_str.Format(_T("select * from Building where Main_BuildingName ='%s'"),m_strCurMainBuildingName);
	m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	


	m_nCurSubBuildingIndex=-1;
	int nTemp=-1;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{
		nTemp++;
		int nDefault=0;
		Building_info temBuildingInfo;
		//memset(&temBuildingInfo,0,sizeof(temBuildingInfo));
		temBuildingInfo.strBuildingName=m_pRs->GetCollect("Building_Name");
		temBuildingInfo.strProtocol=m_pRs->GetCollect("Protocal");
		temBuildingInfo.strIp=m_pRs->GetCollect("Ip_Address");
		temBuildingInfo.strComPort=m_pRs->GetCollect("Com_Port");
		temBuildingInfo.strIpPort=m_pRs->GetCollect("Ip_Port");
		temBuildingInfo.hCommunication=NULL;
		temBuildingInfo.strMainBuildingname=m_pRs->GetCollect("Main_BuildingName");
		temBuildingInfo.strBaudRate=m_pRs->GetCollect("Braudrate");

		nDefault=m_pRs->GetCollect("Default_SubBuilding");
		if(nDefault==-1)
		{
			m_nCurSubBuildingIndex=nTemp;
		}

		if(temBuildingInfo.strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
		{

		}
		m_pRs->MoveNext();
		m_subNetLst.push_back(temBuildingInfo);
	}
	if(m_subNetLst.size()<=0)
		AfxMessageBox(_T("There is no default building,please select a building First！"));
	
	if(m_pRs->State)
		m_pRs->Close(); 


	TV_INSERTSTRUCT tvInsert;////added
	
	m_strCurSubBuldingName=m_subNetLst.at(m_nCurSubBuildingIndex).strBuildingName;
//	for(int k=0;k<m_subNetLst.size();k++)
	{
		CString strBuilding=m_strCurSubBuldingName;//m_subNetLst.at(k).strBuildingName;

		tvInsert.hParent = TVI_ROOT; //
		tvInsert.item.mask = ITEM_MASK; // 
		tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strBuilding;		
		tvInsert.hInsertAfter = TVI_LAST; //
		TVINSERV_BUILDING
		HTREEITEM hTreeSubbuilding=NULL;
		//hTreeSubbuilding=m_pTreeViewCrl->InsertItem(&tvInsert);//
		hTreeSubbuilding=m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//
		if (hTreeSubbuilding!=NULL)
		{
		//	m_subNetLst.at(k).hbuilding_item=hTreeSubbuilding;
		}
	//	m_pRs->Close();
		////begin floor nodes///////////////////////////////////////////////////////////////
		temp_str.Format(_T("select DISTINCT Floor_name from ALL_NODE where Building_Name = '"+strBuilding+"'"));
		HRESULT hR=m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
		
		int nTemp2 = m_pRs->RecordCount;

		vector <tree_floor> tmpfloorLst;//
		tmpfloorLst.empty();
		while(VARIANT_FALSE==m_pRs->EndOfFile)//
		{
			CString strFloorName=m_pRs->GetCollect("Floor_name");
			///*********tree***********************************
			tvInsert.hParent = hTreeSubbuilding; // 
			tvInsert.item.mask = ITEM_MASK; // 
		
			tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strFloorName;		
			tvInsert.hInsertAfter = TVI_LAST; //
			TVINSERV_FLOOR
			
			HTREEITEM hTreeFloor=NULL;
			//hTreeFloor=m_pTreeViewCrl->InsertItem(&tvInsert);//
			hTreeFloor=m_pTreeViewCrl->InsertFloorItem(&tvInsert);//
			tree_floor m_floor_temp;
			m_floor_temp.building_item=hTreeSubbuilding;
			m_floor_temp.floor_item =hTreeFloor;
			m_floor_temp.strFloorName=strFloorName;
			m_floorLst.push_back(m_floor_temp);
			tmpfloorLst.push_back(m_floor_temp);
			m_pRs->MoveNext();
		}
	 if(m_pRs->State)
		m_pRs->Close(); 

		vector <tree_room> tmproomLst;
		////end floor nodes/////////////////////////////////////////////////////////////////
		///begin room nodes///////////////////////////////////////////////////////////////////////
		for(unsigned int i=0;i<tmpfloorLst.size();i++)
		{	//loop for Room Name f
		//	m_pRs->Close();
			CString strFloorName;
			strFloorName=tmpfloorLst.at(i).strFloorName;
			CString temp_str;
			temp_str.Format(_T("select DISTINCT Room_name from ALL_NODE where Building_Name = '"+strBuilding+"' and Floor_name='"+strFloorName+"'"));
			m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
			
			while(VARIANT_FALSE==m_pRs->EndOfFile)
			{
				CString strRoomName;
				strRoomName=m_pRs->GetCollect("Room_name");
				tvInsert.hParent = tmpfloorLst.at(i).floor_item ; // 
				tvInsert.item.mask = ITEM_MASK; // 
				tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strRoomName;		
				tvInsert.hInsertAfter = TVI_LAST; // 
				TVINSERV_ROOM
				HTREEITEM hTreeRoom=NULL;
				//hTreeRoom=m_pTreeViewCrl->InsertItem(&tvInsert);
				hTreeRoom=m_pTreeViewCrl->InsertRoomItem(&tvInsert);
				tree_room m_room_temp;
				
				m_room_temp.building_item=m_floorLst.at(i).building_item ;									
				m_room_temp.floor_item =m_floorLst.at(i).floor_item ;
				m_room_temp.room_item =hTreeRoom;
				m_room_temp.strFloorName=strFloorName;
				m_room_temp.strRoomName=strRoomName;
				m_roomLst.push_back(m_room_temp);	
				tmproomLst.push_back(m_room_temp);	
				m_pRs->MoveNext();
			}		
			 if(m_pRs->State)
			m_pRs->Close(); 
		}
	///////end room nodes//////////////////////////////////////////////////////////////////////
	//////Begin product node/////////////////////////////////////////////////////////////////
	
		for(UINT i=0;i<tmproomLst.size();i++)
		{//loop for product Name
			//m_pRs->Close();	
			CString str_temp;
			CString StrFloorName;
			CString strRoomName;
			strRoomName=tmproomLst.at(i).strRoomName;
			StrFloorName=tmproomLst.at(i).strFloorName;
			str_temp.Format(_T("select * from ALL_NODE where Floor_name = '"+StrFloorName+"' and\
			Room_name = '"+strRoomName+"'and Building_Name = '"+strBuilding+"'  ORDER BY Product_ID ASC "));
			m_pRs->Open(_variant_t(str_temp),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
			
			while(VARIANT_FALSE==m_pRs->EndOfFile)
			{
				CString strProdcut=m_pRs->GetCollect("Product_name");
				tvInsert.hParent = tmproomLst.at(i).room_item ; // 
				tvInsert.item.mask = ITEM_MASK; // 
				tvInsert.item.pszText =(LPTSTR)(LPCTSTR) strProdcut;	
				TRACE(strProdcut);
				tvInsert.hInsertAfter =TVI_SORT;// TVI_LAST; // 
				int temp_product_class_id=m_pRs->GetCollect("Product_class_ID");

#if 1			
				if(temp_product_class_id==PM_NC) 
					TVINSERV_NET_WORK
				else if(temp_product_class_id==LED_PRODUCT_MODEL)
					TVINSERV_LED
				else if(temp_product_class_id==PM_SOLAR)
					TVINSERV_SOLAR	
				else if (temp_product_class_id == PM_CM5 ) //CM5		
					TVINSERV_CMFIVE	
				else if (temp_product_class_id == PM_T38AIOD) //T3
					TVINSERV_NET_WORK
				else if (temp_product_class_id ==PM_T3IOA )//T3-8AI8AO
					TVINSERV_NET_WORK
				else if (temp_product_class_id ==PM_MINIPANEL )//Mini Panel
					TVINSERV_MINIPANEL
				else if (temp_product_class_id == PM_AirQuality) //AirQuality
					TVINSERV_NET_WORK
				else if (temp_product_class_id == PM_LightingController)//Lightingcontroller
					//TVINSERV_NET_WORK  //tree0412
					TVINSERV_LC          //tree0412
				else if ((temp_product_class_id == PM_TSTAT6)||(temp_product_class_id == PM_TSTAT7))//TSTAT7 &TSTAT6 //tree0412
					TVINSERV_LED //tree0412
				else
					TVINSERV_TSTAT
#endif



			//	pBuildingInfo->hbuilding_item=							

				//HTREEITEM hTreeRoom=m_pTreeViewCrl->InsertItem(&tvInsert);
				HTREEITEM hTreeRoom=m_pTreeViewCrl->InsertDeviceItem(&tvInsert);
				tree_product m_product_temp;
				m_product_temp.product_item  =hTreeRoom;
					
				//m_product_temp.serial_number = m_pRs->GetCollect("Serial_ID");
				temp_variant=m_pRs->GetCollect("Serial_ID");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.serial_number=_wtol(strTemp);

				//m_product_temp.product_id =m_pRs->GetCollect("Product_ID");
				temp_variant=m_pRs->GetCollect("Product_ID");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.product_id=_wtoi(strTemp);

			//	m_product_temp.product_class_id = m_pRs->GetCollect("Product_class_ID");
				temp_variant=m_pRs->GetCollect("Product_class_ID");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.product_class_id=_wtoi(strTemp);

			//	m_product_temp.hardware_version= m_pRs->GetCollect("Hardware_Ver");
				temp_variant=m_pRs->GetCollect("Hardware_Ver");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.hardware_version=(float)_wtof(strTemp);
				
				//
				temp_variant=m_pRs->GetCollect("Software_Ver");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.software_version=(float)_wtof(strTemp);

				// 
				temp_variant=m_pRs->GetCollect("EPsize");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.nEPsize=(float)_wtol(strTemp);

				//m_product_temp.baudrate=m_pRs->GetCollect("Bautrate");
			
				temp_variant=m_pRs->GetCollect("Bautrate");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.baudrate=_wtoi(strTemp);
				if (m_product_temp.product_class_id == PM_NC)
				{m_product_temp.baudrate = 19200;
				}
				////

				//m_product_temp.BuildingInfo.strMainBuildingname=m_pRs->GetCollect("MainBuilding_Name");
				//AfxMessageBox(m_product_temp.BuildingInfo.strMainBuildingname);
				//m_product_temp.BuildingInfo.strBuildingName=m_pRs->GetCollect("Building_Name");
				m_product_temp.BuildingInfo=m_subNetLst.at(m_nCurSubBuildingIndex);
//20120423	
				m_product_temp.BuildingInfo.strIp = strTemp;
				temp_variant=m_pRs->GetCollect("Com_Port");//
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				m_product_temp.BuildingInfo.strIpPort=strTemp;
			


				temp_variant=m_pRs->GetCollect("Background_imgID");
				if(temp_variant.vt!=VT_NULL)
					m_product_temp.strImgPathName=temp_variant;
				else
			    	m_product_temp.strImgPathName=_T("");
				m_product.push_back(m_product_temp);
				m_pRs->MoveNext();
			}

			if(m_pRs->State)
			m_pRs->Close(); 

		}
	}
		if(m_pRs->State)
		m_pRs->Close(); 
		if(m_pCon->State)
		m_pCon->Close();
#endif

	if(m_nCurSubBuildingIndex>=0)
	{
		m_strCurSubBuldingName=m_subNetLst.at(m_nCurSubBuildingIndex).strBuildingName;
		ConnectSubBuilding(m_subNetLst.at(m_nCurSubBuildingIndex));
	}
}

void CMainFrame::OnFileOpen()
{
	// TODO: Add your command handler code here
	AfxMessageBox(_T("Open configuration file."));
}
void CMainFrame::OnLoadConfigFile()
{
	//AfxMessageBox(_T("Load configuration file."));
}
 
BOOL CMainFrame::SaveParameterToConfig()
{ 
if (m_strCurSelNodeName.FindOneOf(_T("TStat"))!=-1)
{
	TCHAR exeFullPath[MAX_PATH+1]; //
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); //
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;//
	g_strDatabasefilepath=exeFullPath;//
	g_strExePth=g_strDatabasefilepath;//
	CString filepath=g_strExePth+c_strCfgFileName;
	m_configFile.SetConfigFilePath(filepath);
	 m_configFile.ReadFromCfgFile();
	 m_configFile.m_szCfgFile[CV_TstatLastFlashPage] =(CString(c_strCfgLastFlashMethodItem) +_T("[TStat]"));	
	 m_configFile.m_szCfgFile[CV_TstatDeAddr] =(CString(c_strCfgDefaultAddrItem) +m_tstat.ID);	
     m_configFile.m_szCfgFile[CV_TstatDeCOM] =(CString(c_strCfgDefaultComItem) +m_tstat.COM);
    m_configFile.WriteToCfgFile();

}
else if (m_strCurSelNodeName.FindOneOf(_T("NC"))!=-1){
 

    
}
else if (m_strCurSelNodeName.FindOneOf(_T("LC"))!=-1)
{    



}
else
{
  
	   return FALSE;
}
return TRUE;
}
void CMainFrame::OnBatchFlashHex()
{
//	//if (SaveParameterToConfig())//把要刷新的设备参数保存到配置文件中
//	//{
//	//	int nModel=multi_register_value[MODBUS_PRODUCT_MODEL];
//	//	switch (nModel)
//	//	{
//	//	case 2:m_strModelName=g_strTstat5a;break;
//	//	case 1:m_strModelName=g_strTstat5b;break;
//	//	case 3:m_strModelName=g_strTstat5b;break;
//	//	case 4:m_strModelName=g_strTstat5c;break;
//	//	case 12:m_strModelName=g_strTstat5d;break;
//	//	case PM_NC:m_strModelName=g_strnetWork;break;
//
//	//	case PM_TSTAT5E:m_strModelName=g_strTstat5e;break;
//	//	case 17: m_strModelName=g_strTstat5f;break;
//	//	case 18:m_strModelName=g_strTstat5g;break;
//	//	case 19:m_strModelName=g_strTstat5h;break;
//
//	//	case PM_TSTAT6:m_strModelName=g_strTstat6;break;
//	//	case PM_TSTAT7:m_strModelName=g_strTstat7;break;
//	//	case PM_PRESSURE:m_strModelName=g_strPressure;break;
//
//	//	default:m_strModelName=g_strTstat5a;break;
//	//	}
//	//	//端口放在ISP中处理
//	//	  close_com();
//	//	CString rev;
//	//	rev.Format(_T("%0.1f"),get_curtstat_version());	
///*		Show();*/  
//		//展示Grid_Flash对话框
//	//	if(MessageBox(_T("Did you flash? if yes,Restart T3000"),_T("T3000"),MB_ICONQUESTION|MB_OKCANCEL)==IDOK){
//	//		ShowWindow(SW_HIDE); 
//	//		Sleep(3000);
//	//		WinExec( "T3000.exe",   SW_HIDE); 
//	//		SendMessage(WM_CLOSE);
//	//	}
//	//	else
//	//	{
//	//	}
//
//	//} 
//	//else
//	//{
//	//	 AfxMessageBox(_T("Please Choose a Device to flash!"));
//	//}
//
//
}

void CMainFrame::OnConnect()
{

	
}
void CMainFrame::OnDisconnect()
{

 

}
BOOL CMainFrame::ConnectSubBuilding(Building_info build_info)
{
	CString strInfo;
	if(m_hCurCommunication!=NULL)	
	{
		CloseHandle(m_hCurCommunication);
		m_hCurCommunication=NULL;
	}
	if (build_info.strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
	{
		UINT n1,n2,n3,n4;
		if (ValidAddress(build_info.strIp,n1,n2,n3,n4)==FALSE)  // 
		{


				CString StringIP;
				if(!GetIPbyHostName(build_info.strIp,StringIP))
				{
					AfxMessageBox(_T("Can not get a validate IP adreess from the domain name!"));
					return false;
				}
				CString strPort;
				m_nIpPort=_wtoi(build_info.strIpPort);
				g_CommunicationType=1;
				SetCommunicationType(g_CommunicationType);
				bool b=Open_Socket2(StringIP,m_nIpPort);
				CString strInfo;
				if(b)
				{	strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
					SetPaneString(1,strInfo);
					return TRUE;
				}
				else
				{
					strInfo.Format((_T("Open IP:%s failure")),build_info.strIp);//prompt info;
					SetPaneString(1,strInfo);
					return FALSE;
				}
		}
		else
		{
			CString strPort;
			m_nIpPort=_wtoi(build_info.strIpPort);

			m_strIP=build_info.strIp;

			g_CommunicationType=1;
			SetCommunicationType(g_CommunicationType);
			bool b=Open_Socket2(build_info.strIp,m_nIpPort);
			CString strInfo;
 
			if(b)
			{	
				strInfo.Format((_T("Open IP:%s successful")),build_info.strIp);//prompt info;
				SetPaneString(1,strInfo);
				return TRUE;
			}
			else
			{
				strInfo.Format((_T("Open IP:%s failure")),build_info.strIp);//prompt info;
				SetPaneString(1,strInfo);
				 
				return FALSE;
			}
		}
	}	

	g_CommunicationType = 0;
	SetCommunicationType(g_CommunicationType);
	CString strComport = build_info.strComPort;
	CString strComNum = strComport.Mid(3);
	int nCom = _wtoi(strComNum);

	open_com(nCom);//open*************************************
	if(!is_connect())
	{

		//strInfo.Format(_T("COM: %d Connected: No"), nCom);
		strInfo.Format(_T("COM %d : Not available "), nCom);
		SetPaneString(1,strInfo);
		AfxMessageBox(strInfo);
		return FALSE;
	}
	else
	{
		strInfo.Format(_T("COM %d Connected: Yes"), nCom);
		SetPaneString(1,strInfo);
		Change_BaudRate(default_com1_port_baudrate);
		return TRUE;
	}	
	return FALSE;
}

void CMainFrame::CheckConnectFailure(const CString& strIP) // 
{
	USES_CONVERSION;
	LPCSTR szIP = W2A(strIP);
	DWORD dwIP = inet_addr(szIP);

	IN_ADDR ia;
	ia.S_un.S_addr = dwIP;

	//////////////////////////////////////////////////////////////////////////
	char hostname[256];
	int res = gethostname(hostname, sizeof(hostname));
	if(res != 0)
		return;

	hostent* pHostent = gethostbyname(hostname); 
	if(pHostent==NULL) 
		return;

	hostent& he = *pHostent;
	sockaddr_in sa; 

	memcpy(&sa.sin_addr.s_addr, he.h_addr_list[0],he.h_length); 

	//return   inet_ntoa(sa.sin_addr); 
	CString strHostIP;
	strHostIP.Format(_T("%d.%d.%d.%d"), sa.sin_addr.S_un.S_un_b.s_b1,sa.sin_addr.S_un.S_un_b.s_b2,sa.sin_addr.S_un.S_un_b.s_b3,sa.sin_addr.S_un.S_un_b.s_b4);
	//AfxMessageBox(strIP);

	if ( ia.S_un.S_un_b.s_b1 == sa.sin_addr.S_un.S_un_b.s_b1 &&
		ia.S_un.S_un_b.s_b2 == sa.sin_addr.S_un.S_un_b.s_b2 &&
		ia.S_un.S_un_b.s_b3 == sa.sin_addr.S_un.S_un_b.s_b3 
		)
	{//
		CString strTip;
		strTip.Format(_T("Can not set up the connection with %s, please check its IP address and net cable. "), strIP);
		AfxMessageBox(strTip);
	}
	else
	{
		CString strTip;
		strTip.Format(_T("Your host IP is %s, and NC' IP is %s. They are not in same sub net, please reset your IP address. "),strHostIP, strIP);
		AfxMessageBox(strTip);
	}
	

}



void CMainFrame::OnAddBuildingConfig()
{
	if(g_BuildingsetLevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE_BUILDING);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
		return;
	}

	CAddBuilding Dlg;
	Dlg.DoModal();
}

void CMainFrame::OnScanDevice()
{



	Scan_Product();
	
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(REFRESH_TIMER==nIDEvent)
	{	
		if (g_bEnableRefreshTreeView && m_bEnableRefreshTreeView && !g_bPauseMultiRead )
		{	
// 			g_bEnableRefreshTreeView = FALSE;
// 			

// 			//AfxBeginThread(_FreshTreeView, this);
// 			//RefreshTreeView();
// 			((CT3000View*)m_pViews[0])->Fresh();
// 
// 			g_bEnableRefreshTreeView = TRUE;
			if (m_pRefreshThread)
			{
				m_pRefreshThread->PostThreadMessage(WM_DOREFRESH,NULL,NULL);
			}
		}

	//
	}

	if(SCAN_TIMER==nIDEvent)
	{
		CString strTemp;	
		if(m_bScanFinished)
		{
			CString strTemp;
			//strTemp.Format(_T("Scan finised,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
			strTemp.Format(_T("Scan finished,found %d node(s)"),m_pScanner->m_szTstatScandRet.size()+m_pScanner->m_szNCScanRet.size());
			SetPaneString(1,strTemp);
			g_bPauseMultiRead=FALSE;
			KillTimer(SCAN_TIMER);
			m_wndWorkSpace.m_TreeCtrl.Invalidate();			
// 			CString strTemp;
// 			//strTemp.Format(_T("Scan finised,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
// 			strTemp.Format(_T("Scan finished,found %d node(s)"),m_pScanner->m_szTstatScandRet.size()+m_pScanner->m_szNCScanRet.size());
// 			SetPaneString(1,strTemp);
// 			g_bPauseMultiRead=FALSE;
// 			KillTimer(SCAN_TIMER);
// 			m_wndWorkSpace.m_TreeCtrl.Invalidate();			
		}
		else
		{
			//strTemp.Format(_T("Scanning now,please wait,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
			
// 			CString strTemp;
// 			strTemp.Format(_T("ID %d to %d"),g_nStartID,g_nEndID);
// 			CString strTemp2;
// 			strTemp2.Format(_T("Scanning %s, %s. Please wait."), g_strScanInfoPrompt, strTemp);
			CString strTemp;
			if (g_nStartID == -1) // 
			{
				strTemp = _T("sending scan broadcast");
			}
			else
			{
				strTemp.Format(_T("from address ID %d to %d"),g_nStartID,g_nEndID);
			}

			CString strTip;
			strTip.Format(_T("Scanning %s, %s. Please wait."), g_strScanInfoPrompt, strTemp);


			if(!m_bScanALL)
				SetPaneString(1,strTip);

			if(m_pScanner->m_szTstatScandRet.size() > 0)
			{
				CString strStat = _T("Online.");
				SetPaneString(2, strStat);
			}
			else
			{
				CString strStat = _T("Offline.");
				SetPaneString(2, strStat);
			}			
		}
	}

	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), g_tstat_id, g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);
	SetPaneString(0,str);

	CFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::ClearBuilding()
{
	
	//
	int n=m_buildingLst.size();
	m_pTreeViewCrl->DeleteAllItems();

//	m_subNetLst.clear();
	m_floorLst.clear();
	m_roomLst.clear();

	m_backgroundLst.clear();
//	m_buildingLst.clear();

	
	
}
void CMainFrame::SwitchToPruductType(int nIndex)
{
 	CView* pNewView = m_pViews[nIndex];
 	if (!pNewView)
 		return;
	m_nCurView=0;

	((CT3000View*)m_pViews[m_nCurView])->FreshIOGridTable();		
	((CT3000View*)m_pViews[m_nCurView])->Fresh();
	

}
void CMainFrame::SetPaneConnectionPrompt(CString strInfo)
{
	SetPaneString(2,strInfo);
}
void CMainFrame::SwitchToGraphicView()
{
	SwitchToPruductType(2);
}
void CMainFrame::OnAllNodeDatabase()
{
	if(m_strCurMainBuildingName.IsEmpty())
	AfxMessageBox(_T("Please Select a Building First."));
	CAllNodesDiaolg Dlg;
	Dlg.SetBuildingMainName(m_strCurMainBuildingName);
	Dlg.DoModal();
	if(Dlg.m_bChanged)
	{
		ScanTstatInDB();
	}
	
}
void CMainFrame::Scan_Product()
{


	if(m_pRs->State)
	m_pRs->Close(); 
	if(m_pCon->State)
	m_pCon->Close();
	ClearBuilding();

	

	//g_pauseMultiReadEvent.SetEvent();
	//Sleep(500);

	// oldscan
 	SetTimer(SCAN_TIMER,100,NULL);//lsc





	g_bEnableRefreshTreeView = FALSE;
	g_bScanStart = TRUE;
	if (m_pScanner != NULL)
	{
		delete m_pScanner;
		m_pScanner = NULL;
	}
	m_pScanner = new CTStatScanner;
	g_ScnnedNum=0;
	m_pScanner->SetParentWnd(this);
	m_pScanner->SetSubnetInfo(m_subNetLst);//vector<Building_info>& szSubnets);

	CString strMB485=_T("Modbus 485");
	CString strProtocol = m_subNetLst.at(m_nCurSubBuildingIndex).strProtocol;
	CString strBaudrate = m_subNetLst.at(m_nCurSubBuildingIndex).strBaudRate;
	m_pScanner->SetBaudRate(strBaudrate);
	

	m_pScanner->SetComPort(7);

	g_nStartID = 1;
	g_nEndID = 254;

	m_pWaitScanDlg = new CScanDbWaitDlg;	
	m_pWaitScanDlg->SetTStatScaner(m_pScanner);	


	
	if (strProtocol == strMB485)
	{

		int ret = AfxMessageBox(_T("default will scan via 'modbus 485',\n do you also need 'modbus TCP'?"),MB_YESNOCANCEL ,3);
		if ( ret == IDYES)
		{
		
			m_pScanner->m_scantype = 3;
		}
		else if(ret == IDNO)
		{
			m_pScanner->ScanComDevice();
			m_pScanner->WaitScan();
			m_pScanner->m_scantype = 1;
		}else
			return;
	}
	else
	{

		int ret1 = AfxMessageBox(_T("default will scan via 'modbus TCP',\n do you also need 'modbus 485'?"),MB_YESNOCANCEL,3); 
		if (ret1 == IDYES)
		{

			m_pScanner->m_scantype = 3;
		}
		else if (ret1 == IDNO)
		{
			m_pScanner->ScanNetworkDevice();
			m_pScanner->WaitScan();
			m_pScanner->m_scantype = 2;
		}else
			return;

	}

	m_pWaitScanDlg->DoModal();	

	m_bScanALL = TRUE;
	//m_bScanFinished = FALSE;
	delete m_pWaitScanDlg;
	m_pWaitScanDlg = NULL;

}


void CMainFrame::background_binarysearch_netcontroller()
{
	m_binary_search_networkcontroller_background_thread.clear();
	if (g_CommunicationType==0)
	{	Change_BaudRate(9600);
		binarySearchforview_networkcontroller();
		Change_BaudRate(19200);
		binarySearchforview_networkcontroller();
	}
	else
	{
		binarySearchforview_networkcontroller();
	}

}

void CMainFrame::binarySearchforview_networkcontroller(BYTE devLo, BYTE devHi)
{
//	had_scaned=true;////////////////////////////////////////////////had scan
////	if(net_work_is_exist_or_not==true)
////		Sleep(200);	    
////	else
////		Sleep(10);//
	int a=NetController_CheckTstatOnline(devLo,devHi);

	//int kk=read_one(255,7);
	//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
	if(binary_search_crc(a))
		return ;
	CString temp=_T("");
	if(a>0)
	{
			binary_search_result temp;
			unsigned short SerialNum[9];
			memset(SerialNum,0,sizeof(SerialNum));
			int nRet=0;
			temp.id=a;
			nRet=Read_Multi(temp.id,&SerialNum[0],0,9,3);
			if(nRet>0)
			{
				if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
				{
					srand((unsigned)time(NULL)); 
					SerialNum[0]=rand()%255; 
					SerialNum[1]=rand()%255; 
					SerialNum[2]=rand()%255; 
					SerialNum[3]=rand()%255; 

					write_one(temp.id,0,SerialNum[0]);
					write_one(temp.id,1,SerialNum[1]);
					write_one(temp.id,2,SerialNum[2]);
					write_one(temp.id,3,SerialNum[3]);
				}

				temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
				temp.product_class_id=SerialNum[7];
				temp.hardware_version=SerialNum[8];
			float tstat_version2;
			tstat_version2=SerialNum[4];//tstat version			
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
				tstat_version2 /=10;
			}//tstat_version
			
			temp.software_version=tstat_version2;
					if(read_one(temp.id,185)==0)
						temp.baudrate=9600;
					else
					{
							temp.baudrate=19200;
					}
			temp.nEPsize=read_one(temp.id,326);
			if(temp.serialnumber>=0)
			m_binary_search_networkcontroller_background_thread.push_back(temp);
			}

	}
	switch(a)
	{
	case -2:
		//crc error
		if(devLo!=devHi)
		{
			binarySearchforview_networkcontroller(devLo,(devLo+devHi)/2);
			binarySearchforview_networkcontroller((devLo+devHi)/2+1,devHi);
		}
		else
			binarySearchforview_networkcontroller(devLo,devHi);
		break;
	case -3:
		//more than 2 Tstat is connect
		if(devLo!=devHi)
		{
			binarySearchforview_networkcontroller(devLo,(devLo+devHi)/2);
			binarySearchforview_networkcontroller((devLo+devHi)/2+1,devHi);
		}
		else
		{//Two Tstat have the same ID,fewness
			do
			{
				Sleep(20);//////////////////////////////////for running is better
				char c_temp_arr[100]={'\0'};
				if(Read_One(devLo,10)!=-2)//one times
				{
					CString str_temp;
					for(int j=254;j>=1;j--)
						if(j!=devLo)
						{							
						//	if(!found_same_net_work_controller_by_mac(a))
							if(1)
							{
								bool find=false;//false==no find;true==find
								for(UINT w=0;w<m_binary_search_networkcontroller_background_thread.size();w++)
									if(j==m_binary_search_networkcontroller_background_thread.at(w).id)
									{
										find=true;
										break;
									}
									if(find==false)
									{
										//************************change the Id
										//									Sleep(20);//////////////////////////////////for running is better
										if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
											if(j<devLo)
											{
				

												binary_search_result temp;
											//	temp.baudrate=m_baudrate2;
												unsigned short SerialNum[9];
												memset(SerialNum,0,sizeof(SerialNum));
												int nRet=0;
												temp.id=j;
												nRet=Read_Multi(temp.id,&SerialNum[0],0,9,3);
												if(nRet>0)
												{
													//serial=SerialNum[0]&0x00ff+(SerialNum[0]&0xff00>>8)*256+(SerialNum[1]&0x00ff)*65536
													//	+(SerialNum[1]&0xff00>>8)*16777216;

														if(SerialNum[0]==255&&SerialNum[1]==255&&SerialNum[2]==255&&SerialNum[3]==255)
														{
															srand((unsigned)time(NULL)); 
															SerialNum[0]=rand()%255; 
															SerialNum[1]=rand()%255; 
															SerialNum[2]=rand()%255; 
															SerialNum[3]=rand()%255; 
															write_one(temp.id,0,SerialNum[0]);
															write_one(temp.id,1,SerialNum[1]);
															write_one(temp.id,2,SerialNum[2]);
															write_one(temp.id,3,SerialNum[3]);

														}

													temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													temp.product_class_id=SerialNum[7];
													temp.hardware_version=SerialNum[8];
														float tstat_version2;
													tstat_version2=SerialNum[4];//tstat version			
													if(tstat_version2 >=240 && tstat_version2 <250)
														tstat_version2 /=10;
													else 
													{
														tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
														tstat_version2 /=10;
													}//tstat_version
													
													temp.software_version=tstat_version2;
															if(read_one(temp.id,185)==0)
													temp.baudrate=9600;
												else
												{
														temp.baudrate=19200;
												}
													temp.nEPsize=read_one(temp.id,326);
														
													if(temp.serialnumber>=0)
													
													m_binary_search_networkcontroller_background_thread.push_back(temp);
												}
											}
											binarySearchforview_networkcontroller(devLo,devHi);
											return;
									}
							}
							else
							{
								return;
							}
						}
				}
			}while(1);
		}
		break;
	case -4:break;
		//no connection 
	case -5:break;
		//the input error
	}
}


BOOL CMainFrame::binary_search_crc(int a)
{//use this for binary search ,inspect
	static int for_binary_search_crc=0;//retry times 10;when return value is -2
	if(a==-2)
	{
		if(for_binary_search_crc<10)
			for_binary_search_crc++;
		else
		{
			for_binary_search_crc=0;
			return true;/////////////////more ten time
		}
	}
	else
		for_binary_search_crc=0;
	return false;
}

void CMainFrame::background_binarysearch()
{
	m_binary_search_product_background_thread.clear();////////^0^
	for(UINT k=0;k<m_binary_search_networkcontroller_background_thread.size();k++)
	{
		m_binary_search_product_background_thread.push_back(m_binary_search_networkcontroller_background_thread.at(k));
	}

	scanForTstat();

}
void CMainFrame::scanForTstat()
{
	binarySearchforview();
}
void CMainFrame::binarySearchforview(BYTE devLo, BYTE devHi)
{
	//Sleep(200);	//???
	int nCount=0;
	int a=CheckTstatOnline(devLo,devHi);

	//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
	if(binary_search_crc(a))
		return ;
	char c_array_temp[5]={'0'};
	CString temp=_T("");
	if(a>0)
	{
		int ntempID=0;
		BOOL bFindSameID=false;
		int nPos=-1;

		binary_search_result temp;
//		temp.baudrate=m_baudrate2;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		nRet=Read_Multi(a,&SerialNum[0],0,9,2);
		if(nRet>0)
		{
			temp.id=a;
			temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
			temp.product_class_id=SerialNum[7];
			temp.hardware_version=SerialNum[8];

				float tstat_version2;
			tstat_version2=SerialNum[4];//tstat version			
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
				tstat_version2 /=10;
			}//tstat_version
			
			temp.software_version=tstat_version2;
				if(read_one(temp.id,185)==0)
						temp.baudrate=9600;
					else
					{
							temp.baudrate=19200;
					}
					temp.nEPsize=read_one(temp.id,326);
				if(temp.serialnumber>=0)
				m_binary_search_product_background_thread.push_back(temp);
		}
		else
			return;
	}
	switch(a)
	{
	case -2:
		//crc error
		if(devLo!=devHi)
		{
			binarySearchforview(devLo,(devLo+devHi)/2);
			binarySearchforview((devLo+devHi)/2+1,devHi);
		}
		else
			binarySearchforview(devLo,devHi);
		break;
	case -3:
		//more than 2 Tstat is connect
		if(devLo!=devHi)
		{
			binarySearchforview(devLo,(devLo+devHi)/2);
			binarySearchforview((devLo+devHi)/2+1,devHi);
		}
		else
		{
			do
			{

				Sleep(20);//////////////////////////////////for running is better
				char c_temp_arr[100]={'\0'};
			//	if(read_one(devLo,10)==-2)
			//	{
			//		break;
			//	}
				

			//
				if(Read_One(devLo,10)!=-2)//one times
			//	{

			//	}
			//	else
				{
					CString str_temp;
					for(int j=254;j>=1;j--)
						if(j!=devLo)
						{							
						//	if(!found_same_net_work_controller_by_mac(a))
							if(1)
							{
								bool find=false;//false==no find;true==find
								for(UINT w=0;w<m_binary_search_product_background_thread.size();w++)
								{
									if(j==m_binary_search_product_background_thread.at(w).id)
									{
										find=true;
										break;
									}
								}
								if(find==false)
								{
									if(Write_One(devLo,10,j)>0)//sometimes write failure ,so inspect,important
										if(j<devLo)
										{
												binary_search_result temp;
											//	temp.baudrate=m_baudrate2;
												unsigned short SerialNum[9];
												memset(SerialNum,0,sizeof(SerialNum));
												int nRet=0;
												temp.id=j;
												nRet=Read_Multi(j,&SerialNum[0],0,9,2);
												if(nRet>0)
												{		
													temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
													temp.product_class_id=SerialNum[7];
													temp.hardware_version=SerialNum[8];

														float tstat_version2;
													tstat_version2=SerialNum[4];//tstat version			
													if(tstat_version2 >=240 && tstat_version2 <250)
														tstat_version2 /=10;
													else 
													{
														tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
														tstat_version2 /=10;
													}//tstat_version
													
													temp.software_version=tstat_version2;
															if(read_one(temp.id,185)==0)
														temp.baudrate=9600;
													else
													{
															temp.baudrate=19200;
													}

													temp.nEPsize=read_one(temp.id,326);
											
													if(temp.serialnumber>=0)
													{
															
															m_binary_search_product_background_thread.push_back(temp);
													}
												}

										}
									binarySearchforview(devLo,devHi);
									return;
								}
							}
							else
							{
								return;
							}
						}
				}
			}while(1);
		}
		break;
	case -4:break;
		//no connection 
	case -5:break;
		//the input error
	}
}
int CMainFrame::find_Address_towrite ()
{
	if (m_product.size()<=0)
	{
		return 1;
	}
	bool find=TRUE;
	for(int j=1;j<=254;j++)
	{

		for(UINT i=0;i<m_product.size();i++)
		{
			if(j==m_product.at(i).product_id)
			{//find same id,because the baudrate is different
				find=true;
				break;
			}
			else
			{
				find=FALSE;
				continue;
			}
		}
		if (find)
		{
			continue;
		}
		if (!find)
		{
			return j;
		}
	}
	return -1;

}

CString CMainFrame::GetDeviceClassName(int nClassID)
{
	CString strClassName;
	switch(nClassID)
	{
	case 2:strClassName=g_strTstat5a;break;
	case 1:strClassName=g_strTstat5b;break;
	case 3:strClassName=g_strTstat5b;break;
	case 4:strClassName=g_strTstat5c;break;
	case 12:strClassName=g_strTstat5d;break;
	case PM_NC:strClassName=g_strnetWork;break;
	case NET_WORK_OR485_PRODUCT_MODEL:strClassName=g_strOR485;break;
	case 17: strClassName=g_strTstat5f;break;
	case 18:strClassName=g_strTstat5g;break;
	case 16:strClassName=g_strTstat5e;break;
	case 19:strClassName=g_strTstat5h;break;
	case 27:strClassName=g_strTstat7;break;
	case 26:strClassName=g_strTstat6;break;
	case 13:
	case 14:break;
	default:strClassName=g_strTstat5a;break;
	}

	return strClassName;
}


void CMainFrame::AddScanedDeviceToDatabase(Building_info buildInfo)
{
	ConnectSubBuilding(buildInfo); // 连接building，根据building table
	CString strTemp;
	strTemp.Format(_T("Scanning the subnet:%s..."),buildInfo.strBuildingName);
	SetPaneString(1,strTemp);
	
	if(g_Scanfully)
	{
		background_binarysearch_netcontroller();   // 
		background_binarysearch();
		g_ScnnedNum=254;
	}

	if(g_ScanSecurity)
	{
		m_binary_search_product_background_thread.clear();
		g_ScnnedNum=g_nStartID;
		for(int i=g_nStartID;i<=g_nEndID;i++)
		{
			if(g_bCancelScan)
				break;
			int nID=-1;
			g_ScnnedNum++;
			binary_search_result temp;
			int nBraudRat=19200;
			if (g_CommunicationType==0)
			{
				nBraudRat=19200;
				Change_BaudRate(19200);
				nID=read_one(i,6);
				if(nID<=0)
				{
					nBraudRat=9600;
					Change_BaudRate(9600);
					nID=read_one(i,6);
				}
				temp.id=nID;
			}
			if (g_CommunicationType==1)
			{
				nID=read_one(i,6);
				temp.id=nID;
			}
			if(temp.id>0)
			{
				temp.baudrate=nBraudRat;
				unsigned short SerialNum[9];
				memset(SerialNum,0,sizeof(SerialNum));
				int nRet=0;//
				float tstat_version2;
				nRet=Read_Multi(temp.id,&SerialNum[0],0,9,3);
				if(nRet>0)
				{
					//serial=SerialNum[0]&0x00ff+(SerialNum[0]&0xff00>>8)*256+(SerialNum[1]&0x00ff)*65536
					//	+(SerialNum[1]&0xff00>>8)*16777216;

					temp.serialnumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					temp.product_class_id=SerialNum[7];
					temp.hardware_version=SerialNum[8];
					tstat_version2=SerialNum[4];//tstat version			
					if(tstat_version2 >=240 && tstat_version2 <250)
						tstat_version2 /=10;
					else 
					{
						tstat_version2 = (float)(SerialNum[5]*256+SerialNum[4]);	
						tstat_version2 /=10;
					}//tstat_version
					temp.software_version=tstat_version2;
					if(read_one(temp.id,185)==0)
						temp.baudrate=9600;
					else
					{
							temp.baudrate=19200;
					}
					//nRet=0;
					temp.nEPsize=read_one(temp.id,326);
					if(temp.serialnumber>=0)
					m_binary_search_product_background_thread.push_back(temp);
				}
			}
		}
		g_ScnnedNum=254;
	}


	m_bScanALL=TRUE;
	Sleep(500);

	_ConnectionPtr t_pCon;//for ado connection
	//_RecordsetPtr t_pRs;//for ado 
	t_pCon.CreateInstance(_T("ADODB.Connection"));
	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	CString strfloor_name;
	CString strroom_name;
	CString strproduct_name;
	CString strproduct_class_id;
	CString strproduct_id;
	CString strscreen_name;
	CString strbackground_bmp;
	CString strSerialnumber;
	CString strbuildingName;
	CString stemp;


	int lll=m_binary_search_product_background_thread.size();
	
	for(UINT j=0;j<m_binary_search_product_background_thread.size();j++)
	{
		stemp = GetDeviceClassName(m_binary_search_product_background_thread.at(j).product_class_id);


		BOOL bFind=FALSE;
		int m=-1;
		for (UINT k=0;k<m_product.size();k++)
		{
			if (m_product.at(k).serial_number==m_binary_search_product_background_thread.at(j).serialnumber)
			{
				m=k;
				bFind=TRUE;//
						  //已
				break;
			}
		}
		if (bFind&&m!=-1)
		{
			//if have same serial number ,but different id,changed the scanned id,
			if(m_product.at(m).product_id!=m_binary_search_product_background_thread.at(j).id)
			{
				//changed the id ,
				CChangeIDDlg dlg;
				dlg.SetPromtionTxt(m_product.at(m).serial_number,m_product.at(m).product_id,m_binary_search_product_background_thread.at(j).serialnumber,m_binary_search_product_background_thread.at(j).id);
				if(dlg.DoModal()==IDOK)
				{
					if(dlg.m_nChange==0)
					{
						int nRet=-1;
						//Change_BaudRate(19200);
						for(int i=0;i<3;i++)
						{
							register_critical_section.Lock();
							nRet=Write_One(m_binary_search_product_background_thread.at(j).id,6,m_product.at(m).product_id);
							if(nRet>0)
								multi_register_value[6]=m_product.at(m).product_id;//mark***********************
							register_critical_section.Unlock();
							if(nRet!=-2 && nRet!=-3)
							{
								break;
							}
						}
					}
					if(dlg.m_nChange==2)
					{
						CString strID;
						CString strSerial;
						CString strProductName;
						CString strScreenName;

						CString strSql;
						strID.Format(_T("%d"),m_binary_search_product_background_thread.at(j).id);
						strSerial.Format(_T("%d"),m_binary_search_product_background_thread.at(j).serialnumber);

						strProductName.Format(_T("%s:%u--%u"),stemp,m_binary_search_product_background_thread.at(j).serialnumber,m_binary_search_product_background_thread.at(j).id);
						strScreenName = _T("Screen(S:") + strSerial + _T("--") + strID + _T(")");

						strSql.Format(_T("update ALL_NODE set Product_ID ='%s', Product_Name = '%s', Screen_Name = '%s' where Serial_ID = '%s'"),strID,strProductName,strScreenName,strSerial);
						t_pCon->Execute(strSql.GetString(),NULL,adCmdText);


					}
					if(dlg.m_nChange==3)
					{
						//remove it
						m_binary_search_product_background_thread.erase(m_binary_search_product_background_thread.begin()+j);
						continue;
					}

					
					



				}
				
			}
		//	else
		//	{
				continue;// this alread inserted to database:
		//	}
		}
		
		strfloor_name=_T("Floor_xx");
		strroom_name=_T("Room_xx");
		strproduct_name.Format(_T("%s:%u--%u"),stemp,m_binary_search_product_background_thread.at(j).serialnumber,m_binary_search_product_background_thread.at(j).id);

		strproduct_class_id.Format(_T("%d"),m_binary_search_product_background_thread.at(j).product_class_id);
		strproduct_id.Format(_T("%d"),m_binary_search_product_background_thread.at(j).id);
		strscreen_name.Format(_T("Sceen(S:%d--%d)"),m_binary_search_product_background_thread.at(j).serialnumber,m_binary_search_product_background_thread.at(j).id);
		strbackground_bmp=_T("Clicking here to add a image...");
		strSerialnumber.Format(_T("%d"), m_binary_search_product_background_thread.at(j).serialnumber);
		CString temp_str;

		CString strSubNetName;
		CString strmainBuildingName;

		strSubNetName=buildInfo.strBuildingName;//pView->m_Buildinglst.at(0);
		strmainBuildingName=m_strCurMainBuildingName;

		CString strBaudRate;
		CString strHardVersion;
		CString strSoftwareVersion;
		CString strEpSize;
		CString strIspVersion;

		strBaudRate.Format(_T("%d"),m_binary_search_product_background_thread.at(j).baudrate);
		strHardVersion.Format(_T("%.1f"),m_binary_search_product_background_thread.at(j).hardware_version);
		strSoftwareVersion.Format(_T("%.1f"),m_binary_search_product_background_thread.at(j).software_version);
		strEpSize.Format(_T("%d"),m_binary_search_product_background_thread.at(j).nEPsize);

		CString strSql;
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,EPsize) values('"+strmainBuildingName+"','"+strSubNetName+"','"+strSerialnumber+"','"+strfloor_name+"','"+strroom_name+"','"+strproduct_name+"','"+strproduct_class_id+"','"+strproduct_id+"','"+strscreen_name+"','"+strBaudRate+"','"+strbackground_bmp+"','"+strHardVersion+"','"+strSoftwareVersion+"','"+strEpSize+"')"));
	
		
		try
		{
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
		tree_product pruductNode;
		pruductNode.product_id=m_binary_search_product_background_thread.at(j).serialnumber;
		pruductNode.product_id=m_binary_search_product_background_thread.at(j).id;
		m_product.push_back(pruductNode);


		
	
	}
	if(t_pCon->State)
			t_pCon->Close();
}

void CMainFrame::OnLabel()
{
}
void CMainFrame::OnLabe2()
{
}
void CMainFrame::OnLabe3()
{
}
void CMainFrame::SaveConfigFile()
{



}

LRESULT CMainFrame::OnFreshStatusBar(WPARAM wParam, LPARAM lParam)
{
//pStatusBar->SetPaneText(nIndext,str.GetString());
	status_info*  pinfoStatusbar;
	pinfoStatusbar=(status_info*)wParam;
	//pinfoStatusbar.nIndex=nIndext;
//	infoStatusbar.strInfoText=str;
	m_wndStatusBar.SetPaneText(pinfoStatusbar->nIndex,pinfoStatusbar->strInfoText);


	return 1;
}


void CMainFrame::OnDestroy()
{
#if 1
//	CDialogInfo *pDialogInfo = NULL;

	//try
	//{
	//	// 		if(pDialogInfo==NULL)
	//	// 		{
	//	pDialogInfo = new CDialogInfo;
	//	pDialogInfo->Create(IDD_DIALOG_INFO,this);
	//	pDialogInfo->ShowWindow(SW_SHOW);
	//	pDialogInfo->CenterWindow();
	//	pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("Processing Information,please wait..."));
	//	//		}

	//}
	//catch (...)//这个无效，当pDialogInfo->Create(IDC_STATIC_INFO,this);中的ID写错时，这个函数没有throw抛出错误，所以捕获不到
	//{

	//}



	// 	CCDialogISPTOOL Disptool;
	// 	Disptool.DoModal();
	int temp =1;

//	UpdataSlider(temp);




	g_bEnableRefreshTreeView = FALSE;
	HTREEITEM htiSel = m_pTreeViewCrl->GetSelectedItem();
	SaveTreeNodeRecordToReg(htiSel);

	CFrameWndEx::OnDestroy();

	g_killMultiReadEvent.SetEvent();

	Sleep(500);
	//wait for the end of the thread.
	//if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 1000) != WAIT_OBJECT_0)

		 Sleep(500);//wait for the end of the thread.
		if (m_pFreshMultiRegisters) 

		{
			if (WaitForSingleObject(m_pFreshMultiRegisters->m_hThread, 3000) == WAIT_OBJECT_0)
			{}
			else
			{		
				BOOL bRet = TerminateThread(m_pFreshMultiRegisters->m_hThread,0);
				delete m_pFreshMultiRegisters;
			}

		}

		// 结束线程

		if (m_pRefreshThread) 
		{
			HANDLE hp=m_pRefreshThread->m_hThread;
			g_bEnableRefreshTreeView = FALSE;
			PostThreadMessage(m_pRefreshThread->m_nThreadID,  WM_QUIT,0,0);

			if (WaitForSingleObject(hp, 1000) != WAIT_OBJECT_0)

				if (WaitForSingleObject(m_pRefreshThread->m_hThread, 100) == WAIT_OBJECT_0)

				{
				}
				else
				{
					TerminateThread(m_pRefreshThread->m_hThread, 0);
					//delete m_pRefreshThread;
					//m_pRefreshThread->ExitInstance();
				}
				//CloseHandle(hp);
				//m_pRefreshThread->Delete();
		}

		if (is_connect())
		{
			close_com(); // added by zgq:12-16-2011
		}


		//Alex
		//不提示-处理对话框
		/*if (pDialogInfo!=NULL)
		{
			delete pDialogInfo;
			pDialogInfo = NULL;
		}*/
#endif

		// TODO: Add your message handler code here
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(message==WM_MYMSG_REFRESHBUILDING)
	{
		//Sleep(1000);
		//AfxMessageBox(_T("There is no default building,please select a building First."));
		BOOL bTemp = g_bEnableRefreshTreeView ;
		g_bEnableRefreshTreeView = FALSE;
		ScanTstatInDB();
		g_bEnableRefreshTreeView |= bTemp;
	}

	return CFrameWndEx::WindowProc(message, wParam, lParam);
}
void CMainFrame::OnChangeAdminPassword()
{


	CChangePasswordDlg dlg;
	//dlg.DoModel();
	dlg.DoModal();
	
}
void CMainFrame::OnUserMannageMent()
{
	CManageAccountDlg Dlg;
	Dlg.DoModal();
}
void CMainFrame::OnUserLogin()
{
	CLoginDlg Dlg(g_buser_log_in);
	Dlg.DoModal();

}
void CMainFrame::OnUserMannageMentUpdate(CCmdUI *pCmdUI)
{
	if(g_UserLevel==0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CMainFrame::GetIONanme()
{
	//Get Input Output Name;
			_ConnectionPtr m_ConTmp;
			_RecordsetPtr m_RsTmp;
			m_ConTmp.CreateInstance("ADODB.Connection");
			m_RsTmp.CreateInstance("ADODB.Recordset");
			m_ConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

			CString strSerial;
			strSerial.Format(_T("%d"),g_serialNum);
			strSerial.Trim();
			CString strsql;
			strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
			m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	

			if(VARIANT_FALSE==m_RsTmp->EndOfFile)
			{	
				CString str_temp;
				str_temp.Empty();
				_variant_t temp_variant;
			
				temp_variant=m_RsTmp->GetCollect("SENSORNAME");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Internal Sensor");
				g_strSensorName=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT1");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 1");
				g_strInName1=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT2");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 2");
				g_strInName2=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT3");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 3");
				g_strInName3=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT4");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 4");
				g_strInName4=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT5");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 5");
				g_strInName5=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT6");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 6");
				g_strInName6=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT7");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 7");
				g_strInName7=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT8");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Input 8");
				g_strInName8=str_temp;

				temp_variant=m_RsTmp->GetCollect("INPUT9");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Humidity Sensor");
				g_strInHumName=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT1");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 1");
				g_strOutName1=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT2");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 2");
				g_strOutName2=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT3");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 3");
				g_strOutName3=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT4");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 4");
				g_strOutName4=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT5");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 5");
				g_strOutName5=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT6");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 6");
				g_strOutName6=str_temp;

				temp_variant=m_RsTmp->GetCollect("OUTPUT7");
				if(temp_variant.vt!=VT_NULL)
					str_temp=temp_variant;
				else
					str_temp=_T("Output 7");
				g_strOutName7=str_temp;

			}
			else
			{
				g_strSensorName =_T("Internal Sensor");
				g_strInName1=_T("Input 1");
				g_strInName2=_T("Input 2");
				g_strInName3=_T("Input 3");
				g_strInName4=_T("Input 4");
				g_strInName5=_T("Input 5");
				g_strInName6=_T("Input 6");
				g_strInName7=_T("Input 7");
				g_strInName8=_T("Input 8");
				
				g_strInHumName = _T("Humidity Sensor");


				g_strOutName1=_T("Output 1");
				g_strOutName2=_T("Output 2");
				g_strOutName3=_T("Output 3");
				g_strOutName4=_T("Output 4");
				g_strOutName5=_T("Output 5");
				g_strOutName6=_T("Output 6");
				g_strOutName7=_T("Output 7");
				g_strOutName8=_T("Output 8");


			}
			if(m_RsTmp->State) 
				m_RsTmp->Close(); 
			if(m_ConTmp->State)
				m_ConTmp->Close();	

}
void CMainFrame::OnHelp()
{
	CString strHelp=g_strExePth+_T("T3000help.chm");
	::HtmlHelp(NULL, strHelp, HH_DISPLAY_TOPIC, 0);
}
void CMainFrame::OnImportDatase()
{
	CImportDatabaseDlg Dlg;
	Dlg.DoModal();
	
}
void CMainFrame::OnCaliBrate()
{
	CToolCalibrateDlg Dlg;
	Dlg.DoModal();
}

void CMainFrame::OnGloabalOverrides()
{
	CToolCalibrateDlg Dlg;
	Dlg.DoModal();

}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_F8)
		{
			ReFresh();
			return 1;
		}
		if(pMsg->wParam == VK_F3)
		{
			OnToolRefreshLeftTreee();
		}

		if(pMsg->wParam == VK_F2)
		{
			OnToolErease();
			return 1;
		}

	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}
void CMainFrame::ReFresh()
{

}
BOOL CMainFrame::GetIPbyHostName(CString strHostName,CString& strIP)
{
	 
	WSAData   wsdata; 
	WORD wVersionRequested=MAKEWORD(2,0);   
    int  err=WSAStartup(wVersionRequested,&wsdata);   
    hostent     *pHostent=NULL   ;   


    pHostent   =   gethostbyname(CW2A(strHostName));   
    if(pHostent==NULL)   
       return   false;  
    if(pHostent->h_addr_list==NULL)   
        return   false;   
    sockaddr_in   sa;   
    memcpy(&sa.sin_addr.s_addr,pHostent->h_addr_list[0],pHostent->h_length);     
	strIP.Format(_T("%d.%d.%d.%d"),sa.sin_addr.S_un.S_un_b.s_b1,sa.sin_addr.S_un.S_un_b.s_b2,sa.sin_addr.S_un.S_un_b.s_b3,sa.sin_addr.S_un.S_un_b.s_b4);  
	WSACleanup();
	return TRUE;

}
void CMainFrame::OnToolErease()
{
	CEreaseDlg Dlg;
	Dlg.DoModal();
	
}
void CMainFrame::OnToolFresh()
{

}
void CMainFrame::OnToolRefreshLeftTreee()
{

} 

void CMainFrame::DoConnectToANode( const HTREEITEM& hTreeItem )
{
	HTREEITEM hSelItem=hTreeItem;
	int nCounts=m_product.size();
	tree_product product_Node;
	int nSelectID=-1;
	UINT nSelectSerialNumber;
	for(UINT i=0;i<m_product.size();i++)
	{  
	
 
		if(hSelItem==m_product.at(i).product_item )
		{

			CDialog_Progess* pDlg = new CDialog_Progess(this,1,7);
			pDlg->Create(IDD_DIALOG10_Progress, this);
			//pDlg->CenterWindow();
			pDlg->ShowProgress(0,0);
			pDlg->ShowWindow(SW_SHOW);
			float flagsoftwareversion;


			int nID=-1;
			int nTRet=-1;
			//g_tstat_id=m_product.at(i).product_id;
			product_Node=m_product.at(i);//把当前节点信息 给product_Node 保存

			//************************************
			register_critical_section.Lock();
			g_tstat_id_changed=TRUE;
			g_tstat_id=product_Node.product_id;
			register_critical_section.Unlock();
			//***************************************
			nSelectID=product_Node.product_id;
			nSelectSerialNumber=product_Node.serial_number;
			flagsoftwareversion = product_Node.software_version;


			CString strTitle;
			strTitle=product_Node.BuildingInfo.strMainBuildingname;
			strTitle+=_T("->");
			strTitle+=product_Node.BuildingInfo.strBuildingName;

			m_wndWorkSpace.SetWindowText(strTitle);

			 
		
			g_strImagePathName=product_Node.strImgPathName;
 			if (!((product_Node.BuildingInfo.strIp.CompareNoCase(_T("9600")) ==0)||(product_Node.BuildingInfo.strIp.CompareNoCase(_T("19200"))==0)
				||(product_Node.BuildingInfo.strIp.CompareNoCase(_T(""))) == 0))
 			{

			if(product_Node.BuildingInfo.hCommunication==NULL||m_strCurSubBuldingName.CompareNoCase(product_Node.BuildingInfo.strBuildingName)!=0)
			{
				pDlg->ShowProgress(2,10);
 
				BOOL bRet = ConnectSubBuilding(product_Node.BuildingInfo);
				
				if (!bRet)
				{
			 
						CheckConnectFailure(product_Node.BuildingInfo.strIp);
			 
					pDlg->ShowWindow(SW_HIDE);//20120220
					delete pDlg;//20120220
					pDlg = NULL;
				}
			}
			}else
			{

				if (product_Node.BuildingInfo.strComPort.CompareNoCase(_T("N/A")) == 0)
				{
					AfxMessageBox(_T("Please select COM port"));//\nDatabase->Building config Database
					CAddBuilding  Dlg;
					if(Dlg.DoModal()==IDOK)
					{
 
					}
					
				}
				else
				{
					int nComPort = _wtoi(product_Node.BuildingInfo.strComPort.Mid(3));
					CString strInfo;
					strInfo.Format(_T("COM %d Connected: Yes"), nComPort);					
					SetPaneString(1, strInfo);
                  
					
					//Alex-选择接点后保存当前接点的参数值


					BOOL  bret = open_com(nComPort);
					if (!bret)
					{
						AfxMessageBox(_T("COM port open failure!"));
					}
					else
					{					
					m_tstat.ID.Format(_T("%d"),product_Node.product_id);
					m_tstat.COM=product_Node.BuildingInfo.strComPort;
					
					g_CommunicationType=0;
					}


				}
			}

	 
			if(m_strCurSubBuldingName.CompareNoCase(product_Node.BuildingInfo.strBuildingName)!=0&&m_hCurCommunication!=NULL)
			{
				 
			}

			m_strCurSubBuldingName=product_Node.BuildingInfo.strBuildingName;
			BOOL bOnLine=FALSE;
			UINT nSerialNumber=0;
			if (g_CommunicationType==0)
			{
				m_nbaudrat=19200;
				Change_BaudRate(19200);
				nID=read_one(g_tstat_id,6,2);
				if(nID<0)
				{
					m_nbaudrat=9600;
					Change_BaudRate(9600);
					nID=read_one(g_tstat_id,6,2);
					bOnLine=FALSE;
				}
				if(nID>0)
				{
					unsigned short SerialNum[4];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;//
					nRet=Read_Multi(g_tstat_id,&SerialNum[0],0,4,3);
					
					if(nRet>=0)
					{
			
						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}
					if(nSerialNumber>=0)
					{
						if(nSerialNumber==nSelectSerialNumber)
							bOnLine=TRUE;
					}
				}else
				{
					CString str;
					str.Format(_T("communication failure,error code:%d\nor select other com port to try"),nID);
					AfxMessageBox(str);
				}
			}


			if(bOnLine)
			{ 
				SetPaneConnectionPrompt(_T("Online!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,true);
			}
			else
			{
				
				SetPaneConnectionPrompt(_T("Offline!"));
				m_pTreeViewCrl->turn_item_image(hSelItem ,false);	
				memset(&multi_register_value[0],0,sizeof(multi_register_value));
			
				if (pDlg !=NULL)
				{
					pDlg->ShowWindow(SW_HIDE);
					delete pDlg;
				}
				

				
				return;

			}
				register_critical_section.Lock();


					int i;
					int it = 0;
					for(i=0;i<7;i++)
					{
		
						int itemp = 0;
						itemp = Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64,3);
						if(itemp == -2)
						{

							continue;
						}
						else

							pDlg->ShowProgress(it+1,100/7*(it+1));
							it++;

					}
					CString strInfo;
					strInfo.Format(_T("CMainFrame::DoConnectToANode():Read_Multi7*64end"));			
					SetPaneString(2, strInfo);
					if (it<7)
					{	
						AfxMessageBox(_T("Communication abnormal"));
						pDlg->ShowWindow(SW_HIDE);
						delete pDlg;
						strInfo.Format(_T("CMainFrame::DoConnectToANode():delete pDlg<7"));			
						SetPaneString(2, strInfo);
					}else
					{
						pDlg->ShowProgress(7,100);
						pDlg->ShowWindow(SW_HIDE);
						delete pDlg;
						strInfo.Format(_T("CMainFrame::DoConnectToANode():delete pDlg"));			
						SetPaneString(2, strInfo);
					}

					g_tstat_id_changed=FALSE;
					register_critical_section.Unlock();



	

			g_serialNum=nSerialNumber;//get_serialnumber();
			strInfo.Format(_T("CMainFrame::DoConnectToANode():read_one(g_tstat_id,7,3)"));			
			SetPaneString(2, strInfo);
				nFlag = read_one(g_tstat_id,7,3);		
	
				if(nFlag==PM_NC)	
				{	
					
				}
				else if (nFlag == PM_CM5)
				{
					
				}else if (nFlag == PM_T38AIOD ||nFlag == PM_T3IOA  ||nFlag ==PM_T332AI || nFlag == PM_T34AO)//T3
				{
					SwitchToPruductType(0);

				}
				else if (nFlag == PM_MINIPANEL)
				{
					
				}
				else if(nFlag<PM_NC)	
				{	
					

					//在这里是转向视图的地方
				   //
					SwitchToPruductType(0);

				}		


		}
	 
		 
	}

}



void CMainFrame::InitTreeNodeConn()
{

}


void CMainFrame::GetAllTreeItems( HTREEITEM hItem, vector<HTREEITEM>& szTreeItems  )
{

}


void CMainFrame::RefreshTreeView()
{


}



void CMainFrame::SuspendRefreshThread()
{
	m_bEnableRefreshTreeView = FALSE;
}

void CMainFrame::ContinueRefreshThread()
{
	m_bEnableRefreshTreeView = TRUE;
}


void CMainFrame::DoFreshAll()
{		
	RefreshTreeView();
	if (m_nCurView == 0)
	{
		((CT3000View*)m_pViews[m_nCurView])->PostMessage(WM_FRESHVIEW,0,0)	;
	}
}


UINT _FreshTreeView(LPVOID pParam )
{
	CMainFrame* pMain = (CMainFrame*)pParam;
	pMain->RefreshTreeView();

	
	return 1;
}


LRESULT CMainFrame::OnAddTreeNode(WPARAM wParam, LPARAM lParam)
{
	ScanTstatInDB();

	//////////////////////////////////////////////////////////////////////////
	if(m_pScanner)
	{
		//////////////////////////////////////////////////////////////////////////
		CString strTemp;
		//strTemp.Format(_T("Scan finised,found %d node(s)"),m_binary_search_product_background_thread.size());// oldscan
		strTemp.Format(_T("Scan finished,found %d node(s)"),m_pScanner->m_szTstatScandRet.size()+m_pScanner->m_szNCScanRet.size());
		SetPaneString(1,strTemp);
		g_bPauseMultiRead=FALSE;
		KillTimer(SCAN_TIMER);
		m_wndWorkSpace.m_TreeCtrl.Invalidate();		
		
		SelectTreeNodeFromRecord();//

		//////////////////////////////////////////////////////////////////////////
		delete m_pScanner;
		m_pScanner =NULL;
	}
	g_bEnableRefreshTreeView =TRUE;
	g_bScanStart = FALSE;
	return 1;
}

CString CMainFrame::GetScreenName(int nSerialNumber, int nModbusID)
{
	CString strSerialNum, strMID;
	strSerialNum.Format(_T("%d"), nSerialNumber);
	strMID.Format(_T("%d"), nModbusID);

	CString strScreenName = _T("Screen(S:") + strSerialNum+_T("--")+strMID + _T(")");

	return strScreenName;
}



//////////////////////////////////////////////////////////////////////////
// for edit tree control node name

void CMainFrame::UpdateAllNodesInfo(HTREEITEM& htiEdit)
{
	int nItemData = m_pTreeViewCrl->GetItemData(htiEdit);

	if (nItemData >= 1000 && nItemData < 2000)
	{
		UpdateFloorNode(htiEdit);
	}
	if (nItemData >=2000 && nItemData < 3000)
	{
		UpdateRoomNode(htiEdit);
	}
	if (nItemData >= 3000 && nItemData < 9000)
	{
		UpdateDeviceNodes(htiEdit);
	}
		
}


void CMainFrame::UpdateFloorNode(HTREEITEM& htiEdit)
{
}


void CMainFrame::UpdateRoomNode(HTREEITEM& htiEdit)
{
	
}

void CMainFrame::UpdateDeviceNodes(HTREEITEM& htiEdit)
{
}

//////////////////////////////////////////////////////////////////////////
// for record last time tree control node click result

// 

// write current click result to register table of windows.
const CString strRegRoot = _T("Software\\Temco T3000 Application\\T3000\\TreeNodeRecord\\");
		    
const CString strSubnetRegEntry = _T("Subnet");
const CString strSubnetRegEntryValid = _T("SubnetValid");
const CString strFloorRegEntry = _T("Floor");
const CString strFloorRegEntryValid = _T("FloorValid");
const CString strRoomRegEntry = _T("Room");
const CString strRoomRegEntryValid = _T("RoomValid");
const CString strDeviceRegEntry = _T("Device");
const CString strDeviceRegEntryValid = _T("DeviceValid");


void CMainFrame::SaveTreeNodeRecordToReg(HTREEITEM& htiCurSel)
{
	CRegKey reg;
	

}


// m_nSubnetItemData = 9000;
// m_nFloorItemData = 1000;
// m_nRoomItemData = 2000;
// m_nDeviceItemData = 3000;
BOOL CMainFrame::WriteValueToReg(CRegKey& reg, HTREEITEM& htiItem)
{
	if (!htiItem)
	{
		return FALSE;
	}
	int nData = m_pTreeViewCrl->GetItemData(htiItem);
	CString strText = m_pTreeViewCrl->GetItemText(htiItem);

	if (nData >= 1000 && nData < 2000) // floor
	{
		reg.SetStringValue(strFloorRegEntry, strText);
		reg.SetDWORDValue(strFloorRegEntryValid, 1);
	}
	else if(nData >= 2000 && nData < 3000) // room
	{
		reg.SetStringValue(strRoomRegEntry, strText);
		reg.SetDWORDValue(strRoomRegEntryValid, 1);
	}
	else if(nData >= 3000 && nData < 9000)	// device
	{
		reg.SetStringValue(strDeviceRegEntry, strText);
		reg.SetDWORDValue(strDeviceRegEntryValid, 1);
	}
	else if(nData >= 9000) // subnet
	{
		reg.SetStringValue(strSubnetRegEntry, strText);
		if(reg.SetDWORDValue(strSubnetRegEntryValid, 1) != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("Failed write Registry!"));
		}
	}

	return TRUE;
		
}


// nFlag = 0, read subnet
//         = 1, read floor
//         = 2, read room
//         = 3, read device
void CMainFrame::SelectTreeNodeFromRecord()
{

}

// 
HTREEITEM CMainFrame::GetLastSelNodeFromRecord(CRegKey& reg, HTREEITEM& htiRoot)
{
	HTREEITEM htiSel = NULL;
	CString strSubnetName, strFloorName, strRoomName, strDeviceName;
	DWORD nSubnet, nFloor, nRoom, nDevice;

	
	//
	return htiSel;

}




HTREEITEM CMainFrame::SearchItemByName(HTREEITEM& htiRoot, const CString& strItemName)
{
	CString strRoot = m_pTreeViewCrl->GetItemText(htiRoot);
	if (strItemName.CompareNoCase( strRoot ) == 0 )
	{
		return htiRoot;
	}


	HTREEITEM htiChild = m_pTreeViewCrl->GetChildItem(htiRoot);

	vector<HTREEITEM>  szTreeItem;

	if (htiChild)
	{

		CString strText = m_pTreeViewCrl->GetItemText(htiChild);
		if (strText.CompareNoCase(strItemName) == 0)
		{
			return htiChild;
		}

		szTreeItem.push_back(htiChild);
		HTREEITEM htiSibling = m_pTreeViewCrl->GetNextSiblingItem(htiChild);
		while( htiSibling )
		{
			CString strText = m_pTreeViewCrl->GetItemText(htiSibling);
			if (strText.CompareNoCase(strItemName) == 0)
			{
				return htiSibling;
			}
			szTreeItem.push_back(htiSibling);
			htiSibling = m_pTreeViewCrl->GetNextSiblingItem(htiSibling);
		}
	}

	for(UINT i = 0; i < szTreeItem.size(); i++)
	{
		HTREEITEM htiRet = SearchItemByName(szTreeItem[i], strItemName);

		if (htiRet)
		{
			return htiRet;
		}
	}

	return NULL;

}
BOOL CMainFrame::ImportDataBaseForFirstRun()
{
	CString strFileVer = GetCurrentFileVersion();

	CRegKey reg;
	ReadFileVersionFromRegister(reg);
	
	if (strFileVer.CompareNoCase(m_strFileVersion) == 0) // equal
	{
		// do nothing
	}
	else
	{
		//CImportDatabaseDlg dlg;
		//dlg.DoModal();

		reg.SetStringValue(_T("FileVersion"), strFileVer);
	}	


	return TRUE;
}
const CString c_strFileVersionPath = _T("Software\\Temco T3000 Application\\T3000\\FileVersion\\");
// 从注册表读fileversion
CString CMainFrame::ReadFileVersionFromRegister(CRegKey& reg)
{
	WCHAR szBuf[64] = {0};
	DWORD dwSize = 64;

	if(reg.Create(HKEY_CURRENT_USER, c_strFileVersionPath) == ERROR_SUCCESS)
	{
		if(reg.QueryStringValue(_T("FileVersion"), szBuf, &dwSize) == ERROR_SUCCESS)
		{
			m_strFileVersion = CString(szBuf);
		}
	}	
	else 
		// 没有这个表项
	{
		ASSERT(0);		
	}	

	return m_strFileVersion;
	reg.Close();
}
CString CMainFrame::GetCurrentFileVersion()
{


	return _T("");
}
void CMainFrame::EnableMyToolBarButton(int nIndex, int nID, BOOL bEnable)
{
	return;

} 
void CMainFrame::OnDatabaseTemcoproducts()
{
}
void CMainFrame::ReadExcel()
{

}
void CMainFrame::OnGridFlashhex()
{
	CGridFlashDlg gridflashdlg;
	 gridflashdlg.DoModal();
}

void CMainFrame::OnIspToolForOne()
{
	if (SaveParameterToConfig())
	//把要刷新的设备参数保存到配置文件中
	{
		/*int nModel=multi_register_value[MODBUS_PRODUCT_MODEL];*/
		/*switch (nModel)
		{
		case 2:m_strModelName=g_strTstat5a;break;
		case 1:m_strModelName=g_strTstat5b;break;
		case 3:m_strModelName=g_strTstat5b;break;
		case 4:m_strModelName=g_strTstat5c;break;
		case 12:m_strModelName=g_strTstat5d;break;
		case PM_NC:m_strModelName=g_strnetWork;break;

		case PM_TSTAT5E:m_strModelName=g_strTstat5e;break;
		case 17: m_strModelName=g_strTstat5f;break;
		case 18:m_strModelName=g_strTstat5g;break;
		case 19:m_strModelName=g_strTstat5h;break;

		case PM_TSTAT6:m_strModelName=g_strTstat6;break;
		case PM_TSTAT7:m_strModelName=g_strTstat7;break;
		case PM_PRESSURE:m_strModelName=g_strPressure;break;

		default:m_strModelName=g_strTstat5a;break;
		}*/
		//端口放在ISP中处理
		/*  close_com();
		CString rev;
		rev.Format(_T("%0.1f"),get_curtstat_version());	*/
	           Show(m_strCurSelNodeName);//	展示Grid_Flash对话框
	
	 //  //USE ISP DLL ，Will Make T3000 Crash ，so Must Let T3000 Restart.
		//if(MessageBox(_T("Did you flash? if yes,Restart T3000"),_T("T3000"),MB_ICONQUESTION|MB_OKCANCEL)==IDOK){
		//	ShowWindow(SW_HIDE); 
		//	Sleep(3000);
		//	WinExec( "T3000.exe",   SW_HIDE); 
		//	SendMessage(WM_CLOSE);
		}
	}
//		else
//		{
//		}
//
//	 
//	/*else
//	{
//		 AfxMessageBox(_T("Please Choose a Device to flash!"));
//	}*/
//	 
//}
