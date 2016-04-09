#include "core/arch_detect.h"
#include "core/log.h"
#ifdef TNG_OS_FAMILY_WINDOWS
#include "graphics/graphics.h"
namespace tng{

	LRESULT TngWindow::DefultCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
						   WinMap::iterator it = GetWinMap().find(hWnd);
						   if (it != GetWinMap().end())
						   {
							   WindowManger::GetInstance()->RemoveWindow(it->second->name_);
							   delete it->second;
							   GetWinMap().erase(it);
						   }
						   break;
		}
		case WM_LBUTTONDOWN:
		{
							   DWORD xPos = LOWORD(lParam);
							   DWORD yPos = HIWORD(lParam);
							   float2 pos;
							   pos.x = (float)xPos;
							   pos.y = (float)yPos;
							   WinMap::iterator it = GetWinMap().find(hWnd);
							   if (it != GetWinMap().end())
							   {
								   InputEvent* input = new InputEvent(InputBasePtr(new InputDown(pos, ELButton)));

								   it->second->SendEvent(input, NULL);
							   }
							   break;
		}
		case WM_LBUTTONUP:
		{
							 DWORD xPos = LOWORD(lParam);
							 DWORD yPos = HIWORD(lParam);
							 float2 pos;
							 pos.x = (float)xPos;
							 pos.y = (float)yPos;
							 WinMap::iterator it = GetWinMap().find(hWnd);
							 if (it != GetWinMap().end())
							 {
								 InputEvent* input = new InputEvent(InputBasePtr(new InputClick(pos, ELButton)));

								 it->second->SendEvent(input, NULL);
							 }
							 break;
		}
		case WM_RBUTTONDOWN:
		{
							   DWORD xPos = LOWORD(lParam);
							   DWORD yPos = HIWORD(lParam);
							   float2 pos;
							   pos.x = (float)xPos;
							   pos.y = (float)yPos;
							   WinMap::iterator it = GetWinMap().find(hWnd);
							   if (it != GetWinMap().end())
							   {
								   InputEvent* input = new InputEvent(InputBasePtr(new InputDown(pos, ERButton)));

								   it->second->SendEvent(input, NULL);
							   }
							   break;
		}
		case WM_RBUTTONUP:
		{
							 DWORD xPos = LOWORD(lParam);
							 DWORD yPos = HIWORD(lParam);
							 float2 pos;
							 pos.x = (float)xPos;
							 pos.y = (float)yPos;
							 WinMap::iterator it = GetWinMap().find(hWnd);
							 if (it != GetWinMap().end())
							 {
								 InputEvent* input = new InputEvent(InputBasePtr(new InputClick(pos, ERButton)));

								 it->second->SendEvent(input, NULL);
							 }
							 break;
		}
		case WM_MOUSEMOVE:
		{
							 DWORD xPos = LOWORD(lParam);
							 DWORD yPos = HIWORD(lParam);
							 float2 dir;
							 dir.x = (float)xPos;
							 dir.y = (float)yPos;
							 WinMap::iterator it = GetWinMap().find(hWnd);
							 if (it != GetWinMap().end())
							 {
								 InputEvent* input = new InputEvent(InputBasePtr(new InputMove(dir)));

								 it->second->SendEvent(input, NULL);
							 }
							 break;
		}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

WINHANDLE TngWindow::Create(const string& appname, u32 width, u32 height, u32 style)
{
#ifdef TNG_OS_FAMILY_WINDOWS
	WNDCLASS wndClass = { 0, TngWindow::DefultCallback, 0, 0, NULL,
		NULL,
		NULL,
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		NULL, appname.c_str() };
	RegisterClass(&wndClass);
	RECT rc;
	SetRect(&rc, 0, 0, width, height);
	AdjustWindowRect(&rc, style, false);

	handle_ = CreateWindow(appname.c_str(), "", style,
		100, 100,
		(rc.right - rc.left), (rc.bottom - rc.top), 0,
		NULL,
		NULL, 0);
	SetWindowText(handle_, appname.c_str());
	MSG  msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

#endif
	name_ = appname;
	GetWinMap()[handle_] = this;
	return handle_;
}
}
#endif // TNG_OS_FAMILY_WINDOWS
