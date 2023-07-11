// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/MainMenuGameMode.h"

#include "MenuCameraActor.h"
#include "PolyRacingSpectatorPawn.h"
#include "Camera/CameraActor.h"
#include "Components/AudioComponent.h"
#include "UI/MenuHUD.h"
#include "Controller/MenuPlayerController.h"
#include "Customisation/VehicleCustomiser.h"
#include "Framework/PolyRacingPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


AMainMenuGameMode::AMainMenuGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	bStartPlayersAsSpectators = true;
	bUseSeamlessTravel = false;
	
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();
	PlayerStateClass = APolyRacingPlayerState::StaticClass();
	
	DefaultPawnClass = APolyRacingSpectatorPawn::StaticClass();
	SpectatorClass = APolyRacingSpectatorPawn::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	
	// Setup the vehicle
	TArray<AActor*> StartPositions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPositions);
	if (!StartPositions.IsEmpty())
	{
		FVector Location = StartPositions[0]->GetTransform().GetLocation();
		FRotator Rotation = StartPositions[0]->GetTransform().GetRotation().Rotator();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		VehicleCustomiser->SpawnVehicle(GetWorld(), Location, Rotation, SpawnParameters);
	}
	
}

void AMainMenuGameMode::StartPlay()
{
	Super::StartPlay();

	HandleStartingNewPlayer_Implementation(GetWorld()->GetFirstPlayerController());
}

void AMainMenuGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMenuCameraActor::StaticClass(), Cameras);
	
	if (!Cameras.IsEmpty())
	{
		AMenuCameraActor* MenuCamera = Cast<AMenuCameraActor>(Cameras[0]);
		
		NewPlayer->SetViewTarget(MenuCamera);
		MenuCamera->PossessedBy(NewPlayer);
		NewPlayer->PlayerCameraManager->StartCameraFade(1.f, 0.f, 3, FColor::Black, true);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("MainMenuGameMode: Camera not found"))
}

void AMainMenuGameMode::InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer)
{
	NewPlayer->ClientSetHUD(HUDClass);
	NewPlayer->GetHUD<AMenuHUD>()->ShowMainMenu();
}
