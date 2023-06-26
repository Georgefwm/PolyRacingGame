// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/GameModeWidget.h"


void UGameModeWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APolyRacingPlayerState>();
}
