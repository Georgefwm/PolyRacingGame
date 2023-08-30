// Fill out your copyright notice in the Description page of Project Settings.

#include "Controller/PolyRacingPlayerController.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputTriggers.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "StartPositionActor.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Customisation/VehicleCustomisationComponent.h"
#include "Customisation/VehicleCustomiser.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Framework/GameMode/PolyRacingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/GameModeSubsystem.h"
#include "Subsystem/MapSubsystem.h"
#include "UI/InGameHUD.h"


APolyRacingPlayerController::APolyRacingPlayerController()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(TEXT("/Game/Input/IMC_UI"));
	InputMappingContext = InputMappingContextFinder.Object;
}

void APolyRacingPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APolyRacingPlayerController, VehicleConfiguration);
}

void APolyRacingPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetupInputComponent();
}

void APolyRacingPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	AInGameHUD* HUD = GetHUD<AInGameHUD>();
	if (!HUD)
		return;

	SetupInputMappingContext();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(InputMappingContext->GetMapping(0).Action, ETriggerEvent::Started, HUD, &AInGameHUD::TogglePauseMenu);
}

void APolyRacingPlayerController::SetupInputMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!InputSubsystem)
		return;

	InputSubsystem->AddMappingContext(InputMappingContext, 0);
}

void APolyRacingPlayerController::SetupHUD()
{
	AInGameHUD* HUD = GetHUD<AInGameHUD>();
	
	if (!HUD)
		return;

	APolyRacingWheeledVehiclePawn* PolyRacingPawn = GetPawn<APolyRacingWheeledVehiclePawn>();
	if (!PolyRacingPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is null"))
		return;
	}

	UGameModeSubsystem* GameModeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameModeSubsystem>();
	
	HUD->Init(PolyRacingPawn, GameModeSubsystem->GetCurrentGameModeWidget());
	HUD->ShowPlayerHUD();
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
		PlayerController->SetPawn(NewVehicle);

		NewVehicle->DisableInput(PlayerController);
		
		SetupHUD();
		
		NewVehicle->VehicleCustomisationComponent->CurrentPrimaryColor = DesiredConfiguration.PrimaryColor;
		NewVehicle->VehicleCustomisationComponent->OnRep_PrimaryColorChanged();
		
		NewVehicle->VehicleCustomisationComponent->CurrentAccentColor = DesiredConfiguration.AccentColor;
		NewVehicle->VehicleCustomisationComponent->OnRep_AccentColorChanged();

		GetPlayerState<APolyRacingPlayerState>()->bIsReady = true;
		Server_NotifyReadyToStart();
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
	if (SequencePlayer->IsPlaying())
		SequencePlayer->Stop();
	
	PlayerCameraManager->StartCameraFade(1.f, 0.f, 3, FColor::Black, true);
	
	Client_RequestVehicleSpawn();
}

void APolyRacingPlayerController::Client_PlayLevelIntroSequence_Implementation(ULevelSequence* Sequence)
{
	PlayLevelIntroSequence(Sequence);
}

void APolyRacingPlayerController::OnLevelOutroSequenceEnd()
{
	PlayerCameraManager->StartCameraFade(1.f, 0.f, 3, FColor::Black, true);

	if (SequencePlayer->IsPlaying())
		SequencePlayer->Stop();
	
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
	
	if (Cameras.IsEmpty())
		return;
	
	for (AActor* Actor : Cameras)
	{		
		if (Actor->GetActorNameOrLabel() == FString("EndCamera"))
		{
			SetViewTarget(Actor);
			
			break;
		}
	}
}

void APolyRacingPlayerController::PlayLevelOutroSequence(ULevelSequence* Sequence)
{
	ALevelSequenceActor* LevelSequenceActor;
	ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	GetHUD<AInGameHUD>()->HidePlayerHUD();

	SequencePlayer = LevelSequenceActor->GetSequencePlayer();
	
	if (LevelSequenceActor)
	{
		// We can't use the ULevelSequence.OnStop event, so we just use a timer
		GetWorldTimerManager().SetTimer(SequenceTimerHandle,
			this,
			&APolyRacingPlayerController::OnLevelOutroSequenceEnd,
			SequencePlayer->GetEndTime().AsSeconds() - 0.01);

		SequencePlayer->Play();
	}
}

void APolyRacingPlayerController::Client_PlayLevelOutroSequence_Implementation(ULevelSequence* Sequence)
{
	PlayLevelOutroSequence(Sequence);
}

void APolyRacingPlayerController::NotifyReadyToStart()
{
	APolyRacingGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APolyRacingGameModeBase>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error getting GameMode"))
		return;
	}

	GetPlayerState<APolyRacingPlayerState>()->bIsReady = true;
	
	GameMode->CheckPlayersAreReady();
}

