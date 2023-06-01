// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuHUD.h"
#include "UI/Menu/MainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "UI/Menu/LobbyMenuWidget.h"

void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
}

void AMenuHUD::ShowMainMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MainMenuWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMenuHUD::ShowLobbyMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		LobbyWidget = SNew(SLobbyMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(LobbyWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMenuHUD::RemoveMenu()
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


void AMenuHUD::OnBeginLoading()
{
}

void AMenuHUD::OnEndLoading()
{
	
}
