// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Picross3D/GameLogic/PiPuzzle.h"
#include "PiPlayerController.generated.h"

// TODO should i define this here?
UENUM()
enum class InputState: uint8
{
	//TODO deleted an emun here and mapping in UE is weird now
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

private:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Pi Input", meta=(BlueprintProtected))
	void OnAction();
	
	UFUNCTION(BlueprintCallable, Category="Pi Input", meta=(BlueprintProtected))
	void XRotation(float AxisValue);

	UFUNCTION(BlueprintCallable, Category="Pi Input", meta=(BlueprintProtected))
	void YRotation(float AxisValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pi Input", meta = (AllowPrivateAccess = "true"))
	InputState CurrentState = InputState::MOVEMENT;

	DECLARE_DELEGATE_TwoParams(SetInputStateDelegate, InputState, bool);
	UFUNCTION(BlueprintCallable, Category="Pi Input", meta=(BlueprintProtected))
	void ProcessStateChange(InputState NewState, bool Released = false);

	//TODO can i leave this to be GC?
	//In theory if it gets CG, i can get access to the newer one
	// Or should i say UPROPERTY() with TWeakObjectPtr
	APiGameMode* GameMode;
	bool GetGameMode();

	//TODO can i leave this to be GC?
	//In theory if it gets CG, i can get access to the newer one
	APiCamera* PiCamera;
	bool GetPiCamera();


	DECLARE_DELEGATE_TwoParams(ChangeVisibilityAxisDelegate, EDirection, bool);
	UFUNCTION(BlueprintCallable, Category="Pi Input", meta=(BlueprintProtected))
	void ChangeVisibilityAxis(EDirection AxisValue, bool show);
};
