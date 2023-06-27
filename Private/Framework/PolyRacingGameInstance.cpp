// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/PolyRacingGameInstance.h"
#include "MoviePlayer.h"
#include "Subsystem/MapSubsystem.h"
#include "UI/Menu/LoadingScreenWidget.h"


void UPolyRacingGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPolyRacingGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPolyRacingGameInstance::EndLoadingScreen);
}

void UPolyRacingGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("MAP END LOADING"))
	
	if (IsRunningDedicatedServer())
		return;
	
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreen.MinimumLoadingScreenDisplayTime = 5;

	UMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	
	LoadingScreen.WidgetLoadingScreen = SAssignNew(LoadingScreenWidget, SLoadingScreenWidget).MapSubsystem(MapSubsystem);

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void UPolyRacingGameInstance::BeginLoadingScreen(const FString& MapName)
{
	UE_LOG(LogTemp, Warning, TEXT("MAP START LOADING"))

	UMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	MapSubsystem->SetCurrentMapFromPath(MapName);
	
}
