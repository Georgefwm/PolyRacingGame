#include "Customisation/VehicleCustomiser.h"

#include "DataTables.h"
#include "HairStrandsInterface.h"
#include "Engine/DataTable.h"


TSharedPtr<FVehicleCustomiser> FVehicleCustomiser::Instance = nullptr;

void FVehicleCustomiser::Initialize()
{
	if (!Instance.IsValid())
		Instance = Create();
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

// Sets up the preview vehicle with default configuration
void FVehicleCustomiser::SetupVehicle()
{
	SetupVehicle(FVehicleConfiguration());
}

void FVehicleCustomiser::SetupVehicle(FVehicleConfiguration DesiredConfig)
{
	if (!VehicleTypes || VehicleOptions.IsEmpty())
		return;

	// Set what type of car is being used and available customisation options
	CurrentVehicleType = VehicleTypes->FindRow<FVehicleType>(FName(DesiredConfig.VehicleType), "");
	CurrentOptions = VehicleOptions.FindRef(CurrentVehicleType->VehicleName);
	
	if (!CurrentOptions)
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No vehicle Options found!"));
		return;
	}

	// Mesh and offset setting

	PreviewVehicle->BodyMesh->SetSkeletalMesh(CurrentVehicleType->Mesh.LoadSynchronous());
	
	PreviewVehicle->SetWheelOffsets(CurrentVehicleType);
	
	SetBonnet(		DesiredConfig.Bonnet		);
	SetBumperFront(	DesiredConfig.BumperFront	);
	SetBumperRear(	DesiredConfig.BumperRear	);
	SetSideSkirt(	DesiredConfig.SideSkirt		);
	SetRim(			DesiredConfig.Rim			);
	SetTyre(		DesiredConfig.Tyre			);
}

// Is there a better way to do this? seems very verbose
// See SOptionSelectionWidget
void FVehicleCustomiser::SetComponentFromSlotName(FString &OptionSlotName, int IndexDelta)
{
	if (OptionSlotName == TEXT("Bonnet"))		return SetBonnet(		IndexDelta + *CurrentIndices.Find(OptionSlotName));
	if (OptionSlotName == TEXT("BumperFront"))	return SetBumperFront(	IndexDelta + *CurrentIndices.Find(OptionSlotName));
	if (OptionSlotName == TEXT("BumperRear"))	return SetBumperRear(	IndexDelta + *CurrentIndices.Find(OptionSlotName));
	if (OptionSlotName == TEXT("SideSkirt"))	return SetSideSkirt(	IndexDelta + *CurrentIndices.Find(OptionSlotName));
	if (OptionSlotName == TEXT("Rim"))			return SetRim(			IndexDelta + *CurrentIndices.Find(OptionSlotName));
	if (OptionSlotName == TEXT("Tyre"))			return SetTyre(			IndexDelta + *CurrentIndices.Find(OptionSlotName));
	
	UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: OptionSlot name not found!"));
}

void FVehicleCustomiser::SetBonnet(int DesiredOptionIndex)
{
	FVehicleSlotOptions* Bonnet = CurrentOptions->FindRow<FVehicleSlotOptions>(FName("Bonnet"), "");
	if (Bonnet->Meshes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No Options were found for 'Bonnet'!"));
		return;
	}
	
	int const UsingIndex = DesiredOptionIndex % Bonnet->Meshes.Num();
	
	PreviewVehicle->BonnetMesh->SetSkeletalMesh(Bonnet->Meshes[UsingIndex].LoadSynchronous());
	PreviewVehicle->BonnetMesh->SetRelativeLocation(Bonnet->Offsets[UsingIndex], false);

	CurrentIndices.Add(FString("Bonnet"), UsingIndex);
}

void FVehicleCustomiser::SetBumperFront(int DesiredOptionIndex)
{
	FVehicleSlotOptions* BumperFront = CurrentOptions->FindRow<FVehicleSlotOptions>(FName("BumperFront"), "");
	if (BumperFront->Meshes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No Options were found for 'BumperFront'!"));
		return;
	}
	
	int const UsingIndex = DesiredOptionIndex % BumperFront->Meshes.Num();
	
	PreviewVehicle->BumperFrontMesh->SetSkeletalMesh(BumperFront->Meshes[UsingIndex].LoadSynchronous());
	PreviewVehicle->BumperFrontMesh->SetRelativeLocation(BumperFront->Offsets[UsingIndex], false);

	CurrentIndices.Add(FString("BumperFront"), UsingIndex);
}

