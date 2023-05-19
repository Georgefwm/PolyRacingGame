// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VehiclePreview.generated.h"


UCLASS()
class POLYRACINGGAME_API AVehiclePreview : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVehiclePreview();

	UPROPERTY()
	USceneComponent* SceneRoot;
	
	/** Visual components */
	UPROPERTY()
	USkeletalMeshComponent* BodyMesh;

	UPROPERTY()
	USkeletalMeshComponent* BonnetMesh;

	UPROPERTY()
	USkeletalMeshComponent* BumperFrontMesh;

	UPROPERTY()
	USkeletalMeshComponent* BumperRearMesh;

	UPROPERTY()
	USkeletalMeshComponent* SideSkirtMesh;

	UPROPERTY()
	USkeletalMeshComponent* FenderMesh;

	UPROPERTY()
	USkeletalMeshComponent* UndercarriageMesh;

	// Spoiler requires a position offset
	UPROPERTY()
	USkeletalMeshComponent* SpoilerMesh;

	// wheels require a few more fields than others
	UPROPERTY()
	USkeletalMeshComponent* WheelFrontLeft;
	UPROPERTY()
	USkeletalMeshComponent* WheelFrontRight;
	UPROPERTY()
	USkeletalMeshComponent* WheelRearLeft;
	UPROPERTY()
	USkeletalMeshComponent* WheelRearRight;

	UPROPERTY()
	USkeletalMeshComponent* RimMesh;

	UPROPERTY()
	USkeletalMeshComponent* TyreMesh;

    UPROPERTY()
    UMaterial* ColorScheme;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
