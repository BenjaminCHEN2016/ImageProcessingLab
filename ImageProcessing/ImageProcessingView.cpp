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
#include "BzDlg.h"
#include "RotationDlg.h"
#include "ShiftDlg.h"
#include "AboutBox.h"
#include "ZoomDlg.h"
#include <complex>
#include <algorithm>
#include "GradDlg.h"
#include "FilterDlg.h"

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
		ON_COMMAND(ID_ROTATE, &CImageProcessingView::OnRotate)
		ON_COMMAND(ID_MIRROR_H, &CImageProcessingView::OnMirrorH)
		ON_COMMAND(ID_MIRROR_V, &CImageProcessingView::OnMirrorV)
		ON_COMMAND(ID_ABOUT, &CImageProcessingView::OnAbout)
		ON_COMMAND(ID_ZOOM_FORWARD, &CImageProcessingView::OnZoomForward)
		ON_COMMAND(ID_ZOOM_CLOSEST, &CImageProcessingView::OnZoomClosest)
		ON_COMMAND(ID_ZOOM_BI, &CImageProcessingView::OnZoomBi)
		ON_COMMAND(ID_FFT, &CImageProcessingView::OnFft)
		ON_COMMAND(ID_IFFT, &CImageProcessingView::OnIfft)
		ON_COMMAND(ID_LINEAR_TRANS, &CImageProcessingView::OnLinearTrans)
		ON_COMMAND(ID_POW_TRANS, &CImageProcessingView::OnPowTrans)
		ON_COMMAND(ID_AVERAGE, &CImageProcessingView::OnAverageTrans)
		ON_COMMAND(ID_COLOR_LEVEL, &CImageProcessingView::OnColorLevel)
		ON_COMMAND(ID_COLOR_GRAY, &CImageProcessingView::OnColorGray)
		ON_COMMAND(ID_AVERAGE_FILTER, &CImageProcessingView::OnAverageFilter)
		ON_COMMAND(ID_MID_FILTER, &CImageProcessingView::OnMidFilter)
		ON_COMMAND(ID_GRADIENT, &CImageProcessingView::OnGradient)
		ON_COMMAND(ID_WEIGHT_FILTER, &CImageProcessingView::OnWeightFilter)
		ON_COMMAND(ID_PREWITT, &CImageProcessingView::OnPrewitt)
		ON_COMMAND(ID_SOBEL, &CImageProcessingView::OnSobel)
		ON_COMMAND(ID_LAPLACIAN, &CImageProcessingView::OnLaplacian)
		ON_COMMAND(ID_IDEAL, &CImageProcessingView::OnIdeal)
		ON_COMMAND(ID_BUTTERWORTH, &CImageProcessingView::OnButterworth)
		ON_COMMAND(ID_GAUSS, &CImageProcessingView::OnGauss)
		ON_COMMAND(ID_EXP, &CImageProcessingView::OnExp)
		ON_COMMAND(ID_TI, &CImageProcessingView::OnTi)
		ON_COMMAND(ID_SWAP, &CImageProcessingView::OnSwap)
		ON_COMMAND(ID_IDEAL_HIGH, &CImageProcessingView::OnIdealHigh)
		ON_COMMAND(ID_BUTTERWORTH_HIGH, &CImageProcessingView::OnButterworthHigh)
		ON_COMMAND(ID_GAUSS_HIGH, &CImageProcessingView::OnGaussHigh)
		ON_COMMAND(ID_EXP_HIGH, &CImageProcessingView::OnExpHigh)
		ON_COMMAND(ID_TI_HIGH, &CImageProcessingView::OnTiHigh)
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
		for (int i = 0; i < imageCount; i++)
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

void CImageProcessingView::OnAbout()
{
	CAboutBox aboutBox;
	aboutBox.DoModal();
}

