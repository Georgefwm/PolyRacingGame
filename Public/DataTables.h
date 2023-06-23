// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "DataTables.generated.h"


class ULevelSequence;
class APolyRacingWheeledVehiclePawn;


USTRUCT(BlueprintType)
struct FVehicleSlotOptions : public FTableRowBase
{
    GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<USkeletalMesh>> Meshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> Offsets;
};

USTRUCT(BlueprintType)
struct FPresetVehicleType : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VehicleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<APolyRacingWheeledVehiclePawn>> Presets;
};

UCLASS()
class UColorOptions : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UMaterialInstance>> MaterialInstances;
};

USTRUCT(BlueprintType)
struct FGameModeTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GameModeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY()
	int MaxPlayers = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Path;
};

USTRUCT(BlueprintType)
struct FMapTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<ULevelSequence> LevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture> PreviewImage;
};

// Old system
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
	float FrontAxelOffset = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RearAxelOffset = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AxelLength = 1.f;
};


// Old system
USTRUCT(BlueprintType)
struct FPresetVehicleConfiguration : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VehicleType = TEXT("Exotic");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Preset = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PrimaryColor = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AccentColor = 3;
};


// Old system
USTRUCT(BlueprintType)
struct FVehicleConfiguration : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString VehicleType = TEXT("Exotic");

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