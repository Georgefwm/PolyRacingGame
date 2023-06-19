// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyRacingCheckpointActor.h"

#include "Components/BoxComponent.h"

APolyRacingCheckpointActor::APolyRacingCheckpointActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APolyRacingCheckpointActor::OnOverlap);
}

void APolyRacingCheckpointActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap happened!"))
}

