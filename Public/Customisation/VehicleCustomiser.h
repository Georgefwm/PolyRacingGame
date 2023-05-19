#pragma once
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
	
    AVehiclePreview* PreviewVehicle;

	// Holds base vehicle types
	TSoftObjectPtr<UDataTable> VehicleTypes;
};
