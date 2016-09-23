#include "StdAfx.h"
#include "ComWriter.h"
#include "ISPDlg.h"


extern CString g_strFlashInfo;
 extern Bin_Info        global_fileInfor;
extern unsigned int the_max_register_number_parameter_Count;
extern unsigned int the_max_register_number_parameter_Finished;
UINT Flash_Modebus_Device(LPVOID pParam);
//UINT run_back_ground_flash_thread(LPVOID pParam);
UINT flashThread_ForExtendFormatHexfile(LPVOID pParam);
UINT flashThread_ForExtendFormatHexfile_RAM(LPVOID pParam);
int flash_a_tstat_RAM(BYTE m_ID,int section, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam);
int flash_a_tstat(BYTE m_ID, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam);
CComWriter::CComWriter(void)
{
    m_pFileBuffer = NULL;
    m_nBufLen = 0;

    m_nComPort = -1;
    //m_nModbusID;
    m_nBautrate = 0;			// ������

    m_pWorkThread = NULL;

    m_bStopWrite = FALSE;

    m_nHexFileType = HEXFILE_DATA;
}

CComWriter::~CComWriter(void)
{

}


void CComWriter::SetParentWnd(CWnd* pWnd)
{
    m_pParentWnd = pWnd;
}

void CComWriter::SetFileBuffer(TS_UC* pBuf, int nLen)
{
    m_pFileBuffer = pBuf;
    m_nBufLen = nLen;
}

void CComWriter::SetModbusID(vector<int>& szMdbIds)
{
    m_szMdbIDs = szMdbIds;
}

void CComWriter::SetComPortNO(int nComPortNO)
{
    m_nComPort = nComPortNO;
}

void CComWriter::SetBautrate(int nBautrate)
{
    m_nBautrate = nBautrate;
}

void CComWriter::SetExtendHexFileParam(vector<int>& szFlags, TS_UC* pBuf)
{
    m_szHexFileFlags.clear();
    m_szHexFileFlags = szFlags;

    m_pExtendFileBuffer = pBuf;
}


int CComWriter::BeginWirteByCom()
{
    ASSERT(m_szMdbIDs.size() > 0);
    ASSERT(m_nComPort >= 0);

    if (m_nHexFileType == 0)
    {
// 		if (!WriteCommandtoReset())
// 		{
// 			return 0;
// 		}


        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);

        if(open_com(m_nComPort)==false)
        {
            CString srtInfo = _T("|Error :The com port is occupied!");
            //MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
            //AddStringToOutPuts(_T("Error :The com port is occupied!"));
            OutPutsStatusInfo(srtInfo, FALSE);
            return 0;
        }
        else
        {
            CString strTemp;
            strTemp.Format(_T("COM%d"), m_nComPort);
            CString strTips = _T("|Open ") +  strTemp + _T(" successful.");
            OutPutsStatusInfo(strTips, FALSE);
            // AddStringToOutPuts(strTips);

        }


        CString strTips = _T("|Programming device...");
        OutPutsStatusInfo(strTips);
        //AddStringToOutPuts(strTips);
        //m_pWorkThread=AfxBeginThread(run_back_ground_flash_thread, this); //create thread,read information
        m_pWorkThread=AfxBeginThread(Flash_Modebus_Device, this); //create thread,read information

        ASSERT(m_pWorkThread);
    }

    else if(m_nHexFileType == 2)
    {
        if (Is_Ram)
        {
            WirteExtendHexFileByCom_RAM();
        }
        else
        {
            WirteExtendHexFileByCom();
        }
    }

    return 1;
}




