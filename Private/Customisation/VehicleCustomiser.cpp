#include "Customisation/VehicleCustomiser.h"

#include "DataTables.h"
#include "CoreMinimal.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetMathLibrary.h"


void UVehicleCustomiser::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Vehicles
	FString const VehicleTypesPath = TEXT("/Script/Engine.DataTable'/Game/VehicleCustomisation/VehiclePresets.VehiclePresets'");
	VehicleOptions = LoadDataTableAsset(VehicleTypesPath);
	if (!VehicleOptions) UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No vehicle data table found!"));

	for (FName VehicleName : VehicleOptions->GetRowNames())
		VehicleTypeNames.Add(VehicleName.ToString());

	// Colors
	FSoftObjectPath ColorOptionsPath = FSoftObjectPath(TEXT("/Script/PolyRacingGame.ColorOptions'/Game/VehicleCustomisation/DA_ColorOptions.DA_ColorOptions'"));
	ColorOptions = Cast<UColorOptions>(ColorOptionsPath.ResolveObject());
	if (!ColorOptions)
	{
		ColorOptions = CastChecked<UColorOptions>(ColorOptionsPath.TryLoad());
	}
	if (!ColorOptions) UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No color data found!"));

	// Set default configurations
	for (int Index = 0; Index < 5; Index++)
	{
		FPresetVehicleConfiguration Default = FPresetVehicleConfiguration();
		Default.VehicleType = UVehicleCustomiser::VehicleIndexToName(Index);
		SavedConfigurations->Add(Default);
	}
}

void UVehicleCustomiser::Deinitialize()
{
	Super::Deinitialize();
}

// Sets up the preview vehicle with default configuration
void UVehicleCustomiser::SetupVehicle()
{
	ActiveConfigurationSlotIndex = 0;
	SetupVehicle(SavedConfigurations->GetData()[0]);
}

void UVehicleCustomiser::SetupVehicle(FPresetVehicleConfiguration DesiredConfig)
{
	if (!VehicleOptions)
		return;

	if (Vehicle)
	{
		Vehicle->Destroy();
		//GEngine->GetWorld()->CleanupActors();
	}

	// Set what type of car is being used and available customisation options
	CurrentVehicleTypeRow = *VehicleOptions->FindRow<FPresetVehicleType>(FName(DesiredConfig.VehicleType), "");
	
	CurrentIndices.Add(TEXT("VehicleType"), VehicleNameToIndex(DesiredConfig.VehicleType));

	FVector Location = FVector(-2020.f, -1700.f, 10.f);
	FRotator Rotation = FRotator(0.f, 90.f, 0.f);
	
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	Vehicle = GetWorld()->SpawnActor<APolyRacingWheeledVehiclePawn>(
		CurrentVehicleTypeRow.Presets.GetData()[DesiredConfig.Preset]->GetDefaultObject()->GetClass(),
		Location,
		Rotation,
		SpawnParameters);

	CurrentIndices.Add(TEXT("Preset"), DesiredConfig.Preset);
	
	SetPrimaryColor(DesiredConfig.PrimaryColor);
	SetAccentColor(DesiredConfig.AccentColor);

	CurrentIndices.Add(TEXT("PrimaryColor"), DesiredConfig.PrimaryColor);
	CurrentIndices.Add(TEXT("AccentColor"), DesiredConfig.AccentColor);
}

