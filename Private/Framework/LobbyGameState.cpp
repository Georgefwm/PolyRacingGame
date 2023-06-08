// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LobbyGameState.h"


// Sets default values
ALobbyGameState::ALobbyGameState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

