// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Picross3D/GameLogic/PiCube.h"
#include "PiGameMode.generated.h"

class APiPuzzle;
/**
 * Class representing the game state. Setups a new level based on a given Puzzle
 */
UCLASS()
class PICROSS3D_API APiGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
		virtual void BeginPlay() override;
		
	private:
		//TODO is this the correct way to reference an Asset?
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Puzzle", meta=(AllowPrivateAccess = "true"))
		TSubclassOf<APiPuzzle> PuzzleToLoad;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Puzzle", meta=(AllowPrivateAccess = "true"))
		TSubclassOf<APiCube> CubeClass;
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Puzzle", meta=(AllowPrivateAccess = "true"))
		APiPuzzle* CurrentPuzzle;

	public:
		APiPuzzle* GetCurrentPuzzle() const
		{
			return CurrentPuzzle;
		}

		TSubclassOf<APiCube> GetCubeClass() const
		{
			return CubeClass;
		}
};
