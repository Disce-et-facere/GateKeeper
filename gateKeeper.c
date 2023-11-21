#include "gateKeeper.h"

    // Main Window (hwnd) -->
    HWND hwnd;
    HWND hListView;
    HWND atBtn;
    HWND odBtn;
    HWND cnBtn;
    HWND cidBtn;
    HWND cacBtn;
    HWND rtBtn;

    // Add Tag Popup
    HWND atPopup;
    HWND atNameLabel;
    HWND atName;
    HWND atComboBox;
    HWND atRadioLabel;
    HWND atRadioTrue;
    HWND atRadioFalse;
    HWND atAddBtn;
    HWND atCancelbtn;

    // Change Name Popup
    HWND cnPopup;
    HWND cnCurrentNameLabel;
    HWND cnNewNameLabel;
    HWND cnName;
    HWND cnAddBtn;
    HWND cnCancelbtn;

    // Change ID Popup
    HWND cidPopup;
    HWND cidCurrentIdLabel;
    HWND cidNewIdLabel;
    HWND cidComboBox;
    HWND cidAddBtn;
    HWND cidCancelbtn;

    // Change Access Popup
    HWND caPopup;
    HWND caCurrentAccessLabel;
    HWND caNewAccessLabel;
    HWND caRadioTrue;
    HWND caRadioFalse;
    HWND caAddBtn;
    HWND caCancelbtn;

    // Remove Tag Popup
    HWND rtPopup;
    HWND rtSelectedNameLabel;
    HWND rtSelectedIdLabel;
    HWND rtSelectedAccessLabel;
    HWND rtRemoveBtn;
    HWND rtCancelbtn;

    // globalize list Items
    LVITEM lvItem;
  
BOOL popupRegd = FALSE; // maybe not needed as global?
int currentListItems = 0;

// TODO'S: Before user press the add button in the add tag popup, ask to scan tag before adding -> add after arduino setup
//         Create function to add new tags/cards in the list. and remove the current dummies.
//         Implement safeInput into userinput in add tag popup and change name popup.
//         Add grab function from userInputs

