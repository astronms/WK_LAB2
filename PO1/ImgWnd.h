#pragma once
#include <GdiPlus.h>
using namespace Gdiplus;

// CImgWnd

class CImgWnd : public CWnd
{
	DECLARE_DYNAMIC(CImgWnd)

public:
	CImgWnd();
	virtual ~CImgWnd();

	BOOL Create(const RECT& rect, CWnd*  pParentWnd, UINT nID);
	void SetImg(Bitmap* pImg){ m_pImg = pImg; };

protected:
	Bitmap* m_pImg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


