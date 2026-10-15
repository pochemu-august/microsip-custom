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
#include "StatusBar.h"   
#include "global.h"

 // StatusBar   
IMPLEMENT_DYNAMIC(StatusBar, CStatusBar)
StatusBar::StatusBar()
{
	CStatusBar::CStatusBar();
}

StatusBar::~StatusBar()
{
}

BEGIN_MESSAGE_MAP(StatusBar, CStatusBar)
	//{{AFX_MSG_MAP(StatusBar)   
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP   
END_MESSAGE_MAP()

#include "ThemeManager.h"

LRESULT StatusBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	if (IsWindowVisible())
	{
		CFrameWnd* pParent = (CFrameWnd*)GetParent();
		if (pParent)
			OnUpdateCmdUI(pParent, (BOOL)wParam);
	}
	return 0L;
}

void StatusBar::OnLButtonUp(UINT nFlags, CPoint point)
{
}

void StatusBar::OnMouseMove(UINT nFlags, CPoint point)
{
}

BOOL StatusBar::OnEraseBkgnd(CDC* pDC)
{
	if (ThemeManager::Get().IsDark()) {
		CRect rc;
		GetClientRect(&rc);
		pDC->FillSolidRect(&rc, ThemeManager::COLOR_WINDOW_DARK);
		return TRUE;
	}
	return CStatusBar::OnEraseBkgnd(pDC);
}

void StatusBar::OnPaint()
{
	if (!ThemeManager::Get().IsDark()) {
		CStatusBar::OnPaint();
		return;
	}

	CPaintDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);
	dc.FillSolidRect(&clientRect, ThemeManager::COLOR_WINDOW_DARK);

	// Thin top separator
	CPen pen(PS_SOLID, 1, ThemeManager::COLOR_BORDER_DARK);
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(clientRect.left, clientRect.top);
	dc.LineTo(clientRect.right, clientRect.top);
	dc.SelectObject(pOldPen);

	CFont* pFont = GetFont();
	CFont* pOldFont = pFont ? dc.SelectObject(pFont) : nullptr;
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(ThemeManager::COLOR_TEXT_DARK);

	int count = GetStatusBarCtrl().GetParts(0, NULL);
	for (int i = 0; i < count; i++) {
		CRect itemRect;
		GetItemRect(i, &itemRect);
		CString text = GetPaneText(i);
		if (!text.IsEmpty()) {
			itemRect.DeflateRect(4, 0);
			dc.DrawText(text, &itemRect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
		}
	}

	if (pOldFont) {
		dc.SelectObject(pOldFont);
	}
}

