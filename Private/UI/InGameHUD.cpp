// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUD.h"

#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "UI/PolyRacingInGameWidgetBase.h"
#include "UI/Menu/LobbyMenuWidget.h"
#include "UI/Menu/PauseMenuWidget.h"

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();

	if (PlayerOwner)
	{
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeGameOnly());
	}
}

void AInGameHUD::ShowPlayerHUD()
{
	if (GEngine && GEngine->GameViewport)
	{
		InGameWidget = SNew(SPolyRacingInGameWidgetBase).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(WidgetContainer, SWeakWidget).PossiblyNullContent(InGameWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AInGameHUD::TogglePauseMenu()
{
	if (!GEngine || !GEngine->GameViewport)
		return;

	// Hide game UI and show 'pause' menu
	if (!PauseMenuActive)
	{
		PauseMenuWidget = SNew(SPauseMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(WidgetContainer, SWeakWidget).PossiblyNullContent(PauseMenuWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}

		PauseMenuActive = true;
		return;
	}
	
	// Switch back to game UI
	if (!InGameWidget)
	{
		InGameWidget = SNew(SPolyRacingInGameWidgetBase).OwningHUD(this);
		GEngine->GameViewport->RemoveAllViewportWidgets();
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(WidgetContainer, SWeakWidget).PossiblyNullContent(InGameWidget.ToSharedRef()));
	}
	else
		WidgetContainer.Get()->SetContent(InGameWidget.ToSharedRef());

	if (PlayerOwner)
	{
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeGameOnly());
	}

	PauseMenuActive = false;
}

void AInGameHUD::HidePlayerHUD()
{
	if (GEngine && GEngine->GameViewport && WidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(WidgetContainer.ToSharedRef());
		
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AInGameHUD::OnBeginLoading()
{
}

void AInGameHUD::OnEndLoading()
{
	
}
