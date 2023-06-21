// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/CheckpointGameMode.h"


// Add default functionality here for any ICheckpointGameMode functions that are not pure virtual.
void ICheckpointGameMode::AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints)
{
	UE_LOG(LogTemp, Warning, TEXT("AddCheckpoints() Not implemented yet"))
}