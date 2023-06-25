// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/VehiclePawnWidget.h"

#include "ChaosVehicleMovementComponent.h"
#include "Components/TextBlock.h"


void UVehiclePawnWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void UVehiclePawnWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!SpeedText || !GearText || !MovementComponent)
		return;	

	
	SpeedText->SetText(FText::FromString(FString::FromInt(
		FMath::RoundHalfFromZero(MovementComponent->GetForwardSpeedMPH() * 1.609))));
	
	GearText->SetText(FText::FromString(FString::FromInt(MovementComponent->GetCurrentGear())));
}
