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
	Pawn->SetupPuzzleSize(PuzzleSize.GetMax());

	ForEachComponent<UChildActorComponent>(false, [this](UChildActorComponent* ChildActor)
	{
		//TODO can i do an if like this or should i check ISValid ?
		if (APiCube* Cube = Cast<APiCube>(ChildActor->GetChildActor()))
		{
			Cubes.Add(Cube);
		}
	});


	FRandomStream stream = FRandomStream(11);
	for (auto Cube : Cubes)
	{
		FIntVector Position = Cube->GetPuzzlePosition();
		if (Cube->Hints.X.amount == EHintAmout::NONE && Cube->Hints.Y.amount == EHintAmout::NONE && Cube->Hints.Z.amount == EHintAmout::NONE)
		{
			auto Rand = stream.FRand();

				if(Rand < 0.3)
				{
					SetLineHints(EDirection::X, Position.Y, Position.Z);
				} else if(Rand < 0.66)
				{
					SetLineHints(EDirection::Y, Position.X, Position.Z);
				} else
				{
					SetLineHints(EDirection::Z, Position.X, Position.Y);

				}

			if(stream.FRand() < 0.3)
			{
				Rand = stream.FRand();
				if(Rand < 0.6)
				{
					SetLineHints(EDirection::X, Position.Y, Position.Z);
				} else if(Rand < 0.66)
				{
					SetLineHints(EDirection::Y, Position.X, Position.Z);
				} else
				{
					SetLineHints(EDirection::Z, Position.X, Position.Y);

				}
			}

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
					100 * (i - PuzzleSize.X / 2.0f + 0.5f),
					100 * (j - PuzzleSize.Y / 2.0f + 0.5f),
					100 * (k - PuzzleSize.Z / 2.0f + 0.5f)
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
				Cube->SetXHint({0, EHintAmout::NONE});
				Cube->SetYHint({0, EHintAmout::NONE});
				Cube->SetZHint({0, EHintAmout::NONE});
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
	for (auto Cube : Cubes)
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
	for (auto Cube : Cubes)
	{
		Cube->SetSolutionColor();
	}
}

template <typename Func>
void APiPuzzle::ForEachInDirection(const EDirection Axis, const int OtherAxis1, const int OtherAxis2, Func F)
{
	for (auto Cube : Cubes)
	{
		FIntVector Position = Cube->GetPuzzlePosition();
		int OtherAxis1Value, OtherAxis2Value;
		switch (Axis)
		{
		case EDirection::X:
			OtherAxis1Value = Position.Y, OtherAxis2Value = Position.Z;
			break;
		case EDirection::Y:
			OtherAxis1Value = Position.X, OtherAxis2Value = Position.Z;
			break;
		case EDirection::Z:
			OtherAxis1Value = Position.X, OtherAxis2Value = Position.Y;
			break;
		}

		if (OtherAxis1 == OtherAxis1Value && OtherAxis2 == OtherAxis2Value)
		{
			F(Cube);
		}
	}
}


int APiPuzzle::GetCurrentSize(const EDirection Axis, const int OtherAxis1, const int OtherAxis2)
{
	int Size = 0;
	ForEachInDirection(Axis, OtherAxis1, OtherAxis2, [this, &Size](const APiCube* Cube)
	{
		if (IsValid(Cube))
		{
			Size++;
		}
	});

	return Size;
}


FHint APiPuzzle::GetCubesHint(const EDirection Axis, const int OtherAxis1, const int OtherAxis2)
{
	int Solutions[20];
	int Index = 0;
	ForEachInDirection(Axis, OtherAxis1, OtherAxis2, [this, &Solutions, &Index, Axis](const APiCube* Cube)
	{
		if (IsValid(Cube) && Cube->IsSolution())
		{
			switch (Axis)
			{
			case EDirection::X:
				Solutions[Index] = Cube->GetPuzzlePosition().X;
				Index++;
				break;
			case EDirection::Y:
				Solutions[Index] = Cube->GetPuzzlePosition().Y;
				Index++;
				break;
			case EDirection::Z:
				Solutions[Index] = Cube->GetPuzzlePosition().Z;
				Index++;
				break;
			};
		}
	});

	int Max = -1, Min = MAX_int8;

	for (int i = 0; i < Index; ++i)
	{
		if (Solutions[i] > Max)
		{
			Max = Solutions[i];
		}
		if (Solutions[i] < Min)
		{
			Min = Solutions[i];
		}
	}

	int Spaces = 0;
	bool inSpace = false;
	for (int i = Min; i < Max; ++i)
	{
		bool bFound = false;
		for (int j = 0; j < Index; ++j)
		{
			if (Solutions[j] == i)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound && !inSpace)
		{
			inSpace = true;
			Spaces++;
		}
		else if (bFound)
		{
			inSpace = false;
		}
	}

	if (Spaces == 0)
	{
		return {Index, EHintAmout::ONCE};
	}
	if (Spaces == 1)
	{
		return {Index, EHintAmout::TWICE};
	}

	return {Index, EHintAmout::MANY};
}

void APiPuzzle::SetLineHints(const EDirection Axis, const int OtherAxis1, const int OtherAxis2)
{
	FHint Hint{GetCubesHint(Axis, OtherAxis1, OtherAxis2)};
	ForEachInDirection(Axis, OtherAxis1, OtherAxis2, [this, Axis, Hint](APiCube* Cube)
	{
		if (IsValid(Cube))
		{
			switch (Axis)
			{
			case EDirection::X:
				Cube->SetXHint(Hint);
				break;
			case EDirection::Y:
				Cube->SetYHint(Hint);
				break;
			case EDirection::Z:
				Cube->SetZHint(Hint);
				break;
			}
		}
	});
}

void APiPuzzle::HideAxis(EDirection Axis)
{
	UE_LOG(LogPiPuzzle, Error, TEXT("Hiding AXIS %d"), Axis);


	if (AxisHidden.Direction != Axis)
	{
		AxisHidden = {Axis, 0};
		for (auto Cube : Cubes)
		{
			Cube->SetActorHiddenInGame(false);
			Cube->SetActorEnableCollision(!Cube->IsHidden());
		}
	}


	uint8 SizeInSecondDirection = Axis == EDirection::Z ? PuzzleSize.Y : PuzzleSize.Z;
	for (int i = 0; i < SizeInSecondDirection; ++i)
	{
		if (Axis != EDirection::Z)
		{
			ForEachInDirection(Axis, AxisHidden.Amount, i, [this](APiCube* Cube)
			{
				Cube->SetActorHiddenInGame(true);
				Cube->SetActorEnableCollision(!Cube->IsHidden());
			});
		}
		else
		{
			ForEachInDirection(EDirection::X, i, PuzzleSize.Z - 1 - AxisHidden.Amount, [this](APiCube* Cube)
			{
				Cube->SetActorHiddenInGame(true);
				Cube->SetActorEnableCollision(!Cube->IsHidden());
			});
		}
	}

	AxisHidden.Amount++;
}

void APiPuzzle::ShowAxis(EDirection Axis)
{
	UE_LOG(LogPiPuzzle, Error, TEXT("Showing AXIS %d"), Axis);
	if (AxisHidden.Direction != Axis)
	{
		AxisHidden = {Axis, 0};
		for (auto Cube : Cubes)
		{
			Cube->SetActorHiddenInGame(false);
			Cube->SetActorEnableCollision(!Cube->IsHidden());
		}
	}

	if (AxisHidden.Amount > 0)
	{
		AxisHidden.Amount--;
		uint8 SizeInSecondDirection = Axis == EDirection::Z ? PuzzleSize.Y : PuzzleSize.Z;
		for (int i = 0; i < SizeInSecondDirection; ++i)
		{
			if (Axis != EDirection::Z)
			{
				ForEachInDirection(Axis, AxisHidden.Amount, i, [this](APiCube* Cube)
				{
					Cube->SetActorHiddenInGame(false);
					Cube->SetActorEnableCollision(!Cube->IsHidden());
				});
			}
			else
			{
				ForEachInDirection(EDirection::X, i, PuzzleSize.Z - 1 - AxisHidden.Amount, [this](APiCube* Cube)
				{
					Cube->SetActorHiddenInGame(false);
					Cube->SetActorEnableCollision(!Cube->IsHidden());
				});
			}
		}
	}
}
