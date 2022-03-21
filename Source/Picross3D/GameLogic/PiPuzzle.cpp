// Fill out your copyright notice in the Description page of Project Settings.


#include "PiPuzzle.h"

#include "PiCube.h"
#include "Picross3D/Config/PiGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogPiPuzzle, Log, All);

// Sets default values
APiPuzzle::APiPuzzle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APiPuzzle::BeginPlay()
{
	Super::BeginPlay();

	auto GameMode = Cast<APiGameMode>(GetWorld()->GetAuthGameMode());
	if(!IsValid(GameMode))
	{
		UE_LOG(LogPiPuzzle, Error, TEXT("GameMode failed to be casted to APIGameMode"));
		return;
	}


	UE_LOG(LogPiPuzzle, Log, TEXT("Creating Cubes for %d,%d,%d"),Width,Height,Depth);

	for (auto i = 0; i < Width; i++)
	{
		for (auto j = 0; j < Height; j++)
		{
			for (auto k = 0; k < Depth; k++)
			{
				APiCube* Cube = GetWorld()->SpawnActor<APiCube>(GameMode->GetCubeClass());

				Cube->SetupPuzzlePosition({i, j, k});
				Cube->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				Cube->SetActorRelativeLocation({
					100*(i - Width / 2.0f + 0.5f),
					100*(j - Height / 2.0f + 0.5f),
					100*(k - Depth / 2.0f + 0.5f)
				});
				
			}
		}
	}
}
