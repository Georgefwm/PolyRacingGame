// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/PolyRacingSessionSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "UI/MenuHUD.h"


UPolyRacingSessionSubsystem::UPolyRacingSessionSubsystem()
	: CreateSessionCompleteDelegate(	FOnCreateSessionCompleteDelegate::CreateUObject(	this, &ThisClass::OnCreateSessionCompleted))
	, UpdateSessionCompleteDelegate(	FOnUpdateSessionCompleteDelegate::CreateUObject(	this, &ThisClass::OnUpdateSessionCompleted))
	, StartSessionCompleteDelegate(		FOnStartSessionCompleteDelegate::CreateUObject(		this, &ThisClass::OnStartSessionCompleted))
	, EndSessionCompleteDelegate(		FOnEndSessionCompleteDelegate::CreateUObject(		this, &ThisClass::OnEndSessionCompleted))
	, DestroySessionCompleteDelegate(	FOnDestroySessionCompleteDelegate::CreateUObject(	this, &ThisClass::OnDestroySessionCompleted))
	, FindSessionsCompleteDelegate(		FOnFindSessionsCompleteDelegate::CreateUObject(		this, &ThisClass::OnFindSessionsCompleted))
	, JoinSessionCompleteDelegate(		FOnJoinSessionCompleteDelegate::CreateUObject(		this, &ThisClass::OnJoinSessionCompleted))
{
	FString const GameModeDataTablePath = TEXT("/Script/Engine.DataTable'/Game/GameModes/DT_GameModeTable.DT_GameModeTable'");
	
	GameModes = Cast<UDataTable>(FSoftObjectPath(*GameModeDataTablePath).ResolveObject());
	if (!GameModes) GameModes = Cast<UDataTable>(FSoftObjectPath(*GameModeDataTablePath).TryLoad());
	
	if (!GameModes)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionSubsystem: GameMode data found!"))
	}
}

void UPolyRacingSessionSubsystem::CreateSession(int32 NumPublicConnections, bool IsLANMatch, const FString& DesiredGameMode)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	const FGameModeTableRow* GameMode = GameModes->FindRow<FGameModeTableRow>(FName(DesiredGameMode), "", false);
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode not found, aborting session creation..."))
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}
	
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = GameMode->MaxPlayers;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;

	LastSessionSettings->Set(SETTING_MAPNAME, FString("/Game/Scenes/MainMenuScene"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(SETTING_GAMEMODE, DesiredGameMode, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(SEARCH_PRESENCE, true, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), FName("PolyRacingSession"), *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UPolyRacingSessionSubsystem::UpdateSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));

	// Make session changes/updates
	UpdatedSessionSettings->Set(SETTING_MAPNAME, FString("/Game/Scenes/MainMenuScene"), EOnlineDataAdvertisementType::ViaOnlineService);

	UpdateSessionCompleteDelegateHandle = SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

	if (!SessionInterface->UpdateSession(NAME_GameSession, *UpdatedSessionSettings))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);

		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		LastSessionSettings = UpdatedSessionSettings;
	}
}

void UPolyRacingSessionSubsystem::StartSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	if (!SessionInterface->StartSession(NAME_GameSession))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void UPolyRacingSessionSubsystem::EndSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnEndSessionCompleteEvent.Broadcast(false);
		return;
	}

	EndSessionCompleteDelegateHandle = SessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);

	if (!SessionInterface->EndSession(NAME_GameSession))
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);

		OnEndSessionCompleteEvent.Broadcast(false);
	}
}

void UPolyRacingSessionSubsystem::DestroySession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(FName("PolyRacingSession")))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		OnDestroySessionCompleteEvent.Broadcast(false);
	}
}

void UPolyRacingSessionSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery, const FString& DesiredGameMode)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;

	// Set search params
	LastSessionSearch->QuerySettings.Set(SETTING_MAPNAME, FString("/Game/Scenes/MainMenuScene"), EOnlineComparisonOp::Equals);
	LastSessionSearch->QuerySettings.Set(SETTING_GAMEMODE, DesiredGameMode, EOnlineComparisonOp::Equals);
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UPolyRacingSessionSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), FName("PolyRacingSession"), SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UPolyRacingSessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	OnCreateSessionCompleteEvent.Broadcast(Successful);
	
	UE_LOG(LogTemp, Warning, TEXT("Creating session: %s"), Successful ? *FString("Success") : *FString("Fail"))

	if (Successful)
	{
		FString const LevelOptions = FString(TEXT("?listen game=/Game/GameModes/BP_LobbyGamemode.BP_LobbyGamemode_C"));

		// GetWorld()->GetFirstPlayerController()->GetHUD<AMenuHUD>()->RemoveMenu();

		GetWorld()->GetGameInstance()->EnableListenServer(true, 7779);

		FString TravelPath = FString("/Game/Scenes/MainMenuScene" + LevelOptions);
	
		UE_LOG(LogTemp, Warning, TEXT("TravelPath: %s"), *TravelPath)
		
		GetWorld()->ServerTravel(TravelPath);

		// FString const LevelOptions = FString(TEXT("listen -game=/Game/GameModes/BP_LobbyGamemode.BP_LobbyGamemode_C"));
		// UGameplayStatics::OpenLevel(GetWorld(), "/Game/Scenes/MainMenuScene", true, LevelOptions);
	}
}

void UPolyRacingSessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	OnUpdateSessionCompleteEvent.Broadcast(Successful);
}

void UPolyRacingSessionSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	OnStartSessionCompleteEvent.Broadcast(Successful);
}

void UPolyRacingSessionSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());;
	if (SessionInterface)
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
	}

	OnEndSessionCompleteEvent.Broadcast(Successful);
}

void UPolyRacingSessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	OnDestroySessionCompleteEvent.Broadcast(Successful);

	UE_LOG(LogTemp, Warning, TEXT("Destroying %s"), Successful ? *FString("Success") : *FString("Fail"))
	UE_LOG(LogTemp, Warning, TEXT("Joining new session..."))

	if (Successful)
		GetWorld()->GetGameInstance()->EnableListenServer(false, 7779);

	if (!LastSessionSearch)
		return;
	
	if (LastSessionSearch->SearchResults.IsValidIndex(0))
		JoinGameSession(LastSessionSearch->SearchResults.GetData()[0]);
}

void UPolyRacingSessionSubsystem::OnFindSessionsCompleted(bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
		return;
	}

	OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults, Successful);

	UE_LOG(LogTemp, Warning, TEXT("Session found, Destroying current..."))

	if (GetGameInstance()->GetOnlineSession())
		DestroySession();
	else
	{
		if (!LastSessionSearch)
			return;
	
		if (LastSessionSearch->SearchResults.IsValidIndex(0))
			JoinGameSession(LastSessionSearch->SearchResults.GetData()[0]);
	}
}

void UPolyRacingSessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	OnJoinGameSessionCompleteEvent.Broadcast(Result);
	
	UE_LOG(LogTemp, Warning, TEXT("join: %s"), Result > 0 ? *FString("Fail") : *FString("Success"))
	
	if (Result > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Aborting..."))

		
		Online::GetSessionInterface(GetWorld())->DumpSessionState();
		
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Traveling to session..."))
	
	TryTravelToCurrentSession();
}

bool UPolyRacingSessionSubsystem::TryTravelToCurrentSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		return false;
	}
	
	FString ConnectString;
	if (!SessionInterface->GetResolvedConnectString(FName("PolyRacingSession"), ConnectString))
	{
		return false;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UE_LOG(LogTemp, Warning, TEXT("Connect string: %s"), *ConnectString)
	//GetGameInstance()->ClientTravelToSession(PlayerController->GetUniqueID(), NAME_GameSession);
	
	PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
	return true;
}