BOOL CComWriter::WriteCommandtoReset()
{
    //if(open_com_for_Command(m_nComPort)==false)
    CString strTips ;//= _T("|Error :The com port is occupied!");
    if(open_com(m_nComPort)==false)
    {
        strTips = _T("|Error :The com port is occupied!");
        OutPutsStatusInfo(strTips);
        //AfxMessageBox(strTips);
        return FALSE;
    }

    int nRet = Write_One(m_szMdbIDs[0],16,127);   // ����ISPģʽ
    //Sleep(2000);
    //Add by Fance  �����Ӧ�ô������� ISP  16д127��  ��Ҫ�� 11�żĴ���  11�� ����1  ˵����ת�ɹ�����������ȴ�;
    strTips = _T("Wait device jump to isp mode!");
    OutPutsStatusInfo(strTips);
    int re_count = 0;
    int nnn_ret = 0;

    do
    {
        nnn_ret  = read_one(m_szMdbIDs[0],11);
        if(nnn_ret > 1)
            break;
        Sleep(1000);
        re_count ++;
        if(re_count == 15)
            break;
    }
    while (nnn_ret > 1);


    int ModelID= read_one(m_szMdbIDs[0],7,5);
    if (ModelID>0)
    {
        if (ModelID==6||ModelID==7||ModelID==8)//Tstat6,7���оƬ��С�������ô�����д�Ķ������
        {
            int Chipsize=read_one(m_szMdbIDs[0],11,5);


            if (Chipsize<37)	//64K
            {
                if (m_nHexFileType==0)
                {
                    strTips = _T("|hex file   matches with the chip!");
                    OutPutsStatusInfo(strTips);
                }
                else
                {
                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(m_szMdbIDs[0],16,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    strTips = _T("|hex file doesn't match with the chip!");
                    OutPutsStatusInfo(strTips);
                    //AfxMessageBox(_T("hex file doesn't match with the chip"));
                    return FALSE;
                }
            }
            else	//128K
            {
                if (m_nHexFileType==0)
                {
                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(m_szMdbIDs[0],33,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(m_szMdbIDs[0],16,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    strTips = _T("|hex file doesn't match with the chip!");
                    OutPutsStatusInfo(strTips);
                    //AfxMessageBox(_T("hex file doesn't match with the chip"));
                    return FALSE;
                }
                else
                {
                    strTips = _T("|hex file   matches with the chip!");
                    OutPutsStatusInfo(strTips);
                }
            }
        }
    }




    close_com();
    //Sleep(1000);

    return TRUE;
}



UINT Flash_Modebus_Device(LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)(pParam);
    CString strFailureList;
    CString strTips;
    //	do
    //	{
    int nFlashRet=0;
    UINT i=0;
    int nFailureNum = 0;
    CString strTips_isp1 = _T("Wait device jump to ISP mode.");

    if(GetCommunicationType()==1)
    {
        if(Open_Socket2(pWriter->m_strIPAddr, pWriter->m_nIPPort)==false)
        {
            CString srtInfo = _T("|Error : Network init failed.");
            MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
            //AddStringToOutPuts(_T("Error :The com port is occupied!"));
            pWriter->OutPutsStatusInfo(srtInfo, FALSE);
            pWriter->WriteFinish(0);
            return 0;
        }
        else
        {
            CString strTemp;
            //strTemp.Format(_T("COM%d"), m_nComPort);
            CString strTips = _T("|Connect to ") +  pWriter->m_strIPAddr + _T(" successful.");
            pWriter->OutPutsStatusInfo(strTips, FALSE);
            // AddStringToOutPuts(strTips);
            SetCommunicationType(1);
        }
    }


    //���ж��豸�ڲ�����;
    unsigned short temp_read_reg[50];
    memset(temp_read_reg,0,50);

    int retry_count = 0;
    int temp_mu_ret = 0;


    int test_count = 0;


    pWriter->OutPutsStatusInfo(strTips_isp1);




    for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
    {

       // for (int Time =0; Time < pWriter->m_FlashTimes; Time++)
        {
            retry_count = 0;
            temp_mu_ret = 0;
            do
            {
                temp_mu_ret = read_multi_tap(pWriter->m_szMdbIDs[i],temp_read_reg,0,40);
                if(temp_mu_ret > 0)
                    break;
                retry_count++;
                if(retry_count > 5)
                {
                    CString strTips_isp = _T("Sub device is off line,please check the connection.");
                    pWriter->OutPutsStatusInfo(strTips_isp);
                    nFlashRet = false;
                    goto end_tcp_flash_mode;
                }

                CString temp_123;
                temp_123.Format(_T("Wait device reboot.(%d)"),5 - retry_count);
                pWriter->OutPutsStatusInfo(temp_123,true);
                Sleep(2000);


            }
            while (temp_mu_ret < 0);
            CString strTips_isp = _T("Device is online.");
            pWriter->OutPutsStatusInfo(strTips_isp);
            if(temp_read_reg[11] <=1)
            {
//             CString strTips_isp = _T("Check isp firmware version failed.(reg11 error)");
//             pWriter->OutPutsStatusInfo(strTips_isp);
//             nFlashRet = false;
//             goto end_tcp_flash_mode;
            }

            while(test_count <=8)
            {
                int nRet = Write_One(pWriter->m_szMdbIDs[i],16,127);   // ����ISPģʽ
                if(nRet >= 0)
                    break;
                if(nRet < 0)
                {
                    test_count ++ ;
                }
                if(test_count == 8)
                {
                    CString strTips_isp = _T("Write ISP command failed.(127)");
                    pWriter->OutPutsStatusInfo(strTips_isp);
                    break;
                }

                CString temp_1234;
                temp_1234.Format(_T("Write start isp command to device.(%d)"),8 - test_count);
                pWriter->OutPutsStatusInfo(temp_1234,true);

            }


            Sleep(2000);	//�ȴ��豸����ISP

            CString strID;
            strID.Format(_T("|--------------->>ID-%d-<<---------------"), pWriter->m_szMdbIDs[i]);
            pWriter->OutPutsStatusInfo(strID);
            ////��ʾflash֮ǰ��ʱ��
            pWriter->OutPutsStatusInfo(_T("|--------->>Begin"));
            pWriter->OutPutsStatusInfo(_T("|-->>Begin Time:")+GetSysTime());
            //// ��ʾflash֮ǰ���豸״̬��Ϣ
            BOOL Flag_HEX_BIN=FALSE;
//		     ((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
            pWriter->m_szMdbIDs[i] = temp_read_reg[6];
            if (pWriter->UpdataDeviceInformation_ex(temp_read_reg[7]))
            {
                int Chipsize_6 = 0;
                strTips = _T("|Programming device...");
                pWriter->OutPutsStatusInfo(strTips);


                int ModelID= temp_read_reg[7];
                if (ModelID>0)
                {
                    if (ModelID==6||ModelID==7||ModelID==8)//Tstat6,7���оƬ��С�������ô�����д�Ķ������
                    {
                        Chipsize_6 = temp_read_reg[11];


                        if (Chipsize_6<37)	//64K
                        {
                            if (pWriter->m_nHexFileType==0)
                            {
                                strTips = _T("|hex file   matches with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=TRUE;
                            }
                            else
                            {
                                int ii=0;
                                while(ii<=5)
                                {
                                    int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                strTips = _T("|hex file doesn't match with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                //AfxMessageBox(_T("hex file doesn't match with the chip"));
                                //return FALSE;
                                Flag_HEX_BIN=FALSE;
                            }
                        }
                        else	//128K
                        {
                            if (pWriter->m_nHexFileType==0)
                            {
                                int ii=0;
                                while(ii<=5)
                                {
                                    int ret=Write_One(pWriter->m_szMdbIDs[i],33,1);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                ii=0;
                                while(ii<=5)
                                {
                                    int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                strTips = _T("|hex file doesn't match with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                //AfxMessageBox(_T("hex file doesn't match with the chip"));
                                //	return FALSE;
                                Flag_HEX_BIN=FALSE;
                            }
                            else
                            {
                                strTips = _T("|hex file   matches with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=TRUE;
                            }
                        }
                    }
                    else
                    {
                        Flag_HEX_BIN=TRUE;
                    }
                }
                else
                {
                    strTips = _T("Read product type timeout!");
                    pWriter->OutPutsStatusInfo(strTips);
                    nFlashRet = false;
                    goto	 end_tcp_flash_mode;
                }
                int Chipsize = 0;
                //  pWriter->OutPutsStatusInfo(_T("The device can't match with the hex"));
#if 1		//��λ
                if (ModelID==6||ModelID==7||ModelID==8)
                    Chipsize = Chipsize_6;
                else
                    Chipsize = temp_read_reg[11];

                if (Chipsize<37)	//64K
                {


                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }


                }
                else	//128K
                {

                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i],33,0);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    //ii=0;
                    //while(ii<=5){
                    // int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                    // if (ret>0)
                    // {
                    //  break;
                    // }
                    // ii++;
                    //}



                }
#endif




            }

            if (!Flag_HEX_BIN)
            {
                continue;
            }

            if(pWriter->m_nHexFileType == 2)
            {
                CString show_section;
                show_section.Format(_T("Total section %d"),pWriter->m_szHexFileFlags.size());
                pWriter->OutPutsStatusInfo(show_section);
                int nCount = 0;
                for(UINT p = 0; p < pWriter->m_szHexFileFlags.size(); p++)
                {
                    int nBufLen = pWriter->m_szHexFileFlags[p]-nCount;

                    //if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], pWriter->m_nBufLen, (TS_UC*)pWriter->m_pFileBuffer, pParam)) < 0 )
                    if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i],nBufLen, (TS_UC*)pWriter->m_pFileBuffer+nCount, pParam)) < 0 )
                    {
                        nFailureNum++;
                        CString strTemp=_T("");
                        strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                        strFailureList+=strTemp;  // flash���ʹ��
                        switch(nFlashRet)
                        {
                        case -1:
                            strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                            break;
                        case -2:
                            strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -3:
                            strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -4:
                            strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -5:
                            strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -6:
                            strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -7:
                            strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -8:
                            strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -9:
                            strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                            break;
                        }
                        pWriter->OutPutsStatusInfo(strTemp);
                        //AfxMessageBox(strTemp);
                        pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                        pWriter->OutPutsStatusInfo(_T("|------->>End"));
                        break;

                    }
                    else
                    {
                        nCount += nBufLen;


                        CString strText;
                        strText.Format(_T("|ID %d: Programming section %d finished."), pWriter->m_szMdbIDs[i], p);
                        pWriter->OutPutsStatusInfo(strText);


                        /*CString strText;
                        strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                        pWriter->OutPutsStatusInfo(strText);*/
                        //// ��ʾflash֮ǰ���豸״̬��Ϣ
                        int time_count = 3;
                        for (int i=0; i<time_count; i++)
                        {
                            CString temp_123;
                            temp_123.Format(_T("Wait device initial.(%d)"),3 - i);
                            pWriter->OutPutsStatusInfo(temp_123,true);
                            Sleep(1000);
                        }
                        //Sleep(13000);//�ȴ�������֮��
//					((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
                        pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                        pWriter->OutPutsStatusInfo(_T("|------->>End"));
                        //AfxMessageBox(strText);
                    }


                    if (p==0)
                    {
                        int ii=0;
                        while(ii<=5)
                        {
                            int ret=Write_One(pWriter->m_szMdbIDs[i], 16, 8);
                            if (ret>0)
                            {
                                break;
                            }
                            ii++;
                        }
                        ii=0;
                        while(ii<=5)
                        {
                            int ret=Write_One(pWriter->m_szMdbIDs[i], 33, 1);
                            if (ret>0)
                            {
                                break;
                            }
                            ii++;
                        }

                    }


                    Sleep(1000); //must if not ,have some wrong
                }

            }
            else
            {
                if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], pWriter->m_nBufLen, (TS_UC*)pWriter->m_pFileBuffer, pParam)) < 0 )
                {
                    nFailureNum++;
                    CString strTemp=_T("");
                    strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                    strFailureList+=strTemp;  // flash���ʹ��
                    switch(nFlashRet)
                    {
                    case -1:
                        strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                        break;
                    case -2:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -3:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -4:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -5:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -6:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -7:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -8:
                        strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -9:
                        strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                        break;
                    }
                    pWriter->OutPutsStatusInfo(strTemp);
                    pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                    pWriter->OutPutsStatusInfo(_T("|------->>End"));
                    //AfxMessageBox(strTemp);
                }
                else
                {
                    CString strText;
                    strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(strText);
                    //// ��ʾflash֮ǰ���豸״̬��Ϣ
                    //Sleep(13000);//�ȴ�������֮��
//				((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                    pWriter->OutPutsStatusInfo(_T("|------->>End"));
                    //AfxMessageBox(strText);
                }
            }
        }
    }
end_tcp_flash_mode :
    //}//For Test
    //*******************************************************************************
    //Sleep(500);
    pWriter->WriteFinish(nFlashRet);
    close_com();
    return 1;//close thread
    //	}while(1);


}



