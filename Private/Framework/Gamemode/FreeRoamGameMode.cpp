// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/FreeRoamGameMode.h"

#include "PolyRacingWheeledVehiclePawn.h"
#include "StartPositionActor.h"
#include "Controller/LobbyPlayerController.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Customisation/VehicleCustomiser.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGameHUD.h"


// Sets default values
AFreeRoamGameMode::AFreeRoamGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerControllerClass = APolyRacingPlayerController::StaticClass();
	HUDClass = AInGameHUD::StaticClass();
}

void AFreeRoamGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AFreeRoamGameMode::StartPlay()
{
	Super::StartPlay();
}

void AFreeRoamGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if(NewPlayer->GetPawn())
	{
		NewPlayer->GetPawn()->Destroy();
	}

	NewPlayer->bShowMouseCursor = false;
	NewPlayer->SetInputMode(FInputModeGameOnly());
	
	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	
	// Spawn and possess vehicle 
	TArray<AActor*> StartPositions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStartPositionActor::StaticClass(), StartPositions);
	if (!StartPositions.IsEmpty())
	{
		AStartPositionActor* StartPosition = Cast<AStartPositionActor>(StartPositions[0]);
		const FTransform StartTransform = StartPosition->GetNextSpawnTransform(); // Only call once per player
		
		FVector Location = StartTransform.GetLocation();
		FRotator Rotation = StartTransform.GetRotation().Rotator();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = NewPlayer;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APolyRacingWheeledVehiclePawn* NewVehicle = VehicleCustomiser->SpawnVehicle(GetWorld(),
			Location,
			Rotation,
			SpawnParameters);
		
		NewPlayer->Possess(NewVehicle);
	}
}

void AFreeRoamGameMode::InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::InitializeHUDForPlayer_Implementation(NewPlayer);

	NewPlayer->GetHUD<AInGameHUD>()->ShowPlayerHUD();
}

void AFreeRoamGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFreeRoamGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AFreeRoamGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyPlayerController* JoiningPlayer = Cast<ALobbyPlayerController>(NewPlayer);
	if (!JoiningPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAMEMODE: invalid player tried to join the lobby"))
		return;
	}
	
	ConnectedPlayers.Add(JoiningPlayer);
}

void AFreeRoamGameMode::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);
}
