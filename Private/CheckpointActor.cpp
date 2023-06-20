// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointActor.h"

#include "PolyRacingWheeledVehiclePawn.h"
#include "Components/BoxComponent.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Kismet/GameplayStatics.h"

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
	if (!OtherActor)
		return;
	
	APolyRacingPlayerState* PlayerState = Vehicle->GetPlayerState<APolyRacingPlayerState>();
	if (!PlayerState)
		return;
	

	if (PlayerState->LastCheckpoint + 1 != CheckpointNumber)
	{
		if (PlayerState->LastCheckpoint != CheckpointCount - 1) // Don't add lap when starting behind first checkpoint
			return;
	}
	
	PlayerState->LastCheckpoint = CheckpointNumber;

	if (CheckpointNumber == 0)
	{
		// TODO: Check for final lap and end game
		
		PlayerState->Lap++;
		PlayerState->LastCheckpoint = CheckpointNumber;
	}

	// TODO: Convert FX logic into NetMulticast RPC
	if (CheckpointPassedEffect) {
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
}

void ACheckpointActor::BeginPlay()
{
	Super::BeginPlay();

	if (CheckpointNumber != 0)
		return;
	
	TArray<AActor*> Checkpoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpointActor::StaticClass(), Checkpoints);
	if (Checkpoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No checkpoints found... ?"))
		return;
	}

	CheckpointCount = Checkpoints.Num();
	
	for (AActor* CheckpointActor : Checkpoints)
		Cast<ACheckpointActor>(CheckpointActor)->CheckpointCount = Checkpoints.Num();
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

