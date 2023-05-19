#include "Customisation/VehicleCustomiser.h"

#include "DataTables.h"
#include "Editor.h"
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

	// Import vehicle customisation data tables
	FString const VehicleTypesPath = TEXT("/Script/Engine.DataTable'/Game/VehicleCustomisation/VehicleTypes.VehicleTypes'");
	NewCustomiser->VehicleTypes = LoadDataTableAsset(VehicleTypesPath);

	if (!NewCustomiser->VehicleTypes)
		UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No vehicle type data table found!"));
	
	return NewCustomiser;
}

void FVehicleCustomiser::SetupVehicle()
{
	if (!VehicleTypes)
		return;

	const FVehicleType* DefaultBody = VehicleTypes->FindRow<FVehicleType>(FName("Exotic"), "");
	
	UE_LOG(LogTemp, Warning, TEXT("###### FVehicleCustomiser Start ######"));
	UE_LOG(LogTemp, Warning, TEXT("MyFunction called with value: %s"), *DefaultBody->Mesh.ToString());
	PreviewVehicle->BodyMesh->SetSkeletalMesh(DefaultBody->Mesh.LoadSynchronous());
	
	// PreviewVehicle->WheelFrontLeft->SetRelativeLocation(FVector(-DefaultBody->AxelLength, DefaultBody->FrontAxelOffset, 0.f));
	// PreviewVehicle->WheelFrontRight->SetRelativeLocation(FVector(DefaultBody->AxelLength, DefaultBody->FrontAxelOffset, 0.f));
	// PreviewVehicle->WheelFrontLeft->SetRelativeLocation(FVector(-DefaultBody->AxelLength, DefaultBody->RearAxelOffset, 0.f));
	// PreviewVehicle->WheelFrontRight->SetRelativeLocation(FVector(DefaultBody->AxelLength, DefaultBody->RearAxelOffset, 0.f));
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
