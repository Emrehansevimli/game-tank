// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "BasePawn.h"
#include "TimerManager.h"

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	
	BasePawn = Cast<ABasePawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	//GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretPawn::CheckFireCondition, FireRate, true);

}

// Called every frame
void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BasePawn)
	{
		Dist = FVector::Dist(GetActorLocation(), BasePawn->GetActorLocation());
		if (Dist <= FireRange)
		{
			RotateTurret(BasePawn->GetActorLocation());
		}
	}

}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurretPawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(LookAtRotation);
	UE_LOG(LogTemp, Warning, TEXT("Some warning message") );
}

void ATurretPawn::Fire()
{
	FVector ProjectileSpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
	FHitResult HitResult;
	PlayerControllerRef->GetHitResultUnderCursor(
	ECollisionChannel::ECC_Visibility, false, HitResult);
	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red, false, 3.f);
	
}

void ATurretPawn::CheckFireCondition() 
{
	if (BasePawn) {

		Dist = FVector::Dist(GetActorLocation(), BasePawn->GetActorLocation());
		if (Dist <= FireRange)
		{
			Fire();
		}
	}
}
