// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/MainMenuGameMode.h"

#include "PolyRacingSpectatorPawn.h"
#include "Camera/CameraActor.h"
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
	
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
	
	if (!Cameras.IsEmpty())
		GetWorld()->GetFirstPlayerController()->SetViewTarget(StaticCast<ACameraActor*>(Cameras[0]));
	else
		UE_LOG(LogTemp, Warning, TEXT("LOBBYGAMEMODE: Camera not found"))
}

void AMainMenuGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
	
	if (!Cameras.IsEmpty())
		NewPlayer->SetViewTarget(StaticCast<ACameraActor*>(Cameras[0]));
	else
		UE_LOG(LogTemp, Warning, TEXT("MAINMENUGAMEMODE: Camera not found"))
}

void AMainMenuGameMode::InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer)
{
	NewPlayer->ClientSetHUD(HUDClass);
	NewPlayer->GetHUD<AMenuHUD>()->ShowMainMenu();
}
