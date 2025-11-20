#include "proto_gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
// include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
// #include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

Aproto_gun::Aproto_gun()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);

	MuzzleOffset = FVector(100.f, 0.f, 10.f);
}

void Aproto_gun::BeginPlay()
{
	Super::BeginPlay();
	
}

void Aproto_gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Aproto_gun::Fire()
{
	// Get the current world, which allows us to spawn actors and perform raycasts. We also need world.time and tick functions
	UWorld* World = GetWorld();
	if (!World) return;

	// check for valid owner pawn
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	// check for valid player controller, so we can get player's camera viewpoint
	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PlayerController) return;

	// get camera location and rotation from player controller
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// calculate the muzzle location
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// line trace parameters for distance, location, and hit objects
	FVector EndTrace = CameraLocation + (CameraRotation.Vector() * 10000.f);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	// perform line trace and record results in boolean
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		MuzzleLocation,
		EndTrace,
		ECC_GameTraceChannel1,
		QueryParams
	);

	// draw the debug line (lifetime 2, layer 0, thickness 1)
	DrawDebugLine(World, MuzzleLocation, EndTrace, FColor::Cyan, false, 2.f, 0, 1.f);

	// handle hit event
	if (bHit)
	{
		DrawDebugPoint(World, HitResult.Location, 10.f, FColor::Red, false, 2.f);
	}
}


