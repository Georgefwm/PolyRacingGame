#include "Customisation/VehicleCustomiser.h"

#include "DataTables.h"
#include "Engine/DataTable.h"


TSharedPtr<FVehicleCustomiser> FVehicleCustomiser::Instance = nullptr;

void FVehicleCustomiser::Initialize()
{
	if (!Instance.IsValid())
	{
		Instance = Create();
	}
}

void FVehicleCustomiser::Shutdown()
{
	ensure(Instance.IsUnique()); 
	Instance.Reset();
}

TSharedRef<FVehicleCustomiser> FVehicleCustomiser::Create()
{
	TSharedRef<FVehicleCustomiser> NewCustomiser = MakeShareable(new FVehicleCustomiser());

	// Import vehicle customisation data table
	FString const VehicleTypesPath = TEXT("/Script/Engine.DataTable'/Game/VehicleCustomisation/VehicleTypes.VehicleTypes'");
	NewCustomiser->VehicleTypes = LoadDataTableAsset(VehicleTypesPath);

	if (!NewCustomiser->VehicleTypes)
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No vehicle type data table found!"));

	// Import exotic options data table
	FString const ExoticOptionsPath = TEXT("/Script/Engine.DataTable'/Game/VehicleCustomisation/ExoticOptions.ExoticOptions'");
	UDataTable* Test = LoadDataTableAsset(ExoticOptionsPath);
	NewCustomiser->VehicleOptions.Emplace(FString("Exotic"), Test);

	if (!Test)
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No exotic options data table found!"));
	
	return NewCustomiser;
}

void FVehicleCustomiser::SetupVehicle()
{
	SetupVehicle(FVehicleConfiguration());
}

// Sets up the preview vehicle with default configuration
void FVehicleCustomiser::SetupVehicle(FVehicleConfiguration DesiredConfig)
{
	if (!VehicleTypes || VehicleOptions.IsEmpty())
		return;

	// Set what type of car is being used and available customisation options
	FVehicleType* const SelectedType = VehicleTypes->FindRow<FVehicleType>(FName(DesiredConfig.VehicleType), "");
	UDataTable* const OptionSlots = VehicleOptions.FindRef(SelectedType->VehicleName);
	
	if (!OptionSlots)
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No vehicle Options found!"));
		return;
	}

	// Mesh and offset setting

	PreviewVehicle->BodyMesh->SetSkeletalMesh(SelectedType->Mesh.LoadSynchronous());
	PreviewVehicle->SetWheelOffsets(SelectedType);

	
	FVehicleSlotOptions* Bonnet = OptionSlots->FindRow<FVehicleSlotOptions>(FName("Bonnet"), "");
	if (!Bonnet->Meshes.IsEmpty())
	{
		PreviewVehicle->BonnetMesh->SetSkeletalMesh(Bonnet->Meshes[DesiredConfig.Bonnet].LoadSynchronous());
		PreviewVehicle->BonnetMesh->SetRelativeLocation(Bonnet->Offsets[DesiredConfig.Bonnet], false);
	}

	FVehicleSlotOptions* FrontBumper = OptionSlots->FindRow<FVehicleSlotOptions>(FName("BumperFront"), "");
	if (!FrontBumper->Meshes.IsEmpty())
	{
		PreviewVehicle->BumperFrontMesh->SetSkeletalMesh(FrontBumper->Meshes[DesiredConfig.BumperFront].LoadSynchronous());
		PreviewVehicle->BumperFrontMesh->SetRelativeLocation(FrontBumper->Offsets[DesiredConfig.BumperFront], false);
	}

	FVehicleSlotOptions* RearBumper = OptionSlots->FindRow<FVehicleSlotOptions>(FName("BumperRear"), "");
	if (!RearBumper->Meshes.IsEmpty())
	{
		PreviewVehicle->BumperRearMesh->SetSkeletalMesh(RearBumper->Meshes[DesiredConfig.BumperRear].LoadSynchronous());
		PreviewVehicle->BumperRearMesh->SetRelativeLocation(RearBumper->Offsets[DesiredConfig.BumperRear], false);
	}

	FVehicleSlotOptions* SideSkirts = OptionSlots->FindRow<FVehicleSlotOptions>(FName("SideSkirt"), "");
	if (!SideSkirts->Meshes.IsEmpty())
	{
		PreviewVehicle->SideSkirtMesh->SetSkeletalMesh(SideSkirts->Meshes[DesiredConfig.SideSkirt].LoadSynchronous());
		PreviewVehicle->SideSkirtMesh->SetRelativeLocation(SideSkirts->Offsets[DesiredConfig.SideSkirt], false);
	}

	FVehicleSlotOptions* Rims = OptionSlots->FindRow<FVehicleSlotOptions>(FName("Rim"), "");
	if (!Rims->Meshes.IsEmpty())
	{
		SetRim(Rims->Meshes[DesiredConfig.Rim].LoadSynchronous());
	}

	FVehicleSlotOptions* Tyres = OptionSlots->FindRow<FVehicleSlotOptions>(FName("Tyre"), "");
	if (!Tyres->Meshes.IsEmpty())
	{
		SetTyre(Tyres->Meshes[DesiredConfig.Rim].LoadSynchronous());
	}
	
}

void FVehicleCustomiser::SetRim(USkeletalMesh* Mesh)
{
	PreviewVehicle->FrontLeftRim->SetSkeletalMesh(Mesh);
	PreviewVehicle->FrontRightRim->SetSkeletalMesh(Mesh);
	PreviewVehicle->RearLeftRim->SetSkeletalMesh(Mesh);
	PreviewVehicle->RearRightRim->SetSkeletalMesh(Mesh);
}

void FVehicleCustomiser::SetTyre(USkeletalMesh* Mesh)
{
	PreviewVehicle->FrontLeftTyre->SetSkeletalMesh(Mesh);
	PreviewVehicle->FrontRightTyre->SetSkeletalMesh(Mesh);
	PreviewVehicle->RearLeftTyre->SetSkeletalMesh(Mesh);
	PreviewVehicle->RearRightTyre->SetSkeletalMesh(Mesh);
}

UDataTable* FVehicleCustomiser::LoadDataTableAsset(FString const &Path)
{
	// First check if the table is loaded already
	if (UDataTable* LoadingDataTable = Cast<UDataTable>(FSoftObjectPath(*Path).ResolveObject()))
		return LoadingDataTable;

	// return last attempt
	return Cast<UDataTable>(FSoftObjectPath(*Path).TryLoad());
}

TSharedPtr<FVehicleCustomiser> FVehicleCustomiser::Get()
{
	return Instance;
}
