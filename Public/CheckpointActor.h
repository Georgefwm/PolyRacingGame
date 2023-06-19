// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointActor.generated.h"

class UBoxComponent;

UCLASS()
class POLYRACINGGAME_API ACheckpointActor : public AActor
{
	GENERATED_BODY()

public:

	ACheckpointActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* TriggerBoxRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* LeftSign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* RightSign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CheckpointNumber = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CheckpointWidth = 1000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CheckpointHeight = 200.f;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Updates elements automatically when CheckpointWidth is change in editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
};
