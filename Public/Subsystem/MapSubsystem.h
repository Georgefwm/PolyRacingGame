// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapSubsystem.generated.h"

class ULevelSequence;
class UDataTable;


/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API UMapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	UPROPERTY()
	UDataTable* MapDataTable;

	UPROPERTY()
	FString CurrentMap;

	FName GetMapPath(FString MapName) const;

	void SetCurrentMap(const FString& MapName);

	ULevelSequence* UMapSubsystem::GetCurrentLevelSequence();
};
