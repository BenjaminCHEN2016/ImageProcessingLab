// ImageProcessingView.h : CImageProcessingView ��Ľӿ�
//

#pragma once

#include "CDib.h"
#include <complex>
const int MAX_NUM = 5;
const double pi = 3.1415926535898;

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
	CFile** file;

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
	afx_msg void OnMirrorH();
	afx_msg void OnMirrorV();
	afx_msg void OnAbout();
	afx_msg void OnZoomForward();
	afx_msg void OnZoomClosest();
	afx_msg void OnZoomBi();
	afx_msg void OnFft();
	void FFT_2D(std::complex<double>* pCTData, int nWidth, int nHeight, std::complex<double>* pCFData);
	void IFFT_2D(std::complex<double>* pCFData, std::complex<double>* pCTData, int nWidth, int nHeight);
	void FFT_1D(std::complex<double>* pCTData, std::complex<double>* pCFData, int nLevel);
	void IFFT_1D(std::complex<double>* pCFData, std::complex<double>* pCTData, int nLevel);
	void fourier(std::complex<double>* data, int nTransHeight, int nTransWidth, int isign);
	afx_msg void OnIfft();
	afx_msg void OnLinearTrans();
	afx_msg void OnPowTrans();
	afx_msg void OnAverageTrans();
	afx_msg void OnColorLevel();
	afx_msg void OnColorGray();
	afx_msg void OnAverageFilter();
	afx_msg void OnMidFilter();
	afx_msg void OnGradient();
	void TemplateONE(CDib& lpDIBBits, long lWidth, long lHeight, int iTempH, int iTempW, int iTempMX, int iTempMY,
	              float* fpArray, float fCoef);
	afx_msg void OnWeightFilter();
	afx_msg void OnPrewitt();
	afx_msg void OnSobel();
	void TemplateTWO(CDib& lpDIBBits, long lWidth, long lHeight, int iTempH, int iTempW,
	                 float* fpArray, float fCoef);
	afx_msg void OnLaplacian();
	afx_msg void OnIdeal();
};

#ifndef _DEBUG  // ImageProcessingView.cpp �еĵ��԰汾
inline CImageProcessingDoc* CImageProcessingView::GetDocument() const
   { return reinterpret_cast<CImageProcessingDoc*>(m_pDocument); }
#endif
