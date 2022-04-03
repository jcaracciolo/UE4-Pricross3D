#pragma once

#include "Definitions.generated.h"

UENUM(BlueprintType)
enum class EPiAxis: uint8
{
	X = 0,
	Y,
	Z
};

#define PICK_FOR_AXIS(Axis, value1, value2, value3) ((Axis) == EPiAxis::X ? (value1) : ((Axis) == EPiAxis::Y ? (value2) : (value3)))

#define FAIL_UNKNOWN_AXIS verifyf(0, TEXT("Unknown Axis"));