#pragma once
#include "DataTables.h"
#include "VehiclePreview.h"
#include "Engine/DataTable.h"

class POLYRACINGGAME_API FVehicleCustomiser
{
public:
	// Registers a new TSharedPtr instance
	static void Initialize();

	// Attempts to clean up the singleton instance TSharedPtr
	static void Shutdown();

	// Returns the stored singleton instance
	static TSharedPtr<FVehicleCustomiser> Get();

private:
	// Creates the singleton instance
	static TSharedRef<FVehicleCustomiser> Create(); 
	
	// Stores the singleton instance
	static TSharedPtr<FVehicleCustomiser> Instance;

public:
	void SetupVehicle();
	void SetupVehicle(FVehicleConfiguration);
	
	void SetComponentFromSlotName(FString &OptionSlotName, int IndexDelta);

	void SetVehicleType(int DesiredOptionIndex);
	void SetBonnet(int DesiredOptionIndex);
	void SetBumperFront(int DesiredOptionIndex);
	void SetBumperRear(int DesiredOptionIndex);
	void SetSideSkirt(int DesiredOptionIndex);
	void SetRim(int DesiredOptionIndex);
	void SetTyre(int DesiredOptionIndex);

	FText GetOptionSlotCurrentIndex(FString OptionSlotName);

	TArray<FVehicleConfiguration> SavedConfigurations[5];

	FVehicleType* CurrentVehicleType;
	UDataTable* CurrentOptions;
	TMap<FString, int> CurrentIndices;
	
    AVehiclePreview* PreviewVehicle;

	// Holds base vehicle types
	UDataTable* VehicleTypes;
	TArray<FString> VehicleTypeNames;

	TMap<FString, UDataTable*> VehicleOptions;

	static UDataTable* LoadDataTableAsset(FString const &Path);
};
