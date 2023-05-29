// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PolyRacingSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPROnCreateSessionComplete,		bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPROnUpdateSessionComplete,		bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPROnStartSessionComplete,		bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPROnEndSessionComplete,		bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPROnDestroySessionComplete,	bool, Successful);

DECLARE_MULTICAST_DELEGATE_TwoParams(FPROnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPROnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API UPolyRacingSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPolyRacingSessionSubsystem();

	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);
	void UpdateSession();
	void StartSession();
	void EndSession();
	void DestroySession();
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
	void JoinGameSession(const FOnlineSessionSearchResult& SessionResult);

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession();

public:
	FPROnCreateSessionComplete OnCreateSessionCompleteEvent;
	FPROnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	FPROnStartSessionComplete OnStartSessionCompleteEvent;
	FPROnEndSessionComplete OnEndSessionCompleteEvent;
	FPROnDestroySessionComplete OnDestroySessionCompleteEvent;
	FPROnFindSessionsComplete OnFindSessionsCompleteEvent;
	FPROnJoinSessionComplete OnJoinGameSessionCompleteEvent;

	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	// Create session
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	// Update session
	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	// Start session
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	// End session
	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	// Destroy session
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	// Find session
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;

	// Join session
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
