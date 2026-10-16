#include "stdafx.h"

#include "CListCtrl_Sortable.h"
#include "Resource.h"
#include "mainDlg.h"
#include "ThemeManager.h"

#include <uxtheme.h>

BEGIN_MESSAGE_MAP(CListCtrl_Sortable, CListCtrl_LabelTip)
    ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnHeaderClick)	// Column Click
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
    ON_WM_SIZE()
END_MESSAGE_MAP()


namespace {
    bool IsThemeEnabled()
    {
        return IsAppThemed() && IsThemeActive();
    }
}

BOOL CListCtrl_Sortable::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLISTVIEW* pLV = reinterpret_cast<NMLISTVIEW*>(pNMHDR);
    SetFocus();	// Ensure other controls gets kill-focus

    int colIndex = pLV->iSubItem;

    if (m_SortCol == colIndex)
    {
        m_Ascending = !m_Ascending;
    }
    else
    {
        m_SortCol = colIndex;
        m_Ascending = true;
    }

    if (SortColumn(m_SortCol, m_Ascending))
        SetSortArrow(m_SortCol, m_Ascending);

    return FALSE;	// Let parent-dialog get chance
}

void CListCtrl_Sortable::SetSortArrow(int colIndex, bool ascending)
{
    if (IsThemeEnabled())
    {
        for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
        {
            HDITEM hditem = { 0 };
            hditem.mask = HDI_FORMAT;
            VERIFY(GetHeaderCtrl()->GetItem(i, &hditem));
            hditem.fmt &= ~(HDF_SORTDOWN | HDF_SORTUP);
            if (i == colIndex)
            {
                hditem.fmt |= ascending ? HDF_SORTUP : HDF_SORTDOWN;
            }
            VERIFY(CListCtrl_LabelTip::GetHeaderCtrl()->SetItem(i, &hditem));
        }
    }
    else
    {
        UINT bitmapID = m_Ascending ? IDB_UPARROW : IDB_DOWNARROW;
        for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
        {
            HDITEM hditem = { 0 };
            hditem.mask = HDI_BITMAP | HDI_FORMAT;
            VERIFY(GetHeaderCtrl()->GetItem(i, &hditem));
            if (hditem.fmt & HDF_BITMAP && hditem.fmt & HDF_BITMAP_ON_RIGHT)
            {
                if (hditem.hbm)
                {
                    DeleteObject(hditem.hbm);
                    hditem.hbm = NULL;
                }
                hditem.fmt &= ~(HDF_BITMAP | HDF_BITMAP_ON_RIGHT);
                VERIFY(CListCtrl_LabelTip::GetHeaderCtrl()->SetItem(i, &hditem));
            }
            if (i == colIndex)
            {
                hditem.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT;
                hditem.hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
                VERIFY(hditem.hbm != NULL);
                VERIFY(CListCtrl_LabelTip::GetHeaderCtrl()->SetItem(i, &hditem));
            }
        }
    }
}

void CListCtrl_Sortable::PreSubclassWindow()
{
    CListCtrl_LabelTip::PreSubclassWindow();

    // Focus rectangle is not painted properly without double-buffering
    SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);

    ShowScrollBar(SB_HORZ, FALSE);
    ModifyStyle(WS_HSCROLL, 0);

    if (ThemeManager::Get().IsDark()) {
        SetWindowTheme(GetSafeHwnd(), L"DarkMode_ItemsView", nullptr);
        SetBkColor(ThemeManager::Get().GetColorWindow());
        SetTextBkColor(ThemeManager::Get().GetColorWindow());
        SetTextColor(ThemeManager::Get().GetColorText());
        HWND hHeader = GetHeaderCtrl() ? GetHeaderCtrl()->GetSafeHwnd() : NULL;
        if (hHeader) {
            SetWindowTheme(hHeader, L"DarkMode_ItemsView", nullptr);
        }
    } else {
        SetWindowTheme(GetSafeHwnd(), L"Explorer", nullptr);
    }
}

