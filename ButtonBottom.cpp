/*
 * Copyright (C) 2011-2026 MicroSIP (http://www.microsip.org)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "stdafx.h"
#include "ButtonBottom.h"   

 // CButtonBottom   
IMPLEMENT_DYNAMIC(CButtonBottom, CMFCButton)
CButtonBottom::CButtonBottom()
{
	m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_bTransparent = FALSE;
	//m_bDrawFocus = FALSE;
	m_bHighlightChecked = FALSE;

    m_bEnableActiveAccessibility = FALSE;

	m_clrButton = RGB(255, 255, 255);
	m_clrButtonHover = RGB(224, 238, 249);
	m_clrButtonPushed = RGB(92, 145, 219);
	m_clrButtonText = RGB(0, 0, 0);
	m_clrButtonTextPushed = RGB(255, 255, 255);
}

CButtonBottom::~CButtonBottom()
{
}

BEGIN_MESSAGE_MAP(CButtonBottom, CMFCButton)
	//{{AFX_MSG_MAP(CButtonBottom)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CButtonBottom::OnKillFocus(CWnd* pNewWnd)
{
    // override CMFCButton::OnKillFocus with modified code
	CButton::OnKillFocus(pNewWnd);

	if (m_bCaptured)
	{
		ReleaseCapture();
		m_bCaptured = FALSE;
	}

	//m_bPushed = FALSE;
	//m_bClickiedInside = FALSE;
	//m_bHighlighted = FALSE;
	m_bHover = FALSE;

	Invalidate();
	UpdateWindow();
}


#include "ThemeManager.h"

void CButtonBottom::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	bool isDark = ThemeManager::Get().IsDark();
	COLORREF clrBtn = isDark ? RGB(43, 43, 43) : m_clrButton; // #2b2b2b
	COLORREF clrBtnHover = isDark ? RGB(55, 55, 61) : m_clrButtonHover; // #37373d
	COLORREF clrBtnPushed = m_clrButtonPushed;
	COLORREF clrText = isDark ? RGB(224, 224, 224) : m_clrButtonText;
	COLORREF clrTextPushed = m_clrButtonTextPushed;

	if (m_bCheckButton && m_bChecked) {
		m_clrFace = clrBtnPushed;
		m_clrRegular = clrTextPushed;
	}
	else {
		if (m_bPushed) {
			m_clrFace = clrBtnPushed;
			m_clrRegular = clrTextPushed;
		}
		else if (m_bHover) {
			m_clrFace = clrBtnHover;
			m_clrRegular = clrText;
		}
		else {
			m_clrFace = clrBtn;
			m_clrRegular = clrText;
		}
	}
	CMFCButton::DrawItem(lpDIS);

	if (isDark && lpDIS && lpDIS->hDC) {
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		if (pDC) {
			CRect rc = lpDIS->rcItem;
			CPen pen(PS_SOLID, 1, RGB(63, 63, 63)); // #3f3f3f
			CPen* pOldPen = pDC->SelectObject(&pen);
			CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
			pDC->Rectangle(&rc);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}
	}
}

