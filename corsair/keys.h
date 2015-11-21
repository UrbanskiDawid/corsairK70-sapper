#include "CUESDK.h"

#include <windows.h>
#include <vector>

namespace corsair
{
	namespace keys
	{
		struct CorsairKey
		{
			int r, g, b;
			CorsairLedId ledId;
			int keyCode;
			double top, left, height, width;
			bool pressed;
			long long timePressed;
			std::vector<CorsairKey*> neighbours;

			void setColor(int R, int G, int B)
			{
				r = R; g = G; b = B;
				auto ledColor = CorsairLedColor{ ledId, r,g,b };
				CorsairSetLedsColors(1, &ledColor);
			}
		};

		extern std::vector<CorsairKey> corsairKeys;

		//get all CorsairKey
		std::vector<CorsairLedColor> getAll();

		//get CorsairKey by key number
		CorsairKey* get(int key);

		//get CorsairKey by ledId
		CorsairKey* get(CorsairLedId ledId);

		//CorsairLedPositions as vector of CorsairLedPosition
		std::vector<CorsairLedPosition> getPos();

		//get Number of known keys
		size_t getNum();

		//set color of a key
		bool setColor(const CorsairLedId ledId, int r, int g, int b);

		//set color for all keys
		void setColorForAll(int r, int g, int b);

		namespace layoutUK
		{
			const std::pair<int, CorsairLedId> KeyToCorsairLedId[]{
				//1row
				{ VK_ESCAPE, CLK_Escape },
				{ VK_F1, CLK_F1 },
				{ VK_F2, CLK_F2 },
				{ VK_F3, CLK_F3 },
				{ VK_F4, CLK_F4 },
				{ VK_F5, CLK_F5 },
				{ VK_F6, CLK_F6 },
				{ VK_F7, CLK_F7 },
				{ VK_F8, CLK_F8 },
				{ VK_F9, CLK_F9 },
				{ VK_F10, CLK_F10 },
				{ VK_F11, CLK_F11 },
				{ VK_F12, CLK_F12 },
				//2row
				{ VK_OEM_3, CLK_GraveAccentAndTilde },
				{ 48, CLK_0 },
				{ 49, CLK_1 },
				{ 50, CLK_2 },
				{ 51, CLK_3 },
				{ 52, CLK_4 },
				{ 53, CLK_5 },
				{ 54, CLK_6 },
				{ 55, CLK_7 },
				{ 56, CLK_8 },
				{ 57, CLK_9 },
				{ VK_OEM_MINUS, CLK_MinusAndUnderscore },
				{ VK_OEM_PLUS,  CLK_EqualsAndPlus },
				{ VK_BACK,      CLK_Backspace },
				//3row	
				{ VK_TAB, CLK_Tab },
				{ 81, CLK_Q },
				{ 87, CLK_W },
				{ 69, CLK_E },
				{ 82, CLK_R },
				{ 84, CLK_T },
				{ 89, CLK_Y },
				{ 85, CLK_U },
				{ 73, CLK_I },
				{ 79, CLK_O },
				{ 80, CLK_P },
				{ VK_OEM_4, CLK_BracketLeft },
				{ VK_OEM_6, CLK_BracketRight },
				{ VK_OEM_5, CLK_Backslash },
				//4row
				{ VK_CAPITAL, CLK_CapsLock },
				{ 65, CLK_A },
				{ 83, CLK_S },
				{ 68, CLK_D },
				{ 70, CLK_F },
				{ 71, CLK_G },
				{ 72, CLK_H },
				{ 74, CLK_J },
				{ 75, CLK_K },
				{ 76, CLK_L },
				{ VK_OEM_1,CLK_SemicolonAndColon },// ';:' for US
				{ VK_OEM_7,CLK_ApostropheAndDoubleQuote },//  ''"' for US
				{ VK_RETURN, CLK_Enter },
				//5row
				{ VK_LSHIFT, CLK_LeftShift },
				{ 90, CLK_Z },
				{ 88, CLK_X },
				{ 67, CLK_C },
				{ 86, CLK_V },
				{ 66, CLK_B },
				{ 78, CLK_N },
				{ 77, CLK_M },
				{ VK_OEM_COMMA, CLK_CommaAndLessThan },
				{ VK_OEM_PERIOD,CLK_PeriodAndBiggerThan },
				{ VK_OEM_2, CLK_SlashAndQuestionMark },
				{ VK_RSHIFT, CLK_RightShift },
				//6row
				{ VK_LCONTROL, CLK_LeftCtrl },
				{ VK_LWIN, CLK_LeftGui },
				{ VK_LMENU, CLK_LeftAlt },
				{ VK_SPACE, CLK_Space } ,
				{ VK_RMENU, CLK_RightAlt },
				{ VK_RWIN, CLK_RightGui },
				{ VK_APPS, CLK_Application },
				{ VK_RCONTROL, CLK_RightCtrl },
				//
				{ VK_SNAPSHOT, CLK_PrintScreen },{ VK_SCROLL, CLK_ScrollLock },{ VK_PAUSE, CLK_PauseBreak },
				{ VK_INSERT , CLK_Insert },{ VK_HOME, CLK_Home },{ VK_PRIOR, CLK_PageUp },
				{ VK_DELETE , CLK_Delete },{ VK_END, CLK_End },{ VK_NEXT,  CLK_PageDown },
				//AROWS
				{ VK_LEFT, CLK_LeftArrow },
				{ VK_UP, CLK_UpArrow },
				{ VK_RIGHT, CLK_RightArrow },
				{ VK_DOWN , CLK_DownArrow },
				//NUMPAD
				{ VK_NUMLOCK, CLK_NumLock },
				{ VK_DIVIDE , CLK_KeypadSlash },
				{ VK_MULTIPLY,CLK_KeypadAsterisk },
				{ VK_SUBTRACT,CLK_KeypadMinus },
				{ VK_NUMPAD0,CLK_Keypad0 },
				{ VK_NUMPAD1,CLK_Keypad1 },
				{ VK_NUMPAD2,CLK_Keypad2 },
				{ VK_NUMPAD3,CLK_Keypad3 },
				{ VK_NUMPAD4,CLK_Keypad4 },
				{ VK_NUMPAD5,CLK_Keypad5 },
				{ VK_NUMPAD6,CLK_Keypad6 },
				{ VK_NUMPAD7,CLK_Keypad7 },
				{ VK_NUMPAD8,CLK_Keypad8 },
				{ VK_NUMPAD9,CLK_Keypad9 },
				{ VK_DECIMAL, CLK_KeypadPeriodAndDelete },
				{ VK_ADD, CLK_KeypadPlus },
				{ VK_SEPARATOR, CLK_KeypadEnter },//NONSTANDARD SOLUTION!	
				{ -2 , CLK_Brightness },
				{ -2 , CLK_WinLock },
				{ VK_VOLUME_MUTE,CLK_Mute },
				{ VK_MEDIA_STOP,CLK_Stop },
				{ VK_MEDIA_PREV_TRACK, CLK_ScanPreviousTrack },
				{ VK_MEDIA_NEXT_TRACK, CLK_ScanNextTrack },
				{ VK_MEDIA_PLAY_PAUSE, CLK_PlayPause }
			};

			CorsairLedId getCorsairLedId(int i);

			int getKeyId(CorsairLedId ledId);
		}



		//get Neighbours by ledID or empty list
		std::vector<CorsairKey*> getNeighbours(CorsairLedId ledID);
	}
}