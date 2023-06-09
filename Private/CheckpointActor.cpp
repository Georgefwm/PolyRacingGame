﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckpointActor.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "Components/BoxComponent.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingGameState.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Framework/GameMode/TimeTrialGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/ArrowComponent.h"


ACheckpointActor::ACheckpointActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	TriggerBoxRoot = CreateDefaultSubobject<USceneComponent>("TriggerBoxRoot");
	TriggerBoxRoot->SetupAttachment(SceneRoot);
	TriggerBoxRoot->SetRelativeLocation(FVector(0.f, 0.f, CheckpointHeight));
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetupAttachment(TriggerBoxRoot);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(25.f, CheckpointWidth, CheckpointHeight));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointActor::OnOverlap);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("DirectionArrow");
	ArrowComponent->SetupAttachment(TriggerBoxRoot);

	LeftSign = CreateDefaultSubobject<UStaticMeshComponent>("LeftSign");
	LeftSign->SetupAttachment(SceneRoot);
	LeftSign->SetRelativeLocation(FVector(0.f, -CheckpointWidth, 0.f));
	LeftSign->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.f, 0.f, 90.f)));

	RightSign = CreateDefaultSubobject<UStaticMeshComponent>("RightSign");
	RightSign->SetupAttachment(SceneRoot);
	RightSign->SetRelativeLocation(FVector(0.f, CheckpointWidth, 0.f));
	RightSign->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.f, 0.f, 90.f)));
}

void ACheckpointActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APolyRacingWheeledVehiclePawn* Vehicle = Cast<APolyRacingWheeledVehiclePawn>(OtherActor);
	if (!Vehicle)
		return;
	
	APolyRacingPlayerState* PlayerState = Vehicle->GetPlayerState<APolyRacingPlayerState>();
	if (!PlayerState)
		return;
	
	APolyRacingPlayerController* PlayerController = Vehicle->GetController<APolyRacingPlayerController>();
	if (!PlayerController)
		return;

	if (!PlayerController->IsLocalPlayerController())
		return;

	
	if (PlayerState->LastCheckpoint + 1 != CheckpointNumber)
	{
		if (PlayerState->LastCheckpoint != CheckpointCount - 1) // Don't add lap when crossing the start line
			return;
	}
	
	PlayerState->LastCheckpoint = CheckpointNumber;

	if (CheckpointNumber == 0)  // New Lap
	{
		PlayerState->Lap++;
		PlayerState->LastCheckpoint = CheckpointNumber;

		double const CurrentTime = GetWorld()->GetTimeSeconds();

		PlayerState->LapTimes.Add(CurrentTime - PlayerState->LastLapStartTime);
		PlayerState->LastLapStartTime = CurrentTime;

		APolyRacingGameState* GameState = GetWorld()->GetGameState<APolyRacingGameState>();
		
		if (PlayerState->Lap == GameState->LapCount) // game over cond
		{
			PlayerState->EventEndTime = CurrentTime;
			PlayerState->LastLapStartTime = PlayerState->LapTimes.Last();
			
			PlayerController->Server_NotifyFinishedRace(PlayerController);
		}
	}

	if (PlayerController->IsLocalPlayerController())
		Server_RequestPlayEffects();
}

void ACheckpointActor::BeginPlay()
{
	Super::BeginPlay();

	if (CheckpointNumber != 0 || !HasAuthority())
		return;
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpointActor::StaticClass(), Actors);
	if (Actors.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No checkpoints found... ?"))
		return;
	}

	CheckpointCount = Actors.Num();
	
	TArray<ACheckpointActor*> Checkpoints;
	for (AActor* CheckpointActor : Actors)
	{
		ACheckpointActor* Checkpoint = Cast<ACheckpointActor>(CheckpointActor);
		Checkpoints.Add(Checkpoint);
		
		Checkpoint->CheckpointCount = Actors.Num();
	}
	
	APolyRacingGameModeBase* PotentialCheckpointGameMode = GetWorld()->GetAuthGameMode<APolyRacingGameModeBase>();
	if (!PotentialCheckpointGameMode->Implements<UCheckpointGameMode>())
		return;

	ICheckpointGameMode* GameMode = Cast<ICheckpointGameMode>(PotentialCheckpointGameMode);
	GameMode->AddCheckpoints(Checkpoints);
}

void ACheckpointActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property != nullptr && (
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ACheckpointActor, CheckpointWidth) ||
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ACheckpointActor, CheckpointHeight)))
	{
		// Update shape of the checkpoint in editor
		TriggerBox->SetBoxExtent(FVector(25.f, CheckpointWidth, CheckpointHeight));
		TriggerBoxRoot->SetRelativeLocation(FVector(0.f, 0.f, CheckpointHeight));
		
		LeftSign->SetRelativeLocation(FVector(0.f, -CheckpointWidth, 0.f));
		RightSign->SetRelativeLocation(FVector(0.f, CheckpointWidth, 0.f));
	}
}

void ACheckpointActor::RequestPlayEffects()
{
	if (!HasAuthority())
		return;
	
	NetMulti_PlayEffects_Implementation();
}

void ACheckpointActor::Server_RequestPlayEffects_Implementation()
{
	RequestPlayEffects();
}

void ACheckpointActor::PlayEffects()
{
	if (!CheckpointPassedEffect)
		return;

	// TODO: Add and play sound fx
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(CheckpointPassedEffect,
		LeftSign,
		NAME_None,
		FVector(0.f, 0.f, 210),
		FRotator(0.f),
		EAttachLocation::Type::KeepRelativeOffset,
		true);

	UNiagaraFunctionLibrary::SpawnSystemAttached(CheckpointPassedEffect,
		RightSign,
		NAME_None,
		FVector(0.f, 0.f, 210),
		FRotator(0.f),
		EAttachLocation::Type::KeepRelativeOffset,
		true);
}

void ACheckpointActor::NetMulti_PlayEffects_Implementation()
{
	PlayEffects();
}


