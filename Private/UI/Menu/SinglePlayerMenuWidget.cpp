// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SinglePlayerMenuWidget.h"

#include "Framework/PolyRacingSessionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Style/GlobalMenuStyle.h"
#include "UI/Menu/MainMenuWidget.h"
#include "UI/MenuHUD.h"
#include "UI/Style/UiStyles.h"

#define LOCTEXT_NAMESPACE "singleplayermenu"

void SSinglePlayerMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	

	/** Text */
	const FText TitleText		= LOCTEXT("menu title", "Single Player");
	
	const FText RaceModeText	= LOCTEXT("RaceModeText", "Race");
	const FText FreeModeText	= LOCTEXT("FreeModeText", "Free Roam");
	
	const FText BackText		= LOCTEXT("Back text", "Back");

	
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

	return FReply::Handled();
}

FReply SSinglePlayerMenuWidget::OnFreeRoamClicked() const
{
	UPolyRacingSessionSubsystem* SessionSubsystem = OwningHUD->GetGameInstance()->GetSubsystem<UPolyRacingSessionSubsystem>();
	SessionSubsystem->CreateSession(1, true);
	SessionSubsystem->StartSession();
	
	FString const LevelOptions = FString(TEXT("listen -game=/Game/GameModes/BP_FreeRoamGamemode.BP_FreeRoamGamemode_C"));
	UGameplayStatics::OpenLevel(OwningHUD->GetWorld(), "/Game/Scenes/Docks", true, LevelOptions);

	
	
	return FReply::Handled();
}

FReply SSinglePlayerMenuWidget::OnBackClicked() const
{
	if (!OwningHUD->MainMenuWidget)
		OwningHUD->MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(OwningHUD);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->MainMenuWidget.ToSharedRef());
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
