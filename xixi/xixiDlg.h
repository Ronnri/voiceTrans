
// xixiDlg.h : 头文件
//
#if !defined(AFX_RECTESTDLG_H__290A608A_1E69_46C6_BB47_939AA3598904__INCLUDED_)
#define AFX_RECTESTDLG_H__290A608A_1E69_46C6_BB47_939AA3598904__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma comment(lib,"winmm.lib")
#endif
// CxixiDlg 对话框
#define InBlocks 4 //存储输入音频数据的单元数
#define OutBlocks 4  //存储输出音频数据的单元数
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
// 构造
public:
	CxixiDlg(CWnd* pParent = NULL);	// 标准构造函数
    CString GetError(DWORD error);
// 对话框数据
	enum { IDD = IDD_XIXI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	
	HWAVEIN hWaveIn;    //声音输入
	HWAVEOUT hWaveOut;  //输出设备

	PWAVEHDR pWaveHdr1,pWaveHdr2;    //相关"头结构体"          
	PWAVEHDR pWaveHdrOut;		
	WAVEFORMATEX m_waveformin,m_waveformout;    

	PBYTE pBuffer1,pBuffer2;//输入设备所用缓冲区
	// 生成的消息映射函数
public:
	//用于暂存录入后要发送的及接收到的即将要播放的声音文件的循环队列，
	CAudioData m_AudioDataIn[InBlocks],m_AudioDataOut[OutBlocks];
   	int   nAudioIn, nSend, //录入、发送指针
	      nAudioOut, nReceive;//接收、播放指针
        //对于录音和放音都存在和网络的同步问题，主要靠这些指针进行协调
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