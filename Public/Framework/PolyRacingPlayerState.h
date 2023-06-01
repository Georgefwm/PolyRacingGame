// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PolyRacingPlayerState.generated.h"

UCLASS()
class POLYRACINGGAME_API APolyRacingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APolyRacingPlayerState();

	bool bIsReady = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
