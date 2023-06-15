// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Customisation/VehicleCustomiser.h"
#include "Framework/GameMode/LobbyGameMode.h"


/**
 * 
 */
class SLobbyMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLobbyMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)
	SLATE_ARGUMENT(TSharedPtr<class SCompoundWidget>, PreviousMenu)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnGarageClicked() const;
	FReply OnVetoMapClicked() const;
	FReply OnBackClicked() const;
	
	TArray<TSharedPtr<FLobbyPlayerInfo>> LobbyPlayerInfoList;
	TSharedPtr<SListView<TSharedPtr<FLobbyPlayerInfo>>> ListViewWidget;
	TSharedRef<ITableRow> GenerateItemRow(TSharedPtr<FLobbyPlayerInfo> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<SCompoundWidget> PreviousMenu;

	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
