// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MainMenuGameMode.h"

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
		Camera = StaticCast<ACameraActor*>(Cameras[0]);
		
	if (Camera)
		GetWorld()->GetFirstPlayerController()->SetViewTarget(Camera);
}
