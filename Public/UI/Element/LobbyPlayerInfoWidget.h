// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Framework/LobbyGameMode.h"


/**
 * 
 */
class SLobbyPlayerInfoWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLobbyPlayerInfoWidget) {}

	SLATE_ARGUMENT(int, LobbyPlayerIndex)
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	const struct FGlobalStyle* Style;

	int LobbyPlayerIndex;
	
	TAttribute<FLobbyPlayerInfo> PlayerInfo;
	FLobbyPlayerInfo GetLobbyPlayerInfo() const { return FLobbyPlayerInfo(); }

	TAttribute<FText> PlayerName;
	FText GetLobbyPlayerName() const { return FText::FromString(PlayerInfo.Get().PlayerName); }
	
	TAttribute<FText> ReadyStatus;
	FText GetLobbyPlayerReadyStatus() const
	{
		FText Status = PlayerInfo.Get().bPlayerReadyState ? FText::FromString(TEXT("Ready")) : FText::FromString(TEXT("Not Ready")) ;
		return Status;
	}
};