void CImageProcessingView::OnOpen()
{
	//���õ�ǰ�����ͼƬ
	imageCount = 0;

	//Ϊ���ļ��Ի������ÿ�ѡ�ж��ͼƬ
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "",
	                    OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",
	                    nullptr);
	char title[] = {"��ͼ��"};
	FileDlg.m_ofn.lpstrTitle = title;
	if (FileDlg.DoModal() == IDOK)
	{
		POSITION filePos = FileDlg.GetStartPosition();
		while (filePos != nullptr)
		{
			if (imageCount == MAX_NUM)
			{
				//���ֻ�����ʮ��ͼ��
				AfxMessageBox("ͼƬ�����������ޣ�");
				return;
			}
			file[imageCount] = new CFile();
			CString filename = FileDlg.GetNextPathName(filePos);
			if (!(*(file[imageCount])).Open(filename, CFile::modeRead))
			{
				AfxMessageBox("cannot open the file");
				return;
			}
			if (!mybmp[imageCount].Read(file[imageCount]))
			{
				AfxMessageBox("cannot read the file");
				return;
			}
			imageCount++;
		}
	}
	else
	{
		return;
	}

	for (int i = 0; i < imageCount; i++)
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
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",
	                    nullptr);
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
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (long y = 0; y < (long)newbmp.GetDimensions().cy; y++)
	{
		for (long x = 0; x < (long)newbmp.GetDimensions().cx; x++)
		{
			if (y > 30 && y < 50)
			{
				RGBQUAD color;
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
				newbmp.WritePixel(x, y, color);
			}
			if (x > 30 && x < 50)
			{
				RGBQUAD color;
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
				newbmp.WritePixel(x, y, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnGray()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (long i = 0; i < newbmp.GetDimensions().cx; i++)
	{
		for (long j = 0; j < newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color = mybmp[0].GetPixel(i, j);
			BYTE tmp = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			color.rgbGreen = tmp;
			color.rgbBlue = tmp;
			color.rgbRed = tmp;
			newbmp.WritePixel(i, j, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnBinaryzation()
{
	int threshold;
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	CBzDlg threditdlg;
	if (threditdlg.DoModal() == IDOK)
	{
		threshold = threditdlg.threshold;
	}
	else
	{
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (long i = 0; i < (long)newbmp.GetDimensions().cx; i++)
	{
		for (long j = 0; j < (long)newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color = mybmp[0].GetPixel(i, j);
			BYTE tem = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			if (tem >= threshold)
			{
				color.rgbGreen = 255;
				color.rgbBlue = 255;
				color.rgbRed = 255;
			}
			else
			{
				color.rgbGreen = 0;
				color.rgbBlue = 0;
				color.rgbRed = 0;
			}
			newbmp.WritePixel(i, j, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnMinus()
{
	imageCount = 1;

	//�����������ͼƬ
	CDib tembmp;
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "�����������ͼƬ", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",
	                    nullptr);
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
		if (!tembmp.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	else
	{
		return;
	}
	if (tembmp.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	newbmp.Empty();
	newbmp.CopyDib(&mybmp[0]);
	RGBQUAD color, minusColor;
	int result;
	for (long i = 0; i < (long)newbmp.GetDimensions().cx; i++)
	{
		for (long j = 0; j < (long)newbmp.GetDimensions().cy; j++)
		{
			color = newbmp.GetPixel(i, j);
			minusColor = tembmp.GetPixel(i, j);
			if ((int)color.rgbBlue - (int)minusColor.rgbBlue < 0)
			{
				color.rgbBlue = 0;
			}
			else
			{
				color.rgbBlue -= minusColor.rgbBlue;
			}
			if ((int)color.rgbGreen - (int)minusColor.rgbGreen < 0)
			{
				color.rgbGreen = 0;
			}
			else
			{
				color.rgbGreen -= minusColor.rgbGreen;
			}
			if ((int)color.rgbRed - (int)minusColor.rgbRed < 0)
			{
				color.rgbRed = 0;
			}
			else
			{
				color.rgbRed -= minusColor.rgbRed;
			}
			newbmp.WritePixel(i, j, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnShift()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	CShiftDlg shiftdlg;
	int xoffset = 0, yoffset = 0;
	if (shiftdlg.DoModal() == IDOK)
	{
		xoffset = shiftdlg.x;
		yoffset = shiftdlg.y;
	}
	else
	{
		return;
	}
	newbmp.Empty();
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	RGBQUAD color;
	for (long x = 0; x < (long)newbmp.GetDimensions().cx; x++)
	{
		for (long y = 0; y < (long)newbmp.GetDimensions().cy; y++)
		{
			color = mybmp[0].GetPixel(x, y);
			int newx = x + xoffset;
			int newy = y + yoffset;

			if (newx < 0 || newx >= newbmp.GetDimensions().cx || newy < 0 || newy >= newbmp.GetDimensions().cy)
			{
				//Do nothing.
			}
			else
			{
				newbmp.WritePixel(newx, newy, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnRotate()
{
	imageCount = 1;
	double angle;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	CRotationDlg rotationdlg;
	if (rotationdlg.DoModal() == IDOK)
	{
		angle = rotationdlg.angle;
		angle = angle * pi / 180.0;
	}
	else
	{
		return;
	}

	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	long lWidth = sizeDibDisplay[0].cx; // ��ȡͼ��Ŀ��
	long lHeight = sizeDibDisplay[0].cy; // ��ȡͼ��ĸ߶�

	// ����ת�ǶȴӶ�ת��������
	float fRotateAngle = angle; // ��ת�Ƕȣ����ȣ�	
	float fSina, fCosa; // ��ת�Ƕȵ����Һ�����	
	fSina = (float)sin((double)fRotateAngle); // ������ת�Ƕȵ�����
	fCosa = (float)cos((double)fRotateAngle); // ������ת�Ƕȵ�����

	// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float)(-lWidth / 2);
	fSrcY1 = (float)(lHeight / 2);
	fSrcX2 = (float)(lWidth / 2);
	fSrcY2 = (float)(lHeight / 2);
	fSrcX3 = (float)(-lWidth / 2);
	fSrcY3 = (float)(-lHeight / 2);
	fSrcX4 = (float)(lWidth / 2);
	fSrcY4 = (float)(-lHeight / 2);

	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	// ������ת���ͼ��ʵ�ʿ��
	long lNewWidth = (long)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
	// ������ת���ͼ��߶�
	long lNewHeight = (long)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	newbmp.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp[0].m_lpBMIH->biBitCount);

	// ÿ��
	for (int y = 0; y < lNewHeight; y++)
	{
		// ÿ��
		for (int x = 0; x < lNewWidth; x++)
		{
			RGBQUAD color;

			//�����µ���ԭͼ���ϵ�λ��
			int x0 = (x - lNewWidth / 2) * fCosa - (y - lNewHeight / 2) * fSina + sizeDibDisplay[0].cx / 2.0;
			int y0 = (x - lNewWidth / 2) * fSina + (y - lNewHeight / 2) * fCosa + sizeDibDisplay[0].cy / 2.0;

			if ((x0 >= 0) && (x0 < sizeDibDisplay[0].cx) && (y0 >= 0) && (y0 < sizeDibDisplay[0].cy))
			{
				color = mybmp[0].GetPixel(x0, y0);
			}
			else
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
			}
			newbmp.WritePixel(x, y, color);
		}
	}


	Invalidate(TRUE);
}


void CImageProcessingView::OnMirrorH()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.Empty();
	newbmp.CopyDib(&mybmp[0]);
	for (long x = 0; x < (long)newbmp.GetDimensions().cx / 2; x++)
	{
		for (long y = 0; y < (long)newbmp.GetDimensions().cy; y++)
		{
			RGBQUAD color1, color2;
			color1 = newbmp.GetPixel(x, y);
			color2 = newbmp.GetPixel(newbmp.GetDimensions().cx - x - 1, y);

			newbmp.WritePixel(newbmp.GetDimensions().cx - x - 1, y, color1);
			newbmp.WritePixel(x, y, color2);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnMirrorV()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.Empty();
	newbmp.CopyDib(&mybmp[0]);
	for (long x = 0; x < (long)newbmp.GetDimensions().cx; x++)
	{
		for (long y = 0; y < (long)newbmp.GetDimensions().cy / 2; y++)
		{
			RGBQUAD color1, color2;
			color1 = newbmp.GetPixel(x, y);
			color2 = newbmp.GetPixel(x, newbmp.GetDimensions().cy - y - 1);

			newbmp.WritePixel(x, newbmp.GetDimensions().cy - y - 1, color1);
			newbmp.WritePixel(x, y, color2);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnZoomForward()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	CZoomDlg zoomDlg;
	double zoomRatio;
	if (zoomDlg.DoModal() == IDOK)
	{
		zoomRatio = zoomDlg.ratio;
	}
	else
	{
		return;
	}
	newbmp.Empty();
	CSize mySize = mybmp[0].GetDimensions();

	//�������ź�ͼ���С�������������
	long x = ((double)(mySize.cx - 1) * zoomRatio + 0.5) + 1;
	long y = ((double)(mySize.cy - 1) * zoomRatio + 0.5) + 1;
	int** flag = (int **)malloc(sizeof(int*) * x);
	for (long i = 0; i < x; i++)
	{
		flag[i] = (int *)malloc(sizeof(int*) * y);
		memset(flag[i], 0, sizeof(int) * y);
	}

	newbmp.CreateCDib(CSize(x, y), mybmp[0].m_lpBMIH->biBitCount);

	//ǰ��ӳ��
	for (long i = 0; i < mySize.cx; i++)
	{
		for (long j = 0; j < mySize.cy; j++)
		{
			RGBQUAD color;
			long nx = (double)i * zoomRatio + 0.5;
			long ny = (double)j * zoomRatio + 0.5;
			color = mybmp[0].GetPixel(i, j);
			newbmp.WritePixel(nx, ny, color);
			flag[nx][ny] = 1;
		}
	}

	//�ڽ���ɫ����Χ��ɫ��
	for (long i = 0; i < x; i++)
	{
		for (long j = 0; j < y; j++)
		{
			if (!flag[i][j])
			{
				int co = 0;
				int gr = 0, re = 0, bl = 0;
				RGBQUAD color;
				for (int z = -1; z < 2; z++)
				{
					for (int k = -1; k < 2; k++)
					{
						if (i + z < 0 || i + z >= x || j + k < 0 || j + k >= y || (!flag[i + z][j + k]))
						{
							continue;
						}
						co++;
						color = newbmp.GetPixel(i + z, j + k);
						gr += color.rgbGreen;
						re += color.rgbRed;
						bl += color.rgbBlue;
					}
				}
				if (co != 0)
				{
					gr /= co;
					re /= co;
					bl /= co;
				}
				color.rgbBlue = bl;
				color.rgbRed = re;
				color.rgbGreen = gr;
				newbmp.WritePixel(i, j, color);
			}
		}
	}

	//�ͷű�������ڴ�
	for (long i = 0; i < x; i++)
	{
		free(flag[i]);
	}
	free(flag);
	Invalidate(TRUE);
}


void CImageProcessingView::OnZoomClosest()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	CZoomDlg zoomDlg;
	double zoomRatio;
	if (zoomDlg.DoModal() == IDOK)
	{
		zoomRatio = zoomDlg.ratio;
	}
	else
	{
		return;
	}
	newbmp.Empty();

	// ���ű���
	float fXZoomRatio, fYZoomRatio;
	fXZoomRatio = fYZoomRatio = zoomRatio;

	// Դͼ��Ŀ�Ⱥ͸߶�
	long lWidth = sizeDibDisplay[0].cx; // ��ȡͼ��Ŀ��
	long lHeight = sizeDibDisplay[0].cy; // ��ȡͼ��ĸ߶�

	// ���ź�ͼ��Ŀ�Ⱥ͸߶�
	long lNewWidth = (long)(lWidth * fXZoomRatio + 0.5);
	long lNewHeight = (long)(lHeight * fYZoomRatio + 0.5);

	newbmp.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp[0].m_lpBMIH->biBitCount);

	// ÿ��
	for (int x = 0; x < lNewWidth; x++)
	{
		// ÿ��
		for (int y = 0; y < lNewHeight; y++)
		{
			RGBQUAD color;

			//�����µ���ԭͼ���ϵ�λ��
			int x0 = (long)(x / fXZoomRatio + 0.5);
			int y0 = (long)(y / fYZoomRatio + 0.5);

			if ((x0 >= 0) && (x0 < sizeDibDisplay[0].cx) && (y0 >= 0) && (y0 < sizeDibDisplay[0].cy))
			{
				color = mybmp[0].GetPixel(x0, y0);
			}
			else
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
			}
			newbmp.WritePixel(x, y, color);
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnZoomBi()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	CZoomDlg zoomDlg;
	double zoomRatio;
	if (zoomDlg.DoModal() == IDOK)
	{
		zoomRatio = zoomDlg.ratio;
	}
	else
	{
		return;
	}
	newbmp.Empty();

	// ���ű���
	float fXZoomRatio, fYZoomRatio;
	fXZoomRatio = fYZoomRatio = zoomRatio;

	// Դͼ��Ŀ�Ⱥ͸߶�
	long lWidth = sizeDibDisplay[0].cx; // ��ȡͼ��Ŀ��
	long lHeight = sizeDibDisplay[0].cy; // ��ȡͼ��ĸ߶�

	// ���ź�ͼ��Ŀ�Ⱥ͸߶�
	long lNewWidth = (long)(lWidth * fXZoomRatio + 0.5);
	long lNewHeight = (long)(lHeight * fYZoomRatio + 0.5);

	newbmp.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp[0].m_lpBMIH->biBitCount);

	// ÿ��
	for (int x = 0; x < lNewWidth; x++)
	{
		// ÿ��
		for (int y = 0; y < lNewHeight; y++)
		{
			RGBQUAD color;

			//�����µ���ԭͼ���ϵ�λ��
			float cx = x / fXZoomRatio;
			float cy = y / fYZoomRatio;

			if (((int)(cx) - 1) >= 0 && ((int)(cx) + 1) < sizeDibDisplay[0].cx && ((int)(cy) - 1) >= 0 && ((int)(cy) + 1
				) <
				sizeDibDisplay[0].cy)
			{
				float u = cx - (int)cx;
				float v = cy - (int)cy;
				int i = (int)cx;
				int j = (int)cy;

				int Green = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbGreen + (1 - u) * v * mybmp[0]
				                                                                                 .GetPixel(i, j + 1).
				                                                                                 rgbGreen
					+ u * (1 - v) * mybmp[0].GetPixel(i + 1, j).rgbGreen + u * v * mybmp[0]
					                                                               .GetPixel(i + 1, j + 1).rgbGreen;

				int Blue = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbBlue + (1 - u) * v * mybmp[0]
				                                                                               .GetPixel(i, j + 1).
				                                                                               rgbBlue
					+ u * (1 - v) * mybmp[0].GetPixel(i + 1, j).rgbBlue + u * v * mybmp[0]
					                                                              .GetPixel(i + 1, j + 1).rgbBlue;

				int Red = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbRed + (1 - u) * v * mybmp[0]
				                                                                             .GetPixel(i, j + 1).rgbRed
					+ u * (1 - v) * mybmp[0].GetPixel(i + 1, j).rgbRed + u * v * mybmp[0].GetPixel(i + 1, j + 1).rgbRed;

				color.rgbGreen = Green;
				color.rgbRed = Red;
				color.rgbBlue = Blue;
			}
			else
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
			}
			newbmp.WritePixel(x, y, color);
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnFft()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.Empty();

	int width = sizeDibDisplay[0].cx;
	int height = sizeDibDisplay[0].cy;
	int i, j;

	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	int nTransWidth;
	int nTransHeight;

	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth];

	CSize fftsize;
	fftsize = CSize(nTransWidth, nTransHeight);

	newbmp.CreateCDib(fftsize, mybmp[0].m_lpBMIH->biBitCount);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);


	double* tmp = new double[nTransWidth * nTransHeight];


	for (int x = 0; x < nTransWidth; x++)
	{
		for (int y = 0; y < nTransHeight; y++)
		{
			RGBQUAD color;
			int pos = y * nTransWidth + x;

			double dTemp = sqrt(t[pos].real() * t[pos].real() +
				t[pos].imag() * t[pos].imag()) / 100;

			int gray;

			if (dTemp > 255)
			{
				gray = 255;
			}
			else
				gray = (int)dTemp;


			color.rgbGreen = gray;
			color.rgbRed = gray;
			color.rgbBlue = gray;

			newbmp.WritePixel(x < nTransWidth / 2 ? x + nTransWidth / 2 : x - nTransWidth / 2,
			                  y < nTransHeight / 2 ? y + nTransHeight / 2 : y - nTransHeight / 2, color);
		}
	}

	Invalidate(TRUE);
}

/*************************************************************************
*
* \�������ƣ�
*   FFT_2D()
*
* \�������:
*   complex<double> * pCTData - ͼ������
*   int    nWidth - ���ݿ��
*   int    nHeight - ���ݸ߶�
*   complex<double> * pCFData - ����Ҷ�任��Ľ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ��ά����Ҷ�任��
*
************************************************************************
*/
void CImageProcessingView::FFT_2D(std::complex<double>* pCTData, int nWidth, int nHeight, std::complex<double>* pCFData)
{
	int x, y;
	int nTransWidth = nWidth;
	int nTransHeight = nHeight;

	// x��y�����У������ϵĵ������� 
	int nXLev;
	int nYLev;

	/*	while(nWidth * 2 <= sizeimage.cx)
	{
	nWidth *= 2;
	nYLev++;
	}
	while(nHeight * 2 <= sizeimage.cy)
	{
	nHeight *= 2;
	nXLev++;
	}*/
	// ����x��y�����У������ϵĵ������� 
	nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
	nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

	for (y = 0; y < nTransHeight; y++)
	{
		// y������п��ٸ���Ҷ�任 
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev);
	}

	// pCFData��Ŀǰ�洢��pCTData�����б任�Ľ�� 
	// Ϊ��ֱ������FFT_1D����Ҫ��pCFData�Ķ�ά����ת�ã���һ������FFT_1D���� 
	// ����Ҷ�б任��ʵ�����൱�ڶ��н��и���Ҷ�任�� 
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x];
		}
	}

	for (y = 0; y < nTransWidth; y++)
	{
		// ��x������п��ٸ���Ҷ�任��ʵ�����൱�ڶ�ԭ����ͼ�����ݽ����з���� 
		// ����Ҷ�任 
		FFT_1D(&pCTData[y * nTransHeight], &pCFData[y * nTransHeight], nYLev);
	}

	// pCFData��Ŀǰ�洢��pCTData������ά����Ҷ�任�Ľ��������Ϊ�˷����з��� 
	// �ĸ���Ҷ�任�����������ת�ã����ڰѽ��ת�û��� 
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransWidth * y + x] = pCFData[nTransHeight * x + y];
		}
	}

	memcpy(pCTData, pCFData, sizeof(std::complex<double>) * nTransHeight * nTransWidth);
}

/*************************************************************************
*
* \�������ƣ�
*   IFFT_2D()
*
* \�������:
*   complex<double> * pCFData - Ƶ������
*   complex<double> * pCTData - ʱ������
*   int    nWidth - ͼ�����ݿ��
*   int    nHeight - ͼ�����ݸ߶�
*
* \����ֵ:
*   ��
*
* \˵��:
*   ��ά����Ҷ���任��
*
************************************************************************
*/
void CImageProcessingView::IFFT_2D(std::complex<double>* pCFData, std::complex<double>* pCTData, int nWidth,
                                   int nHeight)
{
	// ѭ�����Ʊ��� 
	int x;
	int y;

	int nTransWidth = nWidth;
	int nTransHeight = nHeight;

	// ���乤����Ҫ���ڴ�ռ� 
	std::complex<double>* pCWork = new std::complex<double>[nTransWidth * nTransHeight];

	//��ʱ���� 
	std::complex<double>* pCTmp;

	// Ϊ�����ø���Ҷ���任,���԰Ѹ���ҶƵ�������ȡ���� 
	// Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ��� 
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCFData[nTransWidth * y + x];
			pCWork[nTransWidth * y + x] = std::complex<double>(pCTmp->real(), -pCTmp->imag());
		}
	}

	// ���ø���Ҷ���任 
	FFT_2D(pCWork, nWidth, nHeight, pCTData);

	// ��ʱ���Ĺ��������ս�� 
	// ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������ 
	// ���һ��ϵ�� 
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCTData[nTransWidth * y + x];
			pCTData[nTransWidth * y + x] =
				std::complex<double>(pCTmp->real() / (nTransWidth * nTransHeight),
				                     -pCTmp->imag() / (nTransWidth * nTransHeight));
		}
	}
	delete pCWork;
	pCWork = nullptr;
}

/*************************************************************************
*
* \�������ƣ�
*   FFT_1D()
*
* \�������:
*   complex<double> * pCTData - ָ��ʱ�����ݵ�ָ�룬�������Ҫ�任������
*   complex<double> * pCFData - ָ��Ƶ�����ݵ�ָ�룬����ľ����任������
*   nLevel ������Ҷ�任�����㷨�ļ�����2��������
*
* \����ֵ:
*   ��
*
* \˵��:
*   һά���ٸ���Ҷ�任��
*
*************************************************************************
*/

void CImageProcessingView::FFT_1D(std::complex<double>* pCTData, std::complex<double>* pCFData, int nLevel)
{
	// ѭ�����Ʊ��� */
	int i;
	int j;
	int k;

	//double PI = 3.1415926;  

	// ����Ҷ�任���� 
	int nCount = 0;

	// ���㸵��Ҷ�任���� 
	nCount = 1 << nLevel;

	// ĳһ���ĳ��� 
	int nBtFlyLen;
	nBtFlyLen = 0;

	// �任ϵ���ĽǶ� ��2 * PI * i / nCount 
	double dAngle;

	std::complex<double>* pCW;

	// �����ڴ棬�洢����Ҷ�仯��Ҫ��ϵ���� 
	pCW = new std::complex<double>[nCount / 2];

	// ���㸵��Ҷ�任��ϵ�� 
	for (i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * 3.141592 * i / nCount;
		pCW[i] = std::complex<double>(cos(dAngle), sin(dAngle));
	}

	// �任��Ҫ�Ĺ����ռ� 
	std::complex<double> *pCWork1, *pCWork2;

	// ���乤���ռ� 
	pCWork1 = new std::complex<double>[nCount];

	pCWork2 = new std::complex<double>[nCount];


	// ��ʱ���� 
	std::complex<double>* pCTmp;

	// ��ʼ����д������ 
	memcpy(pCWork1, pCTData, sizeof(std::complex<double>) * nCount);

	// ��ʱ���� 
	int nInter;
	nInter = 0;

	// �����㷨���п��ٸ���Ҷ�任 
	for (k = 0; k < nLevel; k++)
	{
		for (j = 0; j < (int)pow(2, k); j++)
		{
			//���㳤�� 
			nBtFlyLen = (int)pow(2, (nLevel - k));

			//�������ţ���Ȩ���� 
			for (i = 0; i < nBtFlyLen / 2; i++)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] =
					pCWork1[i + nInter] + pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] =
					(pCWork1[i + nInter] - pCWork1[i + nInter + nBtFlyLen / 2])
					* pCW[(int)(i * pow(2, k))];
			}
		}

		// ���� pCWork1��pCWork2������ 
		pCTmp = pCWork1;
		pCWork1 = pCWork2;
		pCWork2 = pCTmp;
	}

	// �������� 
	for (j = 0; j < nCount; j++)
	{
		nInter = 0;
		for (i = 0; i < nLevel; i++)
		{
			if (j & (1 << i))
			{
				nInter += 1 << (nLevel - i - 1);
			}
		}
		pCFData[j] = pCWork1[nInter];
	}

	// �ͷ��ڴ�ռ� 
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW = nullptr;
	pCWork1 = nullptr;
	pCWork2 = nullptr;
}

/*************************************************************************

* \�������ƣ�
*    IFFT_1D()
*
* \�������:
*   complex<double> * pCTData - ָ��ʱ�����ݵ�ָ�룬�������Ҫ���任������
*   complex<double> * pCFData - ָ��Ƶ�����ݵ�ָ�룬����ľ������任������
*   nLevel ������Ҷ�任�����㷨�ļ�����2��������
*
* \����ֵ:
*   ��
*
* \˵��:
*   һά���ٸ���Ҷ���任��
*
************************************************************************
*/
void CImageProcessingView::IFFT_1D(std::complex<double>* pCFData, std::complex<double>* pCTData, int nLevel)
{
	// ѭ�����Ʊ��� 
	int i;

	// ����Ҷ���任���� 
	int nCount;

	// ���㸵��Ҷ�任���� 
	//nCount = (int)pow(2,nLevel) ; 
	nCount = 1 << nLevel;
	// �任��Ҫ�Ĺ����ռ� 
	std::complex<double>* pCWork;

	// ���乤���ռ� 
	pCWork = new std::complex<double>[nCount];

	// ����Ҫ���任������д�빤���ռ�pCWork 
	memcpy(pCWork, pCFData, sizeof(std::complex<double>) * nCount);

	// Ϊ�����ø���Ҷ���任,���԰Ѹ���ҶƵ�������ȡ���� 
	// Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ��� 
	for (i = 0; i < nCount; i++)
	{
		pCWork[i] = std::complex<double>(pCWork[i].real(), -pCWork[i].imag());
	}

	// ���ÿ��ٸ���Ҷ�任ʵ�ַ��任������洢��pCTData�� 
	FFT_1D(pCWork, pCTData, nLevel);

	// ��ʱ���Ĺ��������ս�� 
	// ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������ 
	// ���һ��ϵ��nCount 
	for (i = 0; i < nCount; i++)
	{
		pCTData[i] =
			std::complex<double>(pCTData[i].real() / nCount, -pCTData[i].imag() / nCount);
	}

	// �ͷ��ڴ� 
	delete pCWork;
	pCWork = nullptr;
}

//////////////////////////////////////////////////////////////
//�ú�������ʵ�ֶ�ά����Ҷ�任
//����height��width�ֱ��ʾͼ��ĸ߶ȺͿ�ȣ�ising��ʾ�����任
//////////////////////////////////////////////////////////////
void CImageProcessingView::fourier(std::complex<double>* data, int nTransHeight, int nTransWidth, int isign)
{
	std::complex<double>* pCTData = new std::complex<double>[nTransHeight * nTransWidth];
	std::complex<double>* pCFData = new std::complex<double>[nTransHeight * nTransWidth];

	if (isign == 1)
	{
		for (int j = 0; j < nTransHeight; j++)
			for (int i = 0; i < nTransWidth; i++)
			{
				pCTData[j * nTransWidth + i] = data[j * nTransWidth + i];
			}
		FFT_2D(pCTData, nTransWidth, nTransHeight, pCFData);

		for (int i = 0; i < nTransWidth; i++)
			for (int j = 0; j < nTransHeight; j++)
				data[j * nTransWidth + i] = pCFData[j * nTransWidth + i];
	}
	else if (isign == -1)
	{
		for (int j = 0; j < nTransHeight; j++)
			for (int i = 0; i < nTransWidth; i++)
			{
				pCFData[j * nTransWidth + i] = data[j * nTransWidth + i];
			}
		IFFT_2D(pCFData, pCTData, nTransWidth, nTransHeight);
		for (int i = 0; i < nTransWidth; i++)
			for (int j = 0; j < nTransHeight; j++)
			{
				data[j * nTransWidth + i] = pCTData[j * nTransWidth + i];
			}
	}
}


void CImageProcessingView::OnIfft()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.Empty();

	int width = sizeDibDisplay[0].cx;
	int height = sizeDibDisplay[0].cy;
	int i, j;


	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;

	int nTransWidth;
	int nTransHeight;

	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth];

	CSize fftsize;
	fftsize = CSize(nTransWidth, nTransHeight);

	newbmp.CreateCDib(fftsize, mybmp[0].m_lpBMIH->biBitCount);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);

	//����Ҷ���任
	fourier(t, nTransHeight, nTransWidth, -1);


	newbmp.CreateCDib(fftsize, mybmp[0].m_lpBMIH->biBitCount);

	double max = 0.0;
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < t[nTransWidth * j + i].real())
				max = t[nTransWidth * j + i].real();
		}


	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					t[nTransWidth * j + i].real() * t[nTransWidth * j + i].real() + t[nTransWidth * j + i].imag() * t[
						nTransWidth * j +
						i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnLinearTrans()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.Empty();

	CSize sizeimage = mybmp[0].GetDimensions();
	newbmp.CopyDib(&mybmp[0]);


	//�ڵ�һ������ȡ������(x1,y1)=��109,0��  (x2,y2)=��115,255��
	float x1 = 109, y1 = 0;
	float x2 = 115, y2 = 255;

	//����a1,b1 
	float a1 = (y1 - y2) * 1.0 / (x1 - x2);
	float b1 = y1 - a1 * x1;


	//�ڵڶ�������ȡ������(x11,y11)=��105,0��  (x22,y22)=��115,255��
	float x11 = 105, y11 = 0;
	float x22 = 115, y22 = 255;

	//����a2,b2 
	float a2 = (y11 - y22) * 1.0 / (x11 - x22);
	float b2 = y11 - a2 * x11;

	//�ڵ���������ȡ������(x111,y111)=��91,0��  (x222,y222)=��96,20��
	float x111 = 91, y111 = 0;
	float x222 = 96, y222 = 0;

	//����a3,b3 
	float a3 = (y111 - y222) * 1.0 / (x111 - x222);
	float b3 = y111 - a3 * x111;

	for (int y = 0; y < sizeimage.cy; y++)
	{
		for (int x = 0; x < sizeimage.cx; x++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(x, y);
			//s = ag + b
			double g = color.rgbRed;

			if (g > x11 && g < x22)
				g = a2 * g + b2;
			else if (g > x111 && g < x222)
				g = a3 * g + b3;
			else if (g > x1 && g < x2)
				g = a1 * g + b1;
			else
				g = (a1 + a2 + a3) / 3 * g + (b1 + b2 + b3) / 3;

			if (g > 255) g = 255;
			if (g < 0) g = 0;


			color.rgbBlue = (unsigned char)g;
			color.rgbGreen = (unsigned char)g;
			color.rgbRed = (unsigned char)g;
			newbmp.WritePixel(x, y, color);
		}
	}
	Invalidate(TRUE);
}

void CImageProcessingView::OnPowTrans()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.Empty();

	//�ҶȻ�
	newbmp.CopyDib(&mybmp[0]);
	for (int i = 0; i < newbmp.GetDimensions().cx; i++)
	{
		for (int j = 0; j < newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color = mybmp[0].GetPixel(i, j);
			BYTE tmp = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			color.rgbGreen = tmp;
			color.rgbBlue = tmp;
			color.rgbRed = tmp;
			newbmp.WritePixel(i, j, color);
			//TRACE("Gray: %d\n", newbmp.GetPixel(i,j).rgbRed);
		}
	}

	// ��ͼ�������ֵ���б任
	// ÿ��
	for (int i = 0; i < newbmp.GetDimensions().cx; i++)
	{
		for (int j = 0; j < newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(i, j);
			//s = ag + b
			//TODO:������δ����
			double g = color.rgbRed;
			double c = 6.0;
			double a = 0.7;
			//TRACE("Before: %lf\n", g);
			g = c * pow(g, a);
			//g = c * log(1 + g);

			if (g > 255)
			{
				g = 255;
			}
			//TRACE("Afther: %lf\n", g);

			color.rgbBlue = (unsigned char)g;
			color.rgbGreen = (unsigned char)g;
			color.rgbRed = (unsigned char)g;
			newbmp.WritePixel(i, j, color);
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnAverageTrans()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (int i = 0; i < newbmp.GetDimensions().cx; i++)
	{
		for (int j = 0; j < newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color = mybmp[0].GetPixel(i, j);
			BYTE tmp = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			color.rgbGreen = tmp;
			color.rgbBlue = tmp;
			color.rgbRed = tmp;
			newbmp.WritePixel(i, j, color);
		}
	}

	CSize mysize = newbmp.GetDimensions();
	long x = mysize.cx;
	long y = mysize.cy;
	int colcolumn[256];
	int maxg = 0, ming = 255;
	memset(colcolumn, 0, sizeof(colcolumn));
	//ͳ��
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			int temp = newbmp.GetPixel(j, i).rgbBlue;
			colcolumn[temp]++;
			maxg = max(maxg, temp);
			ming = min(ming, temp);
		}
	}
	//�Ҷ�����
	int qzh = 0;
	int newGrayMapping[256];
	for (int i = 0; i < 256; i++)
	{
		qzh += colcolumn[i];
		newGrayMapping[i] = (int)(qzh * 1.0 / newbmp.GetDimensions().cx / newbmp.GetDimensions().cy * 255.0 + 0.5);
	}
	for (long i = 0; i < y; i++)
	{
		// ÿ��
		for (long j = 0; j < x; j++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(j, i);
			int gray = newGrayMapping[color.rgbBlue];
			color.rgbBlue = gray;
			color.rgbGreen = gray;
			color.rgbRed = gray;
			newbmp.WritePixel(j, i, color);
		}
	}

	/*
	double range = maxg - ming;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color;
			double tem = newbmp.GetPixel(j, i).rgbBlue - ming;
			tem = tem / range * 255;
			color.rgbBlue = (int)(tem + 0.5);
			color.rgbRed = (int)(tem + 0.5);
			color.rgbGreen = (int)(tem + 0.5);
			newbmp.WritePixel(j, i, color);
		}
	}
	*/
	Invalidate(TRUE);
}


