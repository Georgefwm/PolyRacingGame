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
class POLYRACINGGAME_API UGameModeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	UPROPERTY()
	UDataTable* GameModeDataTable;

	UPROPERTY()
	FName CurrentGameMode;

	FName GetGameModePath(FString GameModeName) const;

	FName GetCurrentGameModeDisplayName() const;

	void SetCurrentGameMode(const FString& GameModeName);
};
