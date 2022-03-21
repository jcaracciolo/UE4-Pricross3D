// Fill out your copyright notice in the Description page of Project Settings.


#include "PiPlayerController.h"

#include "PiGameMode.h"
#include "Picross3D/GameLogic/PiPuzzle.h"

void APiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputComponent->BindAxis("XRotation", this, &APiPlayerController::XRotation);
	InputComponent->BindAxis("YRotation", this, &APiPlayerController::YRotation);
	InputComponent->BindAction<SetInputStateDelegate>("TouchScreen", IE_Pressed, this,
	                                                  &APiPlayerController::SetInputState, InputState::MOVEMENT);
	InputComponent->BindAction<SetInputStateDelegate>("TouchScreen", IE_Released, this,
	                                                  &APiPlayerController::SetInputState, InputState::DEFAULT);
}

void APiPlayerController::XRotation(float AxisValue)
{
	//TODO is this the correct way of retrieving a GameMode
	//TODO Is this neccesary? Should i do IsValid on every Input?
	if (!IsValid(GameMode))
	{
		GameMode = Cast<APiGameMode>(GetWorld()->GetAuthGameMode());
		if (!IsValid(GameMode))
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerController: GameMode failed to be casted to APIGameMode"));
			return;
		}
	}

	if (CurrentState == InputState::MOVEMENT)
	{
		GameMode->GetCurrentPuzzle()->AddActorWorldRotation({0, AxisValue, 0});
	}
}

void APiPlayerController::YRotation(float AxisValue)
{
	//TODO is this the correct way of retrieving a GameMode
	//TODO Is this neccesary? Should i do IsValid on every Input?
	if (!IsValid(GameMode))
	{
		GameMode = Cast<APiGameMode>(GetWorld()->GetAuthGameMode());
		if (!IsValid(GameMode))
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode failed to be casted to APIGameMode"));
			return;
		}
	}

	if (CurrentState == InputState::MOVEMENT)
	{
		GameMode->GetCurrentPuzzle()->AddActorWorldRotation({AxisValue, 0, 0});
	}
}
