// ImageProcessingView.h : CImageProcessingView ��Ľӿ�
//

#pragma once

#include "CDib.h"

class CImageProcessingView : public CView
{
public:
	CDib mybmp;
	CDib mybmp2;
	CSize sizeDibDisplay2;
	CSize sizeDibDisplay;

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
	afx_msg void OnCloseSecond();
	afx_msg void OnSave();
	afx_msg void OnClose();
	afx_msg void OnOpenSecond();
	afx_msg void OnSaveSecond();
	afx_msg void OnChangeColor();
	afx_msg void OnGray();
	afx_msg void OnBinaryzation();
};

#ifndef _DEBUG  // ImageProcessingView.cpp �еĵ��԰汾
inline CImageProcessingDoc* CImageProcessingView::GetDocument() const
   { return reinterpret_cast<CImageProcessingDoc*>(m_pDocument); }
#endif