#if 0
UINT run_back_ground_flash_thread(LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)(pParam);
    CString strFailureList;
    CString strTips;
    //	do
    //	{
    int nFlashRet=0;
    UINT i=0;
    int nFailureNum = 0;


    if(GetCommunicationType()==1)
    {
        if(Open_Socket2(pWriter->m_strIPAddr, pWriter->m_nIPPort)==false)
        {
            CString srtInfo = _T("|Error : Network init failed.");
            MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
            //AddStringToOutPuts(_T("Error :The com port is occupied!"));
            pWriter->OutPutsStatusInfo(srtInfo, FALSE);
            pWriter->WriteFinish(0);
            return 0;
        }
        else
        {
            CString strTemp;
            //strTemp.Format(_T("COM%d"), m_nComPort);
            CString strTips = _T("|Connect to ") +  pWriter->m_strIPAddr + _T(" successful.");
            pWriter->OutPutsStatusInfo(strTips, FALSE);
            // AddStringToOutPuts(strTips);
        }


        if(read_one(pWriter->m_szMdbIDs[0],16,5) < 0)
        {
            CString strTips_isp = _T("Sub device is off line,please check the connection.");
            pWriter->OutPutsStatusInfo(strTips_isp);
            nFlashRet = false;
            goto end_tcp_flash_mode;
        }

        int test_count = 0;
        while(test_count <=5)
        {
            int nRet = Write_One(pWriter->m_szMdbIDs[0],16,127);   // ����ISPģʽ
            if(nRet >= 0)
                break;
            if(nRet < 0)
            {
                test_count ++ ;
            }
            if(test_count == 8)
            {
                break;
                //if(read_one(pWriter->m_szMdbIDs[0],16) != 127)
                //{
                //	CString strTips_isp = _T("Enter ISP Mode fail,please check the connection.");
                //	pWriter->OutPutsStatusInfo(strTips_isp);
                //	nFlashRet = false;
                //	goto end_tcp_flash_mode;
                //}

            }
        }


    }

    //Add by Fance  �����Ӧ�ô������� ISP  16д127��  ��Ҫ�� 11�żĴ���  11�� ����1  ˵����ת�ɹ�����������ȴ�;
    strTips = _T("Wait device jump to isp mode!");
    pWriter->OutPutsStatusInfo(strTips);
    int re_count = 0;
    int nnn_ret = 0;
    do
    {
        nnn_ret  = read_one(pWriter->m_szMdbIDs[0],11);
        if(nnn_ret > 1)
            break;
        Sleep(1000);
        re_count ++;
        if(re_count == 15)
        {
            CString strTips_isp = _T("Device is off line ,read firmware version timeout.");
            pWriter->OutPutsStatusInfo(strTips_isp);
            nFlashRet = false;
            goto end_tcp_flash_mode;
        }
    }
    while (nnn_ret > 1);







    for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
    {

        CString strID;
        strID.Format(_T("|--------------->>ID-%d-<<---------------"), pWriter->m_szMdbIDs[i]);
        pWriter->OutPutsStatusInfo(strID);
        ////��ʾflash֮ǰ��ʱ��
        pWriter->OutPutsStatusInfo(_T("|--------->>Begin"));
        pWriter->OutPutsStatusInfo(_T("|-->>Begin Time:")+GetSysTime());
        //// ��ʾflash֮ǰ���豸״̬��Ϣ
        BOOL Flag_HEX_BIN=FALSE;
//		((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
        if (pWriter->UpdataDeviceInformation(pWriter->m_szMdbIDs[i]))
        {
            int Chipsize_6 = 0;
            strTips = _T("|Programming device...");
            pWriter->OutPutsStatusInfo(strTips);
            int nRet = Write_One(pWriter->m_szMdbIDs[i],16,127);   // ����ISPģʽ
            if(nRet < 0)
                Write_One(pWriter->m_szMdbIDs[i],16,127);   // ����ISPģʽ
            Sleep(2000);
            int ModelID= read_one(pWriter->m_szMdbIDs[i],7,5);
            if (ModelID>0)
            {
                if (ModelID==6||ModelID==7)//Tstat6,7���оƬ��С�������ô�����д�Ķ������
                {
                    Sleep(1000);

                    Chipsize_6 =read_one(pWriter->m_szMdbIDs[i],11,5);


                    if (Chipsize_6<37)	//64K
                    {
                        if (pWriter->m_nHexFileType==0)
                        {
                            strTips = _T("|hex file   matches with the chip!");
                            pWriter->OutPutsStatusInfo(strTips);
                            Flag_HEX_BIN=TRUE;
                        }
                        else
                        {
                            int ii=0;
                            while(ii<=5)
                            {
                                int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                                if (ret>0)
                                {
                                    break;
                                }
                                ii++;
                            }
                            strTips = _T("|hex file doesn't match with the chip!");
                            pWriter->OutPutsStatusInfo(strTips);
                            //AfxMessageBox(_T("hex file doesn't match with the chip"));
                            //return FALSE;
                            Flag_HEX_BIN=FALSE;
                        }
                    }
                    else	//128K
                    {
                        if (pWriter->m_nHexFileType==0)
                        {
                            int ii=0;
                            while(ii<=5)
                            {
                                int ret=Write_One(pWriter->m_szMdbIDs[i],33,1);
                                if (ret>0)
                                {
                                    break;
                                }
                                ii++;
                            }
                            ii=0;
                            while(ii<=5)
                            {
                                int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                                if (ret>0)
                                {
                                    break;
                                }
                                ii++;
                            }
                            strTips = _T("|hex file doesn't match with the chip!");
                            pWriter->OutPutsStatusInfo(strTips);
                            //AfxMessageBox(_T("hex file doesn't match with the chip"));
                            //	return FALSE;
                            Flag_HEX_BIN=FALSE;
                        }
                        else
                        {
                            strTips = _T("|hex file   matches with the chip!");
                            pWriter->OutPutsStatusInfo(strTips);
                            Flag_HEX_BIN=TRUE;
                        }
                    }
                }
                else
                {
                    Flag_HEX_BIN=TRUE;
                }
            }
            else
            {
                strTips = _T("Read product type timeout!");
                pWriter->OutPutsStatusInfo(strTips);
                nFlashRet = false;
                goto	 end_tcp_flash_mode;
            }
            int Chipsize = 0;
            //  pWriter->OutPutsStatusInfo(_T("The device can't match with the hex"));
#if 1		//��λ
            if (ModelID==6||ModelID==7)
                Chipsize = Chipsize_6;
            else
                Chipsize = read_one(pWriter->m_szMdbIDs[i],11,5);
            if(Chipsize < 0)
            {
                strTips = _T("Read chip size timeout!");
                pWriter->OutPutsStatusInfo(strTips);
                nFlashRet = false;
                goto	 end_tcp_flash_mode;
            }
            if (Chipsize<37)	//64K
            {


                int ii=0;
                while(ii<=5)
                {
                    int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                    if (ret>0)
                    {
                        break;
                    }
                    ii++;
                }


            }
            else	//128K
            {

                int ii=0;
                while(ii<=5)
                {
                    int ret=Write_One(pWriter->m_szMdbIDs[i],33,0);
                    if (ret>0)
                    {
                        break;
                    }
                    ii++;
                }
                //ii=0;
                //while(ii<=5){
                // int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                // if (ret>0)
                // {
                //  break;
                // }
                // ii++;
                //}



            }
#endif




        }




        ////////	ASSERT(pWriter->m_nBufLen <= c_nHexFileBufLen);
        //�ڴ˴��������ݿ�У��
        //int result=((CISPDlg*)pWriter->m_pParentWnd)->Judge_Model_Version();
        //if(result==OK_HEXFILEPREFIX)	 //��֤�ɹ�
        //{

        //pWriter->OutPutsStatusInfo(_T("|Verifying successfully!"));
        //pWriter->OutPutsStatusInfo(_T("|Your file matchs with the flashing device."));

        //}
        //else if(result==ERR_HEXFILEPREFIX)	 //��֤ʧ��
        //{
        //pWriter->OutPutsStatusInfo(_T("|Verifying unsuccessfully!"));
        //pWriter->OutPutsStatusInfo(_T("|Your file name can't matchs with the flashing device."));
        //continue;
        //}
        //else					   //���ݿ����ǿյģ�û�в鵽��Ӧ��prefix
        //{	pWriter->OutPutsStatusInfo(_T("|Your device information is not in Database!"));
        //pWriter->OutPutsStatusInfo(_T("|Directly flash your device."));
        //}
        if (!Flag_HEX_BIN)
        {
            continue;
        }

        if(pWriter->m_nHexFileType == 2)
        {
            CString show_section;
            show_section.Format(_T("Total section %d"),pWriter->m_szHexFileFlags.size());
            pWriter->OutPutsStatusInfo(show_section);
            int nCount = 0;
            for(UINT p = 0; p < pWriter->m_szHexFileFlags.size(); p++)
            {
                int nBufLen = pWriter->m_szHexFileFlags[p]-nCount;

                //if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], pWriter->m_nBufLen, (TS_UC*)pWriter->m_pFileBuffer, pParam)) < 0 )
                if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i],nBufLen, (TS_UC*)pWriter->m_pFileBuffer+nCount, pParam)) < 0 )
                {
                    nFailureNum++;
                    CString strTemp=_T("");
                    strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                    strFailureList+=strTemp;  // flash���ʹ��
                    switch(nFlashRet)
                    {
                    case -1:
                        strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                        break;
                    case -2:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -3:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -4:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -5:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -6:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -7:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -8:
                        strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -9:
                        strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                        break;
                    }
                    pWriter->OutPutsStatusInfo(strTemp);
                    //AfxMessageBox(strTemp);
                    pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                    pWriter->OutPutsStatusInfo(_T("|------->>End"));
                    break;

                }
                else
                {
                    nCount += nBufLen;


                    CString strText;
                    strText.Format(_T("|ID %d: Programming section %d finished."), pWriter->m_szMdbIDs[i], p);
                    pWriter->OutPutsStatusInfo(strText);


                    /*CString strText;
                    strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(strText);*/
                    //// ��ʾflash֮ǰ���豸״̬��Ϣ
                    Sleep(13000);//�ȴ�������֮��
//					((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                    pWriter->OutPutsStatusInfo(_T("|------->>End"));
                    //AfxMessageBox(strText);
                }


                if (p==0)
                {
                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i], 16, 8);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i], 33, 1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }

                }


                Sleep(1000); //must if not ,have some wrong
            }

        }
        else
        {
            if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], pWriter->m_nBufLen, (TS_UC*)pWriter->m_pFileBuffer, pParam)) < 0 )
            {
                nFailureNum++;
                CString strTemp=_T("");
                strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                strFailureList+=strTemp;  // flash���ʹ��
                switch(nFlashRet)
                {
                case -1:
                    strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                    break;
                case -2:
                    strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                    break;
                case -3:
                    strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                    break;
                case -4:
                    strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                    break;
                case -5:
                    strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                    break;
                case -6:
                    strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                    break;
                case -7:
                    strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                    break;
                case -8:
                    strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                    break;
                case -9:
                    strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                    break;
                }
                pWriter->OutPutsStatusInfo(strTemp);
                pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                pWriter->OutPutsStatusInfo(_T("|------->>End"));
                //AfxMessageBox(strTemp);
            }
            else
            {
                CString strText;
                strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                pWriter->OutPutsStatusInfo(strText);
                //// ��ʾflash֮ǰ���豸״̬��Ϣ
                //Sleep(13000);//�ȴ�������֮��
//				((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
                pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                pWriter->OutPutsStatusInfo(_T("|------->>End"));
                //AfxMessageBox(strText);
            }
        }
    }
