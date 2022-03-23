// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PiCube.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EHints: uint8
{
	NONE = 0 UMETA(Hidden),
	X = 1 << 0,
	Y = 1 << 1,
	Z = 1 << 2
};
ENUM_CLASS_FLAGS(EHints);

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

	UFUNCTION(BlueprintImplementableEvent, Category="Cubes")
	void SetXHint(int Hint);

	UFUNCTION(BlueprintImplementableEvent, Category="Cubes")
	void SetYHint(int Hint);

	UFUNCTION(BlueprintImplementableEvent, Category="Cubes")
	void SetZHint(int Hint);

	UFUNCTION(BlueprintImplementableEvent, Category="Cubes")
	void SetVisibleHints(UPARAM(meta = (Bitmask, BitmaskEnum = EHints))int32 NewHints);
	
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

	UPROPERTY(EditAnywhere, Category="PiPuzzle", meta=(AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EHints))
	uint32 Hints;
};
