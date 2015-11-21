#pragma once
#include <thread>

namespace corsair 
{
	namespace actions
	{
		extern std::function<void(corsair::keys::CorsairKey)> f_onKeyPress;
		extern std::function<void(corsair::keys::CorsairKey, long long)> f_onKeyRelease;
		extern std::function<void()> f_onKeyUpdate;

		void updateKey(int key, bool down);

		extern std::thread thread;

		void taks();

		void run();
	}
}