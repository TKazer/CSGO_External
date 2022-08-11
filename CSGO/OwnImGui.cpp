#include "OwnImGui.h"

/****************************************************
* Copyright (C): Liv
* @file		: OwnImGui.cpp
* @author	: Liv
* @email		: 1319923129@qq.com
* @version	: 1.0
* @date		: 2022/4/17	15:57
****************************************************/

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void UpdateWindowState(WindowData& Window);
bool Clean = false;
WNDCLASSEX W_class;

void OwnImGui::Start(const char* WindowClassName,const char* WindowName , CallBack MainFunction)
{
    int flag1 = -1, flag2 = -1;
    flag1 = strcmp(WindowName, "");
    flag2 = strcmp(WindowClassName, "");

    if (flag1 == 0 && flag2 == 0)
    {
        std::cout << "[error]: WindowInfo can't be empty." << std::endl;
        return;
    }
    if (reinterpret_cast<int>(MainFunction) == 0)
    {
        std::cout << "[error]: MainFunction can't be null." << std::endl;
        return;
    }

    if (flag1)
        strcpy_s(this->Window.WindowName, strlen(WindowName) + 1, WindowName);
    if (flag2)
        strcpy_s(this->Window.WindowClassName, strlen(WindowClassName) + 1, WindowClassName);

    if (flag1)
    {
        if (flag2)
            this->Window.hWindow = FindWindowA(this->Window.WindowClassName, this->Window.WindowName);
        else
            this->Window.hWindow = FindWindowA(NULL, this->Window.WindowName);
    }
    else if (flag2)
    {
        this->Window.hWindow = FindWindowA(this->Window.WindowClassName, NULL);
    }


    if (reinterpret_cast<int>(this->Window.hWindow) <= 0)
    {
        std::cout << "[error]: Window isn't exist."<<std::endl;
        Clean = true;
        return;
    }
    if (!CreateOwnWindow(this->Window.hWindow))
    {
        Clean = true;
        return;
    }
    Initialize();
    WindowMessageCycle(MainFunction);
}

OwnImGui::OwnImGui(const char* MenuName_)
{
    strcpy_s(MenuName, strlen(MenuName_) + 1, MenuName_);
}

OwnImGui::~OwnImGui()
{
    if (!Clean)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
        ::DestroyWindow(this->Window.hMyWindow);
        ::UnregisterClass(W_class.lpszClassName, W_class.hInstance);
    }
}

void OwnImGui::WindowMessageCycle(CallBack MainFunction)
{
    MSG msg;
    while (true)
    {
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                done = true;
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        if (done)
            break;       

        if (!this->Thread_UpdateWindowData.joinable())
        {
            this->Thread_UpdateWindowData = this->ThreadStart();
        }

        // Start ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        // Main
        ImGui::Begin(MenuName,nullptr,ImGuiWindowFlags_AlwaysAutoResize);
        MainFunction();
        ImGui::End();
        
        // Rendering
        ImGui::Render();

        const float clear_color_with_alpha[4] = { 0,0,0,0 };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(0, 0);
    }
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(this->Window.hMyWindow);
    ::UnregisterClass(W_class.lpszClassName, W_class.hInstance);
    Clean = true;
}

void OwnImGui::Initialize()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();(void)io;

    // 设置imgui
    //ImGui::StyleColorsDark();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    this->MyStyle();
    ImGui::GetStyle().WindowRounding = 4;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", FontSize,0,io.Fonts->GetGlyphRangesChineseFull());

    // Setup backends
    ImGui_ImplWin32_Init(this->Window.hMyWindow);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}

