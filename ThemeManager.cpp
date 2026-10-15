/*
 * ThemeManager.cpp - Implementation of Native Dark Mode & Theme Manager for MicroSIP
 */

#include "stdafx.h"
#include "ThemeManager.h"
#include <commctrl.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

ThemeManager& ThemeManager::Get()
{
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager()
    : m_mode(2) // Default Auto
    , m_sysDark(false)
    , m_gdiInitialized(false)
    , m_hbrWindow(NULL)
    , m_hbrInput(NULL)
    , m_hbrSelection(NULL)
    , m_hbrBorder(NULL)
    , m_hpenBorder(NULL)
    , m_hpenSelection(NULL)
{
    m_sysDark = DetectSystemDark();
    InitGdiObjects();
}

ThemeManager::~ThemeManager()
{
    FreeGdiObjects();
}

void ThemeManager::InitGdiObjects()
{
    if (m_gdiInitialized) {
        FreeGdiObjects();
    }

    m_hbrWindow    = ::CreateSolidBrush(COLOR_WINDOW_DARK);
    m_hbrInput     = ::CreateSolidBrush(COLOR_INPUT_DARK);
    m_hbrSelection = ::CreateSolidBrush(COLOR_SELECT_DARK);
    m_hbrBorder    = ::CreateSolidBrush(COLOR_BORDER_DARK);
    m_hpenBorder   = ::CreatePen(PS_SOLID, 1, COLOR_BORDER_DARK);
    m_hpenSelection= ::CreatePen(PS_SOLID, 1, COLOR_SELECT_DARK);

    m_gdiInitialized = true;
}

void ThemeManager::FreeGdiObjects()
{
    if (m_hbrWindow)    { ::DeleteObject(m_hbrWindow);    m_hbrWindow = NULL; }
    if (m_hbrInput)     { ::DeleteObject(m_hbrInput);     m_hbrInput = NULL; }
    if (m_hbrSelection) { ::DeleteObject(m_hbrSelection); m_hbrSelection = NULL; }
    if (m_hbrBorder)    { ::DeleteObject(m_hbrBorder);    m_hbrBorder = NULL; }
    if (m_hpenBorder)   { ::DeleteObject(m_hpenBorder);   m_hpenBorder = NULL; }
    if (m_hpenSelection){ ::DeleteObject(m_hpenSelection);m_hpenSelection = NULL; }

    m_gdiInitialized = false;
}

bool ThemeManager::DetectSystemDark() const
{
    DWORD value = 1;
    DWORD size = sizeof(value);
    LSTATUS status = ::RegGetValueW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        L"AppsUseLightTheme",
        RRF_RT_REG_DWORD,
        nullptr,
        &value,
        &size
    );
    if (status == ERROR_SUCCESS) {
        return (value == 0);
    }
    return false;
}

bool ThemeManager::IsDark() const
{
    if (m_mode == 0) return false;
    if (m_mode == 1) return true;
    return m_sysDark;
}

void ThemeManager::SetMode(int mode)
{
    if (mode < 0 || mode > 2) mode = 2;
    if (m_mode != mode) {
        m_mode = mode;
        if (m_mode == 2) {
            m_sysDark = DetectSystemDark();
        }
        NotifyThemeChanged();
    }
}

void ThemeManager::OnSystemSettingChange()
{
    if (m_mode == 2) {
        bool currentSysDark = DetectSystemDark();
        if (currentSysDark != m_sysDark) {
            m_sysDark = currentSysDark;
            NotifyThemeChanged();
        }
    }
}

