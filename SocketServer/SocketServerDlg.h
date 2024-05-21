
// SocketServerDlg.h: 헤더 파일
//
#include "ServerSocket.h"
#pragma once


// CSocketServerDlg 대화 상자
class CSocketServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CSocketServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void TextOut(LPCTSTR Text);
	afx_msg void OnDestroy();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
public:
	// 클라이언트에서 전달된 메세지를 저장하는 리스트
	CListBox MessageList;
	ServerSocket serverSocket;	//listen, close용 서버소켓
	CString filePath;	//저장할 파일경로


protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEditServerinfo();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEditClientlist();
	afx_msg void OnBnClickedButtonFileSave();
	void UpdateClientList();
	void SetFilePath();
	void SaveData(CString Text);
};
