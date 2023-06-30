// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameModeWidget.h"
#include "ResultTimeWidget.generated.h"

class APolyRacingPlayerState;
class UTextBlock;


/**
 * 
 */
UCLASS(Abstract)
class UResultTimeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	APolyRacingPlayerState* PlayerState;
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UTextBlock* FinalTimeTextBlock;
};
