// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PiCube.h"
#include "GameFramework/Actor.h"
#include "Picross3D/Utils/Definitions.h"
#include "Picross3D/Utils/FPuzzleMatrix.h"
#include "PiPuzzle.generated.h"

USTRUCT()
struct FCurrentHiddenAxis
{
	GENERATED_BODY()

	UPROPERTY()
	EPiAxis Axis;

	UPROPERTY()
	uint8 Amount;
};


UCLASS()
class PICROSS3D_API APiPuzzle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APiPuzzle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(CallInEditor, Category="Cubes")
	void GenerateCubes();

	UFUNCTION(CallInEditor, Category="Cubes")
	void ShowSolution();
	
	//TODO Do i just pass a pointer here? will i have some issues with GC?
	// The same cube is refereced somewhere else. but what if it isnt?
	UFUNCTION()
	void Break(APiCube* Cube);

	UFUNCTION()
	void Paint(APiCube* Cube);

	UFUNCTION()
	bool IsCompleted();

	UFUNCTION(BlueprintNativeEvent, Category="Cubes")
	void StartCompletedAnimation();
	
	int GetCurrentSize(const FIntVector From, const EPiAxis Axis) const;
	FHint GetCubesHint(const FIntVector From, const EPiAxis Axis) const;
	void SetLineHints(const FIntVector From, const EPiAxis Axis) const;

	UPROPERTY()
	FCurrentHiddenAxis AxisHidden;
	
	UFUNCTION()
	void HideAxis(EPiAxis Axis);

	UFUNCTION()
	void ShowAxis(EPiAxis Axis);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubes", meta=(AllowPrivateAccess="true", ForceInlineRow))
	FIntVector PuzzleSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubes", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<APiCube> CubeClass;

	UPROPERTY()
	TArray<APiCube*> CubesArray;
	FPuzzleMatrix CubesMatrix{&CubesArray};
};
