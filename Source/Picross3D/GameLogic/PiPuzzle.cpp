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

	ForEachComponent<UChildActorComponent>(false, [this] (UChildActorComponent* ChildActor){
		//TODO can i do an if like this or should i check ISValid ?
		if (APiCube* Cube = Cast<APiCube>(ChildActor->GetChildActor()))
		{
			Cubes.Add(Cube);
		}
	});


	for(auto Cube: Cubes)
	{
		FIntVector Position = Cube->GetPuzzlePosition();
		Cube->SetXHint(GetCubesSum(EDirection::X, Position.Y, Position.Z));
		Cube->SetYHint(GetCubesSum(EDirection::Y, Position.X, Position.Z));
		Cube->SetZHint(GetCubesSum(EDirection::Z, Position.X, Position.Y));
	}
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

void APiPuzzle::Break(APiCube* Cube)
{
	FIntVector PuzzlePosition = Cube->GetPuzzlePosition();
	UE_LOG(LogPiPuzzle, Log, TEXT("Breaking Cube %d,%d,%d"),PuzzlePosition.X, PuzzlePosition.Y, PuzzlePosition.Z);
	if(!Cube->IsSolution() && !Cube->IsPainted())
	{
		Cubes.Remove(Cube);
		Cube->Destroy();
		GetCurrentSize(EDirection::X, Cube->GetPuzzlePosition().Y, Cube->GetPuzzlePosition().Z);
		GetCurrentSize(EDirection::Y, Cube->GetPuzzlePosition().X, Cube->GetPuzzlePosition().Z);
		GetCurrentSize(EDirection::Z, Cube->GetPuzzlePosition().X, Cube->GetPuzzlePosition().Y);

	}
}

void APiPuzzle::Paint(APiCube* Cube)
{
	Cube->TogglePaint();
}

bool APiPuzzle::IsCompleted()
{
	for(auto Cube: Cubes)
	{
		FIntVector PuzzlePosition = Cube->GetPuzzlePosition();
		UE_LOG(LogPiPuzzle, Log, TEXT("Checking Cube %d,%d,%d"),PuzzlePosition.X, PuzzlePosition.Y, PuzzlePosition.Z);

		if(IsValid(Cube) && !Cube->IsSolution())
		{
			return false;
		}
	}

	return true;
}

int APiPuzzle::GetCurrentSize(const EDirection Axis, const int OtherAxis1, const int OtherAxis2)
{
	int Size = 0;
	for(auto Cube: Cubes)
	{
		FIntVector Position = Cube->GetPuzzlePosition();
		int OtherAxis1Value, OtherAxis2Value;
		switch (Axis)
		{
		case EDirection::X:
			OtherAxis1Value=Position.Y, OtherAxis2Value=Position.Z;
			break;
		case EDirection::Y:
			OtherAxis1Value=Position.X, OtherAxis2Value=Position.Z;
			break;
		case EDirection::Z:
			OtherAxis1Value=Position.X, OtherAxis2Value=Position.Y;
			break;
		}

		if(OtherAxis1 == OtherAxis1Value && OtherAxis2 == OtherAxis2Value && IsValid(Cube))
		{
			Size++;
		}
	}

	return Size;
}


int APiPuzzle::GetCubesSum(const EDirection Axis, const int OtherAxis1, const int OtherAxis2)
{
	int SolutionsOnRow = 0;
	for(auto Cube: Cubes)
	{
		FIntVector Position = Cube->GetPuzzlePosition();
		int OtherAxis1Value, OtherAxis2Value;
		switch (Axis)
		{
		case EDirection::X:
			OtherAxis1Value=Position.Y, OtherAxis2Value=Position.Z;
			break;
		case EDirection::Y:
			OtherAxis1Value=Position.X, OtherAxis2Value=Position.Z;
			break;
		case EDirection::Z:
			OtherAxis1Value=Position.X, OtherAxis2Value=Position.Y;
			break;
		}

		if(OtherAxis1 == OtherAxis1Value && OtherAxis2 == OtherAxis2Value && Cube->IsSolution())
		{
			SolutionsOnRow++;
		}
	}

	return SolutionsOnRow;
}
