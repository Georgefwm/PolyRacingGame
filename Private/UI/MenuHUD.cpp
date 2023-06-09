// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuHUD.h"

#include "Controller/LobbyPlayerController.h"
#include "UI/Menu/MainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
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

void AMenuHUD::UpdateLobby()
{
	if (!LobbyWidget)
		return;
	
	ALobbyPlayerController* PlayerController = static_cast<ALobbyPlayerController*>(GetOwningPlayerController());
	if (!PlayerController)
		return;

	TArray<TSharedPtr<FLobbyPlayerInfo>> CopiedArray;
	//LobbyWidget->LobbyPlayerInfoList.Empty(PlayerController->LobbyPlayerInfoList.Num());

	for (int Index = 0; Index < 8; ++Index)
	{
		// Override the TSharedPtr in the target array with a copy of the TSharedPtr from the source array
		LobbyWidget->LobbyPlayerInfoList[Index] = MakeShareable(new FLobbyPlayerInfo(*PlayerController->LobbyPlayerInfoList[Index]));
	}
}

void AMenuHUD::OnBeginLoading()
{
}

void AMenuHUD::OnEndLoading()
{
	
}
