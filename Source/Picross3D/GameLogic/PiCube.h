// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PiCube.generated.h"

class APiPuzzle;
UCLASS()
class PICROSS3D_API APiCube : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APiCube();

	// TODO Do i need to declare all functions as UFUNCTIONS() ? Even if i am not using them
	//as delegates or in a blueprint
	// Is it ok if i force this to be an R-Value? Sounds weird
	// Should i use a reference? Or not even just a copy?
	void SetupPuzzlePosition(const FIntVector&& SetupPosition);

	UFUNCTION(BlueprintNativeEvent, Category="Cubes", meta=(BlueprintProtected))
	void TogglePaint();

	// TODO Why do i put UFUNCTION here?
	// Is it ok to inline this on the Header?
	UFUNCTION()
	bool IsSolution() const;
	
	UFUNCTION()
	FIntVector GetPuzzlePosition() const;
	bool IsPainted() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PiPuzzle", meta=(AllowPrivateAccess = "true"))
	FIntVector PuzzlePosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PiPuzzle", meta=(AllowPrivateAccess = "true"))
	bool bIsSolution;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PiPuzzle", meta=(AllowPrivateAccess = "true"))
	bool bIsPainted = false;
};
