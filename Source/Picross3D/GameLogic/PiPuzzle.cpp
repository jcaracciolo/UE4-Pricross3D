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
	if (!IsValid(Pawn))
	{
		UE_LOG(LogPiPuzzle, Error, TEXT("Pawn failed to be casted to APiCamera"));
		return;
	}
	Pawn->FitPuzzleInViewport(PuzzleSize.GetMax());

	ForEachComponent<UChildActorComponent>(false, [this](UChildActorComponent* ChildActor)
	{
		//TODO can i do an if like this or should i check ISValid ?
		if (APiCube* Cube = Cast<APiCube>(ChildActor->GetChildActor()))
		{
			CubesArray.Add(Cube);
		}
	});

	CubesMatrix.SetupMatrix(PuzzleSize.X, PuzzleSize.Y, PuzzleSize.Z);


	for (int Y = 0; Y < PuzzleSize.Y; ++Y)
	{
		for (int Z = 0; Z < PuzzleSize.Z; ++Z)
		{
			SetLineHints({0, Y, Z}, EPiAxis::X);
		}
	}
	for (int X = 0; X < PuzzleSize.X; ++X)
	{
		for (int Z = 0; Z < PuzzleSize.Z; ++Z)
		{
			SetLineHints({X, 0, Z}, EPiAxis::Y);
		}
	}
	for (int X = 0; X < PuzzleSize.X; ++X)
	{
		for (int Y = 0; Y < PuzzleSize.Y; ++Y)
		{
			SetLineHints({X, Y, 0}, EPiAxis::Z);
		}
	}
}

// Called when the game starts or when spawned
void APiPuzzle::GenerateCubes()
{
	UE_LOG(LogPiPuzzle, Log, TEXT("Creating Cubes for %d,%d,%d"), PuzzleSize.X, PuzzleSize.Y, PuzzleSize.Z);

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
					100 * (i - PuzzleSize.X / 2.0f + 0.5f), 100 * (j - PuzzleSize.Y / 2.0f + 0.5f), 100 * (k - PuzzleSize.Z / 2.0f + 0.5f)
				});
			}
		}
	}
}

void APiPuzzle::ShowSolution()
{
	ForEachComponent<UChildActorComponent>(false, [this](UChildActorComponent* ChildActor)
	{
		//TODO can i do an if like this or should i check ISValid ?
		if (APiCube* Cube = Cast<APiCube>(ChildActor->GetChildActor()))
		{
			if (!Cube->IsSolution())
			{
				Cube->SetHidden(true);
			}
			else
			{
				Cube->SetSolutionColor();
			}
		}
	});
}

void APiPuzzle::Break(APiCube* Cube)
{
	FIntVector PuzzlePosition = Cube->GetPuzzlePosition();
	UE_LOG(LogPiPuzzle, Log, TEXT("Breaking Cube %d,%d,%d"), PuzzlePosition.X, PuzzlePosition.Y, PuzzlePosition.Z);
	if (!Cube->IsSolution() && !Cube->IsPainted())
	{
		CubesArray[CubesArray.IndexOfByKey(Cube)] = nullptr;
		Cube->Destroy();
		// GetCurrentSize(Cube, EPiAxis::X);
		// GetCurrentSize(Cube, EPiAxis::Y);
		// GetCurrentSize(Cube, EPiAxis::Z);
	}
}

void APiPuzzle::Paint(APiCube* Cube)
{
	Cube->TogglePaint();
}

bool APiPuzzle::IsCompleted()
{
	for (auto Cube : CubesArray)
	{
		FIntVector PuzzlePosition = Cube->GetPuzzlePosition();
		UE_LOG(LogPiPuzzle, Log, TEXT("Checking Cube %d,%d,%d"), PuzzlePosition.X, PuzzlePosition.Y, PuzzlePosition.Z);

		if (IsValid(Cube) && !Cube->IsSolution())
		{
			return false;
		}
	}

	return true;
}

void APiPuzzle::StartCompletedAnimation_Implementation()
{
	for (auto Cube : CubesArray)
	{
		Cube->SetSolutionColor();
	}
}


int APiPuzzle::GetCurrentSize(const FIntVector From, const EPiAxis Axis) const
{
	int Size = 0;
	for (auto It = CubesMatrix.cbegin(From, Axis); It != CubesMatrix.cend(); ++It)
	{
		const APiCube* Cube = *It;
		if (IsValid(Cube))
		{
			Size++;
		}
	}

	return Size;
}


