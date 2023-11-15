#include <windows.h>
#include <commctrl.h>
#include <stdbool.h>

#define TIMER_ENABLE_BUTTONS 1
#define BUTTON_ENABLE_DELAY 100
#define IDC_EDIT_NAME       1006
#define IDC_COMBO_ROLE      1007
#define IDC_RADIO_TRUE      1008
#define IDC_RADIO_FALSE     1009
#define IDC_BUTTON_ADD      1010
#define IDC_BUTTON_CANCEL   1011

    HWND hwnd;
    HWND hListView;
    HWND atBtn;
    HWND cnBtn;
    HWND cidBtn;
    HWND cpBtn;
    HWND cacBtn;
    HWND atPopup;
    HWND atNameLabel;
    HWND atName;
    HWND atComboBox;
    HWND atRadioLabel;
    HWND atRadioTrue;
    HWND atRadioFalse;
    HWND atAddBtn;
    HWND atCancelbtn;

BOOL newAtPu = TRUE;

// Message handler for hwnd
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {
    
        // Handle pressed buttons
        case WM_COMMAND:
        {
            if (HIWORD(wParam) == BN_CLICKED) {
                int buttonId = LOWORD(wParam);

                switch (buttonId) {

                    case 1001:
                    {
                        if(newAtPu){
                            addTagPopup();
                            newAtPu = FALSE;
                            break;    
                        } else {
                            ShowWindow(atPopup, SW_SHOW);
                            break;
                        }
                        
                        
                    }
                    case 1002:
                    {
                        break;
                    }
                    case 1003:
                    {
                        break;
                    }
                    case 1004:
                    {
                        break;
                    }
                    case 1005:
                    {
                        break;
                    }
                    case IDC_BUTTON_CANCEL:
                    {
                        ShowWindow(atPopup, SW_HIDE);
                        SetWindowText(atName, "");
                        break;
                    }
                    default:
                        break;
                }
            }

            if (HIWORD(wParam) == EN_CHANGE) {
                
                int textLength = GetWindowTextLength(atName);
                if (textLength > 0) {
                    EnableWindow(atAddBtn, TRUE);
                }else {
                    EnableWindow(atAddBtn, FALSE);
                }
            }
            break;
        }

        // Handle list selection
        case WM_NOTIFY: 
        {
            NMLISTVIEW* pnmv = (NMLISTVIEW*)lParam;
            if (pnmv->hdr.code == LVN_ITEMCHANGING) {
                
                BOOL bItemSelected = ListView_GetSelectedCount(hListView) > 0;

                if (bItemSelected && (pnmv->uNewState & LVIS_SELECTED)) {
                    return TRUE;
                }
            }
            else if (pnmv->hdr.code == LVN_ITEMCHANGED) {
                SetTimer(hwnd, TIMER_ENABLE_BUTTONS, BUTTON_ENABLE_DELAY, NULL);
            }
            break;
        }


        case WM_TIMER: {
            switch (wParam) {
                case TIMER_ENABLE_BUTTONS: {

                    BOOL bItemSelected = ListView_GetSelectedCount(hListView) > 0;

                        EnableWindow(cnBtn, bItemSelected);
                        EnableWindow(cidBtn, bItemSelected);
                        EnableWindow(cpBtn, bItemSelected);
                        EnableWindow(cacBtn, bItemSelected);

                    break;
                }
            }
            break;
        }
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }    
            
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, 
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, 
                   int nCmdShow) {
    // Register window
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "gtClass";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create window
    hwnd = CreateWindowEx(
        0,
        "gtClass",
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

    createMainContent();

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

int createMainContent(){

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
        hwnd, (HMENU)1002, GetModuleHandle(NULL), NULL);

    cidBtn = CreateWindowEx(
        0,
        "BUTTON",
        "CHANGE ID",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        640, 130, 130, 30,
        hwnd, (HMENU)1003, GetModuleHandle(NULL), NULL);

    cpBtn = CreateWindowEx(
        0,
        "BUTTON",
        "CHANGE PASS",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        640, 170, 130, 30,
        hwnd, (HMENU)1004, GetModuleHandle(NULL), NULL);

    cacBtn = CreateWindowEx(
        0,
        "BUTTON",
        "CHANGE ACCESS",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        640, 210, 130, 30,
        hwnd, (HMENU)1005, GetModuleHandle(NULL), NULL);

    // Disable inactive buttons to be activated later -->
    EnableWindow(cnBtn, FALSE);
    EnableWindow(cidBtn, FALSE);
    EnableWindow(cpBtn, FALSE);
    EnableWindow(cacBtn, FALSE);

    return 0;
}

int geHwndPos(int i){

    RECT hwndRect;
    GetWindowRect(hwnd, &hwndRect);

    int parentX = hwndRect.left;
    int parentY = hwndRect.top;

    if(i == 1){
        return parentX;
    } else if (i == 2){
        return parentY;
    }else 
    
    return 0;
}

int addTagPopup(){



        WNDCLASS wcPopup = {0};
        wcPopup.lpfnWndProc = WndProc;
        wcPopup.hInstance = GetModuleHandle(NULL);
        wcPopup.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
        wcPopup.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wcPopup.lpszClassName = "popupClass";

        if (!RegisterClass(&wcPopup)) {
            MessageBox(NULL, "AT Popup Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
            
        }
    
    atPopup = CreateWindowEx(
        0,
        "popupClass",
        "",
        WS_POPUP | WS_VISIBLE,
        getHwndPos(1) + 355,
        getHwndPos(2) + 30,
        260, 
        200,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    
    if (atPopup == NULL) {
        MessageBox(NULL, "AT PopUp Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        
    }

    atNameLabel = CreateWindow(
        "STATIC", 
        " Name:", 
        WS_VISIBLE | WS_CHILD,
        10, 
        10, 
        50, 
        20, 
        atPopup, NULL, NULL, NULL);

    atName = CreateWindowEx(
        0,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 
        70, 10, 180, 20, 
        atPopup, (HMENU)IDC_EDIT_NAME, GetModuleHandle(NULL), NULL);
            
    atComboBox = CreateWindowEx(0, 
        "COMBOBOX", 
        "", 
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
        10, 40, 180, 200, 
        atPopup, (HMENU)IDC_COMBO_ROLE, GetModuleHandle(NULL), NULL);

        SendMessage(atComboBox, CB_ADDSTRING, 0, (LPARAM)"Employee");
        SendMessage(atComboBox, CB_ADDSTRING, 0, (LPARAM)"Visitor");
        SendMessage(atComboBox, CB_ADDSTRING, 0, (LPARAM)"Guest");
        SendMessage(atComboBox, CB_SETCURSEL, 0, 0);

    atRadioLabel = CreateWindow(
        "STATIC", 
        " Access:", 
        WS_VISIBLE | WS_CHILD,
        10, 
        80, 
        70, 
        20, 
        atPopup, NULL, NULL, NULL);
    
    atRadioTrue = CreateWindowEx(
        0, 
        "BUTTON", 
        "True", 
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
        90, 80, 60, 20, 
        atPopup, (HMENU)IDC_RADIO_TRUE, GetModuleHandle(NULL), NULL);

    atRadioFalse = CreateWindowEx(0, 
        "BUTTON", 
        "False", 
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
        170, 80, 60, 20, 
        atPopup, (HMENU)IDC_RADIO_FALSE, GetModuleHandle(NULL), NULL);

        SendMessage(atRadioTrue, BM_SETCHECK, BST_CHECKED, 0);

    atAddBtn = CreateWindowEx(
        0, 
        "BUTTON", 
        "ADD", 
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        10, 120, 80, 30, 
        atPopup, (HMENU)IDC_BUTTON_ADD, GetModuleHandle(NULL), NULL);

    EnableWindow(atAddBtn, FALSE);

    atCancelbtn = CreateWindowEx(
        0, 
        "BUTTON", 
        "CANCEL", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
        100, 120, 80, 30, 
        atPopup, (HMENU)IDC_BUTTON_CANCEL, GetModuleHandle(NULL), NULL);
    
    return 0;
}

int changeNamePopup(){

}

int changeIdPopup(){

}

int changeAccessPopup(){

}