end_tcp_flash_mode :
    //}//For Test
    //*******************************************************************************
    //Sleep(500);
    pWriter->WriteFinish(nFlashRet);
    close_com();
    return 1;//close thread
    //	}while(1);


}
#endif
//////////////////////////////////////////////////////////////////////////
//the return value 1,successful,   return < 0 ,have some trouble
int flash_a_tstat(BYTE m_ID, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)pParam;
    const int  RETRY_TIMES = 5;
    TS_UC *register_data=register_data_orginal;
    unsigned int ii=0;

    //*************inspect the flash that last flash position ***********************
    int x = Read_One(m_ID,0xee10);

// 	while(Read_One(m_ID,0xee10)<0) //the return value == -1 ,no connecting
// 	{
// 		//AfxMessageBox("before write,need to open the com!");
// 		//close_com();
// 		if(ii<5)
// 		{
// 			ii++;
// 			continue;
// 		}
// 		return -1;//error -1
// 	}

    //************* begin to flash ***********************
    ii=0;
    if(Read_One(m_ID,0xee10)==0x40 || Read_One(m_ID,0xee10)==0x1f) // ��ee10�� why��
    {
        if(IDOK==AfxMessageBox(_T("Previous Update was interrupted.\nPress OK to Resume.\nCancel to Restart."),MB_OKCANCEL))
        {
            // ѡȷ��
            ii=0xEE00+17;
            int l=0;//temp;<200
            do
            {
                int uc_temp1=Read_One(m_ID,ii);
                int uc_temp2=Read_One(m_ID,ii+1);
                if(uc_temp1==0x00 && uc_temp2==0x00 )
                    ii+=2;
                else if(l==0)
                {
                    if(uc_temp1==0xf0 && uc_temp2==0xf0)
                    {
                        ii=0;
                        break;
                    }
                }
                else
                {
                    ii=uc_temp1*256+uc_temp2;
                    break;
                }
                l++;
            }
            while(l<200);
        }
        else // ѡȡ��
        {
            //from 0000 flash update
            ii=0;//from 0000 register flash
            CString srtInfo = _T("|Initializing device...");
            pWriter->OutPutsStatusInfo(srtInfo);
            //********************write register 16 value 0x7f **************
            Sleep(500);
            do
            {
                if(ii<RETRY_TIMES)
                    if(-2==Write_One(m_ID,16,0x7f))
                    {
                        ii++;
                        Sleep(6000);
                    }
                    else
                        ii=0;
                else
                {
                    //AfxMessageBox("Unable to Initialize...");
                    //close_com();
                    return -2;//error -2 Unable to Initialize...
                }
            }
            while(ii);

            //********************write register 16 value 0x3f **************
            ii=0;
            srtInfo = _T("|Erasing device...");
            pWriter->OutPutsStatusInfo(srtInfo);
            do
            {
                if(ii<RETRY_TIMES)
                {
                    if(-2==Write_One(m_ID,16,0x3f))
                    {
                        ii++;
                    }
                    else
                    {
                        ii=0;
                    }
                }
                else
                {
                    //AfxMessageBox("Unable to Erase...");
                    //close_com();
                    return -3;//error -3
                }
            }
            while(ii);

            //********************write register 16 value 0x1f **************
            ii=0;
            Sleep(7000);//must have this ,the Tstat need
            do
            {
                if(ii<RETRY_TIMES)
                    if(-2==Write_One(m_ID,16,0x1f))
                        ii++;
                    else
                        ii=0;
                else
                {
                    //AfxMessageBox("Unable to start programming...");
                    //close_com();
                    return -4;//error -4
                }
            }
            while(ii);
            //***************send data to com*************************
            ii=0;//to the register 0000
        }
    }
    else  // ��ee10 ʧ�ܣ�
    {
        //from 0000 flash update
        ii=0;//from 0000 register flash
        CString srtInfo = _T("|Initializing device...");
        pWriter->OutPutsStatusInfo(srtInfo);
        //writing_row++;
        Sleep(500);

        //********************write register 16 value 0x7f **************
        //x=Write_One(m_ID,16,0x7f);
        do
        {
            if(Write_One(m_ID,16,0x7f)<0)
            {
                ii++;
                Sleep(1000);
            }
            else
            {
                break;
            }

        }
        while(ii<RETRY_TIMES);
// 		if (ii>=RETRY_TIMES)
// 		{
//
//             //AfxMessageBox("Unable to Initialize...");
//             //close_com();
//             return -5;//error -5
//
// 		}
        //********************write register 16 value 0x3f **************
        ii=0;
        srtInfo = _T("|Erasing device...";);
        pWriter->OutPutsStatusInfo(srtInfo);
        //writing_row++;
// 		do{
// 				//if(ii<RETRY_TIMES)
// 				//	//if(-2==Write_One(m_ID,16,0x3f))
// 				//	if(Write_One(m_ID,16,0x3f)==-2)
// 				//	{
// 				//			ii++;
// 				//	}
// 				//	else
// 				//			ii=0;
//             if(ii<RETRY_TIMES&&(-3==Write_One(m_ID,16,0x3f)))
//
//             {
//                 ii++;
//                 Sleep(1000);
//             }
// 				else
// 				{
// 					//AfxMessageBox("Unable to Erase...");
// 					//close_com();
// 					return -6;//error -6
// 				}
// 			}while(ii);

        do
        {
            if(Write_One(m_ID,16,0x3f)<0)
            {
                ii++;
                Sleep(1000);
            }
            else
            {
                break;
            }

        }
        while(ii<RETRY_TIMES);
        //if (ii>=RETRY_TIMES)
        //{

        //    //AfxMessageBox("Unable to Initialize...");
        //    //close_com();
        //    return -6;//error -5

        //}
        //********************write register 16 value 0x1f **************
        ii=0;
        Sleep(7000);//must have this ,the Tstat need


        do
        {
            if(Write_One(m_ID,16,0x1f)<0)
            {
                ii++;
                Sleep(1000);
            }
            else
            {
                break;
            }

        }
        while(ii<RETRY_TIMES);
//             if (ii>=RETRY_TIMES)
//             {
//
//                 //AfxMessageBox("Unable to Initialize...");
//                 //close_com();
//                 return -7;//error -5
//
//             }

        //do{
        //	if(ii<RETRY_TIMES)
        //		//if(-2==Write_One(m_ID,16,0x1f))
        //		if(Write_One(m_ID,16,0x1f)<0)
        //			ii++;
        //		else
        //			ii=0;
        //	else
        //	{
        //		//AfxMessageBox("Unable to start programming...");
        //		//close_com();
        //		return -7;//error -7
        //	}
        //}while(ii);

        //***************send data to com*************************
        ii=0;//to the register 0000
    } // ��ee10 �Ĵ������

    int persentfinished=0;
    CString srtInfo;
    srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(0%%)"),m_ID,ii,ii+128);
    pWriter->OutPutsStatusInfo(srtInfo);
