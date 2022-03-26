// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PiCube.generated.h"

UENUM(BlueprintType)
enum class EHintAppearance: uint8
{
	NONE=0,
	// Indicates no hint should be given
	ONCE,
	TWICE,
	MANY,
};

USTRUCT(BlueprintType)
struct FHint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	uint8 Number;

	UPROPERTY(BlueprintReadOnly)
	EHintAppearance Appearance;
};

USTRUCT(BlueprintType)
struct FHints
{
	GENERATED_BODY()

	FHint X;
	FHint Y;
	FHint Z;
};

class APiPuzzle;
UCLASS()
class PICROSS3D_API APiCube : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APiCube();

	UFUNCTION()
	void BeginPlay() override;

	// TODO Do i need to declare all functions as UFUNCTIONS() ? Even if i am not using them
	//as delegates or in a blueprint
	// Is it ok if i force this to be an R-Value? Sounds weird
	// Should i use a reference? Or not even just a copy?
	void SetupPuzzlePosition(const FIntVector&& SetupPosition);

	UFUNCTION(BlueprintNativeEvent, Category="Cubes")
	void TogglePaint();

	UFUNCTION(BlueprintNativeEvent, Category="Cubes")
	void SetXHint(FHint Hint);

	UFUNCTION(BlueprintNativeEvent, Category="Cubes")
	void SetYHint(FHint Hint);

	UFUNCTION(BlueprintNativeEvent, Category="Cubes")
	void SetZHint(FHint Hint);

	UFUNCTION(BlueprintImplementableEvent, Category="Cubes")
	void SetSolutionColor();

	FIntVector GetPuzzlePosition() const { return PuzzlePosition; };
	bool IsSolution() const { return bIsSolution; };
	bool IsPainted() const { return bIsPainted; };

	UPROPERTY(EditAnywhere, Category="PiPuzzle", meta=(AllowPrivateAccess = "true"))
	FHints Hints{{0, EHintAppearance::NONE}, {0, EHintAppearance::NONE}, {0, EHintAppearance::NONE}};

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PiPuzzle", meta=(AllowPrivateAccess = "true"))
	FIntVector PuzzlePosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PiPuzzle", meta=(AllowPrivateAccess = "true"))
	bool bIsSolution;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PiPuzzle", meta=(AllowPrivateAccess = "true"))
	bool bIsPainted = false;
};
