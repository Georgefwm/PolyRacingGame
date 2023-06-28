// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Element/ResultTableWidget.h"


void UResultTableWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	GameState = GetWorld()->GetGameState<APolyRacingGameState>();
}
