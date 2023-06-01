// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyHUD.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "UI/Menu/LobbyMenuWidget.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	
	ShowMenu();
}

void ALobbyHUD::ShowMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		LobbyMenuWidget = SNew(SLobbyMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(LobbyMenuWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void ALobbyHUD::RemoveMenu()
{
	if (GEngine && GEngine->GameViewport && MenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
		
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
}
