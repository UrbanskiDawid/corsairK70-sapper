#pragma once
#include "keys.h"

namespace corsair
{
	namespace keys
	{
		std::vector<CorsairKey> corsairKeys;

		//get all CorsairKey
		std::vector<CorsairLedColor> getAll()
		{
			std::vector<CorsairLedColor> colorsVector;
			for (auto deviceIndex = 0; deviceIndex < CorsairGetDeviceCount(); deviceIndex++) {
				if (auto deviceInfo = CorsairGetDeviceInfo(deviceIndex)) {
					switch (deviceInfo->type) {
					case CDT_Mouse: {
						auto numberOfKeys = deviceInfo->physicalLayout - CPL_Zones1 + 1;
						for (auto i = 0; i < numberOfKeys; i++) {
							auto ledId = static_cast<CorsairLedId>(CLM_1 + i);
							colorsVector.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
						}
					} break;
					case CDT_Keyboard: {
						auto ledPositions = CorsairGetLedPositions();
						if (ledPositions) {
							for (auto i = 0; i < ledPositions->numberOfLed; i++) {
								auto ledId = ledPositions->pLedPosition[i].ledId;
								colorsVector.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
							}
						}
					} break;
					case CDT_Headset: {
						colorsVector.push_back(CorsairLedColor{ CLH_LeftLogo, 0, 0, 0 });
						colorsVector.push_back(CorsairLedColor{ CLH_RightLogo, 0, 0, 0 });
					} break;
					}
				}
			}

			return colorsVector;
		}

		//get CorsairKey by key number
		CorsairKey* get(int key)
		{
			for (int i = 0; i < corsairKeys.size(); i++)
			{
				const CorsairKey &k = corsairKeys.at(i);
				if (k.keyCode == key)
				{
					return &corsairKeys[i];
				}
			}
			return 0;
		}

		//get CorsairKey by ledId
		CorsairKey* get(CorsairLedId ledId)
		{
			for (int i = 0; i < corsairKeys.size(); i++)
			{
				const CorsairKey &k = corsairKeys.at(i);
				if (k.ledId == ledId)
				{
					return &corsairKeys[i];
				}
			}
			return 0;
		}

		//CorsairLedPositions as vector of CorsairLedPosition
		std::vector<CorsairLedPosition> getPos()
		{
			std::vector<CorsairLedPosition> ret;
			::CorsairLedPositions *clp = CorsairGetLedPositions();

			CorsairLedPosition *thisLedPos = 0;
			for (int i = 0;
			thisLedPos = &(clp->pLedPosition[i]),
				i < clp->numberOfLed;
				i++)
			{
				ret.push_back(*thisLedPos);
			}
			return ret;
		}

		//get Number of known keys
		size_t getNum()
		{
			return corsairKeys.size();
		}

		//set color of a key
		bool setColor(const CorsairLedId ledId, int r, int g, int b)
		{
			CorsairKey* key = get(ledId);
			if (key == 0) return false;
			key->setColor(r, g, b);
			return true;
		}

		//set color for all keys
		void setColorForAll(int r, int g, int b)
		{
			std::vector<CorsairLedColor> colorsVector;

			for (auto &key : corsair::keys::corsairKeys)
			{
				CorsairLedColor led;
				led.ledId = key.ledId;
				key.r = r;  key.g = g; key.b = b;
				led.r = r;	led.g = g; led.b = b;
				colorsVector.push_back(led);
			}
			CorsairSetLedsColors((int)colorsVector.size(), colorsVector.data());
		}

		namespace layoutUK
		{
			CorsairLedId getCorsairLedId(int i)
			{
				for (std::pair<int, CorsairLedId> p : KeyToCorsairLedId)
				{
					if (p.first == i)
						return p.second;
				}
				return CLI_Invalid;
			}

			int getKeyId(CorsairLedId ledId)
			{
				for (std::pair<int, CorsairLedId> p : KeyToCorsairLedId)
				{
					if (p.second == ledId)
						return p.first;
				}
				return -1;
			}
		}

		std::vector<CorsairKey*> getNeighbours(CorsairLedId ledID)
		{
			std::vector<CorsairKey*> ret;

			CorsairKey* key = get(ledID);
			if (key == 0)
				return ret;
			corsair::keys::CorsairKey k = *key;

			return k.neighbours;
		}

	}
}