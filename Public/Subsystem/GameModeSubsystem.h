﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameModeSubsystem.generated.h"

struct FGameModeTableRow;
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

	FGameModeTableRow* GetGameModeInfo(FString GameModeName); 

	FName GetCurrentGameModeDisplayName() const;

	int GetCurrentGameModeLapCount();

	TSubclassOf<class UUserWidget> GetCurrentGameModeWidget();

	void SetCurrentGameMode(const FString& GameModeName);
};
