// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "CheckpointGameMode.generated.h"

class ACheckpointActor;


// This class does not need to be modified.
UINTERFACE()
class UCheckpointGameMode : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class POLYRACINGGAME_API ICheckpointGameMode
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints);
};
