// Fill out your copyright notice in the Description page of Project Settings.


#include "PiPuzzle.h"

#include "PiCube.h"
#include "Picross3D/Config/PiCamera.h"
#include "Picross3D/Config/PiGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogPiPuzzle, Log, All);

// Sets default values
APiPuzzle::APiPuzzle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APiPuzzle::BeginPlay()
{
	Super::BeginPlay();

	auto Pawn = Cast<APiCamera>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(!IsValid(Pawn))
	{
		UE_LOG(LogPiPuzzle, Error, TEXT("Pawn failed to be casted to APiCamera"));
		return;
	}
	Pawn->SetupPuzzleSize(PuzzleSize.GetMax());
}

// Called when the game starts or when spawned
void APiPuzzle::GenerateCubes()
{
	UE_LOG(LogPiPuzzle, Log, TEXT("Creating Cubes for %d,%d,%d"),PuzzleSize.X, PuzzleSize.Y, PuzzleSize.Z);

	for (auto i = 0; i < PuzzleSize.X; i++)
	{
		for (auto j = 0; j < PuzzleSize.Y; j++)
		{
			for (auto k = 0; k < PuzzleSize.Z; k++)
			{
				APiCube* Cube = GetWorld()->SpawnActor<APiCube>(CubeClass);

				Cube->SetupPuzzlePosition({i, j, k});
				Cube->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				Cube->SetActorRelativeLocation({
					100*(i - PuzzleSize.X / 2.0f + 0.5f),
					100*(j - PuzzleSize.Y / 2.0f + 0.5f),
					100*(k - PuzzleSize.Z / 2.0f + 0.5f)
				});
				
			}
		}
	}
}
