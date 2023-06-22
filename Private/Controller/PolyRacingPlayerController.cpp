// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PolyRacingPlayerController.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "StartPositionActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/InGameHUD.h"

APolyRacingPlayerController::APolyRacingPlayerController()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APolyRacingPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APolyRacingPlayerController, VehicleConfiguration);
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
		
		PlayerController->UnPossess();
		PlayerController->Possess(NewVehicle);
		PlayerController->VehiclePawn = NewVehicle;

		NewVehicle->VehicleCustomisationComponent->CurrentPrimaryColor = DesiredConfiguration.PrimaryColor;
		NewVehicle->VehicleCustomisationComponent->OnRep_PrimaryColorChanged();
		
		NewVehicle->VehicleCustomisationComponent->CurrentAccentColor = DesiredConfiguration.AccentColor;
		NewVehicle->VehicleCustomisationComponent->OnRep_AccentColorChanged();
	}
}

void APolyRacingPlayerController::Server_SpawnVehicleForPlayer_Implementation(const FPresetVehicleConfiguration& DesiredConfiguration, APolyRacingPlayerController* PlayerController)
{
	SpawnVehicleForPlayer(DesiredConfiguration, PlayerController);
}

void APolyRacingPlayerController::RequestVehicleSpawn()
{
	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();

	VehicleConfiguration = VehicleCustomiser->SavedConfigurations->GetData()[VehicleCustomiser->ActiveConfigurationSlotIndex];
	
	Server_SpawnVehicleForPlayer(VehicleConfiguration, this);
}

void APolyRacingPlayerController::Client_RequestVehicleSpawn_Implementation()
{
	RequestVehicleSpawn();
}

void APolyRacingPlayerController::PlayLevelIntroSequence(ULevelSequence* Sequence)
{
	ALevelSequenceActor* LevelSequenceActor;
	ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	SequencePlayer = LevelSequenceActor->GetSequencePlayer();
	
	if (LevelSequenceActor)
	{
		// We can't use the ULevelSequence.OnStop event, so we just use a timer
		GetWorldTimerManager().SetTimer(SequenceTimerHandle,
			this,
			&APolyRacingPlayerController::OnLevelIntroSequenceEnd,
			SequencePlayer->GetEndTime().AsSeconds() - 0.01);

		SequencePlayer->Play();
	}
	else  // If there is any error, skip to vehicle spawn logic
	{
		UE_LOG(LogTemp, Warning, TEXT("Sequence player Error"))
		
		Client_RequestVehicleSpawn();
	}
}

void APolyRacingPlayerController::OnLevelIntroSequenceEnd()
{
	PlayerCameraManager->StartCameraFade(1.f, 0.f, 3, FColor::Black, true);
	
	Client_RequestVehicleSpawn();
}

void APolyRacingPlayerController::Client_PlayLevelIntroSequence_Implementation(ULevelSequence* Sequence)
{
	PlayLevelIntroSequence(Sequence);
}
