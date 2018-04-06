// ImageProcessingCutAddView.cpp : CImageProcessingCutAddView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ImageProcessingCutAdd.h"
#endif

#include "ImageProcessingCutAddDoc.h"
#include "ImageProcessingCutAddView.h"
#include "BiDlg.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessingCutAddView

IMPLEMENT_DYNCREATE(CImageProcessingCutAddView, CView)

BEGIN_MESSAGE_MAP(CImageProcessingCutAddView, CView)
		// ��׼��ӡ����
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessingCutAddView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_COMMAND(ID_OPEN1, &CImageProcessingCutAddView::OnOpen1)
		ON_COMMAND(ID_OPEN2, &CImageProcessingCutAddView::OnOpen2)
		ON_WM_LBUTTONDOWN()
		ON_WM_SETCURSOR()
		ON_COMMAND(ID_BI, &CImageProcessingCutAddView::OnBi)
		ON_COMMAND(ID_OK, &CImageProcessingCutAddView::OnOk)
		ON_COMMAND(ID_SAVE, &CImageProcessingCutAddView::OnSave)
END_MESSAGE_MAP()

// CImageProcessingCutAddView ����/����

CImageProcessingCutAddView::CImageProcessingCutAddView()
{
	m_tracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside | CRectTracker::hatchedBorder;
	m_tracker.m_nHandleSize = 7;
	pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_tracker.m_rect.SetRect(0, 0, 0, 0);
	state = 1;
	ok = false;
}

CImageProcessingCutAddView::~CImageProcessingCutAddView()
{
}

BOOL CImageProcessingCutAddView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CImageProcessingCutAddView ����

void CImageProcessingCutAddView::OnDraw(CDC* pDC)
{
	CImageProcessingCutAddDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_origin2.x = m_origin1.x + sizeImage1.cx + 10;
	m_origin2.y = m_origin1.y;

	switch (state)
	{
	case 1:
		myBmp1.Draw(pDC, m_origin1, sizeImage1);
		myBmp2.Draw(pDC, m_origin2, sizeImage2);
		break;
	case 2:
	case 3:
		//������ʱ�ϳɽ��
		tmp1.Draw(pDC, m_origin1, sizeImage1);

		//������ģ
		tmp2.Draw(pDC, m_origin2, sizeImage2);
	default:
		break;
	}

	//δȷ������£�����ѡ���߿�����
	if (!ok)
	{
		CClientDC dc(this);
		m_tracker.Draw(&dc, &pen);
	}
}


// CImageProcessingCutAddView ��ӡ


//void CImageProcessingCutAddView::OnFilePrintPreview()
//{
//#ifndef SHARED_HANDLERS
//	AFXPrintPreview(this);
//#endif
//}

BOOL CImageProcessingCutAddView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImageProcessingCutAddView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImageProcessingCutAddView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CImageProcessingCutAddView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessingCutAddView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessingCutAddView ���

#ifdef _DEBUG
void CImageProcessingCutAddView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcessingCutAddView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcessingCutAddDoc* CImageProcessingCutAddView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingCutAddDoc)));
	return (CImageProcessingCutAddDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessingCutAddView ��Ϣ�������


void CImageProcessingCutAddView::OnOpen1()
{
	//����ͼ��1
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "�򿪵�һ��ͼ��", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"��ͼ��"};
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!myBmp1.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	else
	{
		return;
	}
	if (myBmp1.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}

	//��ʼ��ͼ��1
	sizeImage1 = myBmp1.GetDimensions();
	m_origin1.x = 0;
	m_origin1.y = 0;
	//��ʼ����ʱͼ��
	tmp1.CreateCDib(sizeImage1, myBmp1.m_lpBMIH->biBitCount);
	//��ʼ��ѡ���λ��
	start1.x = sizeImage1.cx / 4;
	start1.y = sizeImage1.cy / 4;
	end1.x = sizeImage1.cx * 3 / 4;
	end1.y = sizeImage1.cy * 3 / 4;
	m_tracker.m_rect.SetRect(start1.x, start1.y, end1.x, end1.y);
	//��ò�������
	CClientDC dc(this);
	//����ѡ���
	m_tracker.Draw(&dc, &pen);

	//�趨״̬
	state = 1;

	Invalidate(TRUE);
}


