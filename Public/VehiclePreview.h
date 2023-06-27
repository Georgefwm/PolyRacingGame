// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VehiclePreview.generated.h"

struct FVehicleType;


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

	// Require a position offset
	UPROPERTY()
	USkeletalMeshComponent* SpoilerMesh;

	// wheels require a few more fields than others
	UPROPERTY()
	USkeletalMeshComponent* FrontLeftRim;
	
	UPROPERTY()
	USkeletalMeshComponent* FrontRightRim;
	
	UPROPERTY()
	USkeletalMeshComponent* RearLeftRim;
	
	UPROPERTY()
	USkeletalMeshComponent* RearRightRim;
	
	UPROPERTY()
	USkeletalMeshComponent* FrontLeftTyre;
	
	UPROPERTY()
	USkeletalMeshComponent* FrontRightTyre;
	
	UPROPERTY()
	USkeletalMeshComponent* RearLeftTyre;
	
	UPROPERTY()
	USkeletalMeshComponent* RearRightTyre;

    UPROPERTY()
    UMaterial* ColorScheme;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetWheelOffsets(FVehicleType* VehicleType);
};