void CImageProcessingView::OnColorLevel()
{
	int RGBlevel[6][3] =
	{
		{255, 127, 127},
		{142, 219, 255},
		{178, 255, 182},
		{255, 248, 140},
		{237, 155, 255},
		{155, 255, 226}
	};
	double graylevelpixels = 43;
	OnAverageTrans();
	CSize mysize;
	mysize = newbmp.GetDimensions();
	long x = mysize.cx;
	long y = mysize.cy;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color;
			double temp = newbmp.GetPixel(j, i).rgbBlue;
			int colorlevel = (temp / graylevelpixels);
			//TRACE("%d\n", colorlevel);
			color.rgbRed = RGBlevel[colorlevel][0];
			color.rgbGreen = RGBlevel[colorlevel][1];
			color.rgbBlue = RGBlevel[colorlevel][2];
			newbmp.WritePixel(j, i, color);
		}
	}
	Invalidate(TRUE);
}

void CImageProcessingView::OnColorGray()
{
	OnGray();

	//ͼ���Ϊ���ɸ��Ҷȼ�
	int graylevel = 7;
	//���������ɫ
	RGBQUAD* color_tab = new RGBQUAD[graylevel];
	srand((unsigned int)time(NULL));
	for (int i = 0; i < graylevel; i++)
	{
		color_tab[i].rgbBlue = (unsigned char)rand() % 255;
		color_tab[i].rgbGreen = (unsigned char)rand() % 255;
		color_tab[i].rgbRed = (unsigned char)rand() % 255;
	}
	int min = 1000;
	int max = 0;
	// ��ͼ�������ֵ���б任
	for (int x = 0; x < newbmp.GetDimensions().cx; x++)
	{
		for (int y = 0; y < newbmp.GetDimensions().cy; y++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(x, y);
			if (color.rgbBlue > max) max = color.rgbBlue;

			if (color.rgbBlue < min) min = color.rgbBlue;
		}
	}
	int step = (max - min) / graylevel;
	for (int x = 0; x < newbmp.GetDimensions().cx; x++)
	{
		for (int y = 0; y < newbmp.GetDimensions().cy; y++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(x, y);

			int level = (color.rgbBlue - min) / step;
			newbmp.WritePixel(x, y, color_tab[level]);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnAverageFilter()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	int x = mysize.cx;
	int y = mysize.cy;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color;
			double r = 0;
			for (int w = -1; w < 2; w++)
			{
				for (int k = -1; k < 2; k++)
				{
					if (w != 0 || k != 0)
					{
						int xx = (j + k + x) % x;
						int yy = (i + w + y) % y;
						color = mybmp[1].GetPixel(xx, yy);
						r += color.rgbRed;
					}
				}
			}
			color.rgbRed = r / 8.0 + 0.5;
			color.rgbGreen = r / 8.0 + 0.5;
			color.rgbBlue = r / 8.0 + 0.5;
			newbmp.WritePixel(j, i, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnMidFilter()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	int x = mysize.cx;
	int y = mysize.cy;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color;
			int temp[9];
			int co = 0;
			for (int w = -1; w < 2; w++)
			{
				for (int k = -1; k < 2; k++)
				{
					if (w != 0 || k != 0)
					{
						int xx = (j + k + x) % x;
						int yy = (i + w + y) % y;
						color = mybmp[1].GetPixel(xx, yy);
						temp[co++] = color.rgbRed;
					}
				}
			}
			std::sort(temp, temp + 9);
			color.rgbRed = temp[4];
			color.rgbGreen = temp[4];
			color.rgbBlue = temp[4];
			newbmp.WritePixel(j, i, color);
		}
	}
	Invalidate(TRUE);
}

void CImageProcessingView::OnWeightFilter()
{
	double temp[3][3] = {1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16};
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	for (int x = 0 + 1; x < mysize.cx - 1; x++)
	{
		for (int y = 0 + 1; y < mysize.cy - 1; y++)
		{
			RGBQUAD color;

			double gray = 0;
			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++)
				{
					color = mybmp[1].GetPixel(x + i, y + j);
					gray += color.rgbRed * temp[i + 1][j + 1];
				}

			color.rgbBlue = (int)gray;
			color.rgbGreen = (int)gray;
			color.rgbRed = (int)gray;
			newbmp.WritePixel(x, y, color);
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnGradient()
{
	int threshold;
	CGradDlg threditdlg;
	if (threditdlg.DoModal() == IDOK)
	{
		threshold = threditdlg.threshold;
	}
	else
	{
		return;
	}
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	int x = mysize.cx;
	int y = mysize.cy;
	RGBQUAD color, color1, color2;
	for (int i = 0; i < y - 1; i++)
	{
		for (int j = 0; j < x - 1; j++)
		{
			color = newbmp.GetPixel(j, i);
			color1 = newbmp.GetPixel(j + 1, i);
			color2 = newbmp.GetPixel(j, i + 1);
			int temp = abs(color.rgbRed - color1.rgbRed) + abs(color.rgbRed - color2.rgbRed);
			if (temp < 255 && temp >= threshold)
			{
				color.rgbBlue = temp;
				color.rgbGreen = temp;
				color.rgbRed = temp;
				newbmp.WritePixel(j, i, color);
			}
			else
			{
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
				newbmp.WritePixel(j, i, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::TemplateONE(CDib& lpDIBBits, long lWidth, long lHeight, int iTempH, int iTempW, int iTempMX,
                                       int iTempMY, float* fpArray, float fCoef)
{
	CSize sizeimage = lpDIBBits.GetDimensions();
	newbmp.CreateCDib(sizeimage, lpDIBBits.m_lpBMIH->biBitCount);
	for (int i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		for (int j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			RGBQUAD color;
			float fResult = 0;
			for (int k = 0; k < iTempH; k++)
			{
				for (int l = 0; l < iTempW; l++)
				{
					color = lpDIBBits.GetPixel(j - iTempMX + l, i - iTempMY + k);
					fResult += color.rgbRed * fpArray[k * iTempW + 1];
				}
			}
			fResult *= fCoef;
			fResult = fabs(fResult);
			if (fResult > 255)
			{
				color.rgbGreen = 255;
				color.rgbBlue = 255;
				color.rgbRed = 255;
				newbmp.WritePixel(j, i, color);
			}
			else
			{
				color.rgbGreen = (unsigned char)(fResult + 0.5);
				color.rgbBlue = (unsigned char)(fResult + 0.5);
				color.rgbRed = (unsigned char)(fResult + 0.5);
				newbmp.WritePixel(j, i, color);
			}
		}
	}
	lpDIBBits.CopyDib(&newbmp);
}


void CImageProcessingView::OnPrewitt()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	CDib tmpbmp1, tmpbmp2;
	tmpbmp1.CreateCDib(mysize, mybmp[1].m_lpBMIH->biBitCount);
	tmpbmp2.CreateCDib(mysize, mybmp[1].m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp[1]);
	tmpbmp2.CopyDib(&mybmp[1]);
	int x = mysize.cx;
	int y = mysize.cy;
	int iTempH = 3;
	int iTempW = 3;
	float fTempC = 1.0;
	int iTempMX = 1;
	int iTempMY = 1;
	float aTemplate[9] = {
		-1.0, -1.0, -1.0, 0, 0, 0, 1.0, 1.0, 1.0
	};
	float aTemplate1[9] = {
		1.0, 0.0, -1.0, 1.0, 0.0, -1.0, 1.0, 0.0, -1.0
	};
	TemplateONE(tmpbmp1, mysize.cx, mysize.cy, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC);
	TemplateONE(tmpbmp2, mysize.cx, mysize.cy, iTempH, iTempW, iTempMX, iTempMY, aTemplate1, fTempC);
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color, color1;
			color = tmpbmp1.GetPixel(j, i);
			color1 = tmpbmp2.GetPixel(j, i);
			if (color1.rgbBlue > color.rgbBlue)
			{
				newbmp.WritePixel(j, i, color1);
			}
			else
			{
				newbmp.WritePixel(j, i, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnSobel()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	CDib tmpbmp1, tmpbmp2;
	tmpbmp1.CreateCDib(mysize, mybmp[1].m_lpBMIH->biBitCount);
	tmpbmp2.CreateCDib(mysize, mybmp[1].m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp[1]);
	tmpbmp2.CopyDib(&mybmp[1]);
	int x = mysize.cx;
	int y = mysize.cy;
	int iTempH = 3;
	int iTempW = 3;
	float fTempC = 1.0;
	int iTempMX = 1;
	int iTempMY = 1;
	float aTemplate[9] = {
		-1.0, -2.0, -1.0, 0.0, 0.0, 0.0, 1.0, 2.0, 1.0
	};
	float aTemplate1[9] = {
		-1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0
	};
	TemplateONE(tmpbmp1, mysize.cx, mysize.cy, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC);
	TemplateONE(tmpbmp2, mysize.cx, mysize.cy, iTempH, iTempW, iTempMX, iTempMY, aTemplate1, fTempC);
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color, color1;
			color = tmpbmp1.GetPixel(j, i);
			color1 = tmpbmp2.GetPixel(j, i);
			if (color1.rgbBlue > color.rgbBlue)
			{
				newbmp.WritePixel(j, i, color1);
			}
			else
			{
				newbmp.WritePixel(j, i, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::TemplateTWO(CDib& lpDIBBits, long lWidth, long lHeight, int iTempH, int iTempW,
                                       float* fpArray, float fCoef)
{
	CSize sizeimage = lpDIBBits.GetDimensions();
	newbmp.CreateCDib(sizeimage, lpDIBBits.m_lpBMIH->biBitCount);
	for (int y = 2; y < sizeimage.cy - 2; y++) // ��(��ȥ��Ե����)
	{
		for (int x = 2; x < sizeimage.cx - 2; x++) // ��(��ȥ��Ե����)
		{
			RGBQUAD color;

			double fResult = 0;

			for (int j = -2; j <= 2; j++)
			{
				for (int i = -2; i <= 2; i++)
				{
					color = lpDIBBits.GetPixel(x + i, y + j);
					fResult += color.rgbRed * fpArray[(i + 2) * 5 + (j + 2)];
				}
			}

			fResult = fabs(fResult); // ȡ����ֵ
			if (fResult > 255)
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
			}
			else if (fResult > 3.5)
			{
				color.rgbBlue = fResult;
				color.rgbRed = fResult;
				color.rgbGreen = fResult;
			}
			newbmp.WritePixel(x, y, color);
		}
	}
	lpDIBBits.CopyDib(&newbmp);
}

void CImageProcessingView::OnLaplacian()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	// mybmp[0].CopyDib(&newbmp);
	mybmp[1].CopyDib(&newbmp);
	CDib tmpbmp1;
	tmpbmp1.CopyDib(&mybmp[1]);
	int x = mysize.cx;
	int y = mysize.cy;
	int iTempH = 3;
	int iTempW = 3;
	float fTempC = 1.0;
	float aTemplate[25]; // ģ������

	// ����ģ�����
	aTemplate[0] = -2.0;
	aTemplate[1] = -4.0;
	aTemplate[2] = -4.0;
	aTemplate[3] = -4.0;
	aTemplate[4] = -2.0;
	aTemplate[5] = -4.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 8.0;
	aTemplate[8] = 0.0;
	aTemplate[9] = -4.0;
	aTemplate[10] = -4.0;
	aTemplate[11] = 8.0;
	aTemplate[12] = 24.0;
	aTemplate[13] = 8.0;
	aTemplate[14] = -4.0;
	aTemplate[15] = -4.0;
	aTemplate[16] = 0.0;
	aTemplate[17] = 8.0;
	aTemplate[18] = 0.0;
	aTemplate[19] = -4.0;
	aTemplate[20] = -2.0;
	aTemplate[21] = -4.0;
	aTemplate[22] = -4.0;
	aTemplate[23] = -4.0;
	aTemplate[24] = -2.0;
	TemplateTWO(tmpbmp1, mysize.cx, mysize.cy, iTempH, iTempW, aTemplate, fTempC);
	newbmp.CopyDib(&tmpbmp1);
	Invalidate(TRUE);
}


void CImageProcessingView::OnIdeal()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0; //�м����

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 


	d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0);

				//�����ͨ�˲���
				if (sqrt(
						(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2))
					<=
					d0)
					H[position] = std::complex<double>(1.0, 0.0);
				else
					H[position] = std::complex<double>(0.0, 0.0);
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);

	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	double max = 0.0;

	//�˲�
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real()
					+ tmp[nTransWidth * j + i].imag() * tmp[nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnButterworth()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0; //�м����
	int n = dlg.n;

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 


	d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ


				double d = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));
				H[position] = 1 / (1 + (sqrt(2) - 1) * pow((d / d0), (2 * n)));
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);


	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	double max = 0.0;

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnGauss()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0, max = 0.0; //�м����

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ����  
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 


	d0 = u * u + v * v; //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ

				//�����˹��ͨ�˲���
				double d2 = (i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight /
					2);
				H[position] = exp(-d2 / (2 * d0));
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);

	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}

	//�˲�

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnExp()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;
	int n = dlg.n;


	double d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	int i, j;
	double max = 0.0;

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 

	d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ

				//����ָ����ͨ�˲���
				double d = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));
				H[position] = std::complex<double>(exp(-0.347 * pow((d / d0), n)), 0.0);
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);


	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//�˲�
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate();
}


