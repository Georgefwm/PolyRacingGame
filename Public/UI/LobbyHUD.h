// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Customisation/VehicleCustomiser.h"
#include "LobbyHUD.generated.h"


/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// Not initialised by default, on first usage
	TSharedPtr<class SLobbyMenuWidget> LobbyMenuWidget;
	
	TSharedPtr<class SWeakWidget> MenuWidgetContainer;

	UPROPERTY()
	UVehicleCustomiser* VehicleCustomiser;

	virtual void BeginPlay() override;

public:
	void ShowMenu();
	void RemoveMenu();
};