//	while(ii<=the_max_register_number_parameter)
    while(ii<the_max_register_number_parameter)
    {
        if (pWriter->m_bStopWrite)
        {
            return -9;
        }
        TS_UC data_to_send[160]= {0}; // buffer that writefile() will to use
        int itemp=0;

        persentfinished = ((ii+128)*100)/the_max_register_number_parameter;
        if(persentfinished>100)
            persentfinished=100;
        CString srtInfo;
        srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(%d%%)"),m_ID,ii,ii+128,persentfinished);
        pWriter->OutPutsStatusInfo(srtInfo, TRUE);
        do
        {
            if(itemp<RETRY_TIMES)
            {

                if(-2==write_multi(m_ID,&register_data[ii],ii,128))//to write multiple 128 bytes
                    itemp++;
                else
                    itemp=0;
            }
            else
            {
                //srtInfo.Format(_T("Communication was interrupted.Tryiny connect agina!"));
                //pWriter->OutPutsStatusInfo(srtInfo, TRUE);
                Sleep(5000);//��� 5��ʧ�ܣ��� �ȴ�5s���ڿ�ʼ��������2�Ρ�

                if(itemp<RETRY_TIMES+3)
                {
                    Read_One(255,1);//����������Ͽ�����֮�����������Զ� ���ϴ����ӵĶ˿ڵ�;
                    if(-2==write_multi(m_ID,&register_data[ii],ii,128))//to write multiple 128 bytes
                        itemp++;
                    else
                        itemp=0;
                }
                else
                {
                    //AfxMessageBox("update was interrupted!\nPlease verify connection.");
                    //close_com();
                    return -8;//the com connection is wrong! error -8
                }

            }
        }
        while(itemp);
        /*CString temp,aline;
        for (int i=ii;i<ii+128;i++)
        {
        temp.Format(_T("%X"),register_data[i]);
        aline+=temp;
        }
        aline+=_T("\n");
        pWriter->OutPutsStatusInfo(aline,FALSE);*/
        ii+=128;
    }

    //********************write register 16 value 0x01 **************
    ii=0;
    do
    {
        if(ii<RETRY_TIMES)
        {
            if(-2==Write_One(m_ID,16,1))
                ii++;
            else
                ii=0;
        }
        else
        {
            return -8;//error -8
        }
    }
    while(ii);

    return 1;
}
//////////////////////////////////////////////////////////////////////////
//the return value 1,successful,   return < 0 ,have some trouble
int flash_a_tstat_RAM(BYTE m_ID,int section, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)pParam;
    const int  RETRY_TIMES = 5;
    TS_UC *register_data=register_data_orginal;
    unsigned int ii=0;



    int persentfinished=0;
    if(section==0)
    {
        CString srtInfo;
        srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(0%%)"),m_ID,ii,ii+128);
        pWriter->OutPutsStatusInfo(srtInfo);
    }
    else
    {
        persentfinished = ((the_max_register_number_parameter_Finished)*100)/the_max_register_number_parameter_Count;
        CString srtInfo;
        srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(%d%%)"),m_ID,ii,ii+128,persentfinished);
        pWriter->OutPutsStatusInfo(srtInfo,TRUE);
    }
    ii=0;
    do
    {

        if(Write_One(m_ID,12,section)<0)
        {
            ii++;
        }
        else
        {
            ii=0;
            break;
        }
    }
    while(ii<RETRY_TIMES);
    if (ii >= RETRY_TIMES)
    {
        CString str;
        str.Format(_T("Write 12 = %d Failed"),section);
        AfxMessageBox(str);
        return -8;

    }
    //	while(ii<=the_max_register_number_parameter)
    while(ii<the_max_register_number_parameter)
    {
        if (pWriter->m_bStopWrite)
        {
            return -9;
        }
        TS_UC data_to_send[160]= {0}; // buffer that writefile() will to use
        int itemp=0;

        //persentfinished = ((ii+128)*100)/the_max_register_number_parameter;
        persentfinished = ((the_max_register_number_parameter_Finished+ii+128)*100)/the_max_register_number_parameter_Count;
        if(persentfinished>100)
            persentfinished=100;
        CString srtInfo;
        srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(%d%%)"),m_ID,the_max_register_number_parameter_Finished+ii,the_max_register_number_parameter_Finished+ii+128,persentfinished);
        pWriter->OutPutsStatusInfo(srtInfo, TRUE);

         

        do
        {
            /*   if(itemp<RETRY_TIMES)
               {*/
            //ii+0x8000
            if(write_multi(m_ID,&register_data[ii],ii,128)<0)//to write multiple 128 bytes
            {
                itemp++;
            }
            else
            {
                itemp=0;
                break;
            }


            /*     }*/


        }
        while(itemp<RETRY_TIMES);

         

        //srtInfo.Format(_T("Communication was interrupted.Tryiny connect agina!"));
        //pWriter->OutPutsStatusInfo(srtInfo, TRUE);
//             Sleep(5000);//��� 5��ʧ�ܣ��� �ȴ�5s���ڿ�ʼ��������2�Ρ�
//
//             if(itemp<RETRY_TIMES+3)
//             {
//                 if(write_multi(m_ID,&register_data[ii],ii,128)<0)//to write multiple 128 bytes
//                     itemp++;
//
//             }
//             else
//             {
//
//                 return -8;
//             }




        if (itemp >= RETRY_TIMES)
        {
            CString str;
            str.Format(_T("Write Flash ID = %d To = 128*%d "),m_ID,ii);
            AfxMessageBox(str);
            return -8;

        }

        ii+=128;
    }




    return 1;
}

BOOL CComWriter::EndFlash()
{
    return StopWrite();
}

BOOL CComWriter::StopWrite()
{
    m_bStopWrite = TRUE;
    if(WaitForSingleObject(m_pWorkThread->m_hThread, 3000) == WAIT_OBJECT_0)
    {
        //AfxMessageBox(_T("Thread has been stoped!"), MB_OK);
        //return TRUE;
    }
    else
    {
        BOOL bRet = TerminateThread(m_pWorkThread->m_hThread, 0);
        //AfxMessageBox(_T("Thread has been terminated!"), MB_OK);
    }


    m_pWorkThread = NULL;
    return TRUE;
}


void CComWriter::OutPutsStatusInfo(const CString& strInfo, BOOL bReplace)
{
    int nCount = strInfo.GetLength();
    WCHAR* strNew = new WCHAR[nCount+1];
    ZeroMemory(strNew, (nCount+1)*sizeof(WCHAR));
    LPCTSTR str = LPCTSTR(strInfo);
    memcpy(strNew, str, nCount*sizeof(WCHAR));
    //g_strFlashInfo = strInfo;
    int nRet = 0;
    if (bReplace)
    {
        nRet =PostMessage(m_pParentWnd->m_hWnd, WM_REPLACE_STATUSINFO, 0, LPARAM(strNew));
    }
    else
    {
        nRet =PostMessage(m_pParentWnd->m_hWnd, WM_ADD_STATUSINFO, 0,LPARAM(strNew));
    }

}


//////////////////////////////////////////////////////////////////////////
// flash ���ˣ����۳ɹ�����ʧ�ܣ���֪ͨ������
// ��������flash�̵߳ķ���ֵ
void CComWriter::WriteFinish(int nFlashFlag)
{
    int	nRet =PostMessage(m_pParentWnd->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(nFlashFlag));
}


void CComWriter::SetHexFileType(HEXFILE_FORMAT nHexFileType)
{
    m_nHexFileType = nHexFileType;
}


int CComWriter::WirteExtendHexFileByCom()
{
    //WriteCommandtoReset();

    HCURSOR hc;//load mouse cursor
    hc = LoadCursor(NULL,IDC_WAIT);
    hc = SetCursor(hc);

    if(open_com(m_nComPort)==false)
    {
        CString srtInfo = _T("|Error :The com port is occupied!");
        //MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
        //AddStringToOutPuts(_T("Error :The com port is occupied!"));
        OutPutsStatusInfo(srtInfo, FALSE);
        return 0;
    }
    else
    {
        CString strTemp;
        strTemp.Format(_T("COM%d"), m_nComPort);
        CString strTips = _T("|Open ") +  strTemp + _T(" successful.");
        OutPutsStatusInfo(strTips, FALSE);
        // AddStringToOutPuts(strTips);

    }


    CString strTips = _T("|Programming device...");
    OutPutsStatusInfo(strTips);
    //AddStringToOutPuts(strTips);
    m_pWorkThread=AfxBeginThread(flashThread_ForExtendFormatHexfile, this); //create thread,read information

    ASSERT(m_pWorkThread);

    return 1;
}

