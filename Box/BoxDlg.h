
// BoxDlg.h : 头文件
//

#pragma once

#include "Game.h"


// CBoxDlg 对话框
class CBoxDlg : public CDialogEx
{
private:
	// 游戏，后台控制
	Game *game;

// 构造
public:
	CBoxDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BOX_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//--------------事件 -------------------------
public:
	afx_msg void OnBnClickedButtonHalt();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// 覆盖 父类方法
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
//----------功能函数区------------------------
private:
	void DrawNet();
	void DrawBigNet();
	void DrawSmallNet();
	void OnKeyDown(UINT nChar);
};