void CImageProcessingCutAddView::OnOpen2()
{
	//����ͼ��2
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "�򿪵ڶ���ͼ��", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"��ͼ��"};
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!myBmp2.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	else
	{
		return;
	}
	if (myBmp2.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}

	//��ʼ��ͼ��2
	sizeImage2 = myBmp2.GetDimensions();
	//��ʼ����ģ
	tmp2.CreateCDib(sizeImage2, myBmp2.m_lpBMIH->biBitCount);
	//�趨״̬
	state = 1;
	Invalidate(TRUE);
}


void CImageProcessingCutAddView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//����ɻ���ͼ��δ��ֵ��ʱ���ɲ���
	if (ok || state == 1)
		return;

	int nIn; //����һ�����ĵ��ֵ��
	nIn = m_tracker.HitTest(point); //�����㵽������

	CRect Trect; //������Ƥ���ľ���
	CRect Prect; //ͼƬ���ο�

	//�õ�ͼƬ�ľ��δ�С���˴���СΪ��Դ���λ��
	Prect.left = m_origin1.x;
	Prect.top = m_origin1.y;
	Prect.right = sizeImage1.cx;
	Prect.bottom = sizeImage1.cy;

	//CPoint����ҲΪ��Դ���λ��
	if (point.x < Prect.left || point.x > Prect.right || point.y < Prect.top || point.y > Prect.bottom)
	{
		return; //�ж��Ƿ���ͼƬ���ڣ���������ͼƬ���ڵĵ��
	}

	m_tracker.SetCursor(this, nFlags); //�ı�������״
	if (nIn < 0) //�����ı��������ڣ�
	{
		Invalidate(true);
		m_tracker.TrackRubberBand(this, point, false); //���ھ��ο�������Ƥ���    

		Trect = m_tracker.m_rect; //�õ����õ���Ƥ���

		//������С
		Trect.top = (Trect.top < Prect.top ? Prect.top : Trect.top);
		Trect.left = (Trect.left < Prect.left ? Prect.left : Trect.left);
		Trect.bottom = (Trect.bottom > Prect.bottom ? Prect.bottom : Trect.bottom);
		Trect.right = (Trect.right > Prect.right ? Prect.right : Trect.right);

		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom); //���������õľ��ο�

		CClientDC dc(this);

		UpdateImage();
		m_tracker.Draw(&dc, &pen);

		Invalidate(false);
	}
	else //���ı��������ڣ�       
	{
		Invalidate(); //�ػ�����
		CClientDC dc(this);

		//api�����������������ͼƬ����     
		//�����������Ϊ���ϵͳ��Ļ���꣬�˴���Ҫ����ת��
		ClientToScreen(&Prect);
		ClipCursor(&Prect); 

		//��tracker
		m_tracker.Draw(&dc, &pen);
		Invalidate(false);
		m_tracker.Track(this, point, false);

		//�õ����õ���Ƥ��򣬴˴���õ�tracker��������Ϊ��Դ�������
		Trect = m_tracker.m_rect; 
		ScreenToClient(&Prect);

		if (Trect.top < Prect.top) //�������ο��λ��
		{
			//����ͼƬ�򶥲���λ��
			Trect.bottom = Prect.top - Trect.top + Trect.bottom;
			Trect.top = Prect.top;
		}
		if (Trect.bottom > Prect.bottom)
		{
			//�����ײ���λ��
			Trect.top = Prect.bottom - Trect.bottom + Trect.top;
			Trect.bottom = Prect.bottom;
		}
		if (Trect.right > Prect.right)
		{
			//�����ұ�
			Trect.left = Prect.right - Trect.right + Trect.left;
			Trect.right = Prect.right;
		}
		if (Trect.left < Prect.left)
		{
			//�������
			Trect.right = Prect.left - Trect.left + Trect.right;
			Trect.left = Prect.left;
		}
		//���þ��ο��С
		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom);
		
		//���½��������tracker����
		UpdateImage();
		m_tracker.Draw(&dc, &pen);
		Invalidate(TRUE);
		//�ͷŶ���������
		ClipCursor(NULL); 
	}
	CView::OnLButtonDown(nFlags, point);
}

