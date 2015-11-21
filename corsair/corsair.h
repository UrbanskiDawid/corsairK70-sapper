#pragma once
#include "keys.h"
#include "GUI.h"
#include "actions.h"

namespace corsair
{
	const char* toString(CorsairError error);

	double getKeyboardWidth();

	bool init();
}