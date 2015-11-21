#include "corsair\corsair.h"
#include <iostream>
#include <algorithm>

using namespace std;

template<typename T> class uniqueVector
{
public:
	std::vector<T> mem;

	bool contains(T newT)
	{
		for (const T &i : mem)
			if (i == newT)
				return true;
		return false;
	}

	bool insert(T newT)
	{
		if (contains(newT)) return false;
		mem.push_back(newT);
		return true;
	}
	void clear()
	{
		mem.clear();
	}
};

namespace GAME
{
	const int numMines = 6;
	const int diffuseTimeMilis = 1000;

	uniqueVector<CorsairLedId> mines;
	int steps = 0;
	std::thread thead;
	enum eGameStatus
	{
		init,
		runing,
		dead,
		vicotry
	};

	uniqueVector<CorsairLedId> pressMem, marked;

	void mark(corsair::keys::CorsairKey key)
	{
		if (!marked.insert(key.ledId)) return;
		if (mines.contains(key.ledId)) return;

		int num = 0;
		for (auto &k : key.neighbours)
		{
			if (mines.contains(k->ledId))
				num++;
		}

		if (num==0) key.setColor(250, 250, 250);
		if (num==1)	key.setColor(100,   0,   0);
		if (num==1)	key.setColor(200,   0,   0);
		if (num>=2)	key.setColor(255,   0,   0);
	}

	void markSurrounding(corsair::keys::CorsairKey key)
	{
		for (auto &k: key.neighbours )
			mark(*k);
	}

	void run()
	{
		using namespace GAME;
		std::cout << "GAME::task" << std::endl;
		
		eGameStatus gameStatus = init;

		while (true)
		{
			switch (gameStatus)
			{
			case init:
			{
				std::cout << "starting " << numMines << " mines" << std::endl;
				corsair::keys::setColorForAll(0, 0, 0);
				marked.clear();
				mines.clear();
				for (int i = 0; i < numMines; i++)
				{
					int mineID = rand() % corsair::keys::getNum();
					
					corsair::keys::CorsairKey BombLed = corsair::keys::corsairKeys[mineID];

					CorsairLedId BombLedId = BombLed.ledId;
					if (!mines.insert(BombLedId) || BombLed.keyCode <= 0)
					{
						i--;
						continue;
					}
				}
				
				pressMem.clear();
				corsair::actions::f_onKeyRelease = [&gameStatus](corsair::keys::CorsairKey key, long long duration)
				{
					if (!pressMem.insert(key.ledId))
						return;

					if(mines.contains(key.ledId))
					{
						if (duration < diffuseTimeMilis)
						{
							cout << "dead" << duration << "ms" << endl;
							gameStatus = dead;
						}
						else
						{
							steps++;
							cout << "difused"<< duration <<"ms ("<< steps<<"/"<< numMines<<")"<<endl;
							corsair::keys::setColor(key.ledId, 255, 0, 255);
							markSurrounding(key);
						}
					}
					else
					{
						mark(key);
						markSurrounding(key);
					}
					
					if (steps >= numMines)
					{
						gameStatus = vicotry;
						return;
					}
				};

				steps = 0;
				gameStatus = runing;

			}
			break;

			case runing:
			{

			}
			break;

			case dead:
			{
				corsair::actions::f_onKeyRelease = [&gameStatus](corsair::keys::CorsairKey,long long) {};

				std::cout << "you lose" << std::endl;

				for (int k = 0; k < 5; k++)
				{
					corsair::keys::setColorForAll(0, 0, 0);
						for (const auto &mine : mines.mem)
						corsair::keys::get(mine)->setColor(0, 255, 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(250));

					corsair::keys::setColorForAll(255, 0, 0);
						for (const auto &mine : mines.mem)
						corsair::keys::get(mine)->setColor(255, 255, 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(250));
				}

				gameStatus = init;
			}
			break;
			case vicotry:
			{
				corsair::GUI::ForceRedraw();
				corsair::actions::f_onKeyRelease = [&gameStatus](corsair::keys::CorsairKey, long long) {};

				std::cout << "you win" << std::endl;
				for (int k = 0; k < 5; k++)
				{
					corsair::keys::setColorForAll(0, 0, 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(500));

					corsair::keys::setColorForAll(0, 255, 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}
				gameStatus = init;
			}break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(25));
		}//while
	}

	void start()
	{
		thead = std::thread(run);
	}
}

int main()
{
	corsair::init();
	
	corsair::GUI::create();
	corsair::actions::f_onKeyUpdate = [] {		corsair::GUI::ForceRedraw();	};

	GAME::start();

	corsair::actions::taks();

	GAME::thead.join();
			
	return 0;
}