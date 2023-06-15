// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/TimeTrialGameMode.h"

ATimeTrialGameMode::ATimeTrialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeTrialGameMode::BeginPlay()
{
	Super::BeginPlay();
}