int CComWriter::WirteExtendHexFileByCom_RAM()
{
    //WriteCommandtoReset();

    HCURSOR hc;//load mouse cursor
    hc = LoadCursor(NULL,IDC_WAIT);
    hc = SetCursor(hc);

    if(open_com(m_nComPort)==false)
    {
        CString srtInfo = _T("|Error :The com port is occupied!");

        //MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
        //AddStringToOutPuts(_T("Error :The com port is occupied!"));
        OutPutsStatusInfo(srtInfo, FALSE);
        return 0;
    }
    else
    {
        CString strTemp;
        strTemp.Format(_T("COM%d"), m_nComPort);
        CString strTips = _T("|Open ") +  strTemp + _T(" successful.");
        OutPutsStatusInfo(strTips, FALSE);
        Change_BaudRate (m_nBautrate);
        // AddStringToOutPuts(strTips);

    }


    CString strTips = _T("|Programming device...");
    OutPutsStatusInfo(strTips);
    //AddStringToOutPuts(strTips);
    m_pWorkThread=AfxBeginThread(flashThread_ForExtendFormatHexfile_RAM, this); //create thread,read information

    ASSERT(m_pWorkThread);

    return 1;
}


UINT flashThread_ForExtendFormatHexfile(LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)(pParam);
    CString strFailureList;
    CString strTips;
    int nFlashRet=0;
    UINT i=0;
    int nFailureNum = 0;

    UINT times=0;

#if 1

    for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
    {
     //   for (int Time =0; Time < pWriter->m_FlashTimes; Time++)
        {
            CString strID;
            strID.Format(_T("|Current Programming device ID is : %d"), pWriter->m_szMdbIDs[i]);
            pWriter->OutPutsStatusInfo(strID);
            BOOL Flag_HEX_BIN=FALSE;
            if (pWriter->UpdataDeviceInformation(pWriter->m_szMdbIDs[i]))
            {

                int nRet = Write_One(pWriter->m_szMdbIDs[i],16,127);   // ����ISPģʽ
                if(nRet < 0)
                    Write_One(pWriter->m_szMdbIDs[i],16,127);   // ����ISPģʽ

                //Add by Fance  �����Ӧ�ô������� ISP  16д127��  ��Ҫ�� 11�żĴ���  11�� ����1  ˵����ת�ɹ�����������ȴ�;
                strTips = _T("Wait device jump to isp mode!");
                pWriter->OutPutsStatusInfo(strTips);
                int re_count = 0;
                int nnn_ret = 0;
                do
                {
                    nnn_ret  = read_one(pWriter->m_szMdbIDs[i],11);
                    if(nnn_ret > 1)
                        break;
                    Sleep(1000);
                    re_count ++;
                    if(re_count == 15)
                        break;
                }
                while (nnn_ret > 1);

                // Sleep(2000);
                int ModelID= read_one(pWriter->m_szMdbIDs[i],7,5);
                if (ModelID>0)
                {
                    if (ModelID==6||ModelID==7||ModelID==8)//Tstat6,7���оƬ��С�������ô�����д�Ķ������
                    {
                        int Chipsize=read_one(pWriter->m_szMdbIDs[i],11,5);
                        if(Chipsize < 0)
                        {
                            nFlashRet = false;
                            goto end_isp_flash;
                        }


                        if (Chipsize<37)	//64K
                        {
                            if (pWriter->m_nHexFileType==0)
                            {
                                strTips = _T("|hex file   matches with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=TRUE;
                            }
                            else
                            {
                                int ii=0;
                                while(ii<=5)
                                {
                                    int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                strTips = _T("|hex file doesn't match with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=FALSE;

                            }
                        }
                        else	//128K
                        {
                            if (pWriter->m_nHexFileType==0)
                            {
                                int ii=0;
                                while(ii<=5)
                                {
                                    int ret=Write_One(pWriter->m_szMdbIDs[i],33,0);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                //ii=0;
                                //while(ii<=5){
                                // int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                                // if (ret>0)
                                // {
                                //  break;
                                // }
                                // ii++;
                                //}
                                strTips = _T("|hex file doesn't match with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                //AfxMessageBox(_T("hex file doesn't match with the chip"));
                                // return FALSE;
                                Flag_HEX_BIN=FALSE;
                            }
                            else
                            {
                                strTips = _T("|hex file   matches with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=TRUE;
                            }
                        }
                    }
                }
                else
                {
                    Flag_HEX_BIN=TRUE;
                }

                // pWriter->OutPutsStatusInfo(_T("The device can't match with the hex"));
#if 1		//��λ
                int Chipsize=read_one(pWriter->m_szMdbIDs[i],11,5);
                if (Chipsize<37)	//64K
                {


                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }


                }
                else	//128K
                {

                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i],33,0);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    //ii=0;
                    //while(ii<=5){
                    // int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                    // if (ret>0)
                    // {
                    //  break;
                    // }
                    // ii++;
                    //}



                }
#endif

            }

            if (!Flag_HEX_BIN)
            {
                continue;
            }

            int nCount = 0;
            for(UINT p = 0; p < pWriter->m_szHexFileFlags.size(); p++)
            {
                int nBufLen = pWriter->m_szHexFileFlags[p]-nCount;

                if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], nBufLen, (TS_UC*)(pWriter->m_pExtendFileBuffer+nCount), pParam)) < 0 )
                {
                    nFailureNum++;
                    CString strTemp=_T("");
                    strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                    strFailureList+=strTemp;  // flash���ʹ��
                    switch(nFlashRet)
                    {
                    case -1:
                        strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                        break;
                    case -2:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -3:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -4:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -5:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -6:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -7:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -8:
                        strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -9:
                        strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                        break;
                    }
                    pWriter->OutPutsStatusInfo(strTemp);
                    //AfxMessageBox(strTemp);
                    break;
                }
                else
                {
                    nCount += nBufLen;

                    CString strText;
                    strText.Format(_T("|ID %d: Programming section %d finished."), pWriter->m_szMdbIDs[i], p);
                    pWriter->OutPutsStatusInfo(strText);

                    //AfxMessageBox(strText);
                }

                if (p==0)
                {
                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i], 16, 8);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i], 33, 1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                        if(ii == 5)
                        {
                            nFlashRet = false;
                            goto end_isp_flash;
                        }
                    }
                }

                Sleep(500); //must if not ,have some wrong

            }

            if(nFlashRet > 0) // flash �ɹ�
            {
                CString strText;
                strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                pWriter->OutPutsStatusInfo(strText);
                //AfxMessageBox(strText);
            }
        }
    }

#endif









//*******************************************************************************
end_isp_flash:
    pWriter->WriteFinish(nFlashRet);
    Sleep(500);

    close_com();
    return 1;//close thread
//	}while(1);


}
void CComWriter::SetHexInfor(Bin_Info temp)
{
    int i;
    for (  i=0; i<5; i++)
    {
        m_hexinfor.company[i]=temp.company[i];
    }
    for (  i=0; i<10; i++)
    {
        m_hexinfor.product_name[i]=temp.product_name[i];
    }
    for (  i=0; i<3; i++)
    {
        m_hexinfor.reserved[i]=temp.reserved[i];
    }
    m_hexinfor.software_high=temp.software_high;
    m_hexinfor.software_low=temp.software_low;
}



BOOL CComWriter::UpdataDeviceInformation_ex(unsigned short device_productID)
{
    CString strtips;

    CString str_ret,temp;
    
    CString hexproductname=_T("");
    //  int ret=read_multi(ID,&Device_infor[0],0,10);



    CString prodcutname=GetProductName(device_productID);
	if (device_productID == 255 || device_productID == 0)
	{
		return TRUE;
	}
//     if(READ_SUCCESS != Get_HexFile_Information(m_hexbinfilepath.GetBuffer(),temp1,0x100))
//     {
//         //AfxMessageBox(_T("The hex file dones't contains Temco logo,Can't flash into our products!"));
//         strtips.Format(_T("The hex file doesn't contains Temco logo,Can't flash into our products!"));
//         OutPutsStatusInfo(strtips,false);
//         return FALSE;
//     }



    for (int i=0; i<10; i++)
    {
        hexproductname.AppendFormat(_T("%c"),global_fileInfor.product_name[i]);
    }
    prodcutname.MakeLower();


    hexproductname.MakeLower();

    prodcutname.MakeUpper();
    hexproductname.MakeUpper();

	CString Temco_logo;
	MultiByteToWideChar( CP_ACP, 0, (char *)global_fileInfor.company,
		(int)strlen(global_fileInfor.company)+1,
		Temco_logo.GetBuffer(MAX_PATH), MAX_PATH );
	Temco_logo.ReleaseBuffer();
	Temco_logo.MakeUpper();

	if (Temco_logo.Find(L"CO2")!=-1)
	{
		Temco_logo.TrimRight();
		Temco_logo.TrimLeft();
		hexproductname = Temco_logo;
	}

    //   if (hexproductname.CompareNoCase(_T("CO3"))==0)
    //   {
    //      hexproductname=_T("CO2");
    //   }
    if(hexproductname.CompareNoCase(prodcutname)==0)
    {
        return TRUE;
    }//TStatRunar
    else if ((
                 (prodcutname.CompareNoCase(_T("tstat5e"))==0)
                 ||(prodcutname.CompareNoCase(_T("tstat5h"))==0)
                 ||(prodcutname.CompareNoCase(_T("tstat5g"))==0)
             )
             &&(hexproductname.CompareNoCase(_T("tstat5lcd"))==0)
            )
    {
        return TRUE;
    }
    else if (((prodcutname.CompareNoCase(_T("tstat5a"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5b"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5c"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5d"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5f"))==0)
             )
             &&(hexproductname.CompareNoCase(_T("tstat5led"))==0)
            )
    {
        return TRUE;
    }
    else if ((hexproductname.CompareNoCase(_T("tstat6"))==0)&&(prodcutname.CompareNoCase(_T("tstat5i"))==0))
    {
        return TRUE;
    }
    else
    {
        strtips.Format(_T("Your device is %s   Your hex file is fit for %s "),prodcutname.GetBuffer(),hexproductname.GetBuffer());
        OutPutsStatusInfo(strtips,false);
        return FALSE;
    }



}



