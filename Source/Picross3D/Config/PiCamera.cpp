// Fill out your copyright notice in the Description page of Project Settings.


#include "PiCamera.h"

// Sets default values
APiCamera::APiCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APiCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APiCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APiCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