void CImageProcessingView::OnTi()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;

	int u1 = dlg.u1;
	int v1 = dlg.v1;

	double d0 = sqrt(u * u + v * v); //�����ֹƵ��d0
	double d1 = sqrt(u1 * u1 + v1 * v1);


	double max = 0.0;
	int i, j;

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 

	d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ

				//�������ε�ͨ�˲���
				double tmp = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));

				if (tmp <= d0)
					H[position] = std::complex<double>(1.0, 0.0);

				else if (tmp > d1)
					H[position] = std::complex<double>(0.0, 0.0);
				else
				{
					double d = tmp;
					H[position] = (d - d1) / (d0 - d1);
				}
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);

	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//�˲�
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnSwap()
{
	if (mybmp[0].IsEmpty() || newbmp.IsEmpty())
	{
		AfxMessageBox("�޿�ִ�в���");
		return;
	}
	imageCount = 1;
	mybmp[0].CopyDib(&newbmp);
	newbmp.Empty();
	Invalidate(TRUE);
}


void CImageProcessingView::OnIdealHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0; //�м����


	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;


	////////////////////////////////////////
	using namespace std;

	complex<double>* t = new complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	complex<double>* H = new complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	complex<double>* tmp = new complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 


	d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ


				if (sqrt(
						(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2))
					<=
					d0)
					H[position] = complex<double>(0.0, 0.0);
				else
					H[position] = complex<double>(1.0, 0.0);
			}
			else
			{
				t[position] = complex<double>(0.0, 0.0);
				H[position] = complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			//���������˲���
			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	double max = 0.0;

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                       tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			//���������˲���
			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;


	Invalidate(TRUE);
}


