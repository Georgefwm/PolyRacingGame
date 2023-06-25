// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameModeWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UGameModeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;

};
