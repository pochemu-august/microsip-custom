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
#include "ButtonEx.h"   
#include "define.h"
   

 // CButtonEx   
IMPLEMENT_DYNAMIC(CButtonEx, CMFCButton)
CButtonEx::CButtonEx()
{
    m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
    m_bTransparent = false;
}

CButtonEx::~CButtonEx()
{
}

BEGIN_MESSAGE_MAP(CButtonEx, CMFCButton)
    //{{AFX_MSG_MAP(CButtonEx)
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include "ThemeManager.h"

void CButtonEx::OnMouseMove(UINT nFlags, CPoint point)
{
    CButton::OnMouseMove(nFlags, point);
}

BOOL CButtonEx::EnableWindow(BOOL bEnable)
{
    bool isDark = ThemeManager::Get().IsDark();
    if (bEnable) {
        if (isDark && m_FaceColor == _GLOBAL_DIALER_CALL_COLOR) {
            SetTextColor(RGB(255, 255, 255));
            SetFaceColor(RGB(38, 128, 64), FALSE);
        } else {
            SetTextColor(m_TextColor);
            SetFaceColor(m_FaceColor, FALSE);
        }
        Invalidate();
    }
    else {
        if (isDark) {
            SetTextColor(RGB(140, 140, 140));
            SetFaceColor(RGB(43, 43, 43), FALSE);
        } else {
            SetTextColor(RGB(0, 0, 0));
            SetFaceColor(RGB(222, 222, 222), FALSE);
        }
        Invalidate();
    }
    return CMFCButton::EnableWindow(bEnable);
}

void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    bool isDark = ThemeManager::Get().IsDark();
    if (!IsWindowEnabled()) {
        if (isDark) {
            m_clrFace = RGB(43, 43, 43); // #2b2b2b
            m_clrRegular = RGB(140, 140, 140);
        } else {
            m_clrFace = RGB(222, 222, 222);
            m_clrRegular = RGB(0, 0, 0);
        }
    } else {
        if (isDark && m_FaceColor == _GLOBAL_DIALER_CALL_COLOR) {
            m_clrFace = RGB(38, 128, 64); // Muted green for Call button in dark mode
            m_clrRegular = RGB(255, 255, 255);
        } else {
            m_clrFace = m_FaceColor;
            m_clrRegular = m_TextColor;
        }
    }
    CMFCButton::DrawItem(lpDIS);
}