bool OwnImGui::CreateOwnWindow(HWND g_hToWindow)
{
	// 创建窗口
    USES_CONVERSION;
    WNDCLASSEX
	W_class = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW , (WNDPROC)WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, (HBRUSH)RGB(0, 0, 0), NULL, _T("ImGui"), NULL };
	RegisterClassEx(&W_class);
    this->Window.hMyWindow = CreateWindowExA(WS_EX_LAYERED|WS_EX_TRANSPARENT, W2A(W_class.lpszClassName), "DrawWindow", WS_POPUP, 1, 1, 100, 100, 0, 0, 0, 0);
    SetLayeredWindowAttributes(this->Window.hMyWindow, 0, 255, LWA_ALPHA);
    UpdateWindow(this->Window.hMyWindow);
    ShowWindow(this->Window.hMyWindow, SW_SHOW);

    // 获取目标窗口宽高
    GetWindowRect(g_hToWindow, &this->Window.WindowRect);
    this->Window.Width = this->Window.WindowRect.right - this->Window.WindowRect.left;
    this->Window.Height = this->Window.WindowRect.bottom - this->Window.WindowRect.top;

	// 初始化d3d设备
	if (!CreateDeviceD3D(this->Window.hMyWindow))
	{
		CleanupDeviceD3D();
		::UnregisterClass(W_class.lpszClassName, W_class.hInstance);
        std::cout << "[error]: Create D3D Device failed." << std::endl;
        return false;
	}

	// 显示窗口
	::ShowWindow(this->Window.hMyWindow, SW_SHOWDEFAULT);
	::UpdateWindow(this->Window.hMyWindow);
    return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall OwnImGui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
        {
            MARGINS     Margin = { -1 };
            DwmExtendFrameIntoClientArea(hWnd, &Margin);
            break;
        }
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

int  OwnImGui::CalcTextWidth(const char* Text)
{
    if (strcmp(Text, "") == 0)
        return 0;
    return ImGui::GetFont()->CalcTextSizeA(this->FontSize, FLT_MAX, 0, Text).x;
}

void OwnImGui::Text(const char* Text, Vec2 Pos, int color)
{
    ImGui::GetForegroundDrawList()->AddText(ImVec2(Pos.x, Pos.y), color, Text);
}

void OwnImGui::Rectangle(Vec2 Pos, int width, int height,int color, int thickness)
{
    ImGui::GetForegroundDrawList()->AddRect(ImVec2(Pos.x, Pos.y), ImVec2(Pos.x+width, Pos.y+height), color, 0, 15, thickness);
}

void OwnImGui::Line(Vec2 From, Vec2 To, int color, int thickness)
{
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(From.x,From.y), ImVec2(To.x,To.y), color, thickness);
}

void OwnImGui::Circle(Vec2 Center,float radius,int color,float thickness)
{

    ImGui::GetForegroundDrawList()->AddCircle(ImVec2(Center.x,Center.y), radius, color, max(radius, 50), thickness);
}

void OwnImGui::CircleFilled(Vec2 Center, float radius, int color, int num)
{
    ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(Center.x, Center.y), radius, color, num);
}

void OwnImGui::RectangleFilled(Vec2 Pos, int width, int height, int color)
{
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(Pos.x, Pos.y), ImVec2(Pos.x + width, Pos.y + height), color, 0, 0);
}

void OwnImGui::Arc(Vec2 Center, float Radius, ImColor Color, float Yaw, float Proportion, float ThickNess)
{
    float Count = 30;
    float Angle = 360 * Proportion;
    float IncreaseAngle = Angle / Count;
    float Alpha, Beta;

    Alpha = -Yaw - Angle / 2;

    float TempAngle = Alpha;
    Vec2 Previous, Current;
    for (int i = 0; i < Count; i++, TempAngle += IncreaseAngle)
    {
        Current.x = Center.x + Radius * cos(TempAngle * 3.1415926 / 180);
        Current.y = Center.y + Radius * sin(TempAngle * 3.1415926 / 180);
        if (i)
            this->Line(Previous, Current, Color, ThickNess);
        Previous = Current;
    }
}

void OwnImGui::StrokeText(const char* Text, Vec2 Pos, int color_b, int color)
{
    this->Text(Text, Vec2(Pos.x - 1, Pos.y + 1), color_b);
    this->Text(Text, Vec2(Pos.x - 1, Pos.y - 1), color_b);
    this->Text(Text, Vec2(Pos.x + 1, Pos.y + 1), color_b);
    this->Text(Text, Vec2(Pos.x + 1, Pos.y - 1), color_b);
    this->Text(Text, Pos, color);
}

