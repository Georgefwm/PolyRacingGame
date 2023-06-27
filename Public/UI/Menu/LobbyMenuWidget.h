// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AMenuHUD;
class SCompoundWidget;

struct FLobbyPlayerInfo;


/**
 * 
 */
class SLobbyMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLobbyMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AMenuHUD>, OwningHUD)
	SLATE_ARGUMENT(TSharedPtr<SCompoundWidget>, PreviousMenu)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnGarageClicked() const;
	FReply OnVetoMapClicked() const;
	FReply OnBackClicked() const;
	
	TArray<TSharedPtr<FLobbyPlayerInfo>> LobbyPlayerInfoList;
	TSharedPtr<SListView<TSharedPtr<FLobbyPlayerInfo>>> ListViewWidget;
	TSharedRef<ITableRow> GenerateItemRow(TSharedPtr<FLobbyPlayerInfo> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<SCompoundWidget> PreviousMenu;

	TWeakObjectPtr<AMenuHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
