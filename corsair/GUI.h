#pragma once

namespace corsair
{
	namespace GUI
	{
		void ForceRedraw();

		void drawKeys(HDC         hDC, PAINTSTRUCT Ps);

		void onClick(int x, int y, corsair::keys::CorsairKey key);

		void run();

		//create window
		void create();

		//key dimentions to rect
		RECT toRect(const corsair::keys::CorsairKey &key);

		//increase RECT dimentions by x/y factor
		void blow(RECT &r, int factorX, int factorY);
	}
}