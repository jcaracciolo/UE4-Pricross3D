// Fill out your copyright notice in the Description page of Project Settings.


#include "PiGameMode.h"

#include "Picross3D/GameLogic/PiPuzzle.h"

void APiGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentPuzzle = GetWorld()->SpawnActor<APiPuzzle>(PuzzleToLoad, FTransform::Identity);
}
