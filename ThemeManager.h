/*
 * ThemeManager.h - Native Dark Mode & Theme Manager for MicroSIP
 */

#pragma once

#include <afxwin.h>
#include <dwmapi.h>
#include <uxtheme.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1
#define DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1 19
#endif

class ThemeManager
{
public:
    static ThemeManager& Get();

    // Palette Colors
    static const COLORREF COLOR_WINDOW_DARK = RGB(32, 32, 32);  // #202020: Window background
    static const COLORREF COLOR_INPUT_DARK = RGB(43, 43, 43);   // #2b2b2b: Panels & Inputs
    static const COLORREF COLOR_TEXT_DARK = RGB(224, 224, 224); // #e0e0e0: Primary Text
    static const COLORREF COLOR_SELECT_DARK = RGB(55, 55, 61);  // #37373d: Row selection / hover
    static const COLORREF COLOR_BORDER_DARK = RGB(63, 63, 63);  // #3f3f3f: Borders
    static const COLORREF COLOR_ZEBRA_ALT = RGB(39, 39, 39);    // Zebra alternate row
    static const COLORREF COLOR_LINK_DARK = RGB(77, 178, 255);  // SysLink / Accent

    // Theme Mode: 0 = Light, 1 = Dark, 2 = Auto (Registry detect)
    void SetMode(int mode);
    int GetMode() const { return m_mode; }
    bool IsDark() const;

    // Window & Control Theming
    void ApplyTheme(HWND hWnd, bool allowDark = true);
    HBRUSH HandleCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    void NotifyThemeChanged();
    void OnSystemSettingChange();

    // Cached GDI getters (no allocations during WM_CTLCOLOR)
    HBRUSH GetBrushWindow() const { return m_hbrWindow; }
    HBRUSH GetBrushInput() const { return m_hbrInput; }
    HBRUSH GetBrushSelection() const { return m_hbrSelection; }
    HBRUSH GetBrushBorder() const { return m_hbrBorder; }
    HPEN GetPenBorder() const { return m_hpenBorder; }
    HPEN GetPenSelection() const { return m_hpenSelection; }

    COLORREF GetColorWindow() const { return COLOR_WINDOW_DARK; }
    COLORREF GetColorInput() const { return COLOR_INPUT_DARK; }
    COLORREF GetColorText() const { return COLOR_TEXT_DARK; }
    COLORREF GetColorSelection() const { return COLOR_SELECT_DARK; }
    COLORREF GetColorBorder() const { return COLOR_BORDER_DARK; }
    COLORREF GetColorZebraAlt() const { return COLOR_ZEBRA_ALT; }

private:
    ThemeManager();
    ~ThemeManager();
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    void InitGdiObjects();
    void FreeGdiObjects();
    bool DetectSystemDark() const;

    int m_mode;             // 0 = Light, 1 = Dark, 2 = Auto
    mutable bool m_sysDark; // cached system dark state
    bool m_gdiInitialized;

    HBRUSH m_hbrWindow;
    HBRUSH m_hbrInput;
    HBRUSH m_hbrSelection;
    HBRUSH m_hbrBorder;
    HPEN m_hpenBorder;
    HPEN m_hpenSelection;
};
