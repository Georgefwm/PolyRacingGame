// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MapSubsystem.h"

#include "DataTables.h"
#include "Engine/DataTable.h"

void UMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FString const MapDataTablePath = TEXT("/Script/Engine.DataTable'/Game/Maps/DT_Maps.DT_Maps'");
	
	MapDataTable = Cast<UDataTable>(FSoftObjectPath(*MapDataTablePath).ResolveObject());
	
	if (!MapDataTable)
		MapDataTable = Cast<UDataTable>(FSoftObjectPath(*MapDataTablePath).TryLoad());

	SetCurrentMap(FString("Docks"));

	if (MapDataTable)
		UE_LOG(LogTemp, Display, TEXT("UMapSubsystem initialised successfully"))
}

void UMapSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FName UMapSubsystem::GetMapPath(FString MapName) const
{	
	FMapTableRow* MapTableRow = MapDataTable->FindRow<FMapTableRow>(FName(MapName), "");

	if (!MapTableRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map not found"))
		return FName("");
	}
	
	return MapTableRow->Path;
}

UTexture* UMapSubsystem::GetCurrentMapLoadingScreenImage() const
{
	//FMapTableRow* MapTableRow = MapDataTable->FindRow<FMapTableRow>(FName("Docks"), "");

	return CurrentMapImage;
	//return nullptr;
	
	//return MapTableRow->PreviewImage.LoadSynchronous();
}

FName UMapSubsystem::GetCurrentMapDisplayName() const
{
	FMapTableRow* MapTableRow = MapDataTable->FindRow<FMapTableRow>(CurrentMap, "");

	if (!MapTableRow)
		return FName("");
	
	return FName(MapTableRow->DisplayName);
}

void UMapSubsystem::SetCurrentMap(const FString& MapName)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadingMapString: %s"), *MapName)
	
	for (const FName& RowName : MapDataTable->GetRowNames())
	{
		FMapTableRow* MapTableRow = MapDataTable->FindRow<FMapTableRow>(RowName, "");

		if (MapTableRow->MapName == MapName)
		{
			CurrentMap = RowName;
			CurrentMapImage = MapTableRow->PreviewImage.LoadSynchronous();
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UMapSubsystem: loading map not found in map table!"))
}

ULevelSequence* UMapSubsystem::GetCurrentLevelSequence()
{
	FMapTableRow* MapTableRow = MapDataTable->FindRow<FMapTableRow>(CurrentMap, "");

	if (!MapTableRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level Sequence not found"))
		return nullptr;
	}
	
	return MapTableRow->LevelSequence.LoadSynchronous();
}
