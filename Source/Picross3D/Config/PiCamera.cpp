// Fill out your copyright notice in the Description page of Project Settings.


#include "PiCamera.h"

// Sets default values
APiCamera::APiCamera()
{
	PrimaryActorTick.bCanEverTick = false;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	Camera->SetupAttachment(RootComponent);
}

void APiCamera::FitPuzzleInViewport(int Cubes) const
{
	Camera->SetRelativeLocation({Cubes * 160.0f, 0, 0});
}
