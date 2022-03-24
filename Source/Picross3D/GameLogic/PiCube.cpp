// Fill out your copyright notice in the Description page of Project Settings.


#include "PiCube.h"

// Sets default values
APiCube::APiCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void APiCube::BeginPlay()
{
	Super::BeginPlay();
	SetXHint(Hints.X);
	SetYHint(Hints.Y);
	SetZHint(Hints.Z);
}

void inline APiCube::SetupPuzzlePosition(const FIntVector&& SetupPosition)
{
	PuzzlePosition = SetupPosition;
}

void APiCube::SetXHint_Implementation(FHint Hint)
{
	Hints.X = Hint;
}

void APiCube::SetYHint_Implementation(FHint Hint)
{
	Hints.Y = Hint;
}

void APiCube::SetZHint_Implementation(FHint Hint)
{
	Hints.Z = Hint;
}

void APiCube::TogglePaint_Implementation()
{
	bIsPainted = !bIsPainted;
}

FIntVector APiCube::GetPuzzlePosition() const
{
	return PuzzlePosition;
}

bool APiCube::IsPainted() const
{
	return bIsPainted;
}

bool APiCube::IsSolution() const
{
	return bIsSolution;
}
