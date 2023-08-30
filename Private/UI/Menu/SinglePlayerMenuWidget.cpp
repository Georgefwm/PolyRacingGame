// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SinglePlayerMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/GameModeSubsystem.h"
#include "Subsystem/MapSubsystem.h"
#include "UI/Style/GlobalMenuStyle.h"
#include "UI/MenuHUD.h"
#include "UI/Style/UiStyles.h"


#define LOCTEXT_NAMESPACE "singleplayermenu"

void SSinglePlayerMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	PreviousMenu = InArgs._PreviousMenu;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	

	/** Text */
	const FText TitleText		= LOCTEXT("menu title",		"Single Player");
	
	const FText RaceModeText	= LOCTEXT("RaceModeText",	"Race");
	const FText TimeTrialText	= LOCTEXT("TimeTrialText",	"Time Trial");
	const FText FreeModeText	= LOCTEXT("FreeModeText",	"Free Roam");
	
	const FText BackText		= LOCTEXT("Back text",		"Back");

	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(Style->TitleTextMargin)
		[
			// TitleText
			SNew(STextBlock)
			.TextStyle(&Style->MenuTitleStyle)
			.Text(TitleText)
			.LineHeightPercentage(2.f)
		]
		
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(Style->MenuBoxMargin)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(RaceModeText)
				.OnClicked(this, &SSinglePlayerMenuWidget::OnRaceClicked)
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(TimeTrialText)
				.OnClicked(this, &SSinglePlayerMenuWidget::OnTimeTrialClicked)
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(FreeModeText)
				.OnClicked(this, &SSinglePlayerMenuWidget::OnFreeRoamClicked)
			]
		]
		
		// Back Button
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Bottom)
		.Padding(Style->MenuActionButtonContainerMargin)
		[
			SNew(SBox)
			.WidthOverride(1000.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(Style->MenuActionButtonSpacingMargin)
				.AutoWidth()
				[
					SNew(SBox)
					.HAlign(HAlign_Left)
					[
						SNew(SButton)
						.ButtonStyle(&Style->MenuActionButtonStyle)
						.TextStyle(&Style->MenuActionButtonTextStyle)
						.Text(BackText)
						.OnClicked(this, &SSinglePlayerMenuWidget::OnBackClicked)
					]
				]				
			]			
		]
	];
}

FReply SSinglePlayerMenuWidget::OnRaceClicked() const
{
	OwningHUD->ShowNotImplementedWidget();

	return FReply::Handled();
}

FReply SSinglePlayerMenuWidget::OnTimeTrialClicked() const
{
	UMapSubsystem* MapSubsystem = OwningHUD->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	UGameModeSubsystem* GameModeSubsystem = OwningHUD->GetGameInstance()->GetSubsystem<UGameModeSubsystem>();
	
	FString const GameModeName = FString("TimeTrial");
	FString const MapName = FString("Docks");
	FString const LevelOptions = FString("listen game=" + GameModeSubsystem->GetGameModePath(GameModeName).ToString());
	
	GameModeSubsystem->SetCurrentGameMode(GameModeName);
	MapSubsystem->SetCurrentMap(MapName);
	
	UGameplayStatics::OpenLevel(OwningHUD->GetWorld(), MapSubsystem->GetMapPath(MapName), true, LevelOptions);

	return FReply::Handled();
}

FReply SSinglePlayerMenuWidget::OnFreeRoamClicked() const
{
	UMapSubsystem* MapSubsystem = OwningHUD->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	UGameModeSubsystem* GameModeSubsystem = OwningHUD->GetGameInstance()->GetSubsystem<UGameModeSubsystem>();
	
	FString const GameModeName = FString("FreeRoam");
	FString const MapName = FString("Docks");
	FString const LevelOptions = FString("listen game=" + GameModeSubsystem->GetGameModePath(GameModeName).ToString());
	
	GameModeSubsystem->SetCurrentGameMode(GameModeName);
	MapSubsystem->SetCurrentMap(MapName);
	
	UGameplayStatics::OpenLevel(OwningHUD->GetWorld(), MapSubsystem->GetMapPath(MapName), true, LevelOptions);

	return FReply::Handled();
}

FReply SSinglePlayerMenuWidget::OnBackClicked() const
{
	OwningHUD->MenuWidgetContainer.Get()->SetContent(PreviousMenu.ToSharedRef());
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
