# MicroSIP Dark & Extended Edition

[English](#english) | [Русский](#русский)

---

<a name="русский"></a>
## Русский

**MicroSIP Dark & Extended Edition** — это расширенный форк популярного легковесного SIP-софтфона MicroSIP (базовая версия 3.22.16, C++14, Win32/MFC) с глубокой интеграцией нативной тёмной темы Windows, расширенной локализацией и новыми функциональными возможностями.

### Основные возможности и улучшения

1. **Нативная тёмная тема (Dark Mode)**:
   - Полная поддержка тёмного оформления для всех элементов интерфейса: главное окно, диалер, журнал звонков, контакты, окно сообщений, передача вызова, окно настроек и аккаунта.
   - Использование кэшированных системных GDI-объектов (`HBRUSH`, `HPEN`) через централизованный синглтон `ThemeManager`, исключающее утечки дескрипторов GDI и мерцание при перерисовке.
   - Поддержка темной темы заголовков окон Windows через DWM (`DWMWA_USE_IMMERSIVE_DARK_MODE`).
   - Автоматическое определение системной темы Windows (Светлая / Тёмная / Авто).
   - Читаемые светлые заголовки столбцов списков (`Name`, `Number`, `Time`) на тёмном фоне через перехват `NM_CUSTOMDRAW`.
   - Бесшовная и чистая отрисовка вкладок без серых артефактов и рамок Windows.

2. **Встроенный переключатель языков (RU / EN)**:
   - Переключение между русским и английским языками «на лету» прямо из окна Настроек без необходимости перезапуска приложения.
   - Полноценный официальный словарь `langpack_russian.txt` (UTF-8 BOM), покрывающий 100% интерфейса: настройки, аккаунты, диалоги добавления контактов, вызовов и меню.
   - Корректная динамическая перерисовка и пересчет размеров вкладок («Телефон», «Журнал», «Контакты») без наложения текста.

3. **Глобальный хоткей ответа на звонок (Answer Key)**:
   - Возможность назначения глобальной горячей клавиши (по умолчанию `F2`) для мгновенного ответа на входящий вызов из любого приложения.
   - Поддержка очистки хоткея (значение «None»).
   - Корректная стилизация контрола хоткея под тёмную тему.

### Требования к системе
- Windows 7, 8, 8.1, 10, 11 (x86/x64).
- Microsoft Visual C++ Redistributable (при сборке с динамическим CRT) или автономный бинарник (при статической линковке).

### Инструкция по сборке

#### Требования для сборки:
- **Visual Studio 2022** (v143) с установленными компонентами:
  - «Разработка классических приложений на C++» (Desktop development with C++)
  - «MFC и ATL для последних средств сборки v143 (x86 и x64)»
  - Windows 10/11 SDK (10.0.19041.0 или новее)
- Прекомпилированные статические библиотеки **PJSIP** и сторонние зависимости (`pjproject`).

#### Сборка через Visual Studio IDE:
1. Откройте решение `microsip.sln` или файл проекта `microsip.vcxproj` в Visual Studio 2022.
2. Выберите конфигурацию **Release** и платформу **Win32**.
3. В меню выберите **Сборка** -> **Собрать решение** (или нажмите `Ctrl+Shift+B`).
4. Готовый исполняемый файл `microsip.exe` и сопутствующие файлы будут расположены в папке `Release/`.

#### Сборка через командную строку (MSBuild):
Запустите *Developer Command Prompt for VS 2022* или *Developer PowerShell* и выполните:
```cmd
msbuild microsip.vcxproj /p:Configuration=Release /p:Platform=Win32 /m
```

### Лицензия
Данный проект распространяется под свободной лицензией **GNU General Public License v2 (GPLv2)**. Подробности смотрите в файле [LICENSE](LICENSE).

---

<a name="english"></a>
## English

**MicroSIP Dark & Extended Edition** is an enhanced fork of the popular lightweight MicroSIP softphone (based on 3.22.16, C++14, Win32/MFC) featuring native Windows dark mode integration, enhanced internationalization, and quality-of-life improvements.

### Key Features and Improvements

1. **Native Dark Mode**:
   - Comprehensive dark theme styling across all dialogs: Main window, dialer, call logs, contacts list, messaging, transfer dialog, settings, and accounts.
   - GDI resource management via centralized `ThemeManager` singleton using cached `HBRUSH` and `HPEN` objects, preventing GDI handle leaks and render flickering.
   - Immersive dark title bar support via Windows Desktop Window Manager (`DWMWA_USE_IMMERSIVE_DARK_MODE`).
   - Automatic detection of Windows theme preference (Light / Dark / Auto).
   - High-contrast, easily readable column headers (`Name`, `Number`, `Time`) on dark backgrounds using `NM_CUSTOMDRAW` interception.
   - Clean, artifact-free tab strip rendering without legacy gray 3D border artifacts.

2. **Built-in Language Switcher (RU / EN)**:
   - On-the-fly language switching directly from the Settings dialog without restarting the application.
   - Complete official `langpack_russian.txt` dictionary (UTF-8 with BOM) covering 100% of dialogs, settings, buttons, and menus.
   - Dynamic tab reconstruction guaranteeing zero label text clipping or overlap ("Phone", "Logs", "Contacts" / "Телефон", "Журнал", "Контакты").

3. **Global Answer Call Hotkey**:
   - Customizable global hotkey (defaults to `F2`) to instantly pick up incoming calls from any running application.
   - Full support for unbinding/clearing hotkey (displays "None").
   - Dark theme styling for the hotkey input control.

### System Requirements
- Windows 7, 8, 8.1, 10, 11 (x86 / x64).

### Build Instructions

#### Prerequisites:
- **Visual Studio 2022** (v143) with the following workloads installed:
  - "Desktop development with C++"
  - "C++ MFC for latest v143 build tools (x86 & x64)"
  - Windows 10/11 SDK (10.0.19041.0 or newer)
- Compiled static PJSIP libraries and third-party dependencies (`pjproject`).

#### Building via Visual Studio IDE:
1. Open `microsip.sln` or `microsip.vcxproj` in Visual Studio 2022.
2. Select **Release** configuration and **Win32** platform.
3. Choose **Build** -> **Build Solution** (or press `Ctrl+Shift+B`).
4. Output executable `microsip.exe` will be located in the `Release/` directory.

#### Building via Command Line (MSBuild):
Open *Developer Command Prompt for VS 2022* or *Developer PowerShell* and run:
```cmd
msbuild microsip.vcxproj /p:Configuration=Release /p:Platform=Win32 /m
```

### License
This program is free software; you can redistribute it and/or modify it under the terms of the **GNU General Public License v2 (GPLv2)**. See the [LICENSE](LICENSE) file for details.
