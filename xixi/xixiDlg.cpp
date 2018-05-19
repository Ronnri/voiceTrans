
// xixiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "xixi.h"
#include "xixiDlg.h"
#include "afxdialogex.h"
#include "afxsock.h"
#include "mmsystem.h"
#include "afxwin.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib,"winmm.lib")

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CxixiDlg �Ի���




CxixiDlg::CxixiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CxixiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CxixiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CxixiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(start, OnBegin)
	ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)
	ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)
	ON_MESSAGE(MM_WOM_OPEN,OnMM_WOM_OPEN)
	ON_MESSAGE(MM_WOM_DONE,OnMM_WOM_DONE)
	ON_MESSAGE(MM_WOM_CLOSE,OnMM_WOM_CLOSE)
	ON_BN_CLICKED(OK, &CxixiDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CxixiDlg ��Ϣ�������

BOOL CxixiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    WSAData data;
    WSAStartup(MAKEWORD(2, 2), &data);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//allocate memory for wave header
    pWaveHdr1=reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	pWaveHdr2=reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	pWaveHdrOut=reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	//***reinterpret_cast ****  operation is used to convert any type of
	//pointer to other type
	//allocate memory for save buffer
	
	int i;
	for(i=0;i<InBlocks;i++)
	{
		m_AudioDataIn[i].dwLength = 0;
		m_AudioDataIn[i].lpdata = reinterpret_cast<PBYTE>(malloc(1));

	}

	for(i =0;i<OutBlocks;i++)
	{
		m_AudioDataOut[i].dwLength = 0;
		m_AudioDataOut[i].lpdata = reinterpret_cast<PBYTE>(malloc(1));
	}
	nAudioIn = 0;
	nAudioOut = 0;
	nSend = 0;
	nReceive = 0;
   
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CxixiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CxixiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CxixiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT Audio_Listen_Thread(LPVOID lParam)
{
	CxixiDlg *pdlg = (CxixiDlg*)lParam;
    CSocket m_Server;
	DWORD 	length;
	if(!m_Server.Create(4002))//��ʼ������
	     AfxMessageBox("Listen Socket create error"+pdlg->GetError(GetLastError()));//���ɵ������У��ظ�����ͬһ�˿���Ȼ�Ǵ����
	if(!m_Server.Listen()) 
		AfxMessageBox("m_server.Listen ERROR"+pdlg->GetError(GetLastError()));
	CSocket recSo;
   if(! m_Server.Accept(recSo))
	   AfxMessageBox("m_server.Accept() error"+pdlg->GetError(GetLastError()));
	m_Server.Close();	
	int sendcount=0;//just for test
	int ret ;
	while(1)
	{   //��ʼѭ�����������ļ������Ƚ����ļ�����
	    ret = recSo.Receive(&length,sizeof(DWORD));		
		if(ret== SOCKET_ERROR )
		AfxMessageBox("�������˽��������ļ����ȳ���ԭ�� "+pdlg->GetError(GetLastError()));
		if(ret!=sizeof(DWORD))
		{
			//MessageBox("�����ļ�ͷ���󣬽��رո��߳�");
			MessageBox(NULL, "�����ļ�ͷ���󣬽��رո��̡߳�", "��ʾ", MB_OK);
        	recSo.Close();
	        return -1;
		}//����������length�����ڴ�ռ�
		pdlg->m_AudioDataOut[pdlg->nReceive].lpdata =(PBYTE)realloc (0,length);
	    if (pdlg->m_AudioDataOut[pdlg->nReceive].lpdata == NULL)
		{
			MessageBeep (MB_ICONEXCLAMATION) ;
			MessageBox(NULL, "erro memory_ReceiveAudio", "��ʾ", MB_OK);
			//MessageBox(_T("erro memory_ReceiveAudio"));
            pdlg->OnOK();
			recSo.Close();
	        return -1;
		}
		else//�ڴ�����ɹ������Խ���ѭ��������
		{
			DWORD dwReceived = 0,dwret;
			while(length>dwReceived)
			{
				dwret = recSo.Receive((pdlg->m_AudioDataOut[pdlg->nReceive].lpdata+dwReceived),(length-dwReceived));
			    dwReceived +=dwret;
				if(dwReceived ==length)
				{
                    pdlg->m_AudioDataOut[pdlg->nReceive].dwLength = length;
					break;
				}
				
			}
		}//���������ļ��������	
	    pdlg->nReceive=(pdlg->nReceive+1)%OutBlocks;
	}
	recSo.Close();
	pdlg->OnOK();
	return 0;
}

UINT Audio_Send_Thread(LPVOID lParam)//רְ�������ļ������߳�
{                                    // �˿�Ҳ��ר�õ�
	CxixiDlg *pdlg = (CxixiDlg*)lParam;
	CSocket m_Client;
	m_Client.Create();
	if( m_Client.Connect("127.0.0.1",4002))
	{		
		DWORD ret, length;
		int count=0;
		while(1)//ѭ��ʹ��ָ��nSend
		{
		    length =pdlg->m_AudioDataIn[pdlg->nSend].dwLength;			
        	if(length !=0)
			{   //���ȷ��Ϳ�ĳ���
                if(((ret = m_Client.Send(&length,sizeof(DWORD)))!=sizeof(DWORD))||(ret==SOCKET_ERROR))
				{   
				   AfxMessageBox("�����ļ�ͷ�������!"+pdlg->GetError(GetLastError()));
				   pdlg->OnOK();
			       break;	
				}//��η��Ϳ������	,ѭ����ⷢ��
				DWORD dwSent = 0;//�Ѿ����͵����ֽ���
				while(1)//==============================���������ļ���ʼ
				{
					ret = m_Client.Send((pdlg->m_AudioDataIn[pdlg->nSend].lpdata+dwSent),(length-dwSent));
					if(ret==SOCKET_ERROR)//���
					{
					   AfxMessageBox("�����ļ��������!"+pdlg->GetError(GetLastError()));
					   pdlg->OnOK();
					   break;			
					}
					else //����δ�������
					{
						dwSent += ret;
						if(dwSent ==length)
						{   
    					free(pdlg->m_AudioDataIn[pdlg->nSend].lpdata);
						pdlg->m_AudioDataIn[pdlg->nSend].dwLength = 0;
						break;
						}
					}	
				}  //======================================���������ļ�����
			}
			pdlg->nSend = (pdlg->nSend +1)% InBlocks;
			}
			
	}
	else 
		AfxMessageBox("Socket����ʧ��"+pdlg->GetError(GetLastError()));
		//MessageBox(_T("Socket����ʧ��")+pdlg->GetError(GetLastError()));
	m_Client.Close();
	return 0;
}

CString CxixiDlg::GetError(DWORD error)	//���ش�����Ϣ
{
	CString strError;
	switch(error)
	{
	case WSANOTINITIALISED:
		strError="��ʼ������";
		break;
	case WSAENOTCONN:
		strError="�Է�û������";
		break;
	case WSAEWOULDBLOCK :
		strError="�Է��Ѿ��ر�";
		break;
	case WSAECONNREFUSED:
		strError="���ӵĳ��Ա��ܾ�";
		break;
	case WSAENOTSOCK:
		strError="��һ�����׽����ϳ�����һ������";
		break;
	case WSAEADDRINUSE:
		strError="�ض��ĵ�ַ����ʹ����";
		break;
	case WSAECONNRESET:
		strError="�����������ӱ��ر�";
		break;
	default:
		strError="һ�����";	
	}
	return strError;
	
}

void CxixiDlg::OnBegin() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(start)->EnableWindow(false);

	//allocate buffer memory
	pBuffer1=(PBYTE)malloc(INP_BUFFER_SIZE);
	pBuffer2=(PBYTE)malloc(INP_BUFFER_SIZE);
	if (!pBuffer1 || !pBuffer2) 
	{
		if (pBuffer1) free(pBuffer1);
		if (pBuffer2) free(pBuffer2);
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(_T("Memory erro!"));
		return ;
	}

	//open waveform audio for input
	
	//m_waveformin.wFormatTag=WAVE_FORMAT_PCM;
	//m_waveformin.nChannels=1;
	//m_waveformin.nSamplesPerSec=11025;//����Ƶ��
	//m_waveformin.nAvgBytesPerSec=11025;
	//m_waveformin.nBlockAlign=1;
	//m_waveformin.wBitsPerSample=8;
	//m_waveformin.cbSize=0;

	m_waveformin.wFormatTag = WAVE_FORMAT_PCM;//���ò��������ĸ�ʽ
	m_waveformin.nChannels = 1;//������Ƶ�ļ���ͨ������
	m_waveformin.nSamplesPerSec = 8000;//����ÿ���������źͼ�¼ʱ������Ƶ��
	m_waveformin.wBitsPerSample = 16;
	m_waveformin.nBlockAlign = m_waveformin.nChannels * (m_waveformin.wBitsPerSample / 8);//���ֽ�Ϊ��λ���ÿ����
	m_waveformin.nAvgBytesPerSec = m_waveformin.nSamplesPerSec * m_waveformin.nBlockAlign; //���������ƽ�����ݴ�����,��λbyte/s�����ֵ���ڴ��������С�Ǻ����õ�
	m_waveformin.cbSize = 0;//������Ϣ�Ĵ�С
	
	if (waveInOpen(&hWaveIn,WAVE_MAPPER,&m_waveformin,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
	{   //��¼���豸���� 
		free(pBuffer1);
		free(pBuffer2);
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(_T("Audio can not be open!"));
	}
	pWaveHdr1->lpData=(LPTSTR)pBuffer1;
	pWaveHdr1->dwBufferLength=INP_BUFFER_SIZE;
	pWaveHdr1->dwBytesRecorded=0;
	pWaveHdr1->dwUser=0;
	pWaveHdr1->dwFlags=0;
	pWaveHdr1->dwLoops=1;
	pWaveHdr1->lpNext=NULL;
	pWaveHdr1->reserved=0;
	
	waveInPrepareHeader(hWaveIn,pWaveHdr1,sizeof(WAVEHDR));
	
	pWaveHdr2->lpData=(LPTSTR)pBuffer2;
	pWaveHdr2->dwBufferLength=INP_BUFFER_SIZE;
	pWaveHdr2->dwBytesRecorded=0;
	pWaveHdr2->dwUser=0;
	pWaveHdr2->dwFlags=0;
	pWaveHdr2->dwLoops=1;
	pWaveHdr2->lpNext=NULL;
	pWaveHdr2->reserved=0;
	
	waveInPrepareHeader(hWaveIn,pWaveHdr2,sizeof(WAVEHDR));
			
	// Add the buffers
	
	waveInAddBuffer (hWaveIn, pWaveHdr1, sizeof (WAVEHDR)) ;
	waveInAddBuffer (hWaveIn, pWaveHdr2, sizeof (WAVEHDR)) ;
		
	// Begin sampling
	waveInStart (hWaveIn) ;

	::AfxBeginThread(Audio_Listen_Thread,this);
	::AfxBeginThread(Audio_Send_Thread,this);
	m_waveformout.wFormatTag = WAVE_FORMAT_PCM;//���ò��������ĸ�ʽ
	m_waveformout.nChannels = 1;//������Ƶ�ļ���ͨ������
	m_waveformout.nSamplesPerSec = 8000;//����ÿ���������źͼ�¼ʱ������Ƶ��
	m_waveformout.wBitsPerSample = 16;
	m_waveformout.nBlockAlign = m_waveformout.nChannels * (m_waveformout.wBitsPerSample / 8);//���ֽ�Ϊ��λ���ÿ����
	m_waveformout.nAvgBytesPerSec = m_waveformout.nSamplesPerSec * m_waveformout.nBlockAlign; //���������ƽ�����ݴ�����,��λbyte/s�����ֵ���ڴ��������С�Ǻ����õ�
	m_waveformout.cbSize = 0;//������Ϣ�Ĵ�С
	
	
	if (waveOutOpen(&hWaveOut,WAVE_MAPPER,&m_waveformout,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW)) {
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(_T("Audio output erro"));
	}
	
	return ;

}

LRESULT CxixiDlg::OnMM_WIM_DATA(UINT wParam,LONG lParam)
{
   
    int nextBlock = (nAudioIn+1)% InBlocks;	
	if(m_AudioDataIn[nextBlock].dwLength!=0)//��һ���顱û����
	{  //��PWAVEHDR(��pBUfferi)������ݿ�������ǰ���顱��
        
	 	m_AudioDataIn[nAudioIn].lpdata  
		= (PBYTE)realloc (m_AudioDataIn[nAudioIn].lpdata , (((PWAVEHDR) lParam)->dwBytesRecorded+m_AudioDataIn[nAudioIn].dwLength)) ;
		if (m_AudioDataIn[nAudioIn].lpdata == NULL)
		{
			waveInClose (hWaveIn) ;
			MessageBeep (MB_ICONEXCLAMATION) ;
			AfxMessageBox("erro memory OnMM_WIM_DATA");
			return TRUE;
		}
	    CopyMemory ((m_AudioDataIn[nAudioIn].lpdata+m_AudioDataIn[nAudioIn].dwLength), 
				   ((PWAVEHDR) lParam)->lpData,
				   ((PWAVEHDR) lParam)->dwBytesRecorded) ;//(*destination,*resource,nLen);
		
		m_AudioDataIn[nAudioIn].dwLength +=((PWAVEHDR) lParam)->dwBytesRecorded;
        
	}
	else //��PWAVEHDR(��pBUfferi)������ݿ�������һ���顱��
	{
		nAudioIn = (nAudioIn+1)% InBlocks;
		m_AudioDataIn[nAudioIn].lpdata = (PBYTE)realloc
			(0,((PWAVEHDR) lParam)->dwBytesRecorded);
		CopyMemory(m_AudioDataIn[nAudioIn].lpdata,
			    ((PWAVEHDR) lParam)->lpData,
				((PWAVEHDR) lParam)->dwBytesRecorded) ;
	   m_AudioDataIn[nAudioIn].dwLength =((PWAVEHDR) lParam)->dwBytesRecorded;

	}
  	// Send out a new buffer	
	waveInAddBuffer (hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR)) ;
	return TRUE;

	
}

LRESULT CxixiDlg::OnMM_WIM_CLOSE(UINT wParam,LONG lParam)
{

	waveInUnprepareHeader (hWaveIn, pWaveHdr1, sizeof (WAVEHDR)) ;
	waveInUnprepareHeader (hWaveIn, pWaveHdr2, sizeof (WAVEHDR)) ;
	
	free (pBuffer1) ;
	free (pBuffer2) ;

	return TRUE;
}

LRESULT CxixiDlg::OnMM_WOM_OPEN(UINT wParam,LONG lParam)
{   
	// Set up header    
	pWaveHdrOut->lpData          = (LPTSTR)m_AudioDataOut[nAudioOut].lpdata ;
	pWaveHdrOut->dwBufferLength  = m_AudioDataOut[nAudioOut].dwLength ;
    pWaveHdrOut->dwBytesRecorded = 0 ;
	pWaveHdrOut->dwUser          = 0 ;
	pWaveHdrOut->dwFlags         = WHDR_BEGINLOOP ;
	pWaveHdrOut->dwLoops         = 1 ;
	pWaveHdrOut->lpNext          = NULL ;
	pWaveHdrOut->reserved        = 0 ;

    // Prepare and write
	waveOutPrepareHeader (hWaveOut, pWaveHdrOut, sizeof (WAVEHDR)) ;
	waveOutWrite (hWaveOut, pWaveHdrOut, sizeof (WAVEHDR)) ;

  return TRUE;
	
}

LRESULT CxixiDlg::OnMM_WOM_DONE(UINT wParam,LONG lParam)
{  
	free(m_AudioDataOut[nAudioOut].lpdata);
	m_AudioDataOut[nAudioOut].lpdata = reinterpret_cast<PBYTE>(malloc(1));
	m_AudioDataOut[nAudioOut].dwLength = 0;
 
    nAudioOut= (nAudioOut+1)%OutBlocks;
	((PWAVEHDR)lParam)->lpData          = (LPTSTR)m_AudioDataOut[nAudioOut].lpdata ;
	((PWAVEHDR)lParam)->dwBufferLength  = m_AudioDataOut[nAudioOut].dwLength ;
    TRACE("the next length %d\n",((PWAVEHDR)lParam)->dwBufferLength);
	waveOutPrepareHeader (hWaveOut,(PWAVEHDR)lParam,sizeof(WAVEHDR));
    waveOutWrite(hWaveOut,(PWAVEHDR)lParam,sizeof(WAVEHDR));//cut
   return TRUE;

}

LRESULT CxixiDlg::OnMM_WOM_CLOSE(UINT wParam,LONG lParam)
{
	waveOutUnprepareHeader (hWaveOut, pWaveHdrOut, sizeof (WAVEHDR)) ;
		
	int i;
	//release all the memory of the AudioData
	for(i=0;i<InBlocks;i++)
	{
		if(m_AudioDataIn[i].dwLength != 0)
			free(m_AudioDataIn[i].lpdata);
		
	}
	for(i=0;i<OutBlocks;i++)
	{
		if(m_AudioDataOut[i].dwLength != 0)
			free(m_AudioDataOut[i].lpdata);
	}
	return TRUE;
}


void CxixiDlg::OnOK() 
{
	// TODO: Add extra validation here
	waveInReset(hWaveIn);
    waveInClose(hWaveIn);
	
	waveOutReset(hWaveOut);
	waveOutClose (hWaveOut);
	
	
	CDialog::OnOK();
}

void CxixiDlg::OnBnClickedOk()
{
	CDialog::OnOK();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
