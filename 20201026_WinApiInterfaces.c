#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(); 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    char const*const window_class_name = "sample_window_class"; 

    WNDCLASSEX window_class; 
    
    window_class.cbSize = sizeof(WNDCLASSEX); 
    window_class.cbClsExtra = 0; 
    window_class.cbWndExtra = 0; 

    window_class.style = CS_VREDRAW | CS_HREDRAW; 
    
    window_class.lpfnWndProc = WndProc; 
    window_class.lpszClassName = window_class_name; 
    
    window_class.hInstance = hInstance; 
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW); 
    window_class.hCursor = LoadCursor(NULL, IDC_CROSS); 
    window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
    window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION); 

    if(!RegisterClassEx(&window_class)) {
        fprintf(stderr, "Error registering window class.\n"); 
        return EXIT_FAILURE; 
    }

    int window_x = GetSystemMetrics(SM_CXSCREEN); 
    int window_y = GetSystemMetrics(SM_CYSCREEN); 
    printf("window size: %d\n", window_x); 

    HWND hwnd = CreateWindowEx(
        0, 
        window_class_name, 
        "Sample Window", 
        WS_OVERLAPPEDWINDOW, 
        window_x - 500, 
        window_y - 500, 
        500, 
        500, 
        NULL, 
        NULL, 
        hInstance, 
        NULL
    ); 
    if(!hwnd) {
        fprintf(stderr, "Error creating window.\n"); 
        return EXIT_FAILURE; 
    }
    // ShowWindow(hwnd, SW_HIDE); 
    ShowWindow(hwnd, nCmdShow); 
    UpdateWindow(hwnd); 

    MSG message; 
    while(0 < GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message); 
        DispatchMessage(&message); 
    }
    
    
    return 0; 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            HMENU menu, sub_menu; 
            
            menu = CreateMenu(); 
            sub_menu = CreatePopupMenu(); 

            AppendMenu(sub_menu, MF_STRING, (UINT)9000, "&Text that shows up"); 
            AppendMenu(sub_menu, MF_STRING, (UINT)WM_DESTROY, "&Exit"); 
            AppendMenu(menu, MF_STRING | MF_POPUP, (UINT)sub_menu, "&File"); 
            SetMenu(hwnd, menu); 

            SetWindowText(hwnd, TEXT("This is a text string that I am string printing.")); 

            break; 
        }
        case WM_COMMAND: 
            switch(LOWORD(wParam)) {
                case 9000:
                    MessageBox(hwnd, "You selected a menu item", "Information", MB_OKCANCEL); 
                    break; 
                case WM_DESTROY: 
                    PostQuitMessage(0); 
                    break; 
            }
            break; 
        case WM_LBUTTONDOWN: 
            MessageBox(hwnd, "You clicked", "Information", MB_OK | MB_RIGHT); 
            break; 
        case WM_CLOSE: {
            MessageBox(hwnd, "What are you doing, Dave?", "Information", MB_OKCANCEL); 
            DestroyWindow(hwnd); 
            break; 
        }
        case WM_DESTROY: 
            PostQuitMessage(0); 
            break; 
        default: 
            return DefWindowProc(hwnd, msg, wParam, lParam); 
    }
    return 0; 
}