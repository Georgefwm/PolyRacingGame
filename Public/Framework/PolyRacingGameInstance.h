// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PolyRacingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API UPolyRacingGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

	TSharedPtr<SWidget> LoadingScreenWidget;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
};