void ThemeManager::ApplyTheme(HWND hWnd, bool allowDark)
{
    if (!hWnd || !::IsWindow(hWnd)) return;

    bool dark = allowDark && IsDark();
    BOOL dwmDark = dark ? TRUE : FALSE;

    // Apply immersive dark mode to titlebar / frame
    if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmDark, sizeof(dwmDark)))) {
        ::DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1, &dwmDark, sizeof(dwmDark));
    }

    struct ChildData {
        bool allowDark;
        bool isDark;
    } data = { allowDark, dark };

    ::EnumChildWindows(hWnd, [](HWND hChild, LPARAM lParam) -> BOOL {
        ChildData* pData = reinterpret_cast<ChildData*>(lParam);
        bool dark = pData->isDark;
        bool allow = pData->allowDark;

        wchar_t cls[64] = { 0 };
        ::GetClassNameW(hChild, cls, _countof(cls));

        if (wcscmp(cls, L"Button") == 0) {
            LONG_PTR style = ::GetWindowLongPtr(hChild, GWL_STYLE);
            LONG_PTR btnType = (style & BS_TYPEMASK);

            if (style & BS_PUSHLIKE) {
                ::SetWindowTheme(hChild, dark ? L"DarkMode_Explorer" : L"Explorer", nullptr);
            }
            else if (btnType == BS_AUTOCHECKBOX || btnType == BS_AUTORADIOBUTTON ||
                btnType == BS_CHECKBOX || btnType == BS_RADIOBUTTON) {
                if (dark) {
                    ::SetWindowTheme(hChild, L"wstr", L"wstr");
                } else {
                    ::SetWindowTheme(hChild, L"Explorer", nullptr);
                }
            } else if (btnType == BS_PUSHBUTTON || btnType == BS_DEFPUSHBUTTON ||
                      (allow && btnType == BS_OWNERDRAW)) {
                ::SetWindowTheme(hChild, dark ? L"DarkMode_Explorer" : L"Explorer", nullptr);
            }
        }
        else if (wcscmp(cls, L"Edit") == 0 || wcscmp(cls, L"msctls_hotkey32") == 0) {
            ::SetWindowTheme(hChild, dark ? L"DarkMode_CFD" : L"Explorer", nullptr);
        }
        else if (wcscmp(cls, L"ComboBox") == 0) {
            ::SetWindowTheme(hChild, dark ? L"DarkMode_CFD" : L"CFD", nullptr);
        }
        else if (wcscmp(cls, L"ListBox") == 0 || wcscmp(cls, L"Static") == 0) {
            ::SetWindowTheme(hChild, dark ? L"DarkMode_Explorer" : L"Explorer", nullptr);
        }
        else if (wcscmp(cls, WC_LISTVIEW) == 0) {
            ::SetWindowTheme(hChild, dark ? L"DarkMode_ItemsView" : L"Explorer", nullptr);
            if (dark) {
                ListView_SetBkColor(hChild, COLOR_WINDOW_DARK);
                ListView_SetTextBkColor(hChild, COLOR_WINDOW_DARK);
                ListView_SetTextColor(hChild, COLOR_TEXT_DARK);
                ::ShowScrollBar(hChild, SB_HORZ, FALSE);
            } else {

                ListView_SetBkColor(hChild, CLR_NONE);
                ListView_SetTextBkColor(hChild, CLR_NONE);
                ListView_SetTextColor(hChild, ::GetSysColor(COLOR_WINDOWTEXT));
            }
            HWND hHdr = ListView_GetHeader(hChild);
            if (hHdr) {
                ::SetWindowTheme(hHdr, dark ? L"DarkMode_ItemsView" : L"Explorer", nullptr);
            }
        }
        else if (wcscmp(cls, WC_TABCONTROL) == 0) {
            ::SetWindowTheme(hChild, dark ? L"DarkMode_Explorer" : nullptr, nullptr);
        }
        else if (wcscmp(cls, L"SysLink") == 0) {
            LITEM item = { 0 };
            item.iLink = 0;
            item.mask = LIF_ITEMINDEX | LIF_STATE;
            item.state = LIS_DEFAULTCOLORS;
            item.stateMask = LIS_DEFAULTCOLORS;
            ::SendMessage(hChild, LM_SETITEM, 0, (LPARAM)&item);
        }

        return TRUE;
    }, reinterpret_cast<LPARAM>(&data));

    ::InvalidateRect(hWnd, NULL, TRUE);
}

HBRUSH ThemeManager::HandleCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (!IsDark() || !pDC) {
        return NULL;
    }

    switch (nCtlColor)
    {
    case CTLCOLOR_EDIT:
        pDC->SetTextColor(COLOR_TEXT_DARK);
        pDC->SetBkColor(COLOR_INPUT_DARK);
        return m_hbrInput;

    case CTLCOLOR_LISTBOX:
        pDC->SetTextColor(COLOR_TEXT_DARK);
        pDC->SetBkColor(COLOR_INPUT_DARK);
        return m_hbrInput;

    case CTLCOLOR_STATIC:
    {
        wchar_t cls[64] = { 0 };
        if (pWnd && pWnd->m_hWnd) {
            ::GetClassNameW(pWnd->m_hWnd, cls, _countof(cls));
        }
        if (wcscmp(cls, L"msctls_hotkey32") == 0) {
            pDC->SetTextColor(COLOR_TEXT_DARK);
            pDC->SetBkColor(COLOR_INPUT_DARK);
            return m_hbrInput;
        } else if (wcscmp(cls, L"SysLink") == 0) {
            pDC->SetTextColor(COLOR_LINK_DARK);
        } else {
            pDC->SetTextColor(COLOR_TEXT_DARK);
        }
        pDC->SetBkColor(COLOR_WINDOW_DARK);
        pDC->SetBkMode(TRANSPARENT);
        return m_hbrWindow;
    }


    case CTLCOLOR_BTN:
        pDC->SetTextColor(COLOR_TEXT_DARK);
        pDC->SetBkColor(COLOR_WINDOW_DARK);
        pDC->SetBkMode(TRANSPARENT);
        return m_hbrWindow;

    case CTLCOLOR_DLG:
        pDC->SetBkColor(COLOR_WINDOW_DARK);
        return m_hbrWindow;

    case CTLCOLOR_SCROLLBAR:
        return m_hbrWindow;
    }

    return NULL;
}

void ThemeManager::NotifyThemeChanged()
{
    InitGdiObjects();

    DWORD curPid = ::GetCurrentProcessId();
    ::EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
        DWORD pid = 0;
        ::GetWindowThreadProcessId(hWnd, &pid);
        if (pid == *reinterpret_cast<DWORD*>(lParam)) {
            ThemeManager::Get().ApplyTheme(hWnd);
            ::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_FRAME);
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&curPid));
}
