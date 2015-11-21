#include <windows.h>
#include <Windowsx.h>
#include <iostream>
#include <thread>
#include <vector>

#include <chrono>
#include "keys.h"

namespace corsair 
{
	namespace actions
	{
		using namespace corsair::keys;

		std::function<void(CorsairKey)> f_onKeyPress;
		std::function<void(CorsairKey, long long)> f_onKeyRelease;
		std::function<void()> f_onKeyUpdate;

		HHOOK hKeyboardHook;
		long long getTimeNow()//milliseconds
		{
			using namespace std::chrono;
			auto now = std::chrono::steady_clock::now();
			auto duration = now.time_since_epoch();
			return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		}
		void updateKey(int key, bool down)
		{
			CorsairKey *k = get(key);
			if (k == 0)
			{
				//cout << "updateKey(" << key << "= ?? " << down << ")" << endl;
				return;
			}

			if (k->ledId == CLI_Invalid) return;


			if (f_onKeyUpdate) f_onKeyUpdate();

			if (down)
			{
				k->pressed = true;

				if (k->timePressed == 0)
				{
					std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
					auto duration = now.time_since_epoch();
					k->timePressed = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
					//cout << "start" << k->timePressed << endl;
				}


				if (f_onKeyPress)    f_onKeyPress(*k);
			}
			else
			{
				k->pressed = false;

				std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
				auto duration = now.time_since_epoch();
				auto end = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

				//cout <<endl<< "start" << k->timePressed <<" end"<<end << "="<<(end- k->timePressed);
				if (f_onKeyRelease) f_onKeyRelease(*k, (end - k->timePressed));
				k->timePressed = 0;
			}
			//cout << "updateKey(" << key<<"="<<(int)k->keyCode << "," << down << ")"<<endl;
		}

		std::thread thread;
		int lastKeyID = -1;
		LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode < 0 || nCode != HC_ACTION)
				return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);

			KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
			int vk = (int)p->vkCode;

			//cout << "LowLevelKeyboardProc(" << vk<< endl;
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN
				||
				wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
			{
				if ((p->flags & LLKHF_EXTENDED) == 0)
				{
					switch (vk)
					{
					case VK_HOME:  vk = VK_NUMPAD7; break;
					case VK_UP:    vk = VK_NUMPAD8; break;
					case VK_PRIOR: vk = VK_NUMPAD9; break;
					case VK_LEFT:  vk = VK_NUMPAD4; break;
					case 12:       vk = VK_NUMPAD5; break;
					case VK_RIGHT: vk = VK_NUMPAD6; break;
					case VK_END:   vk = VK_NUMPAD1; break;
					case VK_DOWN:  vk = VK_NUMPAD2; break;
					case VK_NEXT:  vk = VK_NUMPAD3; break;
					case VK_INSERT:vk = VK_NUMPAD0; break;
					case VK_DELETE:vk = VK_DECIMAL; break;
					}
				}
				else {
					if (vk == VK_RETURN)
						vk = VK_SEPARATOR;
				}

				lastKeyID = vk;

				if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
				{
					updateKey(vk, true);
				}

				if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
				{
					updateKey(vk, false);
				}
			}

			return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
		}

		void taks()
		{
			hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

			MSG message;
			while (GetMessage(&message, NULL, 0, 0))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			UnhookWindowsHookEx(hKeyboardHook);
		}

		void run()
		{
			//thread = std::thread(taks);
			f_onKeyPress = [](CorsairKey led)
			{
				std::cout << "CorsairLedId " << led.keyCode << " was pressed" << std::endl;
			};
			f_onKeyRelease = [](CorsairKey led, long long)
			{
				std::cout << "CorsairLedId " << led.keyCode << " was released" << std::endl;
			};
		}
	}
}