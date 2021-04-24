
// PO1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "PO1.h"
#include "PO1Dlg.h"
#include "ParamsDlg.h"
#include "afxdialogex.h"

#include <vector>
#include <algorithm>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IMG_WND_ID_IN	100
#define IMG_WND_ID_OUT	101


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPODlg dialog



CPODlg::CPODlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPODlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImgIN = m_pImgOUT = NULL;
	m_imgH = m_imgW = -1;
}

void CPODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo1);
}

BEGIN_MESSAGE_MAP(CPODlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CPODlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CPODlg::OnBnClickedButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPODlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_PARAMS, &CPODlg::OnBnClickedButtonParams)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPODlg message handlers

BOOL CPODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CRect rDlg(7, 7, 407, 407);
	MapDialogRect(rDlg);

	m_imgWndIN.Create(rDlg, this, IMG_WND_ID_IN);

	rDlg = CRect(530, 7, 930, 407);
	MapDialogRect(rDlg);

	m_imgWndOUT.Create(rDlg, this, IMG_WND_ID_OUT);

	// OPCJE
	m_combo1.AddString(L"conversion to greyscale");

	//  Tutaj mo¿na dodaæ kolejne opcje:
	//m_combo1.AddString(L"...");
	m_combo1.AddString(L"usredniajacy");
	m_combo1.AddString(L"Gaussa");
	m_combo1.AddString(L"Sobela pion");
	m_combo1.AddString(L"Sobela poziom");
	m_combo1.AddString(L"laplasjan");
	m_combo1.AddString(L"wyostrzajaca");
	m_combo1.AddString(L"Filtr medianowy 3x3");
	m_combo1.AddString(L"Filtr medianowy 5x5");
	m_combo1.AddString(L"Filtr medianowy 5x5 krzy¿owa");
	m_combo1.AddString(L"LoG");
	m_combo1.SelectString(0, L"usredniajacy");


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPODlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPODlg::OnBnClickedButtonLoad()
{
	WCHAR strFilter[] = { L"Image Files (*.bmp; *.jpg; *.tiff; *.gif; *.png)|*.bmp; *.jpg; *.tiff; *.gif; *.png|All Files (*.*)|*.*||" };

	CFileDialog FileDlg(TRUE, NULL, NULL, 0, strFilter);

	if (FileDlg.DoModal() == IDOK)
	{
		delete m_pImgIN;
		::delete m_pImgOUT;
		m_pImgOUT = NULL;

		m_pImgIN = Bitmap::FromFile(FileDlg.GetPathName());
		m_imgWndIN.SetImg(m_pImgIN);
		m_imgW = m_pImgIN->GetWidth();
		m_imgH = m_pImgIN->GetHeight();

		m_pImgOUT = ::new Bitmap(m_imgW, m_imgH, PixelFormat32bppARGB);// PixelFormat16bppGrayScale);

		m_imgWndOUT.SetImg(m_pImgOUT);

		Invalidate();
	}


}


