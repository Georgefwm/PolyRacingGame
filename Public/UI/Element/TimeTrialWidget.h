// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameModeWidget.h"
#include "TimeTrialWidget.generated.h"


class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
class UTimeTrialWidget : public UGameModeWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UTextBlock* TotalTimeValueTextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UTextBlock* CurrentLapTimeValueTextBlock;
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
