// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PolyRacingGameInstance.h"

#include "MoviePlayer.h"
#include "Subsystem/MapSubsystem.h"

void UPolyRacingGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPolyRacingGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPolyRacingGameInstance::EndLoadingScreen);
}

void UPolyRacingGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("MAP END LOADING"))
	
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
	
	// if (ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(GetFirstLocalPlayerController()))
	// 	PlayerController->OnFinishedLoad();
}

void UPolyRacingGameInstance::BeginLoadingScreen(const FString& MapName)
{
	UE_LOG(LogTemp, Warning, TEXT("MAP START LOADING"))
	//GetFirstLocalPlayerController()->GetHUD();

	UMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	MapSubsystem->SetCurrentMap(MapName);
}