APolyRacingWheeledVehiclePawn* UVehicleCustomiser::SpawnVehicle(UWorld* World, FVector Location, FRotator Rotation, FActorSpawnParameters SpawnParameters)
{
	SavedConfigurations->GetData()[ActiveConfigurationSlotIndex].Preset;
	//UVehicleCustomiser* NewVehicle = World->SpawnActor(, Location, Rotation, SpawnParameters);

	if (!VehicleOptions)
		return nullptr;

	if (Vehicle)
	{
		Vehicle->Destroy();
		//GEngine->GetWorld()->CleanupActors();
	}

	// Set what type of car is being used and available customisation options
	CurrentVehicleTypeRow = *VehicleOptions->FindRow<FPresetVehicleType>(FName(SavedConfigurations->GetData()[ActiveConfigurationSlotIndex].VehicleType), "");

	Vehicle = GetWorld()->SpawnActor<APolyRacingWheeledVehiclePawn>(
		CurrentVehicleTypeRow.Presets.GetData()[SavedConfigurations->GetData()[ActiveConfigurationSlotIndex].Preset]->GetDefaultObject()->GetClass(),
		Location,
		Rotation,
		SpawnParameters);
	
	SetPrimaryColor(SavedConfigurations->GetData()[ActiveConfigurationSlotIndex].PrimaryColor);
	SetAccentColor(SavedConfigurations->GetData()[ActiveConfigurationSlotIndex].AccentColor);

	return Vehicle;
}

// Is there a better way to do this? seems very verbose
// See SOptionSelectionWidget
void UVehicleCustomiser::SetComponentFromSlotName(FString &OptionSlotName, int IndexDelta)
{
	if (OptionSlotName == TEXT("VehicleType"))	{ SetVehicleType(	IndexDelta + *CurrentIndices.Find(OptionSlotName)); return; }
	if (OptionSlotName == TEXT("Preset"))		{ SetPreset(		IndexDelta + *CurrentIndices.Find(OptionSlotName)); return; }
	if (OptionSlotName == TEXT("PrimaryColor"))	{ SetPrimaryColor(	IndexDelta + *CurrentIndices.Find(OptionSlotName)); return; }
	if (OptionSlotName == TEXT("AccentColor"))	{ SetAccentColor(	IndexDelta + *CurrentIndices.Find(OptionSlotName)); return; }

	UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: OptionSlot name not found!"));
}

// Change Car Type, Fully resets options
void UVehicleCustomiser::SetVehicleType(int DesiredOptionIndex)
{
	int UsingIndex = DesiredOptionIndex % VehicleOptions->GetRowNames().Num();
	if (UsingIndex < 0) UsingIndex = VehicleOptions->GetRowNames().Num()-1;

	FPresetVehicleConfiguration NewConfiguration = FPresetVehicleConfiguration();
	NewConfiguration.VehicleType = VehicleIndexToName(UsingIndex);

	// TODO: Choose random Colors
	NewConfiguration.PrimaryColor	= *CurrentIndices.Find(TEXT("PrimaryColor"));
	NewConfiguration.AccentColor	= *CurrentIndices.Find(TEXT("AccentColor"));
	
	SetupVehicle(NewConfiguration);
}

void UVehicleCustomiser::SetPreset(int DesiredOptionIndex)
{
	int UsingIndex = DesiredOptionIndex % CurrentVehicleTypeRow.Presets.Num();
	if (UsingIndex < 0) UsingIndex = CurrentVehicleTypeRow.Presets.Num()-1;
	
	FPresetVehicleConfiguration NewConfiguration = FPresetVehicleConfiguration();
	NewConfiguration.VehicleType	= VehicleIndexToName(*CurrentIndices.Find(TEXT("VehicleType")));
	NewConfiguration.Preset			= UsingIndex;
	NewConfiguration.PrimaryColor	= *CurrentIndices.Find(TEXT("PrimaryColor"));
	NewConfiguration.AccentColor	= *CurrentIndices.Find(TEXT("AccentColor"));
	
	SetupVehicle(NewConfiguration);
}

void UVehicleCustomiser::SetPrimaryColor(int DesiredOptionIndex)
{
	int UsingIndex = DesiredOptionIndex % ColorOptions->MaterialInstances.Num();
	if (UsingIndex < 0) UsingIndex = ColorOptions->MaterialInstances.Num()-1;
	
	Vehicle->VehicleCustomisationComponent->SetPrimaryColor(ColorOptions->MaterialInstances[UsingIndex].LoadSynchronous());
	CurrentIndices.Add(TEXT("PrimaryColor"), UsingIndex);
}

