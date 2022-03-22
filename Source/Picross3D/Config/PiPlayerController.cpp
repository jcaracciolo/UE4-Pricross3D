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

	InputComponent->BindAxis("XRotation", this, &APiPlayerController::XRotation);
	InputComponent->BindAxis("YRotation", this, &APiPlayerController::YRotation);
	InputComponent->BindAction<SetInputStateDelegate>("TouchScreen", IE_Pressed, this,
	                                                  &APiPlayerController::ProcessStateChange,
	                                                  InputState::MOVEMENT, false
	);
	InputComponent->BindAction<SetInputStateDelegate>("TouchScreen", IE_Released, this,
	                                                  &APiPlayerController::ProcessStateChange,
	                                                  InputState::MOVEMENT, true
	);
	InputComponent->BindAction<SetInputStateDelegate>("BreakToggle", IE_Pressed, this,
	                                                  &APiPlayerController::ProcessStateChange,
	                                                  InputState::BREAKING, false
	);
	InputComponent->BindAction<SetInputStateDelegate>("BreakToggle", IE_Released, this,
	                                                  &APiPlayerController::ProcessStateChange,
	                                                  InputState::BREAKING, true
	);
	InputComponent->BindAction<SetInputStateDelegate>("PaintToggle", IE_Pressed, this,
	                                                  &APiPlayerController::ProcessStateChange,
	                                                  InputState::PAINTING, false
	);
	InputComponent->BindAction<SetInputStateDelegate>("PaintToggle", IE_Released, this,
	                                                  &APiPlayerController::ProcessStateChange,
	                                                  InputState::PAINTING, true
	);
	InputComponent->BindAction("TouchScreen", IE_Pressed, this,
	                           &APiPlayerController::OnAction
	);
}

void APiPlayerController::ProcessStateChange(InputState NewState, bool Released)
{
	switch (CurrentState)
	{
	case InputState::MOVEMENT:
		CurrentState = NewState == InputState::MOVEMENT && Released ? InputState::MOVEMENT : NewState;
		break;
	case InputState::BREAKING:
		CurrentState = NewState == InputState::BREAKING && Released ? InputState::MOVEMENT : InputState::BREAKING;
		break;
	case InputState::PAINTING:
		CurrentState = NewState == InputState::PAINTING && Released ? InputState::MOVEMENT : InputState::PAINTING;
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

void APiPlayerController::OnAction()
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
			if (CurrentState == InputState::BREAKING)
			{
				GameMode->GetCurrentPuzzle()->Break(Cube);

				if(GameMode->GetCurrentPuzzle()->IsCompleted())
				{
					UE_LOG(LogPiPlayerController, Error, TEXT("YOU WON!!!!"));
					
				}
			}
			else if (CurrentState == InputState::PAINTING)
			{
				GameMode->GetCurrentPuzzle()->Paint(Cube);
			}
		}
	}
}


void APiPlayerController::XRotation(float AxisValue)
{
	if (!GetGameMode()) { return; }

	if (CurrentState == InputState::MOVEMENT)
	{
		GameMode->GetCurrentPuzzle()->AddActorLocalRotation({0, AxisValue, 0});
	}
}

void APiPlayerController::YRotation(float AxisValue)
{
	if (!GetGameMode()) { return; }

	if (CurrentState == InputState::MOVEMENT)
	{
		GameMode->GetCurrentPuzzle()->AddActorWorldRotation({AxisValue, 0, 0});
	}
}
