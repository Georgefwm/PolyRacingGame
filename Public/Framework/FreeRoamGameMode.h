﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FreeRoamGameMode.generated.h"

UCLASS()
class POLYRACINGGAME_API AFreeRoamGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFreeRoamGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};