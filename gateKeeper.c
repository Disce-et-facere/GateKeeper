#include <windows.h>
#include <commctrl.h>

#define TIMER_ENABLE_BUTTONS 1
#define BUTTON_ENABLE_DELAY 200


// Message handler
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    HWND hListView;
    HWND atBtn;
    HWND cnBtn;
    HWND cidBtn;
    HWND cpBtn;
    HWND cacBtn;

    switch (msg) {
        case WM_CREATE: {
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_LISTVIEW_CLASSES;
            InitCommonControlsEx(&icex);

            // Create the List
            hListView = CreateWindowEx(
                0,
                WC_LISTVIEW,
                "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
                10, 10, 600, 550,
                hwnd, NULL, GetModuleHandle(NULL), NULL);

            SendMessage(hListView, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

            // Add columns
            LVCOLUMN lvColumn = {0};
            lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            lvColumn.pszText = "NAME";
            lvColumn.cx = 150;
            SendMessage(hListView, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

            lvColumn.pszText = "ID";
            lvColumn.cx = 150;
            SendMessage(hListView, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

            lvColumn.pszText = "PASS";
            lvColumn.cx = 150;
            SendMessage(hListView, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

             lvColumn.pszText = "ACCESS";
            lvColumn.cx = 150;
            SendMessage(hListView, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);

            // Add items
            LVITEM lvItem = {0};
            lvItem.mask = LVIF_TEXT;
            lvItem.iItem = 0;
            lvItem.iSubItem = 0;
            lvItem.pszText = "John";
            SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

            lvItem.iSubItem = 1;
            lvItem.pszText = "E0001";
            SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

            lvItem.iSubItem = 2;
            lvItem.pszText ="password1";
            SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

            lvItem.iSubItem = 3;
            lvItem.pszText ="GRANTED";
            SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

            lvItem.iItem = 1;
            lvItem.iSubItem = 0;
            lvItem.pszText = "Jane";
            SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

            lvItem.iSubItem = 1;
            lvItem.pszText = "E0002";
            SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

            lvItem.iSubItem = 2;
            lvItem.pszText = "password2";
            SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

            lvItem.iSubItem = 3;
            lvItem.pszText ="DENIED";
            SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

            // Create buttons
            atBtn = CreateWindowEx(
                0,
                "BUTTON",
                "ADD TAG",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                640, 10, 130, 30,
                hwnd, (HMENU)1001, GetModuleHandle(NULL), NULL);

             cnBtn = CreateWindowEx(
                0,
                "BUTTON",
                "CHANGE NAME",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                640, 90, 130, 30,
                hwnd, NULL, GetModuleHandle(NULL), NULL);

            cidBtn = CreateWindowEx(
                0,
                "BUTTON",
                "CHANGE ID",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                640, 130, 130, 30,
                hwnd, NULL, GetModuleHandle(NULL), NULL);

            cpBtn = CreateWindowEx(
                0,
                "BUTTON",
                "CHANGE PASS",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                640, 170, 130, 30,
                hwnd, NULL, GetModuleHandle(NULL), NULL);

            cacBtn = CreateWindowEx(
                0,
                "BUTTON",
                "CHANGE ACCESS",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                640, 210, 130, 30,
                hwnd, (HMENU)1002, GetModuleHandle(NULL), NULL);

            // Disable inactive buttons to be activated later -->
            EnableWindow(cnBtn, FALSE);
            EnableWindow(cidBtn, FALSE);
            EnableWindow(cpBtn, FALSE);
            EnableWindow(cacBtn, FALSE);

            break;
        }

        case WM_COMMAND:
        {
            if (HIWORD(wParam) == BN_CLICKED) {
                int buttonId = LOWORD(wParam);

                // Identify which button was clicked based on its ID
                switch (buttonId) {
                    // här bråkar det :)
                    case 1001:
                    {
                        EnableWindow(cacBtn, TRUE);
                        invalid(cacBtn);


                        

                        MessageBox(NULL, "button is pressed!", "Error", MB_ICONEXCLAMATION | MB_OK);
                        break;
                    }
                    case 1002:
                        MessageBox(NULL, "cacBtn is pressed!", "Error", MB_ICONEXCLAMATION | MB_OK);
                    default:
                        break;
                }
            }
            break;
        }

        // under progess -->
        case WM_NOTIFY: {
            NMLISTVIEW* pnmv = (NMLISTVIEW*)lParam;
            if (pnmv->hdr.code == LVN_ITEMCHANGED) {
                SetTimer(hwnd, TIMER_ENABLE_BUTTONS, BUTTON_ENABLE_DELAY, NULL);
            }
            break;
        }

        case WM_TIMER: {
            switch (wParam) {
                case TIMER_ENABLE_BUTTONS: {
                    // Check if there is any selection in the ListView
                    BOOL bItemSelected = ListView_GetSelectedCount(hListView) > 0;

                    // Enable or disable buttons based on ListView selection
                        EnableWindow(cnBtn, bItemSelected);
                        EnableWindow(cidBtn, bItemSelected);
                        EnableWindow(cpBtn, bItemSelected);
                        EnableWindow(cacBtn, bItemSelected);
                    break;
                }
            }
            break;
        }
        // <-- under progress
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/*
                        EnableWindow(cnBtn, bItemSelected);
                        EnableWindow(cidBtn, bItemSelected);
                        EnableWindow(cpBtn, bItemSelected);
                        EnableWindow(cacBtn, bItemSelected);
*/


int main() {
    // Register window
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "MyWindowClass";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        "MyWindowClass",
        "GATE KEEPER",
        WS_OVERLAPPED | WS_VISIBLE| WS_SYSMENU | WS_MINIMIZEBOX,
        GetSystemMetrics(SM_CXSCREEN)/2-400,
        GetSystemMetrics(SM_CYSCREEN)/2-300,
        800, 600,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}