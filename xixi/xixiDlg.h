
// xixiDlg.h : ͷ�ļ�
//
#if !defined(AFX_RECTESTDLG_H__290A608A_1E69_46C6_BB47_939AA3598904__INCLUDED_)
#define AFX_RECTESTDLG_H__290A608A_1E69_46C6_BB47_939AA3598904__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma comment(lib,"winmm.lib")
#endif
// CxixiDlg �Ի���
#define InBlocks 4 //�洢������Ƶ���ݵĵ�Ԫ��
#define OutBlocks 4  //�洢�����Ƶ���ݵĵ�Ԫ��
#define  INP_BUFFER_SIZE 16384
#define  PORT 8000
#include "stdafx.h"
#include "mmsystem.h"
#include "afxwin.h"
#include "afxsock.h"
#include "resource.h"
struct CAudioData
{
	PBYTE lpdata;
	DWORD dwLength;
};

class CxixiDlg : public CDialogEx
{
// ����
public:
	CxixiDlg(CWnd* pParent = NULL);	// ��׼���캯��
    CString GetError(DWORD error);
// �Ի�������
	enum { IDD = IDD_XIXI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	
	HWAVEIN hWaveIn;    //��������
	HWAVEOUT hWaveOut;  //����豸

	PWAVEHDR pWaveHdr1,pWaveHdr2;    //���"ͷ�ṹ��"          
	PWAVEHDR pWaveHdrOut;		
	WAVEFORMATEX m_waveformin,m_waveformout;    

	PBYTE pBuffer1,pBuffer2;//�����豸���û�����
	// ���ɵ���Ϣӳ�亯��
public:
	//�����ݴ�¼���Ҫ���͵ļ����յ��ļ���Ҫ���ŵ������ļ���ѭ�����У�
	CAudioData m_AudioDataIn[InBlocks],m_AudioDataOut[OutBlocks];
   	int   nAudioIn, nSend, //¼�롢����ָ��
	      nAudioOut, nReceive;//���ա�����ָ��
        //����¼���ͷ��������ں������ͬ�����⣬��Ҫ����Щָ�����Э��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBegin();
	afx_msg LRESULT OnMM_WIM_DATA(UINT wParam,LONG lParam);
	afx_msg LRESULT OnMM_WIM_CLOSE(UINT wParam,LONG lParam);
	afx_msg LRESULT OnMM_WOM_OPEN(UINT wParam,LONG lParam);
	afx_msg LRESULT OnMM_WOM_DONE(UINT wParam,LONG lParam);
	afx_msg LRESULT OnMM_WOM_CLOSE(UINT wParam,LONG lParam);
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
};
#endif