void CImageProcessingView::OnButterworthHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0; //�м����
	int n = dlg.n;

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 


	d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ


				double d = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));
				H[position] = 1 / (1 + (sqrt(2) - 1) * pow((d0 / d), (2 * n)));
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);


	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	double max = 0.0;

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnGaussHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0, max = 0.0; //�м����

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ����  
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 


	d0 = u * u + v * v; //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ

				//�����˹��ͨ�˲���
				double d2 = (i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight /
					2);
				H[position] = 1 - exp(-d2 / (2 * d0));
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);

	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}

	//�˲�

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnExpHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;
	int n = dlg.n;


	double d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	int i, j;
	double max = 0.0;

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 

	d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ

				//����ָ����ͨ�˲���
				double d = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));
				H[position] = std::complex<double>(exp(-0.347 * pow((d0 / d), n)), 0.0);
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);


	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//�˲�
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate();
}


void CImageProcessingView::OnTiHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("��δ��ͼƬ��");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //ԭͼ��Ŀ�Ⱥ͸߶�    
	int height = mybmp[0].GetDimensions().cy;

	int u1 = dlg.u1;
	int v1 = dlg.v1;

	double d0 = sqrt(u * u + v * v); //�����ֹƵ��d0
	double d1 = sqrt(u1 * u1 + v1 * v1);


	double max = 0.0;
	int i, j;

	////////////////////////////////////////
	//��ȡ�任�߶�	
	double dTmpOne; // ��ʱ���� 
	double dTmpTwo;


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // ���乤���ռ� 

	d0 = sqrt(u * u + v * v); //�����ֹƵ��d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //����ǿ��ֵ

				//�������ε�ͨ�˲���
				double tmp = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));

				if (tmp <= d0)
					H[position] = std::complex<double>(0.0, 0.0);

				else if (tmp > d1)
					H[position] = std::complex<double>(1.0, 0.0);
				else
				{
					double d = tmp;
					H[position] = (d - d1) / (d0 - d1);
				}
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//����Ҷ�任
	fourier(t, nTransHeight, nTransWidth, 1);

	//Ƶ�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//�˲�
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//�����Ļ�
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//����Ҷ���任
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}
