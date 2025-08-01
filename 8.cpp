#include <windows.h>
#include <commdlg.h>

#define IDM_NEW     1
#define IDM_OPEN    2
#define IDM_SAVE    3
#define IDM_EXIT    4
#define IDM_CUT     5
#define IDM_COPY    6
#define IDM_PASTE   7

HWND hEdit;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            hEdit = CreateWindow("EDIT", "",
                                 WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                                 0, 0, 0, 0,
                                 hwnd, (HMENU)1, NULL, NULL);
            break;
        }
        case WM_SIZE: {
            MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
            break;
        }
        case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case IDM_NEW:
                    SetWindowText(hEdit, "");
                    break;

                case IDM_OPEN: {
                    char filename[MAX_PATH] = "";
                    OPENFILENAME ofn = { 0 };
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                    ofn.lpstrFile = filename;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

                    if(GetOpenFileName(&ofn)) {
                        FILE* file = fopen(filename, "r");
                        if(file) {
                            fseek(file, 0, SEEK_END);
                            long size = ftell(file);
                            rewind(file);
                            char* buffer = new char[size + 1];
                            fread(buffer, 1, size, file);
                            buffer[size] = '\0';
                            SetWindowText(hEdit, buffer);
                            delete[] buffer;
                            fclose(file);
                        }
                    }
                    break;
                }

                case IDM_SAVE: {
                    char filename[MAX_PATH] = "";
                    OPENFILENAME ofn = { 0 };
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                    ofn.lpstrFile = filename;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_OVERWRITEPROMPT;

                    if(GetSaveFileName(&ofn)) {
                        int len = GetWindowTextLength(hEdit);
                        char* buffer = new char[len + 1];
                        GetWindowText(hEdit, buffer, len + 1);
                        FILE* file = fopen(filename, "w");
                        if(file) {
                            fwrite(buffer, 1, len, file);
                            fclose(file);
                        }
                        delete[] buffer;
                    }
                    break;
                }

                case IDM_EXIT:
                    PostQuitMessage(0);
                    break;

                case IDM_CUT:
                    SendMessage(hEdit, WM_CUT, 0, 0);
                    break;
                case IDM_COPY:
                    SendMessage(hEdit, WM_COPY, 0, 0);
                    break;
                case IDM_PASTE:
                    SendMessage(hEdit, WM_PASTE, 0, 0);
                    break;
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void AddMenus(HWND hwnd) {
    HMENU hMenubar = CreateMenu();
    HMENU hFile = CreateMenu();
    HMENU hEdit = CreateMenu();

    AppendMenu(hFile, MF_STRING, IDM_NEW, "New");
    AppendMenu(hFile, MF_STRING, IDM_OPEN, "Open...");
    AppendMenu(hFile, MF_STRING, IDM_SAVE, "Save");
    AppendMenu(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFile, MF_STRING, IDM_EXIT, "Exit");

    AppendMenu(hEdit, MF_STRING, IDM_CUT, "Cut");
    AppendMenu(hEdit, MF_STRING, IDM_COPY, "Copy");
    AppendMenu(hEdit, MF_STRING, IDM_PASTE, "Paste");

    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, "File");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hEdit, "Edit");

    SetMenu(hwnd, hMenubar);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyNotepadClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("MyNotepadClass", "Simple Notepad - C++",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                             NULL, NULL, hInstance, NULL);

    AddMenus(hwnd);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}
