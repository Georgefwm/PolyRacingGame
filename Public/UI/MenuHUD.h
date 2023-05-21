// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// Widget class refs
	// Not initialised by default
	TSharedPtr<class SMainMenuWidget> MainMenuWidget;
	TSharedPtr<class SGarageMenuWidget> GarageWidget;
	TSharedPtr<class SVehicleCustomisationMenuWidget> CustomiseWidget;
	
	TSharedPtr<class SWeakWidget> MenuWidgetContainer;

	virtual void BeginPlay() override;

public:
	void ShowMenu();
	void RemoveMenu();
};