void FVehicleCustomiser::SetBumperRear(int DesiredOptionIndex)
{
	FVehicleSlotOptions* BumperRear = CurrentOptions->FindRow<FVehicleSlotOptions>(FName("BumperRear"), "");
	if (BumperRear->Meshes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No Options were found for 'BumperRear'!"));
		return;
	}
	
	int const UsingIndex = DesiredOptionIndex % BumperRear->Meshes.Num();
	
	PreviewVehicle->BumperRearMesh->SetSkeletalMesh(BumperRear->Meshes[UsingIndex].LoadSynchronous());
	PreviewVehicle->BumperRearMesh->SetRelativeLocation(BumperRear->Offsets[UsingIndex], false);

	CurrentIndices.Add(FString("BumperRear"), UsingIndex);
}

void FVehicleCustomiser::SetSideSkirt(int DesiredOptionIndex)
{
	FVehicleSlotOptions* SideSkirt = CurrentOptions->FindRow<FVehicleSlotOptions>(FName("SideSkirt"), "");
	if (SideSkirt->Meshes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No Options were found for 'SideSkirt'!"));
		return;
	}
	
	int const UsingIndex = DesiredOptionIndex % SideSkirt->Meshes.Num();
	
	PreviewVehicle->SideSkirtMesh->SetSkeletalMesh(SideSkirt->Meshes[UsingIndex].LoadSynchronous());
	PreviewVehicle->SideSkirtMesh->SetRelativeLocation(SideSkirt->Offsets[UsingIndex], false);

	CurrentIndices.Add(FString("SideSkirt"), UsingIndex);
}

void FVehicleCustomiser::SetRim(int DesiredOptionIndex)
{
	FVehicleSlotOptions* Rim = CurrentOptions->FindRow<FVehicleSlotOptions>(FName("Rim"), "");
	if (Rim->Meshes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No Options were found for 'Rim'!"));
		return;
	}
	
	int const UsingIndex = DesiredOptionIndex % Rim->Meshes.Num();
	
	USkeletalMesh* NewRim = Rim->Meshes[UsingIndex].LoadSynchronous();
	
	PreviewVehicle->FrontLeftRim->SetSkeletalMesh(NewRim);
	PreviewVehicle->FrontRightRim->SetSkeletalMesh(NewRim);
	PreviewVehicle->RearLeftRim->SetSkeletalMesh(NewRim);
	PreviewVehicle->RearRightRim->SetSkeletalMesh(NewRim);

	CurrentIndices.Add(FString("Rim"), UsingIndex);
}

void FVehicleCustomiser::SetTyre(int DesiredOptionIndex)
{
	FVehicleSlotOptions* Tyre = CurrentOptions->FindRow<FVehicleSlotOptions>(FName("Tyre"), "");
	if (Tyre->Meshes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No Options were found for 'Tyre'!"));
		return;
	}
	
	int const UsingIndex = DesiredOptionIndex % Tyre->Meshes.Num();
	
	USkeletalMesh* NewTyre = Tyre->Meshes[UsingIndex].LoadSynchronous();
	
	PreviewVehicle->FrontLeftTyre->SetSkeletalMesh(NewTyre);
	PreviewVehicle->FrontRightTyre->SetSkeletalMesh(NewTyre);
	PreviewVehicle->RearLeftTyre->SetSkeletalMesh(NewTyre);
	PreviewVehicle->RearRightTyre->SetSkeletalMesh(NewTyre);

	CurrentIndices.Add(FString("Tyre"), UsingIndex);
}

FText FVehicleCustomiser::GetOptionSlotCurrentIndex(FString OptionSlotName)
{
	// TODO: "Style X" unless Vehicle type
	
	FString ret = "Style ";
	ret.AppendInt(*CurrentIndices.Find(OptionSlotName));
	return FText::FromString(ret);
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
