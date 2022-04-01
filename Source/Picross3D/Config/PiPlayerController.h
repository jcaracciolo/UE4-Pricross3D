// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Picross3D/Utils/Definitions.h"
#include "PiPlayerController.generated.h"

// TODO should i define this here?
UENUM()
enum class EInputState: uint8
{
	MOVEMENT = 1,
	BREAKING,
	PAINTING,
};

class APiGameMode;
class APiCamera;
/**
 * 
 */
UCLASS()
class PICROSS3D_API APiPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pi Input", meta = (AllowPrivateAccess = "true"))
	EInputState CurrentState = EInputState::MOVEMENT;
	
	//TODO can i leave this to be GC?
	//In theory if it gets CG, i can get access to the newer one
	// Or should i say UPROPERTY() with TWeakObjectPtr
	APiGameMode* GameMode;
	bool GetGameMode();

	//TODO can i leave this to be GC?
	//In theory if it gets CG, i can get access to the newer one
	APiCamera* PiCamera;
	bool GetPiCamera();
	
	virtual void BeginPlay() override;
	
	void OnScreenAction();
	void RotateXAxis(float AxisValue);
	void RotateYAxis(float AxisValue);
	
	DECLARE_DELEGATE_TwoParams(SetInputStateDelegate, EInputState, bool);
	void InputStateChange(EInputState NewState, bool Released = false);

	DECLARE_DELEGATE_TwoParams(ChangeVisibilityAxisDelegate, EPiAxis, bool);
	void ChangeAxisVisibility(EPiAxis Axis, bool show);
};
