﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/PolyRacingPlayerState.h"


// Sets default values
APolyRacingPlayerState::APolyRacingPlayerState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseCustomPlayerNames = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void APolyRacingPlayerState::BeginPlay()
{
	Super::BeginPlay();

	LastCheckpoint = 0;
}

// Called every frame
void APolyRacingPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

