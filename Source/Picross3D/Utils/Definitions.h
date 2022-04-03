#pragma once

#include "Definitions.generated.h"

UENUM(BlueprintType)
enum class EPiAxis: uint8
{
	X = 0,
	Y,
	Z
};


#define FAIL_UNKNOWN_AXIS verifyf(0, TEXT("Unknown Axis"));