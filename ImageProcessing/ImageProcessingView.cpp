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
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"��ͼ��"};
	FileDlg.m_ofn.lpstrTitle = title;
	if (FileDlg.DoModal() == IDOK)
	{
		POSITION filePos = FileDlg.GetStartPosition();
		while (filePos != NULL)
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
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"Open Image"};
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

			if (((int)(cx) - 1) >= 0 && ((int)(cx) + 1) < sizeDibDisplay[0].cx && ((int)(cy) - 1) >= 0 && ((int)(cy) + 1) <
				sizeDibDisplay[0].cy)
			{
				float u = cx - (int)cx;
				float v = cy - (int)cy;
				int i = (int)cx;
				int j = (int)cy;

				int Green = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbGreen + (1 - u) * v * mybmp[0].GetPixel(i, j + 1).rgbGreen
					+ u * (1 - v) * mybmp[0].GetPixel(i + 1, j).rgbGreen + u * v * mybmp[0].GetPixel(i + 1, j + 1).rgbGreen;

				int Blue = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbBlue + (1 - u) * v * mybmp[0].GetPixel(i, j + 1).rgbBlue
					+ u * (1 - v) * mybmp[0].GetPixel(i + 1, j).rgbBlue + u * v * mybmp[0].GetPixel(i + 1, j + 1).rgbBlue;

				int Red = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbRed + (1 - u) * v * mybmp[0].GetPixel(i, j + 1).rgbRed
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
