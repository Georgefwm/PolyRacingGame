﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PolyRacingGameInstance.h"

#include "Controller/LobbyPlayerController.h"

void UPolyRacingGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPolyRacingGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPolyRacingGameInstance::EndLoadingScreen);
}

void UPolyRacingGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	// if (!IsRunningDedicatedServer())
	// {
	// 	FLoadingScreenAttributes LoadingScreen;
	// 	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	// 	LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	//
	// 	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	// }
	UE_LOG(LogTemp, Warning, TEXT("MAP END LOADING"))

	
	// if (ALobbyPlayerController* PlayerController = static_cast<ALobbyPlayerController*>(GetFirstLocalPlayerController()))
	// 	PlayerController->OnFinishedLoad();
}

void UPolyRacingGameInstance::BeginLoadingScreen(const FString& MapName)
{
	//GetFirstLocalPlayerController()->GetHUD();
	UE_LOG(LogTemp, Warning, TEXT("MAP START LOADING"))
}
