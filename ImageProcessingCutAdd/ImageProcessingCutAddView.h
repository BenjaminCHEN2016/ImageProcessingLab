// ImageProcessingCutAddView.h : CImageProcessingCutAddView ��Ľӿ�
//

#pragma once
#include "cdib.h"
#include "MyTracker.h"


class CImageProcessingCutAddView : public CView
{
protected: // �������л�����
	CImageProcessingCutAddView();
DECLARE_DYNCREATE(CImageProcessingCutAddView)

	// ����
public:
	//��һ��ͼ��
	CDib myBmp1;

	//�ڶ���ͼ��
	CDib myBmp2;

	//��һ��ͼ���С
	CSize sizeImage1;

	//�ڶ���ͼ���С
	CSize sizeImage2;

	//��һ���������λ��
	CPoint m_origin1;

	//�ڶ����������λ��
	CPoint m_origin2;

	//���ο�
	CMyTracker m_tracker;

	//�߽续��
	CPen pen;

	//�ϳ���ʱ���
	CDib tmp1;

	//��ģ
	CDib tmp2;

	//ǰ���ڵ�һ��ͼ����ʼ��
	CPoint start1, end1;

	//ǰ���ڵڶ����е���ʼ��
	CPoint start2, end2;

	//״̬
	int state;

	//ȷ�ϱ�ʶ
	bool ok;

	CImageProcessingCutAddDoc* GetDocument() const;

	// ����
public:

	// ��д
public:
	virtual void OnDraw(CDC* pDC); // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// ʵ��
public:
	virtual ~CImageProcessingCutAddView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	//	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen1();
	afx_msg void OnOpen2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void ScaleImage(float scalex, float scaley);
	void UpdateImage();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBi();
	afx_msg void OnOk();
	afx_msg void OnSave();
};

#ifndef _DEBUG  // ImageProcessingCutAddView.cpp �еĵ��԰汾
inline CImageProcessingCutAddDoc* CImageProcessingCutAddView::GetDocument() const
   { return reinterpret_cast<CImageProcessingCutAddDoc*>(m_pDocument); }
#endif
