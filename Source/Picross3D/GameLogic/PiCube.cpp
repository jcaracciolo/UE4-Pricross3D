// Fill out your copyright notice in the Description page of Project Settings.


#include "PiCube.h"

// Sets default values
APiCube::APiCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void inline APiCube::SetupPuzzlePosition(const FIntVector&& SetupPosition)
{
	PuzzlePosition = SetupPosition;
}

FIntVector APiCube::GetPuzzlePosition() const
{
	return PuzzlePosition;
}

bool APiCube::IsSolution() const
{
	return bIsSolution;
}
