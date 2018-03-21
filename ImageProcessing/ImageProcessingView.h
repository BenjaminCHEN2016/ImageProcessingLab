// ImageProcessingView.h : CImageProcessingView ��Ľӿ�
//

#pragma once

#include "CDib.h"
const int MAX_NUM = 5;
const double pi = 3.1415926;
class CImageProcessingView : public CView
{
public:
	//����ͼƬ
	CDib mybmp[MAX_NUM];

	//����ͼƬ��С
	CSize sizeDibDisplay[MAX_NUM];

	//�������
	CDib newbmp;

	//�����ļ�ָ��
	CFile ** file;

	//����ͼƬ��С
	int imageCount;

protected: // �������л�����
	CImageProcessingView();
DECLARE_DYNCREATE(CImageProcessingView)

	// ����
public:
	CImageProcessingDoc* GetDocument() const;

	// ����
public:

	// ��д
public:
	void OnDraw(CDC* pDC) override; // ��д�Ի��Ƹ���ͼ
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
protected:
	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

	// ʵ��
public:
	virtual ~CImageProcessingView();
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnChangeColor();
	afx_msg void OnGray();
	afx_msg void OnBinaryzation();
	afx_msg void OnMinus();
	afx_msg void OnShift();
	afx_msg void OnRotate();
};

#ifndef _DEBUG  // ImageProcessingView.cpp �еĵ��԰汾
inline CImageProcessingDoc* CImageProcessingView::GetDocument() const
   { return reinterpret_cast<CImageProcessingDoc*>(m_pDocument); }
#endif