void CImageProcessingCutAddView::ScaleImage(float scalex, float scaley)
{
	CSize newSize;
	newSize.cx = (end2.x - start2.x) * scalex;
	newSize.cy = (end2.y - start2.y) * scaley;
	TRACE("start1.x: %d\n", start1.x);
	TRACE("start1.y: %d\n", start1.y);
	TRACE("end1.x: %d\n", end1.x);
	TRACE("end1.y: %d\n", end1.y);
	TRACE("start2.x: %d\n", start2.x);
	TRACE("start2.y: %d\n", start2.y);
	TRACE("end2.x: %d\n", end2.x);
	TRACE("end2.x: %d\n", end2.y);
	for (int x = 0; x < newSize.cx; x++)
	{
		for (int y = 0; y < newSize.cy; y++)
		{
			RGBQUAD color;
			//����ӳ��
			int x0 = (long)(x * 1.0 / scalex + 0.5);
			int y0 = (long)(y * 1.0 / scaley + 0.5);
			//ȷ��ԭͼ��λ�úϷ�
			if (x0 >= 0 && x0 < end2.x - start2.x + 1 && y0 >= 0 && y0 < end2.y - start2.y + 1)
			{
				color = tmp2.GetPixel(x0 + start2.x, y0 + start2.y);
				//�ж���ģ�Ƿ���Ч
				if (color.rgbRed == 255 && color.rgbGreen == 255 && color.rgbBlue == 255)
				{
					RGBQUAD colorReal;
					colorReal = myBmp2.GetPixel(x0 + start2.x, y0 + start2.y);
					//��ɫӳ��
					tmp1.WritePixel(start1.x + x, start1.y + y, colorReal);
				}
			}
		}
	}
}

void CImageProcessingCutAddView::UpdateImage()
{
	//���ͼ�������ģ��ͼ��һ�Ĵ�С
	start1.x = m_tracker.m_rect.left;
	start1.y = m_tracker.m_rect.top;
	end1.x = m_tracker.m_rect.right;
	end1.y = m_tracker.m_rect.bottom;
	//ˢ�»�����
	tmp1.CopyDib(&myBmp1);
	//�������ű���
	float scalex = (end1.x - start1.x + 1) * 1.0 / (end2.x - start2.x + 1);
	float scaley = (end1.y - start1.y + 1) * 1.0 / (end2.y - start2.y + 1);
	ScaleImage(scalex, scaley);
	state = 3;
}


BOOL CImageProcessingCutAddView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd == this && m_tracker.SetCursor(this, nHitTest))
	{
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CImageProcessingCutAddView::OnBi()
{
	int maxX = 0, maxY = 0;
	int minX = 0x3f3f3f3f, minY = 0x3f3f3f3f;
	int threshold;
	if (myBmp2.IsEmpty())
	{
		AfxMessageBox("��δ�򿪵ڶ���ͼƬ��");
		return;
	}
	CBiDlg threditdlg;
	if (threditdlg.DoModal() == IDOK)
	{
		threshold = threditdlg.threshold;
	}
	else
	{
		return;
	}
	tmp2.CreateCDib(sizeImage2, myBmp2.m_lpBMIH->biBitCount);
	for (long i = 0; i < (long)tmp2.GetDimensions().cx; i++)
	{
		for (long j = 0; j < (long)tmp2.GetDimensions().cy; j++)
		{
			RGBQUAD color = myBmp2.GetPixel(i, j);
			//�ҶȻ�
			BYTE tem = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			//��ֵ��
			if (tem >= threshold)
			{
				color.rgbGreen = 255;
				color.rgbBlue = 255;
				color.rgbRed = 255;

				//ȡ����ģλ��
				minX = min(minX, i);
				minY = min(minY, j);
				maxX = max(maxX, i);
				maxY = max(maxY, j);
			}
			else
			{
				color.rgbGreen = 0;
				color.rgbBlue = 0;
				color.rgbRed = 0;
			}
			tmp2.WritePixel(i, j, color);
		}
	}
	//ȷ����ģλ��
	start2.x = minX;
	start2.y = minY;
	end2.x = maxX;
	end2.y = maxY;

	//������ʱͼ��
	tmp1.CopyDib(&myBmp1);
	state = 2;
	Invalidate(TRUE);
}


void CImageProcessingCutAddView::OnOk()
{
	ok = true;
	Invalidate(TRUE);
}


void CImageProcessingCutAddView::OnSave()
{
	//���洦����ͼ��
	if (!ok)
	{
		AfxMessageBox("��δȷ�ϣ�");
		return;
	}
	CFileDialog FileDlg(FALSE, _T("*.bmp"), "�����ͼƬ", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"����ͼ��"};
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			AfxMessageBox("cannot save the file");
			return;
		}
		if (!tmp1.Write(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	Invalidate(TRUE);
}
