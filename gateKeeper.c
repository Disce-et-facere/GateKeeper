#include "gateKeeper.h"

// TODO'S: 1. Before user press the add button in the add tag popup, ask to scan tag before adding -> add after arduino setup
//         2. Implement safeInput into userinput in add tag popup and change name popup.
//         3. Add onHover popup with timestamp info -> created and changed


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

    // listitem Info Popup
    HWND iliPopup;
    HWND iliCreated;
    HWND iliChanged;

    // Scan tag Popup
    HWND stPopup;
    HWND stMessageLabel;
    HWND stTagScannedLabel;
    HWND stTagReggedLabel;

    // globalize list Items
    LVITEM lvItem;
  
BOOL popupRegd = FALSE; // maybe not needed as global?
int currentListItems = 0;

// Message handler for hwnd (Main Window), well all windows actually
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    static HBRUSH hbrush = NULL;  

    static int selectedItemIndex = -1;

    switch (msg) {
        
        case WM_CTLCOLORSTATIC: 
        {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(255, 255, 255));
            SetBkColor(hdcStatic, RGB(100, 100, 100));
            if (!hbrush)
                hbrush = CreateSolidBrush(RGB(100, 100, 100));
            return (LRESULT)hbrush;
        }
        case WM_COMMAND: // Handle pressed buttons
        {
            if (HIWORD(wParam) == BN_CLICKED) {
                int buttonId = LOWORD(wParam);

                switch (buttonId) {

                    case 1001:
                    {
                        if(atPopup == NULL){
                            addPopup(1);
                            onPopupAtaTime(1);
                            break;    
                        } else {
                            ShowWindow(atPopup, SW_SHOW);
                            onPopupAtaTime(1);
                            break;
                        }
        
                    }

                    case 1002:
                    {
                        char selectedName1[256];
                        strcpy(selectedName1, "Selected Name: ");
                        char selectedName2[256];
                        ListView_GetItemText(hListView, selectedItemIndex, 0, selectedName2, sizeof(selectedName2));
                        strcat(selectedName1, selectedName2);

                        if(cnPopup == NULL){
                            addPopup(2);
                            onPopupAtaTime(2);
                            SetWindowText(cnCurrentNameLabel, selectedName1);
                            break;   
                        }else{
                            ShowWindow(cnPopup, SW_SHOW);
                            onPopupAtaTime(2);
                            SetWindowText(cnCurrentNameLabel, selectedName1);
                            break;
                        }
                        
                        break;
                    }
                    case 1003:
                    {   
                        char selectedId1[256];
                        strcpy(selectedId1, "Selected ID: ");
                        char selectedId2[256];
                        ListView_GetItemText(hListView, selectedItemIndex, 1, selectedId2, sizeof(selectedId2));
                        strcat(selectedId1, selectedId2);

                        if(cidPopup == NULL){
                            addPopup(3);
                            onPopupAtaTime(3);
                            SetWindowText(cidCurrentIdLabel, selectedId1);
                            break;   
                        }else{
                            ShowWindow(cidPopup, SW_SHOW);
                            onPopupAtaTime(3);
                            SetWindowText(cidCurrentIdLabel, selectedId1);
                            break;
                        }
                        break;
                    }
                    case 1004: // use macro names for better reading ---> TODO
                    {
                        char selectedAccess1[256];
                        strcpy(selectedAccess1, "Selected Access: ");
                        char selectedAccess2[256];
                        ListView_GetItemText(hListView, selectedItemIndex, 3, selectedAccess2, sizeof(selectedAccess2));
                        strcat(selectedAccess1, selectedAccess2);

                        if(selectedAccess2[0] == 'G'){
                        
                            SendMessage(caRadioTrue, BM_SETCHECK, BST_CHECKED, 0);
                            
                        }else{

                            SendMessage(caRadioFalse, BM_SETCHECK, BST_CHECKED, 0);

                        }

                        if(caPopup == NULL){

                            addPopup(4);

                            if(selectedAccess2[0] == 'G'){
                        
                            SendMessage(caRadioTrue, BM_SETCHECK, BST_CHECKED, 0);
                            
                            }else{

                                SendMessage(caRadioFalse, BM_SETCHECK, BST_CHECKED, 0);

                            }

                            onPopupAtaTime(4);

                            SetWindowText(caCurrentAccessLabel, selectedAccess1);

                            break;   

                        }else{


                            ShowWindow(caPopup, SW_SHOW);

                            if(selectedAccess2[0] == 'G'){
                        
                                SendMessage(caRadioTrue, BM_SETCHECK, BST_CHECKED, 0);
                                SendMessage(caRadioFalse, BM_SETCHECK, BST_UNCHECKED, 0);
                            
                            }else{

                                SendMessage(caRadioFalse, BM_SETCHECK, BST_CHECKED, 0);
                                SendMessage(caRadioTrue, BM_SETCHECK, BST_UNCHECKED, 0);

                            }

                            onPopupAtaTime(4);

                            SetWindowText(caCurrentAccessLabel, selectedAccess1);

                            break;
                        }
                        break;
                    }
                    case 1005:
                    {

                        char selectedName11[256];
                        strcpy(selectedName11, "Selected Name: ");
                        char selectedName22[256];
                        ListView_GetItemText(hListView, selectedItemIndex, 0, selectedName22, sizeof(selectedName22));
                        strcat(selectedName11, selectedName22);

                        char selectedId11[256];
                        strcpy(selectedId11, "Selected ID: ");
                        char selectedId22[256];
                        ListView_GetItemText(hListView, selectedItemIndex, 1, selectedId22, sizeof(selectedId22));
                        strcat(selectedId11, selectedId22);
                    

                        char selectedAccess11[256];
                        strcpy(selectedAccess11, "Selected Access: ");
                        char selectedAccess22[256];
                        ListView_GetItemText(hListView, selectedItemIndex, 3, selectedAccess22, sizeof(selectedAccess22));
                        strcat(selectedAccess11, selectedAccess22);

                        if(rtPopup == NULL){
                            addPopup(5);
                            onPopupAtaTime(5);
                            SetWindowText(rtSelectedNameLabel, selectedName11);
                            SetWindowText(rtSelectedIdLabel, selectedId11);
                            SetWindowText(rtSelectedAccessLabel, selectedAccess11);
                            break;   
                        }else{
                            ShowWindow(rtPopup, SW_SHOW);
                            onPopupAtaTime(5);
                            SetWindowText(rtSelectedNameLabel, selectedName11);
                            SetWindowText(rtSelectedIdLabel, selectedId11);
                            SetWindowText(rtSelectedAccessLabel, selectedAccess11);
                            break;
                        }
                        break;
                    }
                    case 1012:
                    {
                        addPopup(6);
                        //fileReader();
                        //onExit();
                        break;
                    }
                    case AT_BUTTON_ADD:
                    {
                        char inputText[255];

                        GetWindowText(atName, inputText, sizeof(inputText)-1);
                        inputText[sizeof(inputText) - 1] = '\0'; 
                        
                        if(!checkInput(inputText)){ // checks if inputtext contains any non letters with exception for whitespace
                            MessageBox(hwnd, "You cant type that, it can only contain letters!", "Non alphabetical Character!", MB_ICONEXCLAMATION | MB_OK);

                            break; 
                        }else if(GetWindowTextLength(atName) > 254){ 
                            MessageBox(hwnd, "Jesus that's a long name!\n Its need to be shorter, max 254 characters. ", "Text is to Long!", MB_ICONEXCLAMATION | MB_OK);
                            break; 
                        }else{
                            userALI();
                            ShowWindow(atPopup, SW_HIDE);
                            SetWindowText(atName, "");
                            break;  
                        }
                        
                    }
                    case AT_BUTTON_CANCEL:
                    {
                        ShowWindow(atPopup, SW_HIDE);
                        SetWindowText(atName, "");
                        break;
                    }
                    case CN_BUTTON_SAVE:
                    {
                        char inputText[255];

                        GetWindowText(cnName, inputText, sizeof(inputText)-1);
                        inputText[sizeof(inputText) - 1] = '\0'; 
                        
                        if(!checkInput(inputText)){ // checks if inputtext contains any non letters with exception for whitespace
                            MessageBox(hwnd, "You cant type that, it can only contain letters!", "Non alphabetical Character!", MB_ICONEXCLAMATION | MB_OK);

                            break; 
                        }else if(GetWindowTextLength(atName) > 254){ 
                            MessageBox(hwnd, "Jesus that's a long name!\n Its need to be shorter, max 254 characters. ", "Text is to Long!", MB_ICONEXCLAMATION | MB_OK);
                            break; 
                        }else{
                            changedTag(selectedItemIndex, 0, inputText);
                            ShowWindow(cnPopup, SW_HIDE);
                            SetWindowText(cnName, "");
                            break;  
                        }
                    }
                    case CN_BUTTON_CANCEL:
                    {
                        ShowWindow(cnPopup, SW_HIDE);
                        SetWindowText(cnName, "");
                        break;
                    }
                    case CID_BUTTON_SAVE:
                    {
                        int comboIndex = (int)SendMessage(cidComboBox, CB_GETCURSEL, 0,0);
                        char idsBuffer[10];
                        SendMessage(cidComboBox, CB_GETLBTEXT, (WPARAM)comboIndex, (LPARAM)idsBuffer);
                        char comboIdS[255];
                        comboIdS[0] = (char)toupper(idsBuffer[0]);
                        comboIdS[1] = '\0';
                        changedTag(selectedItemIndex, 1, comboIdS);
                        ShowWindow(cidPopup, SW_HIDE);
                        break;
                    }
                    case CID_BUTTON_CANCEL:
                    {
                        ShowWindow(cidPopup, SW_HIDE);
                        break;
                    }
                    case CA_BUTTON_SAVE:
                    {
                        char accessChoice[255];
                        
                        LRESULT result1 = SendMessage(caRadioTrue, BM_GETCHECK, 0,0);
                        BOOL isTrueChecked = (result1 == BST_CHECKED);
                        LRESULT result2 = SendMessage(caRadioFalse, BM_GETCHECK, 0, 0);
                        BOOL isFalseChecked = (result2 == BST_CHECKED);

                        if(isTrueChecked){
                                 
                            strcpy(accessChoice, "GRANTED");
                            
                        }
                        else if(isFalseChecked){
                    
                            strcpy(accessChoice, "DENIED");

                        } 
                        changedTag(selectedItemIndex, 3, accessChoice);
                        ShowWindow(caPopup, SW_HIDE);
                        break;
                    }
                    case CA_BUTTON_CANCEL:
                    {
                        ShowWindow(caPopup, SW_HIDE);
                        break;
                    }
                    case RT_BUTTON_REMOVE:
                    {   
                        if(MessageBox(hwnd, "Are you sure? \n \n The tag can not be restored!", "Remove Tag", MB_ICONEXCLAMATION | MB_YESNO) == 6){
                            printf("yes button!");
                        removeTag(selectedItemIndex);
                        ShowWindow(rtPopup, SW_HIDE);
                        break;

                        }else 
                        printf("No button!");
                        ShowWindow(rtPopup, SW_HIDE);
                        break;
                        
                    }
                    case RT_BUTTON_CANCEL:
                    {
                        ShowWindow(rtPopup, SW_HIDE);
                        break;
                    }
                    
                    
                    default:
                        break;
                }
                // check if oppsite radiobtn is checked in caPopup and enable btn
                BOOL isTrueChecked = SendMessage(caRadioTrue, BM_GETCHECK, 0,0) == BST_CHECKED;
                BOOL isFalseChecked = SendMessage(caRadioFalse, BM_GETCHECK, 0, 0) == BST_CHECKED;
                char currentAccess[8];
                ListView_GetItemText(hListView, selectedItemIndex, 3, currentAccess, sizeof(currentAccess));
                char access[] = "GRANTED";
                
                int check = strcmp(currentAccess, access); 
                if(!check && isFalseChecked){
                            
                    EnableWindow(caAddBtn, TRUE);
                        
                }
                else if(!check && isTrueChecked){

                    EnableWindow(caAddBtn, FALSE);

                }
                else if(check < 0 && isTrueChecked){ 
                        
                    EnableWindow(caAddBtn, TRUE);

                }
                else if(check < 0 && isFalseChecked){

                    EnableWindow(caAddBtn, FALSE);

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
        case WM_NOTIFY:
        {
            NMLISTVIEW* pnmv = (NMLISTVIEW*)lParam;

            if (pnmv->hdr.code == LVN_ITEMCHANGED) {
                
                selectedItemIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);

                if (selectedItemIndex != -1) {
                    int numSubItems = 5; 
                    for (int subItemIndex = 0; subItemIndex < numSubItems; subItemIndex++) {
                        ListView_SetItemState(hListView, selectedItemIndex, LVIS_SELECTED, LVIS_SELECTED);
                    }
                    // Enable or disable buttons here based on the selection
                    EnableWindow(cnBtn, TRUE);
                    EnableWindow(cidBtn, TRUE);
                    EnableWindow(cacBtn, TRUE);
                    EnableWindow(rtBtn, TRUE);
                }
                else {
                    // No item selected, disable buttons
                    EnableWindow(cnBtn, FALSE);
                    EnableWindow(cidBtn, FALSE);
                    EnableWindow(cacBtn, FALSE);
                    EnableWindow(rtBtn, FALSE);
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
            if(stPopup != NULL){

                SetWindowPos(stPopup, NULL, getHwndPos(1) + 270, getHwndPos(2) + 220, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                
            }
            break;
        }
        case WM_DESTROY:
        {
            static int exitCalled = 0;
            if(!exitCalled){
               onExit(); 
               exitCalled = 1;
            }
            if(hbrush){
               DeleteObject(hbrush); 
            } 
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
    lvColumn.cx = 120;
    SendMessage(hListView, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

    lvColumn.pszText = "ID";
    lvColumn.cx = 40;
    SendMessage(hListView, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

    lvColumn.pszText = "PASS";
    lvColumn.cx = 130;
    SendMessage(hListView, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

    lvColumn.pszText = "ACCESS";
    lvColumn.cx = 70;
    SendMessage(hListView, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);

    lvColumn.pszText = "CREATED";
    lvColumn.cx = 135;
    SendMessage(hListView, LVM_INSERTCOLUMN, 4, (LPARAM)&lvColumn);
    
    lvColumn.pszText = "CHANGED";
    lvColumn.cx = 135;
    SendMessage(hListView, LVM_INSERTCOLUMN, 5, (LPARAM)&lvColumn);

    // Add items
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

    lvItem.iSubItem = 4;
    lvItem.pszText ="YYYY-MM-DD 00:00:00";
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 5;
    lvItem.pszText ="YYYY-MM-DD 00:00:00";
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
static BOOL popupRegd = FALSE; // maybe not needed as global? ---> lets try not to

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

    atRadioFalse = CreateWindowEx(
        0, 
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
            cidPopup, (HMENU)CID_BUTTON_SAVE, GetModuleHandle(NULL), NULL);

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

        caRadioFalse = CreateWindowEx(
            0, 
            "BUTTON", 
            "NO", 
            WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 
            160, 60, 50, 20, 
            caPopup, (HMENU)CA_RADIO_FALSE, GetModuleHandle(NULL), NULL);

            //SendMessage(caRadioTrue, BM_SETCHECK, BST_CHECKED, 0);

        caAddBtn = CreateWindowEx(
            0, 
            "BUTTON", 
            "SAVE", 
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
            10, 120, 80, 30, 
            caPopup, (HMENU)CA_BUTTON_SAVE, GetModuleHandle(NULL), NULL);

            EnableWindow(caAddBtn, FALSE);

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
            "", 
            WS_VISIBLE | WS_CHILD,
            10, 
            10, 
            250, 
            20, 
            rtPopup, NULL, NULL, NULL);
        
        rtSelectedIdLabel = CreateWindow(
            "STATIC", 
            "", 
            WS_VISIBLE | WS_CHILD,
            10, 
            40, 
            250, 
            20, 
            rtPopup, NULL, NULL, NULL);

        rtSelectedAccessLabel = CreateWindow(
            "STATIC", 
            "", 
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

    //Scan Tag Popup
    else if(i == 6){

        stPopup = CreateWindowEx(
        0,
        "popupClass",
        "",
        WS_POPUP | WS_VISIBLE,
        getHwndPos(1) + 270,
        getHwndPos(2) + 220,
        260, 
        80,
        hwnd, NULL, GetModuleHandle(NULL), NULL);

        stMessageLabel = CreateWindow(
            "STATIC", 
            "Scan the tag/card...", 
            WS_VISIBLE | WS_CHILD,
            55, 
            10, 
            150, 
            20, 
            stPopup, NULL, NULL, NULL);

        stTagScannedLabel = CreateWindow(
            "STATIC", 
            "", // Tag Identified!
            WS_VISIBLE | WS_CHILD,
            10, 
            35, 
            150, 
            20, 
            stPopup, NULL, NULL, NULL);

        stTagReggedLabel = CreateWindow(
            "STATIC", 
            "",     // Tag Registrated!
            WS_VISIBLE | WS_CHILD,
            10, 
            55, 
            150, 
            20, 
            stPopup, NULL, NULL, NULL);
    }

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

    char listIdD[12];
    int check;

    check = readAndWriteTag(1,listPass, listIdD, sizeof(listIdD)); // gets actual tag id


    if(listIdD != NULL){
        SetWindowText(stTagScannedLabel, "Tag Identified!");
    }else if(check == 1){
        SetWindowText(stTagReggedLabel, "Tag Registrated!");
        Sleep(1000);
        ShowWindow(stPopup, SW_HIDE);
        SetWindowText(stTagScannedLabel, "");
        SetWindowText(stTagReggedLabel, "");
    }
    
    

    //checks which true/false radio button is selected
    char sListAccess[8];
    int listAccess;

    BOOL isTrueChecked = SendMessage(atRadioTrue, BM_GETCHECK, 0,0) == BST_CHECKED;
    BOOL isFalseChecked = SendMessage(atRadioFalse, BM_GETCHECK, 0, 0) == BST_CHECKED;

    if(isTrueChecked){
        
        strcpy(sListAccess, "GRANTED");
        listAccess = 1;

    }
    else if(isFalseChecked){
        
        strcpy(sListAccess, "DENIED");
        listAccess = 0;
    }
    // create timestamp for tag.createdTs
    char listTimestamp[20];
    char *tempTimestamp;
    
        tempTimestamp = getTimestamp();  

    strcpy(listTimestamp, tempTimestamp);

    free(tempTimestamp);

    
    // adds a new list item with gathered values
    int currentListSubItem = 0;

    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;

    lvItem.iItem = currentListItems;
    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = listName;
    SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    // put IdS and IdD togheter with a seperator |
    char idSidD[16];
    sprintf(idSidD, "%s | %d", listIdS, listIdD);
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

    currentListSubItem++;

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = listTimestamp;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = "yyyy-mm-dd 00:00:00";
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem = 0;

    currentListItems++;


    
    // put value in a tag and send to newTag();
    TAG tag;

    strncpy(tag.name, listName, sizeof(tag.name) - 1);
    tag.name[sizeof(tag.name) - 1] = '\0'; 

    strncpy(tag.idS, listIdS, sizeof(tag.idS) - 1);
    tag.idS[sizeof(tag.idS) - 1] = '\0'; 

    //change idD to byte idD[8]; -> tag.idD[i] = receivedId[i];
    strncpy(tag.idD, listIdD, sizeof(tag.idD));
    tag.idD[sizeof(tag.idD) - 1] = '\0';


    strncpy(tag.pass, listPass, sizeof(tag.pass) - 1);
    tag.pass[sizeof(tag.pass) - 1] = '\0';

    tag.access = listAccess;

    strncpy(tag.createdTs, listTimestamp, sizeof(tag.createdTs) - 1);
    tag.pass[sizeof(tag.createdTs) - 1] = '\0';

    strcpy(tag.changedTs, "yyyy-mm-dd 00:00:00");

    int direction = 0;

    //newTag(&tag, &direction);
    arrayHandler(&tag, 0, 0);
    
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
    
    // add convertion from hexadecimal to chars and then add with idS
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
        if(tag->access == 1){
        
            strcpy(sListAccess, "GRANTED");
            sListAccess[7] = '\0';
        }
        else if(tag->access == 0){
            
            strcpy(sListAccess, "DENIED");
            sListAccess[7] = '\0';
        }

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = sListAccess;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = tag->createdTs;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem++;

    lvItem.iSubItem = currentListSubItem;
    lvItem.pszText = tag->changedTs;
    SendMessage(hListView, LVM_SETITEM, 0, (LPARAM)&lvItem);

    currentListSubItem = 0;

    currentListItems++;

} 

void changedTag(int index, int subIndex, char value[255]){

    TAG tag;

    if(subIndex == 0){ // CHANGE NAME

        strncpy(tag.name, value, sizeof(tag.name) - 1);
        tag.name[sizeof(tag.name) - 1] = '\0';

        char tempId[16];
        ListView_GetItemText(hListView, index, 1, tempId, sizeof(tempId));
        tempId[sizeof(tempId) - 1] = '\0';

        strncpy(tag.idS, tempId, 1);    // use parts of this for below except IdS parts

        char tempNumber[12];
        strcpy(tempNumber, tempId + 4);   // use parts of this for below except IdS parts

        strncpy(tag.idD, tempNumber, sizeof(tag.idD) - 1);
        tag.idD[sizeof(tag.idD) - 1] = '\0';

        char tempAccess[256];
        ListView_GetItemText(hListView, index, 3, tempAccess, sizeof(tempAccess));

        if(tempAccess[0] == 'G'){
            tag.access = 1;
        }else if(tempAccess[0] == 'D'){
            tag.access = 0;
        }

    ListView_SetItemText(hListView, index, subIndex, value);
      
    }else if(subIndex == 1){ // CHANGE ID

        ListView_GetItemText(hListView, index, 0, tag.name, sizeof(tag.name));

        strncpy(tag.idS, value, 1); 
        tag.idS[sizeof(tag.idS) - 1] = '\0'; 

        char tempId[16];
        ListView_GetItemText(hListView, index, 1, tempId, sizeof(tempId));
        tempId[sizeof(tempId) - 1] = '\0';

        strncpy(tag.idS, tempId, 1);  

        char tempNumber[12];
        strcpy(tempNumber, tempId + 4);  

        strncpy(tag.idD, tempNumber, sizeof(tag.idD) - 1);
        tag.idD[sizeof(tag.idD) - 1] = '\0';
       
        char tempAccess[256];
        ListView_GetItemText(hListView, index, 3, tempAccess, sizeof(tempAccess));

        if(tempAccess[0] == 'G'){
            tag.access = 1;
        }else if(tempAccess[0] == 'D'){
            tag.access = 0;
        }

        char newValue[255];
        sprintf(newValue, "%s%s", tag.idS, tempNumber);
    
    ListView_SetItemText(hListView, index, subIndex, newValue);;

    }else if(subIndex == 3){ // CHANGE ACCESS

        ListView_GetItemText(hListView, index, 0, tag.name, sizeof(tag.name));

        char tempId[16];
        ListView_GetItemText(hListView, index, 1, tempId, sizeof(tempId));
        tempId[sizeof(tempId) - 1] = '\0';

        strncpy(tag.idS, tempId, 1);    // use parts of this for below except IdS parts

        char tempNumber[12];
        strcpy(tempNumber, tempId + 4);   // use parts of this for below except IdS parts

        strncpy(tag.idD, tempNumber, sizeof(tag.idD) - 1);
        tag.idD[sizeof(tag.idD) - 1] = '\0';

        if(value[0] == 'G'){
            tag.access = 1;
        }else if(value[0] == 'D'){
            tag.access = 0;
        }
            
    ListView_SetItemText(hListView, index, subIndex, value);

    }

    ListView_GetItemText(hListView, index, 2, tag.pass, sizeof(tag.pass));
    ListView_GetItemText(hListView, index, 4, tag.createdTs, sizeof(tag.createdTs));

    char newTimestamp[20];
    char *newTempTimestamp;
    
    newTempTimestamp = getTimestamp();  

    strcpy(newTimestamp, newTempTimestamp);

    free(newTempTimestamp);

    strncpy(tag.changedTs, newTimestamp, sizeof(tag.changedTs) - 1);
    tag.changedTs[sizeof(tag.changedTs) - 1] = '\0';

    ListView_SetItemText(hListView, index, 5, newTimestamp);

    printf("Ett: \n");
    printf("%s\n",tag.name);
    printf("%s\n",tag.idS);
    printf("%d\n",tag.idD);
    printf("pass: \n");
    printf("%s\n",tag.pass);
    printf("pass: \n");
    printf("%d\n",tag.access);
    printf("%s\n",tag.createdTs);
    printf("%s\n",tag.changedTs);
    //changeTag(&tag);
    int option = 1;
    int direction = 3;
    arrayHandler(&tag, option, &direction);
}   

int removeTag(int index){

    TAG tag;

    ListView_GetItemText(hListView, index, 0, tag.name, sizeof(tag.name));

    char tempId[16];
    ListView_GetItemText(hListView, index, 1, tempId, sizeof(tempId));
    tempId[sizeof(tempId) - 1] = '\0';

    strncpy(tag.idS, tempId, 1);    // use parts of this for below except IdS parts

    char tempNumber[12];
    strcpy(tempNumber, tempId + 4);   // use parts of this for below except IdS parts

    strncpy(tag.idD, tempNumber, sizeof(tag.idD) - 1);
    tag.idD[sizeof(tag.idD) - 1] = '\0';

    ListView_GetItemText(hListView, index, 2, tag.pass, sizeof(tag.pass));

    char tempAccess[256];
    ListView_GetItemText(hListView, index, 3, tempAccess, sizeof(tempAccess));

    if(tempAccess[0] == 'G'){
        tag.access = 1;
    }else if(tempAccess[0] == 'D'){
        tag.access = 0;
    }

    ListView_GetItemText(hListView, index, 4, tag.createdTs, sizeof(tag.createdTs));

    ListView_GetItemText(hListView, index, 5, tag.changedTs, sizeof(tag.changedTs));

    printf("Ett: \n");
    printf("%s\n",tag.name);
    printf("%s\n",tag.idS);
    printf("%d\n",tag.idD);
    printf("%s\n",tag.pass);
    printf("%d\n",tag.access);
    printf("%s\n",tag.createdTs);
    printf("%s\n",tag.changedTs);
    //deleteTag(&tag);
    int option = 4;
    int direction = 3;
    arrayHandler(&tag, option, &direction);

    ListView_DeleteItem(hListView, index);

    return 0;
}
int checkInput(const char *text){

    while (*text) {
        if (!isalpha(*text) && *text != ' ') {
            return 0; // non letter
        }
        text++;
    }
    return 1; // letter and whitespace
}

int onPopupAtaTime(int popup){

    if(popup == 1){
        ShowWindow(cnPopup, SW_HIDE);
        ShowWindow(cidPopup, SW_HIDE);
        ShowWindow(caPopup, SW_HIDE);
        ShowWindow(rtPopup, SW_HIDE);
    }
    else if(popup == 2){
        ShowWindow(atPopup, SW_HIDE);
        ShowWindow(cidPopup, SW_HIDE);
        ShowWindow(caPopup, SW_HIDE);
        ShowWindow(rtPopup, SW_HIDE);
    }
    else if(popup == 3){
        ShowWindow(atPopup, SW_HIDE);
        ShowWindow(cnPopup, SW_HIDE);
        ShowWindow(caPopup, SW_HIDE);
        ShowWindow(rtPopup, SW_HIDE);
    }
    else if(popup == 4){
        ShowWindow(atPopup, SW_HIDE);
        ShowWindow(cnPopup, SW_HIDE);
        ShowWindow(cidPopup, SW_HIDE);
        ShowWindow(rtPopup, SW_HIDE);
    }
    else if(popup == 5){
        ShowWindow(atPopup, SW_HIDE);
        ShowWindow(cnPopup, SW_HIDE);
        ShowWindow(cidPopup, SW_HIDE);
        ShowWindow(caPopup, SW_HIDE);
    }
return 0;
}
// <--- extra functions