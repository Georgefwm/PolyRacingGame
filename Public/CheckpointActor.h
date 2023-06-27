// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CheckpointActor.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UNiagaraSystem;


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

	UPROPERTY()
	int CheckpointCount = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Checkpoint Attributes")
	int CheckpointNumber = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Checkpoint Attributes")
	float CheckpointWidth = 1000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Checkpoint Attributes")
	float CheckpointHeight = 200.f;

	UPROPERTY(EditAnywhere, Category = "Checkpoint Attributes")
	UNiagaraSystem* CheckpointPassedEffect;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void BeginPlay() override;

	// Updates elements automatically when CheckpointWidth is change in editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Ask server to play this actors fx
	void RequestPlayEffects();
	
	UFUNCTION(Server, Unreliable)
	void Server_RequestPlayEffects();
	void Server_RequestPlayEffects_Implementation();

	// Server tells all clients to play fx
	void PlayEffects();
	
	UFUNCTION(NetMulticast, Unreliable)
	void NetMulti_PlayEffects();
	void NetMulti_PlayEffects_Implementation();
	
};
