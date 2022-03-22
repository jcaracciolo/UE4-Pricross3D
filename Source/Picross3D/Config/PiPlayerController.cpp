// Fill out your copyright notice in the Description page of Project Settings.


#include "PiPlayerController.h"

#include "PiGameMode.h"
#include "Picross3D/GameLogic/PiPuzzle.h"

DEFINE_LOG_CATEGORY_STATIC(LogPiPuzzle, Log, All);

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
	case InputState::DEFAULT:
		CurrentState = Released ? InputState::DEFAULT : NewState;
		break;
	case InputState::MOVEMENT:
		CurrentState = NewState == InputState::MOVEMENT && Released ? InputState::DEFAULT : NewState;
		break;
	case InputState::BREAKING:
		CurrentState = NewState == InputState::BREAKING && Released ? InputState::DEFAULT : InputState::BREAKING;
		break;
	case InputState::PAINTING:
		CurrentState = NewState == InputState::PAINTING && Released ? InputState::DEFAULT : InputState::PAINTING;
		break;
	}
}

void APiPlayerController::OnAction()
{
	UE_LOG(LogPiPuzzle, Error, TEXT("CLICK %d"), CurrentState);

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
			UE_LOG(LogPiPuzzle, Error, TEXT("GameMode failed to be casted to APIGameMode"));
			return;
		}
	}

	if (CurrentState == InputState::MOVEMENT)
	{
		GameMode->GetCurrentPuzzle()->AddActorLocalRotation({0, AxisValue, 0});
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
			UE_LOG(LogPiPuzzle, Error, TEXT("GameMode failed to be casted to APIGameMode"));
			return;
		}
	}

	if (CurrentState == InputState::MOVEMENT)
	{
		GameMode->GetCurrentPuzzle()->AddActorWorldRotation({AxisValue, 0, 0});
	}
}
