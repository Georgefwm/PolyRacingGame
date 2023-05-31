// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PolyRacingGameInstance.h"

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
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("MAP LOAD END"));
}

void UPolyRacingGameInstance::BeginLoadingScreen(const FString& MapName)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("MAP LOAD START"));
}