BOOL CComWriter::UpdataDeviceInformation(int& ID)
{
    CString strtips;
    unsigned short Device_infor[10];
    CString str_ret,temp;
 
    CString hexproductname=_T("");

    //  int ret=read_multi(ID,&Device_infor[0],0,10);
    int ret=0;
    int resend_count = 0;
    do
    {
        ret = read_multi_tap(ID,&Device_infor[0],0,10);
        if(ret >= 0)
            break;
        resend_count ++ ;
        if(resend_count >15)
        {
            strtips.Format(_T("Device is offline,Please check the connection!"));
            OutPutsStatusInfo(strtips,false);
            return FALSE;
        }
        Sleep(1000);

    }
    while (ret < 0);

	if (Device_infor[7] == 255 || Device_infor[7] == 0)
	{
		return TRUE;
	}
	 

    CString prodcutname=GetProductName(Device_infor[7]);

 





    for (int i=0; i<10; i++)
    {
        hexproductname.AppendFormat(_T("%c"),global_fileInfor.product_name[i]);
    }
	CString Temco_logo;
	MultiByteToWideChar( CP_ACP, 0, (char *)global_fileInfor.company,
		(int)strlen(global_fileInfor.company)+1,
		Temco_logo.GetBuffer(MAX_PATH), MAX_PATH );
	Temco_logo.ReleaseBuffer();
	Temco_logo.MakeUpper();

	if (Temco_logo.Find(L"CO2")!=-1)
	{
		hexproductname = Temco_logo;
	}

    prodcutname.MakeLower();


    hexproductname.MakeLower();

    prodcutname.MakeUpper();
    hexproductname.MakeUpper();
	hexproductname.TrimLeft();
	hexproductname.TrimRight();
    BOOL Ret_Result = TRUE;


    if(hexproductname.CompareNoCase(prodcutname)==0)
    {
        Ret_Result= TRUE;
    }//TStatRunar
    else if ((
                 (prodcutname.CompareNoCase(_T("tstat5e"))==0)
                 ||(prodcutname.CompareNoCase(_T("tstat5h"))==0)
                 ||(prodcutname.CompareNoCase(_T("tstat5g"))==0)
             )
             &&(hexproductname.CompareNoCase(_T("tstat5lcd"))==0)
            )
    {
        Ret_Result= TRUE;
    }
    else if (((prodcutname.CompareNoCase(_T("tstat5a"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5b"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5c"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5d"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5f"))==0)
             )
             &&(hexproductname.CompareNoCase(_T("tstat5led"))==0)
            )
    {
        Ret_Result= TRUE;
    }
    else if ((hexproductname.CompareNoCase(_T("tstat6"))==0)&&(prodcutname.CompareNoCase(_T("tstat5i"))==0))
    {
        Ret_Result= TRUE;
    }
    else
    {
        strtips.Format(_T("Your device is %s   Your hex file is fit for %s "),prodcutname.GetBuffer(),hexproductname.GetBuffer());
        OutPutsStatusInfo(strtips,false);
        return FALSE;
    }
    return Ret_Result;
    if (Ret_Result)
    {
        if (Device_infor[4]==global_fileInfor.software_low&&Device_infor[5]==global_fileInfor.software_high)
        {
            strtips.Format(_T("The current firmware already is latest version"),prodcutname.GetBuffer(),hexproductname.GetBuffer());
            OutPutsStatusInfo(strtips,false);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }


}
void CComWriter::SetIPAddr(const CString& strIPAddr)
{
    m_strIPAddr = strIPAddr;
}

void CComWriter::SetIPPort(int nIPPort)
{
    m_nIPPort = nIPPort;
}

int CComWriter::BeginWirteByTCP()
{
    ASSERT(m_szMdbIDs.size() > 0);

    if (m_nHexFileType == 2&&Is_Ram)	//Marked by Fance
    {
        if(GetCommunicationType()==1)
        {
            if(Open_Socket2(m_strIPAddr, m_nIPPort)==false)
            {
                CString srtInfo = _T("|Error : Network init failed.");
                MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
                //AddStringToOutPuts(_T("Error :The com port is occupied!"));
                OutPutsStatusInfo(srtInfo, FALSE);
                WriteFinish(0);
                return 0;
            }
            else
            {
                CString strTemp;
                //strTemp.Format(_T("COM%d"), m_nComPort);
                CString strTips = _T("|Connect to ") +  m_strIPAddr + _T(" successful.");
                OutPutsStatusInfo(strTips, FALSE);
                // AddStringToOutPuts(strTips);
                SetCommunicationType(1);
            }
        }
        CString strTips = _T("|Programming device...");
        OutPutsStatusInfo(strTips);
        //AddStringToOutPuts(strTips);
        m_pWorkThread=AfxBeginThread(flashThread_ForExtendFormatHexfile_RAM, this); //create thread,read information
        ASSERT(m_pWorkThread);
    }
    else
    {

        //AddStringToOutPuts(strTips);


        //m_pWorkThread=AfxBeginThread(run_back_ground_flash_thread, this); //create thread,read information
        m_pWorkThread=AfxBeginThread(Flash_Modebus_Device, this); //create thread,read information
        ASSERT(m_pWorkThread);
    }

    return 1;
}



UINT flashThread_ForExtendFormatHexfile_RAM(LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)(pParam);
    CString strFailureList;
    CString strTips;
    int nFlashRet=0;
    UINT i=0;
    int nFailureNum = 0;

    UINT times=0;

#if 1
    const int  RETRY_TIMES = 5;
    CString StrCurrentTime;
    StrCurrentTime.Format(_T("|>>>StartTime:%s"),GetSysTime());
    pWriter->OutPutsStatusInfo(StrCurrentTime);
    BOOL Flag_HEX_BIN = FALSE;
    for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
    {
       //for (int Time =0; Time < pWriter->m_FlashTimes; Time++)
        {
            CString strID;
            strID.Format(_T("|Current Programming device ID is : %d"), pWriter->m_szMdbIDs[i]);
            pWriter->OutPutsStatusInfo(strID);

            BOOL Flag_HEX_BIN=FALSE;
            if (pWriter->UpdataDeviceInformation(pWriter->m_szMdbIDs[i]))
            {
                Flag_HEX_BIN =TRUE;
                int  nRet = Write_One(pWriter->m_szMdbIDs[i],16,127);   // ����ISPģʽ
                Sleep (2000);
                nRet = Read_One(pWriter->m_szMdbIDs[i],11);
                if (nRet <= 0)
                {
                    AfxMessageBox(_T("Fail to enter ISP Mode!"));
                    goto end_isp_flash;
                }

                if (nRet >=41)//֧�ֶ���������л���
                {
                    if (GetCommunicationType () == 0)
                    {
                        nRet = Write_One (pWriter->m_szMdbIDs[i],15,4); //�������������л��� 115200
                        close_com ();
                        if(open_com(pWriter->m_nComPort)==false)
                        {
                            return 0;
                        }
                        else
                        {
                            bool is_ok =  Change_BaudRate (115200);
							SetResponseTime(10);
                        }
                        // Change_BaudRate (115200);
                    }

                }
				 
                Sleep (500);

                int m_ID=pWriter->m_szMdbIDs[i];

                Sleep(2000);


#if 1
                //*************inspect the flash that last flash position ***********************
                int ii=0;
                while(Read_One(m_ID,0xee10)<0) //the return value == -1 ,no connecting
                {
                    //AfxMessageBox("before write,need to open the com!");
                    //close_com();
                    if(ii<5)
                    {
                        ii++;
                        continue;
                    }
                    else
                    {
                        ii=0;
                        break;
                    }//error -1
                }
                if (ii >= 5)
                {

                    AfxMessageBox(_T("No connection!"));
                    goto end_isp_flash;

                }
                //************* begin to flash ***********************
                ii=0;
                if(Read_One(m_ID,0xee10)==0x40 || Read_One(m_ID,0xee10)==0x1f) // ��ee10�� why��
                {
                    if(IDOK==AfxMessageBox(_T("Previous Update was interrupted.\nPress OK to Resume.\nCancel to Restart."),MB_OKCANCEL))  // ѡȷ��
                    {
                        ii=0xEE00+17;
                        int l=0;//temp;<200
                        do
                        {
                            int uc_temp1=Read_One(m_ID,ii);
                            int uc_temp2=Read_One(m_ID,ii+1);
                            if(uc_temp1==0x00 && uc_temp2==0x00 )
                                ii+=2;
                            else if(l==0)
                            {
                                if(uc_temp1==0xf0 && uc_temp2==0xf0)
                                {
                                    ii=0;
                                    break;
                                }
                            }
                            else
                            {
                                ii=uc_temp1*256+uc_temp2;
                                break;
                            }
                            l++;
                        }
                        while(l<200);
                    }
                    else // ѡȡ��
                    {
                        //from 0000 flash update
                        ii=0;//from 0000 register flash
                        CString srtInfo = _T("|Initializing device...");
                        pWriter->OutPutsStatusInfo(srtInfo);
                        //********************write register 16 value 0x7f **************
                        Sleep(500);
                        do
                        {
                            if(ii<RETRY_TIMES)
                                if(-2==Write_One(m_ID,16,0x7f))
                                {
                                    ii++;
                                    Sleep(6000);
                                }
                                else
                                {
                                    //AfxMessageBox("Unable to Initialize...");
                                    //close_com();
                                    ii=0;//error -2 Unable to Initialize...
                                    break;
                                }
                        }
                        while(ii);

                        if (ii >= RETRY_TIMES)
                        {

                            AfxMessageBox(_T("Write 16 = 0x7f Failed"));
                            goto end_isp_flash;

                        }

                        //********************write register 16 value 0x3f **************
                        ii=0;
                        srtInfo = _T("|Erasing device...");
                        pWriter->OutPutsStatusInfo(srtInfo);
                        do
                        {
                            if(ii<RETRY_TIMES)
                            {
                                if(-2==Write_One(m_ID,16,0x3f))
                                {
                                    ii++;
                                }

                            }
                            else
                            {
                                //AfxMessageBox("Unable to Erase...");
                                //close_com();
                                ii=0;
                                break;
                                //return -3;//error -3
                            }
                        }
                        while(ii);
                        if (ii >= RETRY_TIMES)
                        {

                            AfxMessageBox(_T("Write 16 = 0x3f Failed"));
                            goto end_isp_flash;

                        }
                        //********************write register 16 value 0x1f **************
                        ii=0;
                        Sleep(7000);//must have this ,the Tstat need
                        do
                        {
                            if(ii<RETRY_TIMES)
                                if(-2==Write_One(m_ID,16,0x1f))
                                    ii++;

                                else
                                {
                                    //AfxMessageBox("Unable to start programming...");
                                    //close_com();
                                    ii=0;
                                    break;
                                    //return -4;//error -4
                                }
                        }
                        while(ii);

                        if (ii >= RETRY_TIMES)
                        {

                            AfxMessageBox(_T("Write 16 = 0x1f Failed"));
                            goto end_isp_flash;

                        }

                        //***************send data to com*************************
                        ii=0;//to the register 0000
                    }
                }
                else  // ��ee10 ʧ�ܣ�
                {
                    //from 0000 flash update
                    ii=0;//from 0000 register flash
                    CString srtInfo = _T("|Initializing device...");
                    pWriter->OutPutsStatusInfo(srtInfo);
                    //writing_row++;
                    Sleep(500);

                    //********************write register 16 value 0x7f **************
                    do
                    {
                        if(ii<RETRY_TIMES)

                        {
                            if(Write_One(m_ID,16,0x7f)<0)
                            {
                                ii++;
                                continue;
                            }
                            else
                            {
                                //AfxMessageBox("Unable to Initialize...");
                                //close_com();
                                ii=0;
                                break;
                                //return -5;//error -5
                            }
                        }

                        else
                        {
                            //AfxMessageBox("Unable to Initialize...");
                            //close_com();
                            ii=0;
                            break;
                            //return -5;//error -5
                        }


                    }
                    while(ii<RETRY_TIMES);

                    //********************write register 16 value 0x3f **************
                    ii=0;
                    srtInfo = _T("|Erasing device...";);
                    pWriter->OutPutsStatusInfo(srtInfo);
                    //writing_row++;
                    do
                    {
                        if(ii<RETRY_TIMES)
                        {
                            //if(-2==Write_One(m_ID,16,0x3f))
                            if(Write_One(m_ID,16,0x3f)<0)
                            {
                                ii++;
                                continue;
                            }
                            else
                            {
                                //AfxMessageBox("Unable to Initialize...");
                                //close_com();
                                ii=0;
                                break;
                                //return -5;//error -5
                            }
                        }
                        else
                        {
                            //AfxMessageBox("Unable to Erase...");
                            //close_com();
                            ii=0;
                            break;
                            //return -6;//error -6
                        }
                    }
                    while(ii<RETRY_TIMES);
                    //********************write register 16 value 0x1f **************
                    ii=0;
                    Sleep(7000);//must have this ,the Tstat need

                    do
                    {
                        if(ii<RETRY_TIMES)
                        {
                            //if(-2==Write_One(m_ID,16,0x1f))
                            if(Write_One(m_ID,16,0x1f)<0)
                            {
                                ii++;
                                continue;
                            }
                            else
                            {
                                //AfxMessageBox("Unable to Initialize...");
                                //close_com();
                                ii=0;
                                break;
                                //return -5;//error -5
                            }
                        }
                        else
                        {
                            //AfxMessageBox("Unable to Initialize...");
                            //close_com();
                            ii=0;
                            break;
                            //return -5;//error -5
                        }


                    }
                    while(ii<RETRY_TIMES);

                    //***************send data to com*************************
                    ii=0;//to the register 0000
                } // ��ee10 �Ĵ������

#endif

            }
            else
            {
                continue;
            }
            if (!Flag_HEX_BIN)
            {
                continue;
            }





            the_max_register_number_parameter_Count=pWriter->m_szHexFileFlags[pWriter->m_szHexFileFlags.size()-1];
            the_max_register_number_parameter_Finished = 0;

            int nCount = 0;
            UINT p = 0;
            for(  p = 0; p < pWriter->m_szHexFileFlags.size(); p++)
            {
                int nBufLen = pWriter->m_szHexFileFlags[p]-nCount;

                if((nFlashRet = flash_a_tstat_RAM(pWriter->m_szMdbIDs[i],p, nBufLen, (TS_UC*)(pWriter->m_pExtendFileBuffer+nCount), pParam)) < 0 )
                {
                    nFailureNum++;
                    CString strTemp=_T("");
                    strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                    strFailureList+=strTemp;  // flash���ʹ��
                    switch(nFlashRet)
                    {
                    case -1:
                        strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                        break;
                    case -2:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -3:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -4:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -5:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -6:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -7:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -8:
                        strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -9:
                        strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                        break;
                    }
                    pWriter->OutPutsStatusInfo(strTemp);
                    //AfxMessageBox(strTemp);
                    // break;
                    goto end_isp_flash;

                }
                else
                {
                    nCount += nBufLen;
                    the_max_register_number_parameter_Finished=nCount;

                }

                Sleep(500); //must if not ,have some wrong

            }


#if 1
            int ii=0;
            do
            {

                if(Write_One(pWriter->m_szMdbIDs[i],16,1)<0)
                {
                    ii++;
                }


                else
                {
                    ii=0;
                    break;
                    //return -8;//error -8
                }
            }
            while(ii<RETRY_TIMES);
#endif
            Sleep (6000);

            if (GetCommunicationType () == 0)
            {
                if (pWriter->m_index_Baudrate>=0)
                {
                    Write_One (pWriter->m_szMdbIDs[i],15,pWriter->m_index_Baudrate); //�лص�ǰ�Ĳ�����
                }
            }


            if(nFlashRet > 0) // flash �ɹ�
            {
                CString strText;
                strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                pWriter->OutPutsStatusInfo(strText);
                StrCurrentTime.Format(_T("|>>>EndTime:%s"),GetSysTime());
                pWriter->OutPutsStatusInfo(StrCurrentTime);
            }
        }


    }

#endif


//*******************************************************************************
end_isp_flash:
    pWriter->WriteFinish(nFlashRet);
    Sleep(500);

    close_com();
    return 1;//close thread
//	}while(1);


}