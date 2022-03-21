// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PiPuzzle.generated.h"

class APiCube;

UCLASS()
class PICROSS3D_API APiPuzzle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APiPuzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubes", meta=(AllowPrivateAccess="true"))
	int Height;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubes", meta=(AllowPrivateAccess="true"))
	int Depth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubes", meta=(AllowPrivateAccess="true"))
	int Width;


	UPROPERTY()
	TArray<APiCube*> Cubes;
};
