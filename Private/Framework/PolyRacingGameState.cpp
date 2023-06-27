// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PolyRacingGameState.h"


// Sets default values
APolyRacingGameState::APolyRacingGameState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APolyRacingGameState::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APolyRacingGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

