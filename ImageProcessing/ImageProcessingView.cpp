
// ImageProcessingView.cpp : CImageProcessingView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ImageProcessing.h"
#endif

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPEN, &CImageProcessingView::OnOpen)
	ON_COMMAND(ID_RED_BLUE, &CImageProcessingView::OnRedBlue)
	ON_COMMAND(ID_CLOSE_SECOND, &CImageProcessingView::OnCloseSecond)
	ON_COMMAND(ID_SAVE, &CImageProcessingView::OnSave)
	ON_COMMAND(ID_CLOSE, &CImageProcessingView::OnClose)
	ON_COMMAND(ID_OPEN_SECOND, &CImageProcessingView::OnOpenSecond)
END_MESSAGE_MAP()

// CImageProcessingView ����/����

CImageProcessingView::CImageProcessingView()
{
	// TODO: �ڴ˴���ӹ������

}

CImageProcessingView::~CImageProcessingView()
{
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CImageProcessingView ����

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	mybmp.Draw(pDC, CPoint(0, 0), sizeDibDisplay);
	mybmp2.Draw(pDC, CPoint(sizeDibDisplay.cx, 0), sizeDibDisplay2);
}


// CImageProcessingView ��ӡ


void CImageProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CImageProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessingView ���

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessingView ��Ϣ�������


void CImageProcessingView::OnOpen()
{
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = { "Open Image" };
	FileDlg.m_ofn.lpstrTitle = title;

	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!mybmp.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}

	if (mybmp.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeDibDisplay = mybmp.GetDimensions();

	Invalidate(TRUE);
}


void CImageProcessingView::OnRedBlue()
{
	CSize ImageSize = mybmp.GetDimensions();

	for (int y = 0; y < ImageSize.cy; y++)
	{
		for (int x = 0; x < ImageSize.cx; x++)
		{
			RGBQUAD RedColor = mybmp.GetPixel(x, y);
			if (y <70 && y>50)
			{
				RedColor.rgbRed = 255;
				RedColor.rgbGreen = 0;
				RedColor.rgbBlue = 0;
				mybmp.WritePixel(x, y, RedColor);
			}

			if (x <60 && x>30)
			{

				RedColor.rgbRed = 0;
				RedColor.rgbGreen = 0;
				RedColor.rgbBlue = 255;
				mybmp.WritePixel(x, y, RedColor);
			}

		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnCloseSecond()
{
	// TODO: Add your command handler code here
	mybmp2.Empty();
	Invalidate(TRUE);
}


void CImageProcessingView::OnSave()
{
	// TODO: Add your command handler code here
	mybmp.Save("Result.bmp");
}


void CImageProcessingView::OnClose()
{
	// TODO: Add your command handler code here
	mybmp.Empty();
	Invalidate(TRUE);
}


void CImageProcessingView::OnOpenSecond()
{
	// TODO: Add your command handler code here
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = { "Open Image" };
	FileDlg.m_ofn.lpstrTitle = title;

	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!mybmp2.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}

	if (mybmp2.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeDibDisplay2 = mybmp2.GetDimensions();

	Invalidate(TRUE);
}