FHint APiPuzzle::GetCubesHint(const FIntVector From, const EPiAxis Axis) const
{
	int Spaces = -1; // Starting at -1 because outside of the cube is space
	bool bInSpace = true;

	uint8 Solutions = 0;
	for (auto It = CubesMatrix.cbegin(From, Axis); It != CubesMatrix.cend(); ++It)
	{
		const APiCube* Cube = *It;
		if (IsValid(Cube))
		{
			if (Cube->IsSolution())
			{
				Solutions++;
				if (bInSpace)
				{
					bInSpace = false;
					Spaces++;
				}
			}
			else
			{
				bInSpace = true;
			}
		}
	}

	if (Spaces == 0 || Spaces == -1)
	{
		return {Solutions, EHintAppearance::ONCE};
	}
	if (Spaces == 1)
	{
		return {Solutions, EHintAppearance::TWICE};
	}

	return {Solutions, EHintAppearance::MANY};
}

void APiPuzzle::SetLineHints(const FIntVector From, const EPiAxis Axis) const
{
	FHint Hint{GetCubesHint(From, Axis)};
	for (auto It = CubesMatrix.begin(From, Axis); It != CubesMatrix.end(); ++It)
	{
		APiCube* Cube = *It;
		if (IsValid(Cube))
		{
			switch (Axis)
			{
			case EPiAxis::X:
				Cube->SetXHint(Hint);
				break;
			case EPiAxis::Y:
				Cube->SetYHint(Hint);
				break;
			case EPiAxis::Z:
				Cube->SetZHint(Hint);
				break;
			}
		}
	}
}

void APiPuzzle::HideAxis(EPiAxis Axis)
{
	UE_LOG(LogPiPuzzle, Error, TEXT("Hiding AXIS %d"), Axis);


	if (AxisHidden.Axis != Axis)
	{
		AxisHidden = {Axis, 0};
		for (auto Cube : CubesArray)
		{
			Cube->SetActorHiddenInGame(false);
			Cube->SetActorEnableCollision(!Cube->IsHidden());
		}
	}


	// uint8 SizeInSecondAxis = Axis == EPiAxis::Z ? PuzzleSize.Y : PuzzleSize.Z;
	// for (int i = 0; i < SizeInSecondAxis; ++i)
	// {
	// 	if (Axis != EPiAxis::Z)
	// 	{
	// 		ForEachInAxis(CubesMatrix.GeAxis, AxisHidden.Amount, i, [this](APiCube* Cube)
	// 		{
	// 			Cube->SetActorHiddenInGame(true);
	// 			Cube->SetActorEnableCollision(!Cube->IsHidden());
	// 		});
	// 	}
	// 	else
	// 	{
	// 		ForEachInAxis(EPiAxis::X, i, PuzzleSize.Z - 1 - AxisHidden.Amount, [this](APiCube* Cube)
	// 		{
	// 			Cube->SetActorHiddenInGame(true);
	// 			Cube->SetActorEnableCollision(!Cube->IsHidden());
	// 		});
	// 	}
	// }

	AxisHidden.Amount++;
}

void APiPuzzle::ShowAxis(EPiAxis Axis)
{
	UE_LOG(LogPiPuzzle, Error, TEXT("Showing AXIS %d"), Axis);
	if (AxisHidden.Axis != Axis)
	{
		AxisHidden = {Axis, 0};
		for (auto Cube : CubesArray)
		{
			Cube->SetActorHiddenInGame(false);
			Cube->SetActorEnableCollision(!Cube->IsHidden());
		}
	}

	// if (AxisHidden.Amount > 0)
	// {
	// 	AxisHidden.Amount--;
	// 	uint8 SizeInSecondAxis = Axis == EPiAxis::Z ? PuzzleSize.Y : PuzzleSize.Z;
	// 	for (int i = 0; i < SizeInSecondAxis; ++i)
	// 	{
	// 		if (Axis != EPiAxis::Z)
	// 		{
	// 			ForEachInAxis(Axis, AxisHidden.Amount, i, [this](APiCube* Cube)
	// 			{
	// 				Cube->SetActorHiddenInGame(false);
	// 				Cube->SetActorEnableCollision(!Cube->IsHidden());
	// 			});
	// 		}
	// 		else
	// 		{
	// 			ForEachInAxis(EPiAxis::X, i, PuzzleSize.Z - 1 - AxisHidden.Amount, [this](APiCube* Cube)
	// 			{
	// 				Cube->SetActorHiddenInGame(false);
	// 				Cube->SetActorEnableCollision(!Cube->IsHidden());
	// 			});
	// 		}
	// 	}
	// }
}
