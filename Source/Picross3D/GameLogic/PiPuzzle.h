// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PiCube.h"
#include "GameFramework/Actor.h"
#include "PiPuzzle.generated.h"

UENUM()
enum class EDirection: uint8
{
	X = 0,
	Y,
	Z
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

	//TODO Do i just pass a pointer here? will i have some issues with GC?
	// The same cube is refereced somewhere else. but what if it isnt?
	UFUNCTION()
	void Break(APiCube* Cube);

	UFUNCTION()
	void Paint(APiCube* Cube);

	UFUNCTION()
	bool IsCompleted();

	template<typename Func>
	void ForEachInDirection(const EDirection Axis, const int OtherAxis1, const int OtherAxis2, Func F);
	
	int GetCurrentSize(const EDirection Axis, const int OtherAxis1, const int OtherAxis2);
	FHint GetCubesHint(const EDirection Axis, const int OtherAxis1, const int OtherAxis2);
	void FixHints(const EDirection Axis, const int OtherAxis1, const int OtherAxis2);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubes", meta=(AllowPrivateAccess="true", ForceInlineRow))
	FIntVector PuzzleSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubes", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<APiCube> CubeClass;

	UPROPERTY()
	TArray<APiCube*> Cubes;
};
