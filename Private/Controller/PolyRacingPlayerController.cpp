// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PolyRacingPlayerController.h"

#include "PolyRacingWheeledVehiclePawn.h"
#include "StartPositionActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGameHUD.h"

APolyRacingPlayerController::APolyRacingPlayerController()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APolyRacingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
}

void APolyRacingPlayerController::SetupHUD()
{
	if (AInGameHUD* HUD = GetHUD<AInGameHUD>())
	{
		HUD->ShowPlayerHUD();
	}
}

void APolyRacingPlayerController::Client_SetupHUD_Implementation()
{
	SetupHUD();
}

void APolyRacingPlayerController::SpawnVehicleForPlayer(const FPresetVehicleConfiguration& DesiredConfiguration, APolyRacingPlayerController* PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("controller invalid"))
		return;
	}
	
	if(PlayerController->GetPawn())
	{
		PlayerController->GetPawn()->Destroy();
	}

	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(FInputModeGameOnly());
	
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
		SpawnParameters.Owner = PlayerController;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APolyRacingWheeledVehiclePawn* NewVehicle = VehicleCustomiser->SpawnVehicle(DesiredConfiguration,
			GetWorld(),
			Location,
			Rotation,
			SpawnParameters);
		
		PlayerController->Possess(NewVehicle);
	}
}

void APolyRacingPlayerController::Server_SpawnVehicleForPlayer_Implementation(const FPresetVehicleConfiguration& DesiredConfiguration, APolyRacingPlayerController* PlayerController)
{
	SpawnVehicleForPlayer(DesiredConfiguration, PlayerController);
}

void APolyRacingPlayerController::RequestVehicleSpawn()
{
	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	
	Server_SpawnVehicleForPlayer(VehicleCustomiser->SavedConfigurations->GetData()[VehicleCustomiser->ActiveConfigurationSlotIndex], this);
}

void APolyRacingPlayerController::Client_RequestVehicleSpawn_Implementation()
{
	RequestVehicleSpawn();
}
