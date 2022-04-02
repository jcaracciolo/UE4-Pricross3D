// Fill out your copyright notice in the Description page of Project Settings.

#include "FPuzzleMatrix.h"
#include "Picross3D/GameLogic/PiCube.h"

FPuzzleMatrix::FPuzzleMatrix(): Array(nullptr)
{
}

FPuzzleMatrix::FPuzzleMatrix(TArray<APiCube*>* NewArray): Array(NewArray)
{
}

void FPuzzleMatrix::SetupMatrix(const uint32 X, const uint32 Y, const uint32 Z)
{
	checkf(X > 0 && Y > 0 && Z > 0, TEXT("TPuzzleMatrix: Dimensions are %d,%d,%d"),
	       X, Y, Z);

	checkf(Array->Num() == X*Y*Z, TEXT("TPuzzleMatrix: Size of the array does not contain %d elements"), X*Y*Z);

	SizeX = X;
	SizeY = Y;
	SizeZ = Z;

	Array->Sort([](const APiCube& One, const APiCube& Two)
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

APiCube* FPuzzleMatrix::Get(const uint32 X, const uint32 Y, const uint32 Z) const
{
	checkf(SizeX > X && SizeY > Y && SizeZ > Z,
	       TEXT("TPuzzleMatrix: Access outside of scope %d,%d,%d of %d,%d,%d"), X, Y, Z, SizeX,
	       SizeY, SizeZ);

	return Array->operator[](Z + Y * SizeZ + X * SizeZ * SizeY);
}

uint32 FPuzzleMatrix::GetMaxSize(EPiAxis Axis) const
{
	switch (Axis)
	{
	case EPiAxis::X: return SizeX;
	case EPiAxis::Y: return SizeY;
	case EPiAxis::Z: return SizeZ;
	}

	check(0);
	return 0;
}

