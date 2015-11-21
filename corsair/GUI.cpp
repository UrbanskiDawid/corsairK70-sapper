#include <windows.h>
#include <Windowsx.h>
#include <iostream>
#include <thread>
#include <vector>

#include <iostream>
#include <windows.h>
#include "keys.h"

namespace corsair
{
	namespace GUI
	{
		HWND window;

		void ForceRedraw()
		{
			InvalidateRect(window, 0, TRUE);
		}

		//key dimentions to rect
		RECT toRect(const corsair::keys::CorsairKey &key)
		{
			RECT rect;

			rect.left = (int)key.left;
			rect.right = (int)(key.left + key.width);

			rect.top = (int)(key.top);
			rect.bottom = (int)(key.top + key.height);

			return rect;
		}

		void blow(RECT &r, int factorX, int factorY)
		{
			int x2 = factorX / 2;
			r.left -= x2;
			r.right += x2;

			int y2 = factorY / 2;
			r.top -= y2;
			r.bottom += y2;
		}

		std::vector<CorsairLedId> colide;

		void drawKeys(HDC         hDC, PAINTSTRUCT Ps)
		{
			HPEN	   hPen = CreatePen(PS_SOLID, 1, RGB(255, 25, 5));
			HBRUSH     bgBrush = CreateSolidBrush(RGB(250, 250, 0));

			SelectObject(hDC, hPen);

			for (const auto &a : corsair::keys::corsairKeys)
			{
				RECT rect = toRect(a);

				if (a.pressed)
				{
					int f = (int)((a.width < a.height) ? a.width : a.height);
					blow(rect, f, f);
				}

				bool on = false;
				if (a.pressed) on = true;
				for (CorsairLedId &led : colide)
					if (a.ledId == led)
						on = true;
				if (on)
				{
					FillRect(hDC, &rect, bgBrush);
				}
				else
				{
					Rectangle(hDC,
						rect.left, rect.top,
						rect.right, rect.bottom);
				}
			}
		}


		void onClick(int x, int y, corsair::keys::CorsairKey key)
		{
			colide.clear();
			colide.push_back(key.ledId);

			for (const auto &key : key.neighbours)
			{
				colide.push_back(key->ledId);
			}

			ForceRedraw();
		}

		LRESULT WINAPI WindowProcedure(HWND hwnd, unsigned int msg, WPARAM wp, LPARAM lp)
		{
			HDC         hDC;
			PAINTSTRUCT Ps;

			RECT clientRect;	HBRUSH bgBrush;

			switch (msg)
			{
			case WM_DESTROY:
				std::cout << "\ndestroying window\n";
				PostQuitMessage(0);

				return 0L;
			case WM_PAINT:

				hDC = BeginPaint(hwnd, &Ps);

				GetClientRect(hwnd, &clientRect);
				bgBrush = CreateSolidBrush(RGB(0, 0, 0));
				FillRect(hDC, &clientRect, bgBrush);

				drawKeys(hDC, Ps);

				EndPaint(hwnd, &Ps);
				break;
			case WM_MOUSEMOVE:			ForceRedraw();			break;
			case WM_LBUTTONUP:
			{
				int xPos = GET_X_LPARAM(lp);
				int yPos = GET_Y_LPARAM(lp);

				for (const auto &a : corsair::keys::corsairKeys)
				{
					RECT rect = toRect(a);

					if (rect.left <= xPos && rect.right >= xPos
						&&
						rect.top <= yPos && rect.bottom >= yPos)
					{
						onClick(xPos, yPos, a);
					}
				}

				return 0;
			}
			}
			return DefWindowProc(hwnd, msg, wp, lp);
		}

		void run()
		{
			MSG msg;
			while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
		}

		void create()
		{
			const wchar_t * const myclass = L"myclass";

			WNDCLASSEX wndclass = {
				sizeof(WNDCLASSEX), CS_DBLCLKS, WindowProcedure,
				0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
				LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
				0, myclass, LoadIcon(0,IDI_APPLICATION)
			};

			int width=0,
				height=0;
			for (const auto&key : corsair::keys::corsairKeys)
			{
				double r = key.left + key.width;
				double b = key.top  + key.height;

				width  = (int)((width > r) ? width : r);
				height = (int)((height> b) ? height: b);
			}

			width += 40;
			height+=GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CXSIZEFRAME)+40;

			if (RegisterClassEx(&wndclass))
			{
				window = CreateWindowEx(0, myclass, L"keyboardGUI",
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, CW_USEDEFAULT, width, height,
					0, 0, GetModuleHandle(0), 0);
				if (window)
				{
					ShowWindow(window, SW_SHOWDEFAULT);
				}
			}
		}
	}
}