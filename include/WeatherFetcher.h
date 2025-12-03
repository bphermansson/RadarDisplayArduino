#pragma once
#include "Weather.h"

// Fill the passed Weather object.
// In a real app this could fetch JSON from the network and parse it.
void fetchWeather(Weather &out);
