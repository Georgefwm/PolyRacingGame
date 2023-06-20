// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointActor.h"
#include "Components/BoxComponent.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Overlap happened on checkpoint %i"), CheckpointNumber)
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

