#pragma once
#include "corsair.h"
#include <iostream>
#include <algorithm>

namespace corsair
{
	const char* toString(CorsairError error)
	{
		switch (error)
		{
		case CE_Success:					return "CE_Success";
		case CE_ServerNotFound:				return "CE_ServerNotFound";
		case CE_NoControl:					return "CE_NoControl";
		case CE_ProtocolHandshakeMissing:	return "CE_ProtocolHandshakeMissing";
		case CE_IncompatibleProtocol:		return "CE_IncompatibleProtocol";
		case CE_InvalidArguments:			return "CE_InvalidArguments";
		}
		return "unknown error";
	}

	double getKeyboardWidth()
	{
		const CorsairLedPositions &ledPositions = *CorsairGetLedPositions();
		const auto minmaxLeds = std::minmax_element(ledPositions.pLedPosition, ledPositions.pLedPosition + ledPositions.numberOfLed,
			[](const CorsairLedPosition &clp1, const CorsairLedPosition &clp2) {
			return clp1.left < clp2.left;
		});
		return minmaxLeds.second->left + minmaxLeds.second->width - minmaxLeds.first->left;
	}

	bool init()
	{
		CorsairProtocolDetails d = CorsairPerformProtocolHandshake();
		if (const auto error = CorsairGetLastError())
		{
			std::cout << "Handshake failed: " << corsair::toString(error) << std::endl;
			getchar();
			return false;
		}

		CorsairRequestControl(CAM_ExclusiveLightingControl);
		if (const auto error = CorsairGetLastError())
		{
			std::cout << "CorsairRequestControl failed: " << corsair::toString(error) << std::endl;
			getchar();
			return false;
		}

		//corsair::actions::run();
		std::vector<CorsairLedColor> colorsVector = corsair::keys::getAll();

		sort(colorsVector.begin(), colorsVector.end(),
			[](const CorsairLedColor & a, const CorsairLedColor & b) -> bool
		{
			return a.ledId <b.ledId;
		});

		auto corsairLedPositions = corsair::keys::getPos();

		for (CorsairLedColor &c : colorsVector)
		{
			corsair::keys::CorsairKey newKey;
			newKey.ledId = c.ledId;
			newKey.keyCode = corsair::keys::layoutUK::getKeyId(c.ledId);

			newKey.pressed = false;
			newKey.timePressed = 0;

			newKey.r = c.r;
			newKey.g = c.g;
			newKey.r = c.b;

			newKey.left = 0;
			newKey.width = 0;
			newKey.top = 0;
			newKey.height = 0;

			newKey.left = -1;
			for (CorsairLedPosition &p : corsairLedPositions)
			{
				if (p.ledId == c.ledId)
				{
					newKey.left = p.left;
					newKey.width = p.width;
					newKey.top = p.top;
					newKey.height = p.height;
					break;
				}
			}
			if (newKey.left == -1)			return false;
			else
				corsair::keys::corsairKeys.push_back(newKey);
		}

		//load all neighbours
		{
			typedef std::pair<corsair::keys::CorsairKey*, RECT> KeyRectPair;
			std::vector<KeyRectPair> list;

			//generate KeyRectPair list (aka extended RECTS)
			for (auto &key : corsair::keys::corsairKeys)
			{
				KeyRectPair a;
				a.first = &key;
				a.second = corsair::GUI::toRect(key);

				int blowFactor = (int)(1.2*(key.width <= key.height) ? key.width : key.height);
				corsair::GUI::blow(a.second, blowFactor, blowFactor);

				list.push_back(a);
			}

			//find coliding
			for (auto &key2rectA : list)
			{
				corsair::keys::CorsairKey & keyA = *key2rectA.first;
				const RECT & rectA = key2rectA.second;

				for (auto &key2rectB : list)
				{
					corsair::keys::CorsairKey & keyB = *key2rectB.first;
					const RECT & rectB = key2rectB.second;

					if (keyB.ledId == keyA.ledId)
						continue;

					if ((rectA.left   < rectB.right) && (rectA.right > rectB.left)
						&&
						!(rectA.bottom < rectB.top) && !(rectA.top   > rectB.bottom))
					{
						keyA.neighbours.push_back(&keyB);
					}
				}
			}
		}
		return true;
	}
}
