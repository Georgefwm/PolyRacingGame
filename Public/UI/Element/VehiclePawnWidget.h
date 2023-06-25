// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "VehiclePawnWidget.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class UVehiclePawnWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;

};
