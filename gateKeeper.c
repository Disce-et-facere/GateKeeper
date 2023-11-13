#include <windows.h>
#define IDC_EXIT_BUTTON 1001
#define IDC_MINIMIZE_BUTTON 1002

HWND toolbarW;
HWND clientW;
HWND toolbarExitBtn;
HWND toolbarMinBtn;

// Message Handler Main
LRESULT CALLBACK WndProc(HWND mainW, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
            
        break;

        case WM_PAINT:
        {

            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(mainW, &ps);
            HDC hdc2 = BeginPaint(toolbarW, &ps);
            HDC hdc3 = BeginPaint(clientW, &ps);

            // Fill the entire window with black
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            HBRUSH hBrush2 = CreateSolidBrush(RGB(23, 11, 11));
            HBRUSH hBrush3 = CreateSolidBrush(RGB(61, 54, 54));

            RECT rcClient;

            GetClientRect(mainW, &rcClient);
            GetClientRect(toolbarW, &rcClient);
            GetClientRect(clientW, &rcClient);

            FillRect(hdc, &rcClient, hBrush);
            FillRect(hdc2, &rcClient, hBrush2);
            FillRect(hdc3, &rcClient, hBrush3);

            EndPaint(mainW, &ps);
            EndPaint(toolbarW, &ps);
            EndPaint(clientW, &ps);

            DeleteObject(hBrush);
            DeleteObject(hBrush2);
            DeleteObject(hBrush3);

            return 0;
        }

        case WM_COMMAND:
        {
            if (HIWORD(wParam) == BN_CLICKED) {
                int buttonId = LOWORD(wParam);

                // Identify which button was clicked based on its ID
                switch (buttonId) {

                    case 1001:
                    {
                        SendMessage(mainW, WM_CLOSE, 0, 0);
                        break;
                    }
                    case 1002:
                    {
                        PostMessage(mainW, WM_SYSCOMMAND, SC_MINIMIZE, 0);
                        break;
                    }
                    // Add more buttons later!
                    default:
                        break;

                }
            }
            break;

        }

        case WM_SYSCOMMAND:
        {
            // Handle system commands, including SC_MINIMIZE
            switch (wParam & 0xFFF0) {
                case SC_MINIMIZE:
                    // Handle the minimize command if needed
                    // ...
                    break;
                default:
                    // Call the default window procedure for unhandled cases
                    return DefWindowProc(mainW, msg, wParam, lParam);
            }
            break;
        }

        case WM_CLOSE:
            DestroyWindow(mainW);
            DestroyWindow(toolbarW);
            DestroyWindow(clientW);
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
        break;

        default:

            HBRUSH hbrBackground = GetSysColorBrush(COLOR_WINDOW);
            SetClassLongPtr(mainW, GCLP_HBRBACKGROUND, (LONG_PTR)hbrBackground);
            return DefWindowProc(mainW, msg, wParam, lParam);
    }
    return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, 
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, 
                   int nCmdShow)
{

    WNDCLASSEX wcToolbar;
    WNDCLASSEX wcClient;
    WNDCLASSEX wcMain;
    HWND mainW;
    MSG Msg;

    //Register Main window --->
    wcMain.cbSize        = sizeof(WNDCLASSEX);
    wcMain.style         = CS_VREDRAW | CS_HREDRAW;
    wcMain.lpfnWndProc   = WndProc;
    wcMain.cbClsExtra    = 0;
    wcMain.cbWndExtra    = 0;
    wcMain.hInstance     = hInstance;
    wcMain.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcMain.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcMain.hbrBackground = (HBRUSH)(COLOR_WINDOW+3);
    wcMain.lpszMenuName  = NULL;
    wcMain.lpszClassName = "mainWindow";
    wcMain.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

      if(!RegisterClassEx(&wcMain))
    {
        MessageBox(NULL, "Main Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //Create Main Window --->
    mainW = CreateWindowEx(
        WS_EX_CONTROLPARENT,
        "mainWindow",
        "GATE KEEPER",
        WS_POPUP | WS_VISIBLE,
        GetSystemMetrics(SM_CXSCREEN)/2-400,
        GetSystemMetrics(SM_CYSCREEN)/2-300,
        800,
        600,
        NULL, NULL, hInstance, NULL);

    if(mainW == NULL)
    {
        MessageBox(NULL, "Main Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //Register Toolbar Window --->
    wcToolbar.cbSize        = sizeof(WNDCLASSEX);
    wcToolbar.style         = CS_VREDRAW | CS_HREDRAW;
    wcToolbar.lpfnWndProc   = WndProc;
    wcToolbar.cbClsExtra    = 0;
    wcToolbar.cbWndExtra    = 0;
    wcToolbar.hInstance     = hInstance;
    wcToolbar.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcToolbar.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcToolbar.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcToolbar.lpszMenuName  = NULL;
    wcToolbar.lpszClassName = "toolbarWindow";
    wcToolbar.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

      if(!RegisterClassEx(&wcToolbar))
    {
        MessageBox(NULL, "Toolbar Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //Create Toolbar Window --->
    toolbarW = CreateWindowEx(
        0, // change this class to something else!
        "toolbarWindow",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 800, 30,
        mainW, NULL, hInstance, NULL); 

    if(toolbarW == NULL)
    {
        MessageBox(NULL, "Toolbar Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    } 

    //Create Toolbar Buttons, exit + minimize --->
    toolbarExitBtn = CreateWindow(
        "BUTTON", // change this class to something else!
        "x",
        WS_CHILD | WS_VISIBLE | BS_FLAT,
        755, 0, 45, 30,
        toolbarW,(HMENU)IDC_EXIT_BUTTON, hInstance, NULL); 

    if(toolbarExitBtn == NULL)
    {
        MessageBox(NULL, "Exit Button Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

     toolbarMinBtn = CreateWindow(
        "BUTTON", // change this class to something else!
        "_",
        WS_CHILD | WS_VISIBLE | BS_FLAT | BS_TOP,
        710, 0, 45, 30,
        toolbarW,(HMENU)IDC_MINIMIZE_BUTTON, hInstance, NULL); 

    if(toolbarMinBtn == NULL)
    {
        MessageBox(NULL, "Minimize Button Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    } 

    //Register client Area Window --->
    wcClient.cbSize        = sizeof(WNDCLASSEX);
    wcClient.style         = CS_VREDRAW | CS_HREDRAW;
    wcClient.lpfnWndProc   = WndProc;
    wcClient.cbClsExtra    = 0;
    wcClient.cbWndExtra    = 0;
    wcClient.hInstance     = hInstance;
    wcClient.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcClient.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcClient.hbrBackground = (HBRUSH)(COLOR_WINDOW+2);
    wcClient.lpszMenuName  = NULL;
    wcClient.lpszClassName = "clientWindow";
    wcClient.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

      if(!RegisterClassEx(&wcClient))
    {
        MessageBox(NULL, "Client Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //Create Client Window --->
    clientW = CreateWindowEx(
        0,
        "clientWindow",
        "",
        WS_CHILD | WS_VISIBLE,
        0, 30, 800, 570,
        mainW, NULL, hInstance, NULL); 

    if(clientW == NULL)
    {
        MessageBox(NULL, "Client Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    } 

    ShowWindow(mainW, nCmdShow);
    ShowWindow(toolbarW, nCmdShow);
    UpdateWindow(mainW);
    UpdateWindow(toolbarW);
    

    // Message loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}


/*
LRESULT CALLBACK WndProcToolbar(HWND toolbarW, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
            
        break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(toolbarW, &ps);

            HBRUSH hBrush = CreateSolidBrush(RGB(23, 11, 11));
            RECT rcClient;
            GetClientRect(toolbarW, &rcClient);
            FillRect(hdc, &rcClient, hBrush);

            EndPaint(toolbarW, &ps);
            DeleteObject(hBrush);

            return 0;
        }

        case WM_COMMAND: // MOVED!
        {
            if (HIWORD(wParam) == BN_CLICKED) {
                int buttonId = LOWORD(wParam);

                // Identify which button was clicked based on its ID
                switch (buttonId) {

                    case 1001:
                    {
                        PostQuitMessage(0);
                        break;
                    }
                    case 1002:
                    {
                        ShowWindow(toolbarW, SW_MINIMIZE);
                        break;
                    }
                    default:
                        break;

                }
            }
            break;

        }

        default:

            HBRUSH hbrBackground = GetSysColorBrush(COLOR_WINDOW);
            SetClassLongPtr(toolbarW, GCLP_HBRBACKGROUND, (LONG_PTR)hbrBackground);
            return DefWindowProc(toolbarW, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcClient(HWND clientW, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
            
        break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(clientW, &ps);

            // Fill the entire window with black
            HBRUSH hBrush = CreateSolidBrush(RGB(61, 54, 54));
            RECT rcClient;
            GetClientRect(clientW, &rcClient);
            FillRect(hdc, &rcClient, hBrush);

            EndPaint(clientW, &ps);
            DeleteObject(hBrush);

            return 0;
        }

        default:

            HBRUSH hbrBackground = GetSysColorBrush(COLOR_WINDOW);
            SetClassLongPtr(clientW, GCLP_HBRBACKGROUND, (LONG_PTR)hbrBackground);
            return DefWindowProc(clientW, msg, wParam, lParam);
    }
    return 0;
} */