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
		ON_COMMAND(ID_SAVE, &CImageProcessingView::OnSave)
		ON_COMMAND(ID_CHANGE_COLOR, &CImageProcessingView::OnChangeColor)
		ON_COMMAND(ID_GRAY, &CImageProcessingView::OnGray)
		ON_COMMAND(ID_BINARYZATION, &CImageProcessingView::OnBinaryzation)
		ON_COMMAND(ID_MINUS, &CImageProcessingView::OnMinus)
		ON_COMMAND(ID_SHIFT, &CImageProcessingView::OnShift)
END_MESSAGE_MAP()

// CImageProcessingView ����/����

CImageProcessingView::CImageProcessingView()
{
	file = (CFile **)malloc(sizeof(CFile *) * MAX_NUM);
	newbmp.Empty();
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
	//ȡ�û���
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//ȡ�ý����С
	CRect rectDlg;
	GetClientRect(rectDlg);
	int sysX = rectDlg.Width();

	int startX = 20;
	int startY = 20, line = 0;

	//�ɴ򿪶���ͼƬ
	if (newbmp.IsEmpty())
	{
		for (int i = 0; i < imageNumber; i++)
		{
			if (startX + sizeDibDisplay[i].cx > sysX)
			{
				startX = 20;
				startY += line + 20;
				line = 0;
			}
			mybmp[i].Draw(pDC, CPoint(startX, startY), sizeDibDisplay[i]);
			startX += sizeDibDisplay[i].cx + 20;
			if (sizeDibDisplay[i].cy > line)
			{
				line = sizeDibDisplay[i].cy;
			}
		}
	}

	//��Ե�һ��ͼƬ��������ͼ����
	if (!newbmp.IsEmpty())
	{
		mybmp[0].Draw(pDC, CPoint(startX, 20), sizeDibDisplay[0]);
		newbmp.Draw(pDC, CPoint(50 + sizeDibDisplay[0].cx, 20), newbmp.GetDimensions());
	}
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
	//���õ�ǰ�����ͼƬ
	imageNumber = 0;

	//Ϊ���ļ��Ի������ÿ�ѡ�ж��ͼƬ
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "",
	                    OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"Open Image"};
	FileDlg.m_ofn.lpstrTitle = title;
	if (FileDlg.DoModal() == IDOK)
	{
		POSITION filePos = FileDlg.GetStartPosition();
		while (filePos != NULL)
		{
			if (imageNumber == MAX_NUM)
			{
				//���ֻ�����ʮ��ͼ��
				AfxMessageBox("ͼƬ�����������ޣ�");
				return;
			}
			file[imageNumber] = new CFile();
			CString filename = FileDlg.GetNextPathName(filePos);
			if (!(*(file[imageNumber])).Open(filename, CFile::modeRead))
			{
				AfxMessageBox("cannot open the file");
				return;
			}
			if (!mybmp[imageNumber].Read(file[imageNumber]))
			{
				AfxMessageBox("cannot read the file");
				return;
			}
			imageNumber++;
		}
	}
	else
	{
		return;
	}

	for (int i = 0; i < imageNumber; i++)
	{
		if (mybmp[i].m_lpBMIH->biCompression != BI_RGB)
		{
			AfxMessageBox("Can not read compressed file.");
			return;
		}
		sizeDibDisplay[i] = mybmp[i].GetDimensions();
		file[i]->Close();
	}

	//���ô����ͼ�����
	newbmp.Empty();
	Invalidate(TRUE);
}


void CImageProcessingView::OnSave()
{
	//���洦����ͼ��
	if (newbmp.IsEmpty())
	{
		AfxMessageBox("��δ����ͼƬ��");
		return;
	}
	CFileDialog FileDlg(FALSE, _T("*.bmp"), "�����ͼƬ", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"Save Image"};
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			AfxMessageBox("cannot save the file");
			return;
		}
		if (!newbmp.Write(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnChangeColor()
{
	imageNumber = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (int y = 0; y < newbmp.GetDimensions().cy; y++)
	{
		for (int x = 0; x < newbmp.GetDimensions().cx; x++)
		{
			if (y > 30 && y < 50)
			{
				RGBQUAD color;
				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
				newbmp.WritePixel(x, y, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnGray()
{
	Invalidate(TRUE);
}


void CImageProcessingView::OnBinaryzation()
{
	Invalidate(TRUE);
}


void CImageProcessingView::OnMinus()
{
	Invalidate(TRUE);
}


void CImageProcessingView::OnShift()
{
	Invalidate(TRUE);
}
