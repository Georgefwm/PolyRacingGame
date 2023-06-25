// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\Framework\GameMode\PolyRacingGameModeBase.h"


// Sets default values
APolyRacingGameMode::APolyRacingGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APolyRacingGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APolyRacingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

