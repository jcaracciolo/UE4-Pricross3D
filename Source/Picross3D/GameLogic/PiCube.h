// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PiCube.generated.h"

UCLASS()
class PICROSS3D_API APiCube : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APiCube();

	void SetupPuzzlePosition(const FIntVector&& SetupPosition);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Position", meta=(AllowPrivateAccess))
	FIntVector PuzzlePosition;
};
