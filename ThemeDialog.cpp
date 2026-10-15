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
#include "ThemeDialog.h"
#include "ThemeManager.h"
#include "microsip.h"

CThemeDialog::CThemeDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
    : CDialog(nIDTemplate, pParent)
    , m_bAllowDark(true)
    , lastDark(0xFF)
{
}

BEGIN_MESSAGE_MAP(CThemeDialog, CDialog)
    //{{AFX_MSG_MAP(CThemeDialog)
    ON_WM_CTLCOLOR()
    ON_WM_SETTINGCHANGE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CThemeDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    ApplyTheme();

    return TRUE;
}

HBRUSH CThemeDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (m_bAllowDark && ThemeManager::Get().IsDark()) {
        HBRUSH hBr = ThemeManager::Get().HandleCtlColor(pDC, pWnd, nCtlColor);
        if (hBr) {
            return hBr;
        }
    }
    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CThemeDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    if (lpszSection && wcscmp(lpszSection, L"ImmersiveColorSet") == 0) {
        ThemeManager::Get().OnSystemSettingChange();
    }
    CDialog::OnSettingChange(uFlags, lpszSection);
}

void CThemeDialog::ApplyTheme()
{
    ThemeManager::Get().ApplyTheme(m_hWnd, m_bAllowDark);
}
