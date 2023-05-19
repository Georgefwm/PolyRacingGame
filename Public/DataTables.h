// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "DataTables.generated.h"


USTRUCT(BlueprintType)
struct FCustomisationOption : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ComponentName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshLocationOffset;
};

USTRUCT(BlueprintType)
struct FColorOptions : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterial> ColorScheme;
};

USTRUCT(BlueprintType)
struct FVehicleType : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VehicleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> Mesh;
	
	// Body-specific offsets

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WheelSizeModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FrontAxelOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RearAxelOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AxelLength;
};

USTRUCT(BlueprintType)
struct FVehicleConfiguration : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int VehicleType = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Bonnet = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BumperFront = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BumperRear = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SideSkirt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Fender = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Undercarriage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Rim = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Tyre = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ColorScheme = 0;
};