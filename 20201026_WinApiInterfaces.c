#include <windows.h>
#include <stdio.h>

#define APP_SAVE 9000

LRESULT CALLBACK WndProc(); 

int text_cursor = 0; 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    FILE* text_cursor_file = fopen("text_cursor.txt", "r"); 
    if(text_cursor_file) {
        fscanf(text_cursor_file, "%d", &text_cursor); 
    }
    printf("You are at position %d\n", text_cursor); 
    fclose(text_cursor_file); 

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

            AppendMenu(sub_menu, MF_STRING, (UINT)APP_SAVE, "&Save"); 
            AppendMenu(sub_menu, MF_STRING, (UINT)WM_DESTROY, "&Exit"); 
            AppendMenu(menu, MF_STRING | MF_POPUP, (UINT)sub_menu, "&File"); 
            SetMenu(hwnd, menu); 

            break; 
        }
        case WM_COMMAND: 
            switch(LOWORD(wParam)) {
                case APP_SAVE: {
                    FILE* text_cursor_file = fopen("text_cursor.txt", "w"); 
                    fprintf(text_cursor_file, "%d", text_cursor); 
                    fflush(text_cursor_file); 
                    fclose(text_cursor_file); 
                    MessageBox(hwnd, "You selected a menu item", "Information", MB_OKCANCEL | MB_APPLMODAL); 
                    break; 
                }
                case WM_DESTROY: 
                    PostQuitMessage(0); 
                    break; 
            }
            break; 
        case WM_CHAR: {
            printf("%c", wParam); 
            if(wParam == 's') {
                text_cursor++; 
                printf(" was correct, you are now at position %d", text_cursor); 
            }
            printf("\n"); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 
        }
        case WM_PAINT: {
            InvalidateRect(hwnd, NULL, TRUE); 
            PAINTSTRUCT ps; 
            HDC hdc = BeginPaint(hwnd, &ps); 

            char buffer[1024] = {0}; 
            sprintf(buffer, "You are at position %d", text_cursor); 
            TextOut(hdc, 100, 200, TEXT(buffer), strlen(buffer)); 

            EndPaint(hwnd, &ps); 
            break; 
        }
        case WM_CLOSE: {
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