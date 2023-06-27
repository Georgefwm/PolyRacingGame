// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

class UVehicleCustomiser;


/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AInGameHUD();

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

	UPROPERTY()
	TSubclassOf<UUserWidget> CountDownWidgetClass;
	
	UPROPERTY()
	UUserWidget* CountDownWidget;

	virtual void BeginPlay() override;
	
	void ShowPlayerHUD();
	void HidePlayerHUD();

	void Init(APolyRacingWheeledVehiclePawn* NewPawn, TSubclassOf<UUserWidget> NewGameModeWidget);

	void PlayCountDown();

	void TogglePauseMenu();

	void OnBeginLoading();
	void OnEndLoading();
};