void CPODlg::OnBnClickedButtonProcess()
{
	CString sOption;
	m_combo1.GetLBText(m_combo1.GetCurSel(), sOption);
	BeginWaitCursor();


	if (sOption == L"usredniajacy") {
		int mask[3][3] = {
			{1,1,1},
			{1,1,1},
			{1,1,1}
		};

		ApplyMask3(mask);
	}
	else if (sOption == L"Gaussa") {
		int mask[3][3] = {
			{1,4,1},
			{4,12,4},
			{1,4,1}
		};

		ApplyMask3(mask);
	}
	else if (sOption == L"Sobela pion") {
		int mask[3][3] = {
			{1,2,1},
			{0,0,0},
			{-1,-2,-1}
		};

		ApplyMask3(mask);
	}
	else if (sOption == L"Sobela poziom") {
		int mask[3][3] = {
			{-1,0,1},
			{-2,0,2},
			{-1,0,1}
		};

		ApplyMask3(mask);
	}
	else if (sOption == L"laplasjan") {
		int mask[3][3] = {
			{-2,1,-2},
			{1,4,1},
			{-2,1,-2}
		};

		ApplyMask3(mask);
	}
	else if (sOption == L"wyostrzajaca") {
		int mask[3][3] = {
			{0,-1,0},
			{-1,5,-1},
			{0,-1,0}
		};

		int wSum = 0;
		bool hasNegative = false;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				wSum += mask[i][j];
				if (mask[i][j] < 0) {
					hasNegative = true;
				}
			}
		}

		for (int x = 1; x < m_imgW - 1; x++) {
			for (int y = 1; y < m_imgH - 1; y++)
			{
				int JsSum = 0;
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						BYTE Js = GetPixel(x - i, y - j);
						JsSum += Js * mask[i + 1][j + 1];
					}
				}

				int newJs;
				if (hasNegative) {
					newJs = JsSum;
				}
				else {
					newJs = JsSum / wSum;
				}

				if (newJs < 0) newJs = 0;
				if (newJs > 255) newJs = 255;
				SetPixel(x, y, newJs);
			}
		}
	}

	if (sOption == L"Filtr medianowy 3x3")
	{
		int a = m_imgH;
		int b = m_imgW;

		BYTE** tempTab = new BYTE * [b];
		for (int i = 0; i < b; ++i)
		{
			tempTab[i] = new BYTE[a];
		}


		for (int y = 0; y < m_imgH; y++)
		{
			for (int x = 0; x < m_imgW; x++)
			{
				tempTab[x][y] = GetPixel(x, y);
			}
		}

		BYTE J[9] = {};
		BYTE temp = {};

		for (int y = 1; y < m_imgH - 1; y++) {
			for (int x = 1; x < m_imgW - 1; x++) {
				J[0] = GetPixel(x - 1, y - 1);
				J[1] = GetPixel(x - 1, y);
				J[2] = GetPixel(x - 1, y + 1);

				J[3] = GetPixel(x, y - 1);
				J[4] = GetPixel(x, y);
				J[5] = GetPixel(x, y + 1);

				J[6] = GetPixel(x + 1, y - 1);
				J[7] = GetPixel(x + 1, y);
				J[8] = GetPixel(x + 1, y + 1);

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (J[j] > J[j + 1])
						{
							temp = J[j];
							J[j] = J[j + 1];
							J[j + 1] = temp;
						}
					}
				}
				tempTab[x][y] = J[4];

			}
		}

		for (int y = 0; y < m_imgH; y++)
		{
			for (int x = 0; x < m_imgW; x++)
			{
				SetPixel(x, y, tempTab[x][y]);
			}
		}


	}

	if (sOption == L"Filtr medianowy 5x5 krzy¿owa")
	{
		int a = m_imgH;
		int b = m_imgW;

		BYTE** tempTab = new BYTE * [b];
		for (int i = 0; i < b; ++i)
		{
			tempTab[i] = new BYTE[a];
		}

		for (int y = 0; y < m_imgH; y++)
		{
			for (int x = 0; x < m_imgW; x++)
			{
				tempTab[x][y] = GetPixel(x, y);
			}
		}

		BYTE J[25] = {};
		BYTE temp = {};

		for (int y = 2; y < m_imgH - 2; y++) {
			for (int x = 2; x < m_imgW - 2; x++) {
				J[0] = GetPixel(x - 1, y - 2);
				J[1] = GetPixel(x - 1, y - 1);
				J[2] = GetPixel(x - 1, y);
				J[3] = GetPixel(x - 1, y + 1);
				J[4] = GetPixel(x - 1, y + 2);

				J[5] = GetPixel(x - 2, y - 2);
				J[6] = GetPixel(x - 2, y - 1);
				J[7] = GetPixel(x - 2, y);
				J[8] = GetPixel(x - 2, y + 1);
				J[9] = GetPixel(x - 2, y + 2);

				J[10] = GetPixel(x, y - 2);
				J[11] = GetPixel(x, y - 1);
				J[12] = GetPixel(x, y);
				J[13] = GetPixel(x, y + 1);
				J[14] = GetPixel(x, y + 2);

				J[15] = GetPixel(x + 1, y - 2);
				J[16] = GetPixel(x + 1, y - 1);
				J[17] = GetPixel(x + 1, y);
				J[18] = GetPixel(x + 1, y);
				J[19] = GetPixel(x + 1, y + 2);

				J[20] = GetPixel(x + 2, y - 2);
				J[21] = GetPixel(x + 2, y - 1);
				J[22] = GetPixel(x + 2, y);
				J[23] = GetPixel(x + 2, y + 1);
				J[24] = GetPixel(x + 2, y + 2);

				for (int i = 0; i < 24; i++)
				{
					for (int j = 0; j < 24; j++)
					{
						if (J[j] > J[j + 1])
						{
							temp = J[j];
							J[j] = J[j + 1];
							J[j + 1] = temp;
						}
					}
				}
				tempTab[x][y] = J[12];
			}
		}

		for (int y = 0; y < m_imgH; y++)
		{
			for (int x = 0; x < m_imgW; x++)
			{
				SetPixel(x, y, tempTab[x][y]);
			}
		}
	}

	if (sOption == L"Filtr medianowy 5x5")
	{
		int a = m_imgH;
		int b = m_imgW;

		BYTE** tempTab = new BYTE * [b];
		for (int i = 0; i < b; ++i)
		{
			tempTab[i] = new BYTE[a];
		}

		for (int y = 0; y < m_imgH; y++)
		{
			for (int x = 0; x < m_imgW; x++)
			{
				tempTab[x][y] = GetPixel(x, y);
			}
		}

		BYTE J[5] = {};
		BYTE temp = {};

		for (int y = 1; y < m_imgH - 1; y++) {
			for (int x = 1; x < m_imgW - 1; x++) {
				J[0] = GetPixel(x - 1, y);
				J[1] = GetPixel(x + 1, y);
				J[2] = GetPixel(x, y + 1);
				J[3] = GetPixel(x, y - 1);
				J[4] = GetPixel(x, y);

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (J[j] > J[j + 1])
						{
							temp = J[j];
							J[j] = J[j + 1];
							J[j + 1] = temp;
						}
					}
				}
				tempTab[x][y] = J[2];
			}
		}

		for (int y = 0; y < m_imgH; y++)
		{
			for (int x = 0; x < m_imgW; x++)
			{
				SetPixel(x, y, tempTab[x][y]);
			}
		}
	}
	else if (sOption == L"LoG") {
		float sigma = 1.5f;


		int maskSize = (1 + 2 * ceil(2.5f * sigma));
		float** mask = new float* [maskSize];
		int maskOffset = maskSize / 2;

		for (int i = 0; i < maskSize; i++) {
			mask[i] = new float[maskSize];
		}
		float sum = 0;
		for (int i = 0; i < maskSize; i++) {
			for (int j = 0; j < maskSize; j++) {
				float a = (-1) * (i * i + j * j - (sigma * sigma)) / pow(sigma, 4);
				float expPow = (-1) * ((i * i + j * j) / (2 * sigma * sigma));
				float expo = exp(expPow);
				mask[i][j] = a * expo;
				sum += mask[i][j];
			}
		}
		float b = sum;
		sum = 0;
		for (int i = 0; i < maskSize; i++) {
			for (int j = 0; j < maskSize; j++) {
				mask[i][j] /= mask[0][maskOffset] * (-1);
				sum += mask[i][j];
			}
		}
		b = sum;
		int size = m_imgW * m_imgH;
		int* intPixels = new int[size];


		for (int x = 0; x < m_imgW; x++) {
			for (int y = 0; y < m_imgH; y++) {

				float licznik = 0;
				for (int i = -maskOffset; i < maskOffset + 1; i++) {
					for (int j = -maskOffset; j < maskOffset + 1; j++) {

						int pixel;
						if (x - i < 0 || x - i >= m_imgW || y - j < 0 || y - j >= m_imgH)
							pixel = GetPixel(x, y);
						else
							pixel = GetPixel(x - i, y - j);
						licznik += (mask[maskOffset - i][maskOffset - j] * pixel);
					}
				}
				float newValue = licznik + 127;
				if (newValue < 0) newValue = 0;
				else newValue = 255;

				intPixels[y * m_imgW + x] = newValue;
			}
		}
		for (int x = 0; x < m_imgW; x++) {
			for (int y = 0; y < m_imgH; y++) {
				SetPixel(x, y, intPixels[y * m_imgW + x]);
			}
		}
		delete[]intPixels;
		for (int i = 0; i < maskSize; ++i)
			delete mask[i];
		delete[] mask;
	}


	/*********************************************************************************************************************************
	TU NALE¯Y WSTAWIC OBS£UGÊ POZOSTA£YCH OPCJI

	Zmienne m_imgH i m_imgW zawieraj¹ informacje o wysokoœci i szerokoœci przetwarzanego obrazu.

	Funkcja GetPixelXY(x,y) zwraca wartoœæ jasnoœci piksela o wspó³rzêdnych (x,y) w obrazie Ÿród³owym (w przypadku obrazów RGB nastêpuje
	automatyczna konwersja na poziom szaroœci).

	Funkcja SetPixelXY(x,y,Js) ustawia w obrazie wynikowym jasnoœæ piksela o wspó³rzêdnych (x,y) na wartoœæ Js.

	Wartoœci jasnoœci s¹ typu BYTE (0..255).

	Dodawanie opcji do listy rozwijanej zrealizowane jest na koñcu kodu metody OnInitDialog().

	W metodzie OnBnClickedButtonParams() pobierany jest ³añcuch znaków wpisany przez u¿ytkownika w oknie pojawiaj¹cym siê po naciœniêciu
	przycisku "Params".

	***********************************************************************************************************************************/


	Invalidate();
	EndWaitCursor();
}

