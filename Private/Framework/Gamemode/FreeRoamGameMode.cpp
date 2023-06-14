// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/FreeRoamGameMode.h"

#include "PolyRacingWheeledVehiclePawn.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Customisation/VehicleCustomiser.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFreeRoamGameMode::AFreeRoamGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerControllerClass = APolyRacingPlayerController::StaticClass();
}

void AFreeRoamGameMode::BeginPlay()
{
	Super::BeginPlay();
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
	
	// Setup the vehicle
	TArray<AActor*> StartPositions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPositions);
	if (!StartPositions.IsEmpty())
	{
		FVector Location = StartPositions[0]->GetTransform().GetLocation();
		FRotator Rotation = StartPositions[0]->GetTransform().GetRotation().Rotator();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = NewPlayer;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APolyRacingWheeledVehiclePawn* NewVehicle = VehicleCustomiser->SpawnVehicle(GetWorld(), Location, Rotation, SpawnParameters);
		NewPlayer->Possess(NewVehicle);
	}
}
