// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

class UVehicleCustomiser;
class UUserWidget;

/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AMenuHUD();
	
	// Not initialised by default, on first usage
	TSharedPtr<class SMainMenuWidget> MainMenuWidget;
	TSharedPtr<class SSinglePlayerMenuWidget> SinglePlayerMenuWidget;
	TSharedPtr<class SMultiplayerMenuWidget> MultiplayerMenuWidget;
	TSharedPtr<class SGarageMenuWidget> GarageWidget;
	TSharedPtr<class SVehicleCustomisationMenuWidget> CustomiseWidget;
	TSharedPtr<class SLobbyMenuWidget> LobbyWidget;
	
	
	TSharedPtr<class SWeakWidget> MenuWidgetContainer;

	UPROPERTY()
	UVehicleCustomiser* VehicleCustomiser;

	virtual void BeginPlay() override;
	
	void ShowMainMenu();
	void ShowLobbyMenu();
	void RemoveMenu();

	void UpdateLobby();

	UPROPERTY()
	TSubclassOf<UUserWidget> NotImplementedWidgetClass;

	void ShowNotImplementedWidget();

	void OnBeginLoading();
	void OnEndLoading();
};
