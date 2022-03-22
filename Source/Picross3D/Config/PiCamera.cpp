// Fill out your copyright notice in the Description page of Project Settings.


#include "PiCamera.h"

// Sets default values
APiCamera::APiCamera()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	Camera->SetupAttachment(RootComponent);
}

void APiCamera::SetupPuzzleSize(int Cubes) const
{
	Camera->SetRelativeLocation({Cubes * 160.0f, 0, 0});
}
