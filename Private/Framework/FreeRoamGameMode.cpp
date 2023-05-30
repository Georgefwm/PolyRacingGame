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

	TArray<AActor*> StartPositions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPositions);
	
	if(StartPositions.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Gamemode: No start locations found"))
	}
	
	//FTransform const StartTransform = StartPositions[0]->GetActorTransform();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = NewPlayer;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	 
	// FVector const Location = StartTransform.GetLocation();
	// FRotator const Rotation = StartTransform.GetRotation().Rotator();
	
	FVector const Location = FVector(7540.f,-10765.f,193.f);
	FRotator const Rotation = FRotator(0.f,180.f,0.f);

	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	
	APolyRacingWheeledVehiclePawn* NewVehicle = VehicleCustomiser->SpawnVehicle(GetWorld(), Location, Rotation, SpawnParameters);
	
	NewPlayer->Possess(NewVehicle);

	
}


