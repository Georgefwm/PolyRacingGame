// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameModeWidget.h"
#include "ResultTableWidget.generated.h"

class APolyRacingGameState;


/**
 * 
 */
UCLASS(Abstract)
class UResultTableWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	APolyRacingGameState* GameState;
	
	virtual void NativeConstruct() override;
};
