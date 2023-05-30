// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyRacingSpectatorPawn.h"


// Sets default values
APolyRacingSpectatorPawn::APolyRacingSpectatorPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APolyRacingSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APolyRacingSpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APolyRacingSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

