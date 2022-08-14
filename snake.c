#include "utils.c"
#include "rendering.c"
#include "game.c"

LRESULT CALLBACK WindowMessageHandle(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);
void InputMessageHandling(MSG Message, HWND Window);


int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int ShowCode){
    
    
    MSG Message;
    
    /*Window Class*/
    
    WNDCLASSA WindowClass = {0};
    
    WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowMessageHandle;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "SnakeWindowClass";
    
    RegisterClass(&WindowClass);
    
    /*Create WindoW*/
    HWND Window = CreateWindowEx(0, WindowClass.lpszClassName, "Snake", WS_VISIBLE|WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1030, 797, 0, 0, Instance, 0);
    
    HDC DeviceContext = GetDC(Window);
    
    LARGE_INTEGER LINTFrequency;
    QueryPerformanceFrequency(&LINTFrequency);
    float Frequency = (float) LINTFrequency.QuadPart;
    
    LARGE_INTEGER StartCounter;
    QueryPerformanceCounter(&StartCounter);
    
    
    /*Game Loop*/
    Start();
    while(Running != 0){
        /*Input*/
        InputMessageHandling(Message, Window);
        
        /*Draw Buffer*/
        Update();
        Draw();
        
        
        /*Render*/
        StretchDIBits(DeviceContext, 0, 0, RenderBuffer.Width, RenderBuffer.Height, 0, 0, RenderBuffer.Width, RenderBuffer.Height, RenderBuffer.Pixels, &RenderBuffer.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
        
        /*Get Frame Time*/
        LARGE_INTEGER EndCounter;
        QueryPerformanceCounter(&EndCounter);
        Time.Frame = (float)(EndCounter.QuadPart - StartCounter.QuadPart) / Frequency;
        StartCounter = EndCounter;
        printf("%f\n", Time.Frame);
        
        Sleep(16);
    }
    
    return 0;
}


LRESULT CALLBACK WindowMessageHandle(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam){
    
    LRESULT Result = 0;
    
    switch(Message){
        case WM_SIZE:{
            /*Get Buffer Width and Height*/
            RECT ClientRect = {0}; 
            GetClientRect(Window, &ClientRect);
            RenderBuffer.Width = ClientRect.right - ClientRect.left;
            RenderBuffer.Height = ClientRect.bottom - ClientRect.top;
            
            /*Allocate the Buffer*/
            if(RenderBuffer.Pixels != 0){
                /*Free*/
                VirtualFree(RenderBuffer.Pixels, 0, MEM_RELEASE);
            }
            RenderBuffer.Pixels = VirtualAlloc(0, sizeof(uint32) * RenderBuffer.Width * RenderBuffer. Height, MEM_COMMIT, PAGE_READWRITE);
            
            /*Fill the bitmap info*/
            
            RenderBuffer.BitMapInfo.bmiHeader.biSize = sizeof(RenderBuffer.BitMapInfo.bmiHeader);
			RenderBuffer.BitMapInfo.bmiHeader.biWidth = RenderBuffer.Width;
			RenderBuffer.BitMapInfo.bmiHeader.biHeight = -RenderBuffer.Height;
			RenderBuffer.BitMapInfo.bmiHeader.biPlanes = 1;
			RenderBuffer.BitMapInfo.bmiHeader.biBitCount = 32;
			RenderBuffer.BitMapInfo.bmiHeader.biCompression = BI_RGB;
            
        }break;
        
        case WM_CLOSE:{
            Running = 0;
        }break;
        
        case WM_DESTROY:{
            Running = 0;
        }break;
        
        default:{
            Result = DefWindowProc(Window, Message, wParam, lParam);
        }break;
        
    }
    
    return Result;
}


void InputMessageHandling(MSG Message, HWND Window){
    
    int i;
    for(i = 0; i < BUTTON_COUNT; i++){
        input.button[i].Changed = 0;
    }
    
    while (PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)){
        switch(Message.message){
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:{
                uint32 VKCode = (uint32)Message.wParam;
                //uint8 WasDown = ((Message.lParam & (1 << 30)) != 0);
                //uint8 IsDown = ((Message.lParam & (1 << 31)) == 0);
                
#define ProcessButton(vk, b) if(VKCode == vk){ input.button[b].IsDown = 1; input.button[b].Changed = 1; }
                
                ProcessButton(VK_RIGHT, RIGHT);
                ProcessButton(VK_LEFT, LEFT);
                ProcessButton(VK_UP, UP);
                ProcessButton(VK_DOWN, DOWN);
                
                default:{
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
            }
            
        }
        
    }
    
    return;
}