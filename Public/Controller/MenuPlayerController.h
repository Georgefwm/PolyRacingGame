// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

	AMenuPlayerController();
	
	virtual void BeginPlay() override;
};