// Message handler for hwnd (Main Window), well all windows actually
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
                        if(atPopup == NULL){
                            addPopup(1);
                            //disableAllButtons();
                            break;    
                        } else {
                            ShowWindow(atPopup, SW_SHOW);
                            //disableAllButtons();
                            break;
                        }
        
                    }

                    case 1002: // todo: fix enable disable buttons collision with enable on select in list!
                    {
                        if(cnPopup == NULL){
                            addPopup(2);
                            //disableAllButtons();
                            break;   
                        }else{
                            ShowWindow(cnPopup, SW_SHOW);
                            //disableAllButtons();
                            break;
                        }
                        
                        break;
                    }
                    case 1003:
                    {   
                        if(cidPopup == NULL){
                            addPopup(3);
                            //disableAllButtons();
                            break;   
                        }else{
                            ShowWindow(cidPopup, SW_SHOW);
                            //disableAllButtons();
                            break;
                        }
                        break;
                    }
                    case 1004:
                    {
                        if(caPopup == NULL){
                            addPopup(4);
                            //disableAllButtons();
                            break;   
                        }else{
                            ShowWindow(caPopup, SW_SHOW);
                            //disableAllButtons();
                            break;
                        }
                        break;
                    }
                    case 1005:
                    {
                        if(rtPopup == NULL){
                            addPopup(5);
                            //disableAllButtons();
                            break;   
                        }else{
                            ShowWindow(rtPopup, SW_SHOW);
                            //disableAllButtons();
                            break;
                        }
                        break;
                    }
                    case 1012:
                    {
                        fileReader();
                        //onExit();
                        break;
                    }
                    case AT_BUTTON_ADD:
                    {
                        userALI();
                        ShowWindow(atPopup, SW_HIDE);
                        SetWindowText(atName, "");
                        break;
                    }
                    case AT_BUTTON_CANCEL:
                    {
                        ShowWindow(atPopup, SW_HIDE);
                        SetWindowText(atName, "");
                        //enableAllButtons();
                        break;
                    }
                    case CN_BUTTON_CANCEL:
                    {
                        ShowWindow(cnPopup, SW_HIDE);
                        SetWindowText(cnName, "");
                        //enableAllButtons();
                        break;
                    }
                    case CID_BUTTON_CANCEL:
                    {
                        ShowWindow(cidPopup, SW_HIDE);
                        // add current id in combobox
                        //enableAllButtons();
                        break;
                    }
                    case CA_BUTTON_CANCEL:
                    {
                        ShowWindow(caPopup, SW_HIDE);
                        //enableAllButtons();
                        break;
                    }
                    case RT_BUTTON_CANCEL:
                    {
                        ShowWindow(rtPopup, SW_HIDE);
                        //enableAllButtons();
                        break;
                    }
                    default:
                        break;
                }
            }

            if (HIWORD(wParam) == EN_CHANGE) {
                
                int textLengthAt = GetWindowTextLength(atName);
                if (textLengthAt > 0) {
                    EnableWindow(atAddBtn, TRUE);
                }else {
                    EnableWindow(atAddBtn, FALSE);
                }
                int textLengthCn = GetWindowTextLength(cnName);
                if (textLengthCn > 0) {
                    EnableWindow(cnAddBtn, TRUE);
                }else {
                    EnableWindow(cnAddBtn, FALSE);
                }
            }
            break;
        }

        // Handle list selection
        case WM_NOTIFY: // remove timer here if possible? --->
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

                        // fix disable rest of buttons when one button is pressed. shit ton of if statements?, same as line 114

                        EnableWindow(cnBtn, bItemSelected);
                        EnableWindow(cidBtn, bItemSelected);
                        EnableWindow(cacBtn, bItemSelected);
                        EnableWindow(rtBtn, bItemSelected);

                    break;
                }
            }
            break;
        }

        case WM_MOVE:
        {   // auto reposition for popups --->
            // no else if because the if statements checks if popup window is created or not.... fix/change later if needed
            // change to IsWindowActive function() ? then an else if statement would work, swenglish
            if(atPopup != NULL){

                SetWindowPos(atPopup, NULL, getHwndPos(1) + 354, getHwndPos(2) + 36, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                
            }
            
            if(cnPopup != NULL){

                SetWindowPos(cnPopup, NULL, getHwndPos(1) + 354, getHwndPos(2) + 110, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                
            }
            
            if(cidPopup != NULL){

                SetWindowPos(cidPopup, NULL, getHwndPos(1) + 354, getHwndPos(2) + 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                
            }
            
            if(caPopup != NULL){

                SetWindowPos(caPopup, NULL, getHwndPos(1) + 354, getHwndPos(2) + 190, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                
            }
            
            if(rtPopup != NULL){

                SetWindowPos(rtPopup, NULL, getHwndPos(1) + 354, getHwndPos(2) + 230, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                
            }
            break;
        }
        
        case WM_DESTROY:
        {
            //arrayHandler(tag, 9, -1);
            PostQuitMessage(0);
            break;
        }    
            
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// hmm either int main or WINAPI... WINAPI gives unused instances warnings.
int WINAPI WinMain(HINSTANCE hInstance, 
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, 
                   int nCmdShow) {

    // Register Main window
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

    // Create Main window
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

// Creates the Content in the Main Window (hwnd)
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

    // Add items - TODO: Create function for adding items dynamicly
    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = currentListItems;
    // removes dummies from here -->
    lvItem.iSubItem = 0;
    lvItem.pszText = "John";
    SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 1;
    lvItem.pszText = "E0001";
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 2;
    lvItem.pszText ="*******";
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 3;
    lvItem.pszText ="GRANTED";
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListItems++;

    lvItem.iItem = currentListItems;
    lvItem.iSubItem = 0;
    lvItem.pszText = "Jane";
    SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 1;
    lvItem.pszText = "E0002";
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 2;
    lvItem.pszText = "*******";
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 3;
    lvItem.pszText ="DENIED";
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListItems++; // added until dummies are removed

    // Create buttons
    atBtn = CreateWindowEx(
        0,
        "BUTTON",
        "ADD TAG",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        640, 10, 130, 30,
        hwnd, (HMENU)1001, GetModuleHandle(NULL), NULL);

    odBtn = CreateWindowEx(
        0,
        "BUTTON",
        "OPEN DOOR",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        640, 50, 130, 30,
        hwnd, (HMENU)1012, GetModuleHandle(NULL), NULL);

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

    cacBtn = CreateWindowEx(
        0,
        "BUTTON",
        "CHANGE ACCESS",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        640, 170, 130, 30,
        hwnd, (HMENU)1004, GetModuleHandle(NULL), NULL);

    rtBtn = CreateWindowEx(
        0,
        "BUTTON",
        "REMOVE TAG",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        640, 210, 130, 30,
        hwnd, (HMENU)1005, GetModuleHandle(NULL), NULL);

    // Disables "inactive" buttons to be activated later -->
    EnableWindow(cnBtn, FALSE);
    EnableWindow(cidBtn, FALSE);
    EnableWindow(cacBtn, FALSE);
    EnableWindow(rtBtn, FALSE);

    return 0;
}

// Creates Popup Windows depending on i value 1 - 5
void addPopup(int i){

// change from BOOL to int popupRegd = 0 -> if(popupRegd == 0) -> popupRegd++;
    // Popup class registration
    if(!popupRegd){
    
    WNDCLASS wcPopup = {0};
    wcPopup.lpfnWndProc = WndProc;
    wcPopup.hInstance = GetModuleHandle(NULL);
    wcPopup.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcPopup.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcPopup.lpszClassName = "popupClass";

    if (!RegisterClass(&wcPopup)) {
        MessageBox(NULL, "AT Popup Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
    }
    popupRegd = TRUE;
    }

    // Add Tag Popup --->
    if(i == 1){
    
    atPopup = CreateWindowEx(
        0,
        "popupClass",
        "",
        WS_POPUP | WS_VISIBLE,
        getHwndPos(1) + 354,
        getHwndPos(2) + 36,
        260, 
        160,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    
    if (atPopup == NULL) {
        MessageBox(NULL, "Add Tag PopUp Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        
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
        atPopup, (HMENU)IDC_EDIT_NAME_AT, GetModuleHandle(NULL), NULL);
            
    atComboBox = CreateWindowEx(0, 
        "COMBOBOX", 
        "", 
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
        10, 40, 180, 200, 
        atPopup, (HMENU)IDC_COMBO_ROLE, GetModuleHandle(NULL), NULL);

        SendMessage(atComboBox, CB_ADDSTRING, 0, (LPARAM)"Employee");
        SendMessage(atComboBox, CB_ADDSTRING, 0, (LPARAM)"Customer");
        SendMessage(atComboBox, CB_ADDSTRING, 0, (LPARAM)"Visitor");
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
        "YES", 
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
        90, 80, 60, 20, 
        atPopup, (HMENU)IDC_RADIO_TRUE, GetModuleHandle(NULL), NULL);

    atRadioFalse = CreateWindowEx(0, 
        "BUTTON", 
        "NO", 
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
        160, 80, 50, 20, 
        atPopup, (HMENU)IDC_RADIO_FALSE, GetModuleHandle(NULL), NULL);

        SendMessage(atRadioTrue, BM_SETCHECK, BST_CHECKED, 0);

    atAddBtn = CreateWindowEx(
        0, 
        "BUTTON", 
        "ADD", 
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        10, 120, 80, 30, 
        atPopup, (HMENU)AT_BUTTON_ADD, GetModuleHandle(NULL), NULL);

    EnableWindow(atAddBtn, FALSE);

    atCancelbtn = CreateWindowEx(
        0, 
        "BUTTON", 
        "CANCEL", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
        100, 120, 80, 30, 
        atPopup, (HMENU)AT_BUTTON_CANCEL, GetModuleHandle(NULL), NULL);

    }// <--- Add Tag Popup
    

    // Change Name Popup --->
        else if(i == 2){
        
        cnPopup = CreateWindowEx(
        0,
        "popupClass",
        "",
        WS_POPUP | WS_VISIBLE,
        getHwndPos(1) + 354,
        getHwndPos(2) + 110,
        260, 
        160,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    
        if (cnPopup == NULL) {
            MessageBox(NULL, "Name PopUp Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
            
        }

        cnCurrentNameLabel = CreateWindow(
            "STATIC", 
            " Current Name:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            10, 
            250, 
            20, 
            cnPopup, NULL, NULL, NULL);


        cnNewNameLabel = CreateWindow(
            "STATIC", 
            " New Name:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            60, 
            90, 
            20, 
            cnPopup, NULL, NULL, NULL);

        cnName = CreateWindowEx(
            0,
            "EDIT",
            "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 
            90, 60, 170, 20, 
            cnPopup, (HMENU)CN_EDIT_NAME, GetModuleHandle(NULL), NULL);

        cnAddBtn = CreateWindowEx(
        0, 
        "BUTTON", 
        "SAVE", 
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        10, 120, 80, 30, 
        cnPopup, (HMENU)CN_BUTTON_SAVE, GetModuleHandle(NULL), NULL);

        EnableWindow(cnAddBtn, FALSE);

        cnCancelbtn = CreateWindowEx(
            0, 
            "BUTTON", 
            "CANCEL", 
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
            100, 120, 80, 30, 
            cnPopup, (HMENU)CN_BUTTON_CANCEL, GetModuleHandle(NULL), NULL);
        

    }// <--- Change Name Popup
    
    // Change ID Popup --->
    else if(i == 3){

        cidPopup = CreateWindowEx(
        0,
        "popupClass",
        "",
        WS_POPUP | WS_VISIBLE,
        getHwndPos(1) + 354,
        getHwndPos(2) + 150,
        260, 
        160,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    
        if (cidPopup == NULL) {
            MessageBox(NULL, "ID PopUp Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
            
        }

        cidCurrentIdLabel = CreateWindow(
            "STATIC", 
            " Current ID:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            10, 
            250, 
            20, 
            cidPopup, NULL, NULL, NULL);


        cidNewIdLabel = CreateWindow(
            "STATIC", 
            " New ID:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            62, 
            60, 
            20, 
            cidPopup, NULL, NULL, NULL);

        cidComboBox = CreateWindowEx(0, 
            "COMBOBOX", 
            "", 
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
            70, 60, 190, 150, 
            cidPopup, (HMENU)CID_COMBO_ROLE, GetModuleHandle(NULL), NULL);

            SendMessage(cidComboBox, CB_ADDSTRING, 0, (LPARAM)"Employee");
            SendMessage(cidComboBox, CB_ADDSTRING, 0, (LPARAM)"Customer");
            SendMessage(cidComboBox, CB_ADDSTRING, 0, (LPARAM)"Visitor");
            SendMessage(cidComboBox, CB_SETCURSEL, 0, 0);

        cidAddBtn = CreateWindowEx(
            0, 
            "BUTTON", 
            "SAVE", 
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
            10, 120, 80, 30, 
            cidPopup, (HMENU)CN_BUTTON_SAVE, GetModuleHandle(NULL), NULL);

        EnableWindow(cnAddBtn, FALSE);

        cidCancelbtn = CreateWindowEx(
            0, 
            "BUTTON", 
            "CANCEL", 
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
            100, 120, 80, 30, 
            cidPopup, (HMENU)CID_BUTTON_CANCEL, GetModuleHandle(NULL), NULL);

    }// <--- Change ID Popup

    // Change Access Popup --->
    else if(i == 4){

        caPopup = CreateWindowEx(
        0,
        "popupClass",
        "",
        WS_POPUP | WS_VISIBLE,
        getHwndPos(1) + 354,
        getHwndPos(2) + 190,
        260, 
        160,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    
        if (caPopup == NULL) {
            MessageBox(NULL, "Access PopUp Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
            
        }

        caCurrentAccessLabel = CreateWindow(
            "STATIC", 
            " Current Access:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            10, 
            250, 
            20, 
            caPopup, NULL, NULL, NULL);

        caNewAccessLabel = CreateWindow(
            "STATIC", 
            " Access:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            60, 
            60, 
            20, 
            caPopup, NULL, NULL, NULL);

        caRadioTrue = CreateWindowEx(
            0, 
            "BUTTON", 
            "YES", 
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
            90, 60, 60, 20, 
            caPopup, (HMENU)CA_RADIO_TRUE, GetModuleHandle(NULL), NULL);

        caRadioFalse = CreateWindowEx(0, 
            "BUTTON", 
            "NO", 
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
            160, 60, 50, 20, 
            caPopup, (HMENU)CA_RADIO_FALSE, GetModuleHandle(NULL), NULL);

            SendMessage(atRadioTrue, BM_SETCHECK, BST_CHECKED, 0);

        caAddBtn = CreateWindowEx(
            0, 
            "BUTTON", 
            "SAVE", 
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
            10, 120, 80, 30, 
            caPopup, (HMENU)CA_BUTTON_SAVE, GetModuleHandle(NULL), NULL);

            EnableWindow(cnAddBtn, FALSE);

        caCancelbtn = CreateWindowEx(
            0, 
            "BUTTON", 
            "CANCEL", 
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
            100, 120, 80, 30, 
            caPopup, (HMENU)CA_BUTTON_CANCEL, GetModuleHandle(NULL), NULL);

    }// <--- Change Access Popup

    //Remove Tag Popup --->
    else if(i == 5){

        rtPopup = CreateWindowEx(
        0,
        "popupClass",
        "",
        WS_POPUP | WS_VISIBLE,
        getHwndPos(1) + 354,
        getHwndPos(2) + 230,
        260, 
        160,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    
        if (rtPopup == NULL) {
            MessageBox(NULL, "Remove PopUp Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
            
        }

        rtSelectedNameLabel = CreateWindow(
            "STATIC", 
            " Selected Name:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            10, 
            250, 
            20, 
            rtPopup, NULL, NULL, NULL);
        
        rtSelectedIdLabel = CreateWindow(
            "STATIC", 
            " Selected ID:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            40, 
            250, 
            20, 
            rtPopup, NULL, NULL, NULL);

        rtSelectedAccessLabel = CreateWindow(
            "STATIC", 
            " Selected Access:", 
            WS_VISIBLE | WS_CHILD,
            10, 
            70, 
            250, 
            20, 
            rtPopup, NULL, NULL, NULL);

        rtRemoveBtn = CreateWindowEx(
            0, 
            "BUTTON", 
            "REMOVE", 
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
            10, 120, 80, 30, 
            rtPopup, (HMENU)RT_BUTTON_REMOVE, GetModuleHandle(NULL), NULL);

            EnableWindow(cnAddBtn, FALSE);

        rtCancelbtn = CreateWindowEx(
            0, 
            "BUTTON", 
            "CANCEL", 
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
            100, 120, 80, 30, 
            rtPopup, (HMENU)RT_BUTTON_CANCEL, GetModuleHandle(NULL), NULL);

    }// <--- Remove Tag Popup
}

// extra functions --->

// Gets Parent Window position (Main Window/hwnd)
int getHwndPos(int i){

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

// disables all button ---> temp function, not used
void disableAllButtons(){
    EnableWindow(cnBtn, FALSE);
    EnableWindow(cidBtn, FALSE);
    EnableWindow(rtBtn, FALSE);
    EnableWindow(cacBtn, FALSE);
    EnableWindow(atBtn, FALSE);
    EnableWindow(odBtn, FALSE);
}

// enables all button ---> temp function, not used
void enableAllButtons(){
    EnableWindow(cnBtn, TRUE);
    EnableWindow(cidBtn, TRUE);
    EnableWindow(rtBtn, TRUE);
    EnableWindow(cacBtn, TRUE);
    EnableWindow(atBtn, TRUE);
    EnableWindow(odBtn, TRUE);
}

void userALI(){ // user add list item <---
    
    // fetch name from add popup
    char listName[256];
    // add check if atName windowtext <= 255
    GetWindowText(atName, listName, sizeof(listName)-1);
    listName[sizeof(listName) - 1] = '\0'; 

    //fetch choosen value in combobox from add popup
    int comboIndex = (int)SendMessage(atComboBox, CB_GETCURSEL, 0,0);
    char idsBuffer[10];
    SendMessage(atComboBox, CB_GETLBTEXT, (WPARAM)comboIndex, (LPARAM)idsBuffer);
    char listIdS[2];
    listIdS[0] = (char)toupper(idsBuffer[0]);
    listIdS[1] = '\0';

    // checks for available ID number
    int listIdD = asignIdD();   

    // create new pass and check if it exists in the array
    char listPass[17];
    char *tempPass;
    BOOL passExist = 1;
    
    do{

        tempPass = randomPass();
        passExist = checkPass(tempPass);

    }while(passExist); // if passexist is 1  continue loop and exit loop when passExist is 0

    strcpy(listPass, tempPass);

    free(tempPass);

    //checks which true/false radio button is selected
    char sListAccess[8];
    int listAccess;

    BOOL isTrueChecked = SendMessage(atRadioTrue, BM_GETCHECK, 0,0) == BST_CHECKED;
    BOOL isFalseChecked = SendMessage(atRadioFalse, BM_GETCHECK, 0, 0) == BST_CHECKED;

    if(isTrueChecked){
        
        strcpy(sListAccess, "GRANTED");
        listAccess = 0;

    }
    else if(isFalseChecked){
        
        strcpy(sListAccess, "DENIED");
        listAccess = 1;
    }
    
    // adds a new list item with gathered values
    int currentListSubItem = 0;

    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;

    lvItem.iItem = currentListItems;
    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = listName;
    SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    // converts int to char and adds the chars together
    char idSidD[256];
    sprintf(idSidD, "%s%d", listIdS, listIdD);
    idSidD[sizeof(idSidD)-1] = '\0';

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = idSidD;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = listPass;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = sListAccess;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem = 0;

    currentListItems++;


    
    // put value in a tag and send to newTag();
    TAG tag;

    strncpy(tag.name, listName, sizeof(tag.name) - 1);
    tag.name[sizeof(tag.name) - 1] = '\0'; 

    strncpy(tag.idS, listIdS, sizeof(tag.idS) - 1);
    tag.idS[sizeof(tag.idS) - 1] = '\0'; 

    tag.idD = listIdD;

    strncpy(tag.pass, listPass, sizeof(tag.pass) - 1);
    tag.pass[sizeof(tag.pass) - 1] = '\0';

    tag.access = listAccess;

    int direction = 0;

    newTag(&tag, &direction);
    
}

void fileALI(TAG *tag){ // adds list item from file

    int currentListSubItem = 0;

    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;

    lvItem.iItem = currentListItems;
    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = tag->name;
    SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    // converts int to char and adds the chars together
    char idSidD[256];
    sprintf(idSidD, "%s%d", tag->idS, tag->idD);
    idSidD[sizeof(idSidD)-1] = '\0';

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = idSidD;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = tag->pass;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    char sListAccess[8];
        if(tag->access == 0){
        
            strcpy(sListAccess, "GRANTED");
            sListAccess[7] = '\0';
        }
        else if(tag->access == 1){
            
            strcpy(sListAccess, "DENIED");
            sListAccess[7] = '\0';
        }

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = sListAccess;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem = 0;

    currentListItems++;

}