void APolyRacingPlayerController::Server_NotifyReadyToStart_Implementation()
{
	NotifyReadyToStart();
}

void APolyRacingPlayerController::PlayCountDown()
{
	GetHUD<AInGameHUD>()->PlayCountDown();
}

void APolyRacingPlayerController::Client_PlayCountDown_Implementation()
{
	PlayCountDown();
}

void APolyRacingPlayerController::OnCountDownSequenceEnd()
{
	APolyRacingPlayerState* PolyRacingPlayerState = GetPlayerState<APolyRacingPlayerState>();

	double const CurrentTimeStamp = GetWorld()->GetTimeSeconds();
	
	PolyRacingPlayerState->EventStartTime = CurrentTimeStamp;
	PolyRacingPlayerState->LastLapStartTime = CurrentTimeStamp;

	if (APolyRacingWheeledVehiclePawn* Vehicle = Cast<APolyRacingWheeledVehiclePawn>(GetPawn()))
	{
		Vehicle->EnableInput(this);
	}
	else UE_LOG(LogTemp, Warning, TEXT("APolyRacingPlayerController: Cast to APolyRacingWheeledVehiclePawn failed"))
}

void APolyRacingPlayerController::Client_OnCountDownSequenceEnd_Implementation()
{
	OnCountDownSequenceEnd();
}

void APolyRacingPlayerController::SetGameMode(const FString& GameModeName)
{
	if (HasAuthority())
		return;

	UGameModeSubsystem* GameModeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameModeSubsystem>();
	if (!GameModeSubsystem)
		return;
	
	GameModeSubsystem->SetCurrentGameMode(GameModeName);
	
	UE_LOG(LogTemp, Warning, TEXT("Current GameMode: %s"), *GameModeSubsystem->CurrentGameMode.ToString())
}

void APolyRacingPlayerController::Client_SetGameMode_Implementation(const FString& GameModeName)
{
	SetGameMode(GameModeName);
}

void APolyRacingPlayerController::NotifyFinishedRace(APolyRacingPlayerController* PlayerController)
{
	GetWorld()->GetAuthGameMode<APolyRacingGameModeBase>()->HandlePlayerHasFinishedEvent(PlayerController);
}

void APolyRacingPlayerController::Server_NotifyFinishedRace_Implementation(APolyRacingPlayerController* PlayerController)
{
	NotifyFinishedRace(PlayerController);
}

void APolyRacingPlayerController::RequestCheckpointRestart(APolyRacingPlayerController* PlayerController)
{
	GetWorld()->GetAuthGameMode<APolyRacingGameModeBase>()->RestartPlayerAtCheckpoint(PlayerController);
}

void APolyRacingPlayerController::Server_RequestCheckpointRestart_Implementation(
	APolyRacingPlayerController* PlayerController)
{
	RequestCheckpointRestart(PlayerController);
}

void APolyRacingPlayerController::AddWidgetToScreen(TSubclassOf<UUserWidget> Widget)
{
	// TODO: Remove as this is only called during end event
	GEngine->GameViewport->RemoveAllViewportWidgets();
	
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetGameInstance(), Widget);
	NewWidget->AddToViewport();
}

void APolyRacingPlayerController::Client_AddWidgetToScreen_Implementation(TSubclassOf<UUserWidget> Widget)
{
	AddWidgetToScreen(Widget);
}

void APolyRacingPlayerController::StartLeavingMatchSinglePlayer()
{
	float constexpr FadeDuration = 1.f;
	
	PlayerCameraManager->StartCameraFade(0.f, 1.f, FadeDuration - 0.1f, FColor::Black, true, true);	

	FTimerHandle LeaveGameTimerHandle = FTimerHandle();
	GetWorld()->GetTimerManager().SetTimer(LeaveGameTimerHandle,
		this,
		&APolyRacingPlayerController::LeaveMatchSinglePlayer,
		FadeDuration,
		false);
}

void APolyRacingPlayerController::LeaveMatchSinglePlayer()
{	
	UMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	UGameModeSubsystem* GameModeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameModeSubsystem>();
	
	FString const GameModeName = FString("MainMenu");
	FString const MapName = FString("MainMenu");
	FString const LevelOptions = FString("game=" + GameModeSubsystem->GetGameModePath(GameModeName).ToString());
	
	GameModeSubsystem->SetCurrentGameMode(GameModeName);
	MapSubsystem->SetCurrentMap(MapName);
	
	UGameplayStatics::OpenLevel(GetWorld(), MapSubsystem->GetMapPath(MapName), true, LevelOptions);
}

void APolyRacingPlayerController::StartLeavingMatchMultiPlayer()
{
}

void APolyRacingPlayerController::LeaveMatchMultiPlayer()
{
}


