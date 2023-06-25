// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Customisation/VehicleCustomiser.h"
#include "InGameHUD.generated.h"


/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	bool PauseMenuActive = false;
	
	// Not initialised by default, on first usage
	TSharedPtr<class SPauseMenuWidget> PauseMenuWidget;

	UPROPERTY()
	class UGameModeWidget* GameModeWidget;

	UPROPERTY()
	class UVehiclePawnWidget* VehicleWidget;
	
	TSharedPtr<class SWeakWidget> PauseMenuWidgetContainer;

	UPROPERTY()
	UVehicleCustomiser* VehicleCustomiser;

	virtual void BeginPlay() override;

public:
	void ShowPlayerHUD();
	void HidePlayerHUD();

	void Init(APolyRacingWheeledVehiclePawn* NewPawn, TSubclassOf<UUserWidget> NewGameModeWidget);

	void TogglePauseMenu();

	void OnBeginLoading();
	void OnEndLoading();
};