void OwnImGui::UpdateWindowData()
{
    while (!done)
    {
        ImGuiIO& io = ImGui::GetIO();
        // 更新窗口数据
        POINT TempPoint{ 0,0 };
        RECT TempRect;
        GetClientRect(Window.hWindow, &TempRect);

        ClientToScreen(Window.hWindow, &TempPoint);
        Window.WindowRect.left = TempPoint.x;
        Window.WindowRect.right = Window.WindowRect.left + TempRect.right;
        Window.WindowRect.top = TempPoint.y;
        Window.WindowRect.bottom = Window.WindowRect.top + TempRect.bottom;

        Window.Width = Window.WindowRect.right - Window.WindowRect.left;
        Window.Height = Window.WindowRect.bottom - Window.WindowRect.top;
        SetWindowPos(Window.hMyWindow, HWND_TOPMOST, Window.WindowRect.left, Window.WindowRect.top, Window.Width, Window.Height, SWP_SHOWWINDOW);
        if (io.WantCaptureMouse)
            SetWindowLong(Window.hMyWindow, GWL_EXSTYLE, GetWindowLong(Window.hMyWindow, GWL_EXSTYLE) & (~WS_EX_LAYERED));
        else
            SetWindowLong(Window.hMyWindow, GWL_EXSTYLE, GetWindowLong(Window.hMyWindow, GWL_EXSTYLE) | WS_EX_LAYERED);
    }
}


void OwnImGui::ReSetFontSize(size_t FontSize_)
{
    FontSize = FontSize_;
}

void OwnImGui::ConnectPoint(std::vector<Vec2> Points, int color, float thickness)
{
    if (Points.size() <= 0)
        return;
    for (int i = 0; i < Points.size() - 1; i++)
    {
        Line(Points[i], Points[i + 1], color, thickness);
        if (i == Points.size() - 2)
            Line(Points[i + 1], Points[0], color, thickness);
    }
}

void OwnImGui::CheckboxEx(const char* Text, bool* Point)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(Text, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *Point = !*Point;

    float t = *Point ? 1.0f : 0.0f;

    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(Text))
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *Point ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg;
    if (ImGui::IsItemHovered())
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.86f, 0.07f, 0.23f, 1.0f), t));
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.78f, 0.07f, 0.15f, 1.0f), t));

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
    ImGui::SameLine();
    ImGui::Text(Text);
}

void OwnImGui::MyStyle()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.69f, 0.69f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.15f, 0.19f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.17f, 0.21f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.17f, 0.19f, 0.23f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.22f, 0.22f, 0.28f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.19f, 0.21f, 0.28f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.30f, 0.38f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.28f, 0.33f, 0.41f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.23f, 0.27f, 0.34f, 0.54f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.17f, 0.21f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.17f, 0.21f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.15f, 0.18f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.22f, 0.24f, 0.27f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.17f, 0.19f, 0.21f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.43f, 0.50f, 0.57f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.33f, 0.39f, 0.46f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.32f, 0.36f, 0.42f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.30f, 0.39f, 0.49f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.32f, 0.41f, 0.51f, 0.40f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.33f, 0.41f, 0.40f);
    colors[ImGuiCol_Header] = ImVec4(0.34f, 0.39f, 0.46f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.44f, 0.50f, 0.31f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.29f, 0.34f, 0.40f, 0.31f);
    colors[ImGuiCol_Separator] = ImVec4(0.29f, 0.32f, 0.38f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.32f, 0.36f, 0.42f, 0.50f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.29f, 0.34f, 0.50f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.17f, 0.22f, 0.28f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.23f, 0.29f, 0.36f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.14f, 0.18f, 0.23f, 0.20f);
    colors[ImGuiCol_Tab] = ImVec4(0.19f, 0.21f, 0.23f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.29f, 0.35f, 0.86f);
    colors[ImGuiCol_TabActive] = ImVec4(0.26f, 0.31f, 0.38f, 0.86f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