BOOL CListCtrl_Sortable::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    // The header control sends NM_CUSTOMDRAW to its parent, i.e. to this list view
    // (not reflected through the dialog), so it has to be intercepted here.
    NMHDR* pNMHDR = reinterpret_cast<NMHDR*>(lParam);
    if (pNMHDR && pNMHDR->code == NM_CUSTOMDRAW) {
        HWND hHeader = GetHeaderCtrl() ? GetHeaderCtrl()->GetSafeHwnd() : NULL;
        if (hHeader && pNMHDR->hwndFrom == hHeader) {
            if (OnHeaderCustomDraw(pNMHDR, pResult)) {
                return TRUE;
            }
        }
    }
    return CListCtrl_LabelTip::OnNotify(wParam, lParam, pResult);
}

// Light header text (Name / Number / Time) on the dark header background.
bool CListCtrl_Sortable::OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    HWND hHeader = GetHeaderCtrl() ? GetHeaderCtrl()->GetSafeHwnd() : NULL;
    if (pNMHDR->hwndFrom == hHeader) {
        LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
        if (pNMCD->dwDrawStage == CDDS_PREPAINT) {
            *pResult = CDRF_NOTIFYITEMDRAW;
            return true;
        }
        if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT) {
            if (ThemeManager::Get().IsDark()) {
                ::SetTextColor(pNMCD->hdc, RGB(220, 220, 220));
                ::SetBkColor(pNMCD->hdc, ThemeManager::COLOR_WINDOW_DARK);
                *pResult = CDRF_NEWFONT;
                return true;
            }
            *pResult = CDRF_DODEFAULT;
            return true;
        }
    }
    return false;
}

void CListCtrl_Sortable::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    *pResult = CDRF_DODEFAULT;

    // Header notifications are handled in OnHeaderCustomDraw
    HWND hHeader = GetHeaderCtrl() ? GetHeaderCtrl()->GetSafeHwnd() : NULL;
    if (pNMHDR->hwndFrom == hHeader) {
        OnHeaderCustomDraw(pNMHDR, pResult);
        return;
    }

    if (!ThemeManager::Get().IsDark()) {
        return;
    }

    switch (pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
    {
        int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
        bool bSelected = (GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED) != 0;

        if (bSelected) {
            pLVCD->clrText = RGB(255, 255, 255);
            pLVCD->clrTextBk = ThemeManager::Get().GetColorSelection(); // #37373d
            // Clear default highlight so system doesn't draw bright blue box
            pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;
        } else {
            pLVCD->clrText = ThemeManager::Get().GetColorText(); // #e0e0e0
            // Subtle alternating zebra rows
            pLVCD->clrTextBk = (nItem % 2 == 1) ? ThemeManager::Get().GetColorZebraAlt() : ThemeManager::Get().GetColorWindow();
        }

        *pResult = CDRF_NEWFONT;
        break;
    }
    }
}

void CListCtrl_Sortable::ResetSortOrder()
{
    m_Ascending = true;
    m_SortCol = -1;
    SetSortArrow(m_SortCol, m_Ascending);
}

// The column version of GetItemData(), one can specify an unique
// identifier when using InsertColumn()
int CListCtrl_Sortable::GetColumnData(int col) const
{
    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_SUBITEM;
    VERIFY(GetColumn(col, &lvc));
    return lvc.iSubItem;
}

void CListCtrl_Sortable::SetSortColumn(int columnIndex, bool ascending)
{
    m_SortCol = columnIndex;
    m_Ascending = ascending;
    if (SortColumn(m_SortCol, m_Ascending)) {
        SetSortArrow(m_SortCol, m_Ascending);
    }
}

void CListCtrl_Sortable::OnSize(UINT nType, int cx, int cy)
{
    CListCtrl_LabelTip::OnSize(nType, cx, cy);
    ShowScrollBar(SB_HORZ, FALSE);
    ModifyStyle(WS_HSCROLL, 0);
    if (ThemeManager::Get().IsDark()) {
        SetWindowTheme(GetSafeHwnd(), L"DarkMode_ItemsView", NULL);
    }
}

