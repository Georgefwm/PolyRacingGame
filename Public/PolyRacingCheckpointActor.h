// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PolyRacingCheckpointActor.generated.h"

class UBoxComponent;

UCLASS()
class POLYRACINGGAME_API APolyRacingCheckpointActor : public AActor
{
	GENERATED_BODY()

public:

	APolyRacingCheckpointActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CheckpointNumber = 0;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
