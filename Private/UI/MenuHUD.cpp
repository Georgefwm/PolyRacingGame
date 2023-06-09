// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "Controller/LobbyPlayerController.h"
#include "UI/Menu/MainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "UI/Menu/LobbyMenuWidget.h"


AMenuHUD::AMenuHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> NotImplementedWidgetFinder(TEXT("/Game/UI/WidgetBlueprints/WBP_NotImplentedYet"));
	NotImplementedWidgetClass = NotImplementedWidgetFinder.Class;
	if (!NotImplementedWidgetClass)
		UE_LOG(LogTemp, Warning, TEXT("NotImplementedWidget not found!"))
}

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
		if (!LobbyWidget.IsValid())
		{
			LobbyWidget = SNew(SLobbyMenuWidget).OwningHUD(this);
			GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(LobbyWidget.ToSharedRef()));
		}
		else
			MenuWidgetContainer.Get()->SetContent(LobbyWidget.ToSharedRef());
		
		
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
	ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(GetOwningPlayerController());
	if (!PlayerController)
		return;

	if (!LobbyWidget.IsValid())
		return;
	
	for (int Index = 0; Index < 8; ++Index)
	{
		// Override the TSharedPtr in the target array with a copy of the TSharedPtr from the source array
		LobbyWidget->LobbyPlayerInfoList[Index] = PlayerController->LobbyPlayerInfoList[Index];
	}

	LobbyWidget->ListViewWidget->RebuildList();
}

void AMenuHUD::ShowNotImplementedWidget()
{
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetGameInstance(), NotImplementedWidgetClass);
	NewWidget->AddToViewport();
}

void AMenuHUD::OnBeginLoading()
{
}

void AMenuHUD::OnEndLoading()
{
	
}
