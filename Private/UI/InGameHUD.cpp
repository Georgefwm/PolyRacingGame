// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGameHUD.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "Subsystem/GameModeSubsystem.h"
#include "UI/Element/GameModeWidget.h"
#include "UI/Element/VehiclePawnWidget.h"
#include "UI/Menu/PauseMenuWidget.h"


AInGameHUD::AInGameHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> CountDownWidgetFinder(TEXT("/Game/UI/WidgetBlueprints/WBP_CountDown"));
	CountDownWidgetClass = CountDownWidgetFinder.Class;
	if (!CountDownWidgetClass)
		UE_LOG(LogTemp, Warning, TEXT("CountDownWidget not found!"))
}

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
		if (VehicleWidget)
			VehicleWidget->AddToViewport();

		if (GameModeWidget)
			GameModeWidget->AddToViewport();

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
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(PauseMenuWidgetContainer, SWeakWidget).PossiblyNullContent(PauseMenuWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}

		PauseMenuActive = true;
		return;
	}

	// Switch back to game UI
	GEngine->GameViewport->RemoveAllViewportWidgets();
	
	if (!VehicleWidget)
	{
		if (APolyRacingWheeledVehiclePawn* VehiclePawn = GetOwningPlayerController()->GetPawn<APolyRacingWheeledVehiclePawn>())	
			VehicleWidget = CreateWidget<UVehiclePawnWidget>(GetGameInstance(), VehiclePawn->VehicleHUD);
	}
	
	if (!GameModeWidget)
	{
		if (UGameModeSubsystem* GameModeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameModeSubsystem>())
			GameModeWidget = CreateWidget<UGameModeWidget>(GetGameInstance(), GameModeSubsystem->GetCurrentGameModeWidget());
	}

	if (VehicleWidget)
		VehicleWidget->AddToViewport();

	if (GameModeWidget)
		GameModeWidget->AddToViewport();

	if (PlayerOwner)
	{
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeGameOnly());
	}

	PauseMenuActive = false;
}

void AInGameHUD::HidePlayerHUD()
{
	if (!GEngine || !GEngine->GameViewport)
		return;
	
	GEngine->GameViewport->RemoveAllViewportWidgets();
	
	if (PlayerOwner)
	{
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeGameOnly());
	}
}

void AInGameHUD::Init(APolyRacingWheeledVehiclePawn* NewPawn, TSubclassOf<UUserWidget> NewGameModeWidget)
{
	if (NewPawn)
	{
		VehicleWidget = CreateWidget<UVehiclePawnWidget>(GetGameInstance(), NewPawn->VehicleHUD);
		VehicleWidget->MovementComponent = NewPawn->GetVehicleMovementComponent();
	}

	if (NewGameModeWidget)
		GameModeWidget = CreateWidget<UGameModeWidget>(GetGameInstance(), NewGameModeWidget);
}

void AInGameHUD::PlayCountDown()
{
	if (!CountDownWidgetClass)
		return;

	if (CountDownWidget)
		CountDownWidget->RemoveFromParent();

	// Count down widget is set/assumed to start upon creation and remove itself on completion
	CountDownWidget = CreateWidget<UUserWidget>(GetGameInstance(), CountDownWidgetClass);
	CountDownWidget->AddToViewport();
}

void AInGameHUD::OnBeginLoading()
{
}

void AInGameHUD::OnEndLoading()
{
	
}
