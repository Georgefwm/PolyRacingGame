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
	FMapTableRow* GameModeTableRow = GameModeDataTable->FindRow<FMapTableRow>(FName(GameModeName), "");

	if (!GameModeTableRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map not found"))
		return FName("");
	}
	
	return GameModeTableRow->Path;
}

FName UGameModeSubsystem::GetCurrentGameModeDisplayName() const
{
	FMapTableRow* GameModeTableRow = GameModeDataTable->FindRow<FMapTableRow>(CurrentGameMode, "");

	if (!GameModeTableRow)
		return FName("");
	
	return FName(GameModeTableRow->DisplayName);
}

void UGameModeSubsystem::SetCurrentGameMode(const FString& GameModeName)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadingMapString: %s"), *GameModeName)
	
	for (const FName& RowName : GameModeDataTable->GetRowNames())
	{
		FMapTableRow* GameModeTableRow = GameModeDataTable->FindRow<FMapTableRow>(RowName, "");

		if (GameModeTableRow->Path.ToString() == GameModeName)
		{
			CurrentGameMode = RowName;
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UGameModeSubsystem: Map loading map not found in map table!"))
}