void CPODlg::ApplyMask3(int mask[3][3]) {
	int wSum = 0;
	bool hasNegative = false;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			wSum += mask[i][j];
			if (mask[i][j] < 0) {
				hasNegative = true;
			}
		}
	}

	for (int x = 1; x < m_imgW - 1; x++) {
		for (int y = 1; y < m_imgH - 1; y++)
		{
			int JsSum = 0;
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					BYTE Js = GetPixel(x - i, y - j);
					JsSum += Js * mask[i + 1][j + 1];
				}
			}

			int newJs;
			if (hasNegative) {
				newJs = JsSum + 127;
			}
			else {
				newJs = JsSum / wSum;
			}

			if (newJs < 0) newJs = 0;
			if (newJs > 255) newJs = 255;
			SetPixel(x, y, newJs);
		}
	}
}

void CPODlg::OnBnClickedButtonSave()
{
	WCHAR strFilter[] = { L"Bitmap (*.bmp)|*.bmp|JPEG (*.jpg)|*.jpg||" };

	CFileDialog FileDlg(FALSE, NULL, NULL, 0, strFilter);

	if (FileDlg.DoModal() == IDOK)
	{
		CLSID Clsid;
		CString sClsId;
		CString sPath = FileDlg.GetPathName();
		CString sExt = (FileDlg.GetFileExt()).MakeUpper();

		if (sExt == "")
		{
			switch (FileDlg.m_ofn.nFilterIndex)
			{
			case 2:
				sExt = "JPG";
				break;
			default:
				sExt = "BMP";
			}

			sPath += L"." + sExt;
		}

		if (sExt == "BMP")
			sClsId = "image/bmp";

		if (sExt == "JPG")
			sClsId = "image/jpeg";

		GetEncoderClsid(sClsId, &Clsid);
		m_pImgOUT->Save(sPath, &Clsid, NULL);
	}
}


void CPODlg::OnBnClickedButtonParams()
{
	CParamsDlg paramsDlg;
	CString s;

	if (paramsDlg.DoModal() == IDOK)
	{
		s = paramsDlg.m_sParams;
		param = s;
	}
}


void CPODlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete m_pImgIN;
	::delete m_pImgOUT;

	GdiplusShutdown(m_gdiplusToken);

	// TODO: Add your message handler code here
}


BYTE CPODlg::GetPixel(int x, int y)
{
	Color pixelColor;
	m_pImgIN->GetPixel(x, y, &pixelColor);

	double red = pixelColor.GetR();
	double green = pixelColor.GetG();
	double blue = pixelColor.GetB();
	double I = 0.299 * red + 0.587 * green + 0.114 * blue;

	return (BYTE)I;
}

void CPODlg::SetPixel(int x, int y, BYTE Js)
{
	Color pixelColor(Js, Js, Js);
	Status s = m_pImgOUT->SetPixel(x, y, pixelColor);
}

int CPODlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // success
		}
	}

	free(pImageCodecInfo);
	return -1;  // failure
}
