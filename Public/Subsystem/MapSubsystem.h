﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	FName CurrentMap;

	UPROPERTY()
	UTexture* CurrentMapImage;

	FName GetMapPath(FString MapName) const;

	UTexture* GetCurrentMapLoadingScreenImage() const;

	FName GetCurrentMapDisplayName() const;

	void SetCurrentMap(const FString& MapName);
	
	void SetCurrentMapFromPath(const FString& MapPath);

	ULevelSequence* UMapSubsystem::GetCurrentLevelIntroSequence();
	
	ULevelSequence* UMapSubsystem::GetCurrentLevelOutroSequence();
};
