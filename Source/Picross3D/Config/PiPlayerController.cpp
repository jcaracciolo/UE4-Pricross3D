// Fill out your copyright notice in the Description page of Project Settings.


#include "PiPlayerController.h"

#include "DrawDebugHelpers.h"
#include "PiCamera.h"
#include "PiGameMode.h"
#include "Picross3D/GameLogic/PiPuzzle.h"

DEFINE_LOG_CATEGORY_STATIC(LogPiPlayerController, Log, All);

void APiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputComponent->BindAxis("XRotation", this, &APiPlayerController::RotateXAxis);
	InputComponent->BindAxis("YRotation", this, &APiPlayerController::RotateYAxis);
	
	InputComponent->BindAction<SetInputStateDelegate>("TouchScreen", IE_Pressed, this,
	                                                  &APiPlayerController::InputStateChange,
	                                                  EInputState::MOVEMENT, false
	);
	InputComponent->BindAction<SetInputStateDelegate>("TouchScreen", IE_Released, this,
	                                                  &APiPlayerController::InputStateChange,
	                                                  EInputState::MOVEMENT, true
	);
	InputComponent->BindAction<SetInputStateDelegate>("BreakToggle", IE_Pressed, this,
	                                                  &APiPlayerController::InputStateChange,
	                                                  EInputState::BREAKING, false
	);
	InputComponent->BindAction<SetInputStateDelegate>("BreakToggle", IE_Released, this,
	                                                  &APiPlayerController::InputStateChange,
	                                                  EInputState::BREAKING, true
	);
	InputComponent->BindAction<SetInputStateDelegate>("PaintToggle", IE_Pressed, this,
	                                                  &APiPlayerController::InputStateChange,
	                                                  EInputState::PAINTING, false
	);
	InputComponent->BindAction<SetInputStateDelegate>("PaintToggle", IE_Released, this,
	                                                  &APiPlayerController::InputStateChange,
	                                                  EInputState::PAINTING, true
	);
	InputComponent->BindAction("TouchScreen", IE_Pressed, this,
	                           &APiPlayerController::OnScreenAction
	);

	InputComponent->BindAction<ChangeVisibilityAxisDelegate>("HideX", IE_Released, this,
	                                                         &APiPlayerController::ChangeAxisVisibility,
	                                                         EPiAxis::X, false
	);

	InputComponent->BindAction<ChangeVisibilityAxisDelegate>("ShowX", IE_Released, this,
	                                                         &APiPlayerController::ChangeAxisVisibility,
	                                                         EPiAxis::X, true
	);

	InputComponent->BindAction<ChangeVisibilityAxisDelegate>("HideY", IE_Released, this,
	                                                         &APiPlayerController::ChangeAxisVisibility,
	                                                         EPiAxis::Y, false
	);
	InputComponent->BindAction<ChangeVisibilityAxisDelegate>("ShowY", IE_Released, this,
	                                                         &APiPlayerController::ChangeAxisVisibility,
	                                                         EPiAxis::Y, true
	);
	InputComponent->BindAction<ChangeVisibilityAxisDelegate>("HideZ", IE_Released, this,
	                                                         &APiPlayerController::ChangeAxisVisibility,
	                                                         EPiAxis::Z, false
	);
	InputComponent->BindAction<ChangeVisibilityAxisDelegate>("ShowZ", IE_Released, this,
	                                                         &APiPlayerController::ChangeAxisVisibility,
	                                                         EPiAxis::Z, true
	);
}

void APiPlayerController::InputStateChange(EInputState NewState, bool Released)
{
	switch (CurrentState)
	{
	case EInputState::MOVEMENT:
		CurrentState = !Released ? NewState : EInputState::MOVEMENT;
		break;
	case EInputState::BREAKING:
		CurrentState = NewState == EInputState::BREAKING && Released ? EInputState::MOVEMENT : EInputState::BREAKING;
		break;
	case EInputState::PAINTING:
		CurrentState = NewState == EInputState::PAINTING && Released ? EInputState::MOVEMENT : EInputState::PAINTING;
		break;
	}
}

bool APiPlayerController::GetGameMode()
{
	//TODO is this the correct way of retrieving a GameMode
	if (!IsValid(GameMode))
	{
		GameMode = Cast<APiGameMode>(GetWorld()->GetAuthGameMode());
		if (!IsValid(GameMode))
		{
			UE_LOG(LogPiPlayerController, Error, TEXT("GameMode failed to be casted to APIGameMode"));
			return false;
		}
	}

	return true;
}

bool APiPlayerController::GetPiCamera()
{
	//TODO is this the correct way of retrieving a GameMode
	PiCamera = Cast<APiCamera>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!IsValid(PiCamera))
	{
		UE_LOG(LogPiPlayerController, Error, TEXT("Camera failed to be casted to APiCamera"));
		return false;
	}

	return true;
}

void APiPlayerController::OnScreenAction()
{
	if (!GetGameMode() || !GetPiCamera()) { return; }

	FHitResult OutHit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, OutHit))
	{
		//TODO is this how you cast out of a Hit?
		auto Cube = Cast<APiCube>(OutHit.GetActor());

		//TODO here IsValid is not required because Cube would be null (that's how GetActor works
		// since it has a WeakPtr inside). Just null check then?
		if (IsValid(Cube))
		{
			if (CurrentState == EInputState::BREAKING)
			{
				GameMode->GetCurrentPuzzle()->Break(Cube);

				if (GameMode->GetCurrentPuzzle()->IsCompleted())
				{
					GameMode->GetCurrentPuzzle()->StartCompletedAnimation();
				}
			}
			else if (CurrentState == EInputState::PAINTING)
			{
				GameMode->GetCurrentPuzzle()->Paint(Cube);
			}
		}
	}
}


void APiPlayerController::RotateXAxis(float AxisValue)
{
	if (!GetGameMode()) { return; }

	if (CurrentState == EInputState::MOVEMENT)
	{
		GameMode->GetCurrentPuzzle()->AddActorLocalRotation({0, AxisValue, 0});
	}
}

void APiPlayerController::RotateYAxis(float AxisValue)
{
	if (!GetGameMode()) { return; }

	if (CurrentState == EInputState::MOVEMENT)
	{
		GameMode->GetCurrentPuzzle()->AddActorWorldRotation({AxisValue, 0, 0});
	}
}

void APiPlayerController::ChangeAxisVisibility(EPiAxis AxisValue, bool show)
{
	if (!GetGameMode()) { return; }

	if (show)
	{
		GameMode->GetCurrentPuzzle()->ShowAxis(AxisValue);
	}
	else
	{
		GameMode->GetCurrentPuzzle()->HideAxis(AxisValue);
	}
}
