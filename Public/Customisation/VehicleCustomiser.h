#pragma once

#include "DataTables.h"
#include "Engine/DataTable.h"
#include "VehicleCustomiser.Generated.h"

UCLASS()
class POLYRACINGGAME_API UVehicleCustomiser : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void SetupVehicle();
	void SetupVehicle(FPresetVehicleConfiguration);

	APolyRacingWheeledVehiclePawn* SpawnVehicle(UWorld* World, FVector &Location, FRotator &Rotation, FActorSpawnParameters &SpawnParameters);
	
	void SetComponentFromSlotName(FString &OptionSlotName, int IndexDelta);

	void SetVehicleType(int DesiredOptionIndex);
	void SetPreset(int DesiredOptionIndex);
	void SetPrimaryColor(int DesiredOptionIndex);
	void SetAccentColor(int DesiredOptionIndex);
	
	// Returns current index of specified option slot by name
	FText GetOptionSlotCurrentIndex(FString OptionSlotName);

	static FString VehicleIndexToName(int VehicleIndex);
	
	static int VehicleNameToIndex(FString &VehicleName);

	UPROPERTY()
	int CurrentConfigurationIndex = 0;

	UPROPERTY()
	FPresetVehicleType CurrentVehicleTypeRow;

	UPROPERTY()
	UDataTable* CurrentOptions;

	UPROPERTY()
	TMap<FString, int> CurrentIndices;

	UPROPERTY()
	APolyRacingWheeledVehiclePawn* Vehicle;

	// Config shown on menu and used by default in races
	UPROPERTY()
	int ActiveConfigurationSlotIndex = 0;
	
	TArray<FPresetVehicleConfiguration> SavedConfigurations[5];
	
	void LoadConfiguration(int ConfigurationSlotIndex);
	
	void SaveConfiguration(int ConfigurationSlotIndex);

	// Holds base customisation options
	UPROPERTY()
	UDataTable* VehicleOptions;

	UPROPERTY()
	TArray<FString> VehicleTypeNames;

	UPROPERTY()
	UColorOptions* ColorOptions;

	static UDataTable* LoadDataTableAsset(FString const &Path);
};
