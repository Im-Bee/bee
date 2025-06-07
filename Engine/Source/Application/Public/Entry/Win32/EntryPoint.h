#pragma once

INT WINAPI WinMain(_In_     HINSTANCE,
                   _In_opt_ HINSTANCE,
                   _In_     PSTR     ,
                   _In_     INT      )
{
    MSG msg = { 0 };

    return static_cast<INT>(msg.wParam);
}

