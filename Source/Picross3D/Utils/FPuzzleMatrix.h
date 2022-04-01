// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPuzzleMatrix.generated.h"

class APiCube;
/**
 * 
 */
USTRUCT()
struct FPuzzleMatrix
{
	GENERATED_BODY()

public:
	FPuzzleMatrix();
	FPuzzleMatrix(TArray<APiCube*>* NewArray);


	void SetupMatrix(const uint32 X, const uint32 Y, const uint32 Z);

	APiCube* Get(const uint32 X, const uint32 Y, const uint32 Z) ;

private:
	uint32 SizeX = 0, SizeY = 0, SizeZ = 0;
	TArray<APiCube*>* Array;
};
