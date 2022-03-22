// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "PiCamera.generated.h"

UCLASS()
class PICROSS3D_API APiCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APiCamera();

	void SetupPuzzleSize(int Cubes) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PiCamera", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
};
