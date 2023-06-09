// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Customisation/VehicleCustomiser.h"
#include "Framework/LobbyGameMode.h"
#include "MenuHUD.generated.h"


/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
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

public:
	void ShowMainMenu();
	void ShowLobbyMenu();
	void RemoveMenu();

	void UpdateLobby();

	void OnBeginLoading();
	void OnEndLoading();
};
