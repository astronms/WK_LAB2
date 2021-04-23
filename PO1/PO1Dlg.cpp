
// PO1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "PO1.h"
#include "PO1Dlg.h"
#include "ParamsDlg.h"
#include "afxdialogex.h"

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
	CRect rDlg(7,7,407,407);
	MapDialogRect(rDlg);

	m_imgWndIN.Create(rDlg, this, IMG_WND_ID_IN);
		
	rDlg = CRect(530, 7, 930, 407);
	MapDialogRect(rDlg);

	m_imgWndOUT.Create(rDlg, this, IMG_WND_ID_OUT);
	
	// OPCJE
	m_combo1.AddString(L"convert to greyscale");
	m_combo1.SelectString(0, L"convert to greyscale");



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

	if (sOption == L"convert to greyscale")
	{
		for (int x = 0; x < m_imgW; x++)
			for (int y = 0; y < m_imgH; y++)
			{
				BYTE J = GetPixel(x, y);
				SetPixel(x, y, J);
			}

	}

	/*********************************************************************************************************************************
	TU NALE¯Y WSTAWIC OBS£UGÊ POZOSTA£YCH OPCJI
	
	Zmienne m_imgH i m_imgW zawieraj¹ informacje o wysokoœci i szerokoœci przetwarzanego obrazu.

	Funkcja GetPixel(x,y) zwraca wartoœæ jasnoœci piksela o wspó³rzêdnych (x,y) w obrazie Ÿród³owym (w przypadku obrazów RGB nastêpuje 
	automatyczna konwersja na poziom szaroœci).
	
	Funkcja SetPixel(x,y,J) ustawia w obrazie wynikowym jasnoœæ piksela o wspó³rzêdnych (x,y) na wartoœæ J. 

	Wartoœci jasnoœci s¹ typu BYTE (0..255).
	
	Dodawanie opcji do listy rozwijanej zrealizowane jest na koñcu kodu metody OnInitDialog(). 
	
	W metodzie OnBnClickedButtonParams() pobierany jest ³añcuch znaków wpisany przez u¿ytkownika w oknie pojawiaj¹cym siê po naciœniêciu
	przycisku "Params".
		
	***********************************************************************************************************************************
	TO DO: ADD THE IMPLEMENTATION OF OTHER IMAGE PROCESSING OPERATIONS HERE

	Variables m_imgH and m_imgW store the information about the height and the width of the image which should be processed.

	GetPixel(x,y) returns the intensity of the pixel located at (x,y) position within the source image (in the case of RGB images the 
	conversion to grey level value is performed automatically).

	SetPixel(x,y,J) sets the intensity of the pixel located at (x,y) position within the output image to J.
	The intensity values are of type BYTE (0..255).

	New options can be added to the drop-down list by using m_combo1.AddString() method (see the last lines of OnInitDialog() implementation). 
	
	OnBnClickedButtonParams() retrieves the string of parameters entered in the window that appears after clicking the "Params" button.  
	
	***********************************************************************************************************************************/

	Invalidate();
	EndWaitCursor();
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

	double r = pixelColor.GetR();
	double g = pixelColor.GetG();
	double b = pixelColor.GetB();
	double J = 0.299*r + 0.587*g + 0.114*b;

	return (BYTE)J;
}

void CPODlg::SetPixel(int x, int y, BYTE J)
{
	Color pixelColor(J,J,J);
	Status s = m_pImgOUT->SetPixel(x, y, pixelColor);
}

int CPODlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}
