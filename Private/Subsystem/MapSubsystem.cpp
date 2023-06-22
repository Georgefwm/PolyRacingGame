// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MapSubsystem.h"

#include "DataTables.h"
#include "Engine/DataTable.h"

void UMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FString const MapDataTablePath = TEXT("/Script/Engine.DataTable'/Game/VehicleCustomisation/VehiclePresets.VehiclePresets'");
	
	MapDataTable = Cast<UDataTable>(FSoftObjectPath(*MapDataTablePath).ResolveObject());
	
	if (!MapDataTable)
		MapDataTable = Cast<UDataTable>(FSoftObjectPath(*MapDataTablePath).TryLoad());

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
