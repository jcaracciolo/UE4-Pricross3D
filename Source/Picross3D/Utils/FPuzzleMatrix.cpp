// Fill out your copyright notice in the Description page of Project Settings.

#include "FPuzzleMatrix.h"
#include "Picross3D/GameLogic/PiCube.h"

FPuzzleMatrix::FPuzzleMatrix()
{
}

void FPuzzleMatrix::SetupMatrix(const uint32 X, const uint32 Y, const uint32 Z)
{
	ensureMsgf(X > 0 && Y > 0 && Z > 0, TEXT("FPuzzleMatrix: Dimensions are %d,%d,%d"), X, Y, Z);

	ensureMsgf(Array.Num() == X*Y*Z, TEXT("FPuzzleMatrix: Size of the array does not contain %d elements"), X*Y*Z);

	SizeX = X;
	SizeY = Y;
	SizeZ = Z;

	Array.Sort([](const APiCube& One, const APiCube& Two)
	{
		if (One.GetPuzzlePosition().X == Two.GetPuzzlePosition().X)
		{
			if (One.GetPuzzlePosition().Y == Two.GetPuzzlePosition().Y)
			{
				return One.GetPuzzlePosition().Z < Two.GetPuzzlePosition().Z;
			}

			return One.GetPuzzlePosition().Y < Two.GetPuzzlePosition().Y;
		}

		return One.GetPuzzlePosition().X < Two.GetPuzzlePosition().X;
	});
}
inline uint32 FPuzzleMatrix::ToIndex(FIntVector Position) const
{
	ensureMsgf(SizeX > Position.X && SizeY > Position.Y && SizeZ > Position.Z,
		   TEXT("FPuzzleMatrix: Access outside of scope %d,%d,%d of %d,%d,%d"), Position.X, Position.Y, Position.Z, SizeX, SizeY, SizeZ);

	return Position.Z + Position.Y * SizeZ + Position.X * SizeZ * SizeY;
}

APiCube* FPuzzleMatrix::operator()(FIntVector Position) const
{
	return Array[ToIndex(Position)];
}

uint32 FPuzzleMatrix::GetMaxSize(EPiAxis Axis) const
{
	switch (Axis)
	{
	case EPiAxis::X:
		return SizeX;
	case EPiAxis::Y:
		return SizeY;
	case EPiAxis::Z:
		return SizeZ;
	default: FAIL_UNKNOWN_AXIS;
	}

	return 0;
}

void FPuzzleMatrix::Remove(FIntVector Position)
{
	Array[ToIndex(Position)] = nullptr;
}