void UVehicleCustomiser::SetAccentColor(int DesiredOptionIndex)
{
	int UsingIndex = DesiredOptionIndex % ColorOptions->MaterialInstances.Num();
	if (UsingIndex < 0) UsingIndex = ColorOptions->MaterialInstances.Num()-1;
	
	Vehicle->VehicleCustomisationComponent->SetAccentColor(ColorOptions->MaterialInstances[UsingIndex].LoadSynchronous());
	CurrentIndices.Add(TEXT("AccentColor"), UsingIndex);
}
 
FText UVehicleCustomiser::GetOptionSlotCurrentIndex(FString OptionSlotName)
{
	if (OptionSlotName == TEXT("VehicleType"))
		return FText::FromString(CurrentVehicleTypeRow.VehicleName);
	
	FString IndexText = TEXT("Style ");
	IndexText.AppendInt(*CurrentIndices.Find(OptionSlotName));
	
	return FText::FromString(IndexText);
}

FString UVehicleCustomiser::VehicleIndexToName(int VehicleIndex)
{
	if (VehicleIndex == 0)	return TEXT("Exotic");
	if (VehicleIndex == 1)	return TEXT("Sedan");
	if (VehicleIndex == 2)	return TEXT("Hatch");
	if (VehicleIndex == 3)	return TEXT("Muscle");
	if (VehicleIndex == 4)	return TEXT("Sport");
	
	UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: Unrecognised vehicle index!"));
	return TEXT("");
}

int UVehicleCustomiser::VehicleNameToIndex(FString &VehicleName)
{
	if (VehicleName == TEXT("Exotic"))	return 0;
	if (VehicleName == TEXT("Sedan"))	return 1;
	if (VehicleName == TEXT("Hatch"))	return 2;
	if (VehicleName == TEXT("Muscle"))	return 3;
	if (VehicleName == TEXT("Sport"))	return 4;

	UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: Unrecognised vehicle name!"));
	return 0;
}

void UVehicleCustomiser::LoadConfiguration(int ConfigurationSlotIndex)
{
	if (ConfigurationSlotIndex == CurrentConfigurationIndex)
		return;
	
	if (!SavedConfigurations->IsValidIndex(ConfigurationSlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: Save - Invalid configuration slot index"));
		return;
	}
	
	CurrentConfigurationIndex = ConfigurationSlotIndex;
	SetupVehicle(SavedConfigurations->GetData()[CurrentConfigurationIndex]);
}

void UVehicleCustomiser::SaveConfiguration(int ConfigurationSlotIndex)
{
	if (!SavedConfigurations)
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: SavedConfigurations is not initialised"));
		return;
	}
	
	if (!SavedConfigurations->IsValidIndex(ConfigurationSlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: Save - Invalid configuration slot index"));
		return;
	}
	
	FPresetVehicleConfiguration ConfigurationToSave = FPresetVehicleConfiguration();
	
	ConfigurationToSave.VehicleType		= VehicleIndexToName(*CurrentIndices.Find("VehicleType"));
	ConfigurationToSave.Preset			= *CurrentIndices.Find("Preset");
	ConfigurationToSave.PrimaryColor	= *CurrentIndices.Find("PrimaryColor");
	ConfigurationToSave.AccentColor		= *CurrentIndices.Find("AccentColor");
	
	SavedConfigurations->GetData()[ConfigurationSlotIndex] = ConfigurationToSave;
}

UDataTable* UVehicleCustomiser::LoadDataTableAsset(FString const &Path)
{
	// First check if the table is loaded already
	if (UDataTable* LoadingDataTable = Cast<UDataTable>(FSoftObjectPath(*Path).ResolveObject()))
		return LoadingDataTable;

	// return last attempt
	return Cast<UDataTable>(FSoftObjectPath(*Path).TryLoad());
}
