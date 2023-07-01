// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/MapSubsystem.h"
#include "UI/InGameHUD.h"
#include "UI/Style/GlobalMenuStyle.h"
#include "UI/Style/UiStyles.h"


#define LOCTEXT_NAMESPACE "pausemenu"

void SPauseMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");

	/** Text */
	const FText ResumeText			= LOCTEXT("ResumeText", "Resume");
	const FText SettingsText		= LOCTEXT("SettingsButtonText", "Settings");
	const FText LeaveText			= LOCTEXT("LeaveText", "Leave");

	
	ChildSlot
		[
			SNew(SOverlay)
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
					.Text(ResumeText)
					.OnClicked(this, &SPauseMenuWidget::OnResumeClicked)
				]
				
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(SettingsText)
					.OnClicked(this, &SPauseMenuWidget::OnSettingsClicked)
				]
				
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(LeaveText)
					.OnClicked(this, &SPauseMenuWidget::OnLeaveClicked)
				]
			]
		];
}

FReply SPauseMenuWidget::OnResumeClicked() const
{
	OwningHUD->TogglePauseMenu();
	
	return FReply::Handled();
}

FReply SPauseMenuWidget::OnSettingsClicked() const
{
	OwningHUD->ShowNotImplementedWidget();
	
	return FReply::Handled();
}

FReply SPauseMenuWidget::OnLeaveClicked() const
{
	UMapSubsystem* MapSubsystem = OwningHUD->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	
	FString const LevelOptions = FString(TEXT("game=/Game/GameModes/BP_MainMenuGamemode.BP_MainMenuGamemode_C"));

	MapSubsystem->SetCurrentMap(FString("MainMenu"));
	
	UGameplayStatics::OpenLevel(OwningHUD->GetWorld(), MapSubsystem->GetMapPath(FString("MainMenu")), true, LevelOptions);
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
