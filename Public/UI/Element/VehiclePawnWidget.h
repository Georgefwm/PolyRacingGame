// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "VehiclePawnWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UChaosVehicleMovementComponent;

/**
 * 
 */
UCLASS(Abstract)
class UVehiclePawnWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(BindWidget))
	UTextBlock* SpeedText;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(BindWidget))
	UTextBlock* GearText;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(BindWidget))
	UProgressBar* TachometerProgressBar;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UChaosVehicleMovementComponent* MovementComponent;
};
