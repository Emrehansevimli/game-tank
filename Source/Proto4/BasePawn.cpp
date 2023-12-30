// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	//tank fizik
	ForwardDistancePoint = CreateDefaultSubobject<USceneComponent>(TEXT("ForwardDistancePoint"));
	ForwardDistancePoint->SetupAttachment(BaseMesh);

	RearDistancePoint = CreateDefaultSubobject<USceneComponent>(TEXT("RearDistancePoint"));
	RearDistancePoint->SetupAttachment(BaseMesh);
	
	MiddleDistancePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MiddleDistancePoint"));
	MiddleDistancePoint->SetupAttachment(BaseMesh);
}

void ABasePawn::HandleDestruction()
{
	// TODO: Visual/sound effects
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}


// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABasePawn::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABasePawn::Turn);
	PlayerInputComponent->BindAxis(TEXT("Rotate"), this, &ABasePawn::Rotate);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABasePawn::Fire);

}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TurretPlayerController)
	{	
		FHitResult HitResult;
		FHitResult Null;
		TurretPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult);
		RotateTurret(HitResult.ImpactPoint); 
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f);
	}

	//FVector Location = ForwardDistancePoint->GetComponentLocation();
	//FRotator Rotation = RearDistancePoint->GetComponentRotation();
	//FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;

	//ForwardDistance 
	FVector EndForward = ForwardDistancePoint->GetComponentLocation() + ForwardDistancePoint->GetForwardVector() * 1000;
	GetWorld()->LineTraceSingleByChannel(ForwardHit ,ForwardDistancePoint->GetComponentLocation(), EndForward, ECollisionChannel::ECC_Visibility);
	int DistanceForward = ForwardHit.ImpactPoint.Z - ForwardDistancePoint->GetComponentLocation().Z;
	
	//MiddleDistance 
	FVector MiddleRear = MiddleDistancePoint->GetComponentLocation() + MiddleDistancePoint->GetForwardVector() * 1000;
	GetWorld()->LineTraceSingleByChannel(MiddleHit,MiddleDistancePoint->GetComponentLocation(), MiddleRear ,ECollisionChannel::ECC_Visibility);
	int DistanceMiddle = MiddleHit.ImpactPoint.Z - MiddleDistancePoint->GetComponentLocation().Z;

	//RearDistance 
	FVector EndRear = RearDistancePoint->GetComponentLocation() + RearDistancePoint->GetForwardVector() * 1000;
	GetWorld()->LineTraceSingleByChannel(RearHit,RearDistancePoint->GetComponentLocation(), EndRear ,ECollisionChannel::ECC_Visibility);
	int DistanceRear = RearHit.ImpactPoint.Z - RearDistancePoint->GetComponentLocation().Z;

	DrawDebugSphere(GetWorld(), ForwardHit.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f);
	DrawDebugSphere(GetWorld(), MiddleHit.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f);
	DrawDebugSphere(GetWorld(), RearHit.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f);
	DrawDebugLine(GetWorld(), ForwardDistancePoint->GetComponentLocation(), ForwardHit.ImpactPoint, FColor::Red);
	DrawDebugLine(GetWorld(), MiddleDistancePoint->GetComponentLocation(), MiddleHit.ImpactPoint, FColor::Red);
	DrawDebugLine(GetWorld(), RearDistancePoint->GetComponentLocation(), RearHit.ImpactPoint, FColor::Red);

	ArrangeHeight(DistanceRear, DistanceMiddle, DistanceForward);

}

void ABasePawn::ArrangeHeight(int DistanceRear, int DistanceMiddle, int DistanceForward)
{	
	FVector UpVector = FVector::UpVector;
	FVector DownVector = FVector::DownVector;
	FVector OnVector = FVector::UpVector;

	if(DistanceForward == DistanceRear && DistanceMiddle <= -10)
	{
		AddActorLocalOffset(DownVector * 10, true);
		
	}
	else if(DistanceForward >= DistanceRear)
	{	
		AddActorLocalOffset(DownVector * (DistanceRear - DistanceForward), true);
		if(IsClimbed == false){
			AddActorLocalOffset(DownVector * (DistanceRear - DistanceForward), true);
			IsClimbed = true;
		}
		UE_LOG(LogTemp, Display, TEXT("Forward = %d"),DistanceForward);
		UE_LOG(LogTemp, Display, TEXT("Middle = %d"),DistanceMiddle);
		UE_LOG(LogTemp, Display, TEXT("Rear = %d"),DistanceRear);
	}
	else if(DistanceForward == DistanceRear && IsClimbed == true)
	{
		IsClimbed = false;
	}
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	TurretPlayerController = Cast<APlayerController>(GetController());
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(TurretMesh->GetComponentRotation(), 
			LookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			5.f));

}

void ABasePawn::Move(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	// x = value * deltatime + speed

	DeltaLocation.X = Value * speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ABasePawn::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	// yaw = value * deltatime * turnrate
	DeltaRotation.Yaw = Value * turnrate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}

void ABasePawn::Rotate(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	// yaw = value * deltatime * turnrate
	DeltaRotation.Yaw = Value * turnrate * UGameplayStatics::GetWorldDeltaSeconds(this);
	TurretMesh->AddLocalRotation(DeltaRotation, true);

}

void ABasePawn::Fire() 
{	
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	//DrawDebugSphere(GetWorld(),Location,25.f,12,FColor::Red,false,3.f);

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	Projectile->SetOwner(this);

}
