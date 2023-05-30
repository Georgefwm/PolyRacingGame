// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TimeTrialGameMode.h"

ATimeTrialGameMode::ATimeTrialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeTrialGameMode::BeginPlay()
{
	Super::BeginPlay();
}
