#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"
#include "Struct.h"
#include <d3d11.h>
#include <iostream>
#include <tchar.h>
#include <dwmapi.h>
#include <atlstr.h>
#include <thread>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dwmapi.lib")

/****************************************************
* Copyright (C): Liv
* @file		: OwnImGui.h
* @author	: Liv
* @email		: 1319923129@qq.com
* @version	: 1.0
* @date		: 2022/4/17	15:57
****************************************************/

// Datas
static ID3D11Device* g_pd3dDevice;
static ID3D11DeviceContext* g_pd3dDeviceContext;
static IDXGISwapChain* g_pSwapChain;
static ID3D11RenderTargetView* g_mainRenderTargetView;
typedef void(*CallBack)();
extern bool Clean;
extern WNDCLASSEX W_class;

struct WindowData {
	HWND hWindow;		// Ŀ�괰�ھ��
	HWND hMyWindow;		// ���ƴ��ھ��
	RECT WindowRect;	// Ŀ�괰������
	char WindowName[256];
	char WindowClassName[256];
	int  Width, Height; // Ŀ�괰�ڴ�С
};

class OwnImGui
{
private:
	// ��ʼ��
	void Initialize();
	// ��������
	bool CreateOwnWindow(HWND ToWindow);
	// ������Ϣѭ��
	void WindowMessageCycle(CallBack MainFunction);
	// ���ڻص�
	static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// �������ݻ�ȡ�߳�
	void UpdateWindowData();
	std::thread ThreadStart()
	{
		return std::thread(&OwnImGui::UpdateWindowData,this);
	}
	std::thread Thread_UpdateWindowData;
	// ��ʼ�����С
	size_t FontSize = 15;
	// ����
	bool done = false;
public:
	OwnImGui(const char* MenuName_);
	OwnImGui() { strcpy_s(MenuName, 4, "Menu"); }
	~OwnImGui();
	// �˵�����
	char MenuName[256]{};

	// ������Ϣ
	WindowData Window;
	// ����������
	void Start(const char* WindowClassName, const char* WindowName, CallBack MainFunction);
	// �˳�����
	void End() { done = true; }
	// ���������С
	void ReSetFontSize(size_t FontSize_);
	// ��������(����ǰ����)
	void MyStyle();
	// Բ��ѡ���
	void CheckboxEx(const char* Text, bool* Point);
	// �����ı����Ƴ���
	int  CalcTextWidth(const char* Text);
	// �ı�
	void Text(const char* Text, Vec2 Pos,int color);
	// ����
	void Rectangle(Vec2 Pos, int width, int height, int color, int thickness);
	// ��
	void Line(Vec2 From, Vec2 To,int color,int thickness);
	// Բ
	void Circle(Vec2 Center, float radius, int color, float thickness);
	// ���Բ
	void CircleFilled(Vec2 Center, float radius, int color, int num = 360);
	// ������
	void RectangleFilled(Vec2 Pos, int width, int height, int color);
	// ����ı�
	void StrokeText(const char* Text, Vec2 Pos, int color_bg, int color);
	//  ����Բ��
	void Arc(Vec2 Center, float Radius, ImColor Color, float Yaw, float Proportion, float ThickNess);
	// ���ӵ�
	void ConnectPoint(std::vector<Vec2> Points, int color, float thickness);
};