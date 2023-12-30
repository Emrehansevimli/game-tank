// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class PROTO4_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();

	void HandleDestruction();

	APlayerController* TurretPlayerController;
	APlayerController* GetTurretPlayerController() const { return TurretPlayerController; }

protected:
	virtual void BeginPlay() override;

	void RotateTurret(FVector LookAtTarget);
	void Move(float Value);
	void Turn(float Value); 
	void Rotate(float Value); 
	void Fire();
	void ArrangeHeight(int DistanceRear, int DistanceMiddle, int DistanceForward);
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

	//tank fizik
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ForwardDistancePoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RearDistancePoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Comps", meta = (AllowPrivateAccess = "true"))
	USceneComponent* MiddleDistancePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float speed = 700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float turnrate = 100.f;
	float FireRange = 700.f;
	float Dist;
	bool IsClimbed = false;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	FHitResult ForwardHit;
	FHitResult RearHit;
	FHitResult MiddleHit;
};
