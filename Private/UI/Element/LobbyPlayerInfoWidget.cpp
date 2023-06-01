// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/LobbyPlayerInfoWidget.h"

#include "UI/Style/GlobalMenuStyle.h"
#include "UI/Style/UiStyles.h"

#define LOCTEXT_NAMESPACE "lobbyplayerinfo"

void SLobbyPlayerInfoWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	// OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	
	LobbyPlayerIndex = InArgs._LobbyPlayerIndex;

	PlayerInfo.Bind(this,		&SLobbyPlayerInfoWidget::GetLobbyPlayerInfo);
	PlayerInfo.Bind(this,		&SLobbyPlayerInfoWidget::GetLobbyPlayerInfo);
	ReadyStatus.Bind(this,				&SLobbyPlayerInfoWidget::GetLobbyPlayerReadyStatus);

	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SButton)
				.ButtonStyle(&Style->OptionSelectButtonStyle)
				.TextStyle(&Style->OptionSelectTextStyle)
				.Text(PlayerName)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(1.f)
			[
				SNew(SButton)
				.ButtonStyle(&Style->OptionSelectButtonStyle)
				.TextStyle(&Style->OptionSelectTextStyle)
				.Text(ReadyStatus)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
			]
		]
	];
}



#undef LOCTEXT_NAMESPACE
