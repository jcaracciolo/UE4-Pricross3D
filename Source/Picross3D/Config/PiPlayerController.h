// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PiGameMode.h"
#include "GameFramework/PlayerController.h"
#include "PiPlayerController.generated.h"

// TODO should i define this here?
UENUM()
enum class InputState: uint8
{
	DEFAULT,
	MOVEMENT,
	BREAKING,
	PAINTING,
  };

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
		void XRotation(float AxisValue);

		UFUNCTION(BlueprintCallable, Category="Pi Input", meta=(BlueprintProtected))
		void YRotation(float AxisValue);

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pi Input", meta = (AllowPrivateAccess = "true"))
		InputState CurrentState = InputState::DEFAULT;

		DECLARE_DELEGATE_OneParam(SetInputStateDelegate, InputState);
		UFUNCTION(BlueprintCallable, Category="Pi Input", meta=(BlueprintProtected))
		void SetInputState(InputState NewState)
		{
			CurrentState = NewState;
		}

		//TODO can i leave this to be GC?
		APiGameMode* GameMode;
};
