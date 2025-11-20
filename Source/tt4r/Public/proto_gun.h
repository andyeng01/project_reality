#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "proto_gun.generated.h"

UCLASS()
class TT4R_API Aproto_gun : public AActor
{
	GENERATED_BODY()
	
public:	
	Aproto_gun();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* GunMesh;

	void Fire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;
};
