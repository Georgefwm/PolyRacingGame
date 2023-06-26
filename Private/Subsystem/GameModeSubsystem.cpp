// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/GameModeSubsystem.h"

#include "DataTables.h"
#include "Engine/DataTable.h"

void UGameModeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FString const GameModeDataTablePath = TEXT("/Script/Engine.DataTable'/Game/GameModes/DT_GameModeTable.DT_GameModeTable'");
	
	GameModeDataTable = Cast<UDataTable>(FSoftObjectPath(*GameModeDataTablePath).ResolveObject());
	
	if (!GameModeDataTable)
		GameModeDataTable = Cast<UDataTable>(FSoftObjectPath(*GameModeDataTablePath).TryLoad());

	SetCurrentGameMode(FString("MainMenu"));

	if (GameModeDataTable)
		UE_LOG(LogTemp, Display, TEXT("UGameModeSubsystem initialised successfully"))
}

void UGameModeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FName UGameModeSubsystem::GetGameModePath(FString GameModeName) const
{	
	FGameModeTableRow* GameModeTableRow = GameModeDataTable->FindRow<FGameModeTableRow>(FName(GameModeName), "");

	if (!GameModeTableRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map not found"))
		return FName("");
	}
	
	return FName(GameModeTableRow->Path);
}

FName UGameModeSubsystem::GetCurrentGameModeDisplayName() const
{
	FGameModeTableRow* GameModeTableRow = GameModeDataTable->FindRow<FGameModeTableRow>(CurrentGameMode, "");

	if (!GameModeTableRow)
		return FName("");
	
	return FName(GameModeTableRow->DisplayName);
}

TSubclassOf<UUserWidget> UGameModeSubsystem::GetCurrentGameModeWidget()
{
	FGameModeTableRow* GameModeTableRow = GameModeDataTable->FindRow<FGameModeTableRow>(CurrentGameMode, "");

	if (!GameModeTableRow)
		return nullptr;
	
	return GameModeTableRow->Widget;
}

void UGameModeSubsystem::SetCurrentGameMode(const FString& GameModeName)
{
	for (const FName& RowName : GameModeDataTable->GetRowNames())
	{
		FGameModeTableRow* GameModeTableRow = GameModeDataTable->FindRow<FGameModeTableRow>(RowName, "");

		if (GameModeTableRow->GameModeName == GameModeName)
		{
			CurrentGameMode = RowName;
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UGameModeSubsystem: GameMode '%s' not found"), *GameModeName)
}
