// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h"
#include "LightInteractionInterface.h"
#include "Bob.generated.h"

UCLASS()
class LIGHTDETECTIONGAME_API ABob : public ACharacter, public ICombatInterface, public ILightInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABob();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when damage is aplied
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual float GetHealthPercentage_Implementation(AActor* Interactor) override;

	// Toggle particle effects when player is seen/not seen
	virtual void ActivateDetectionParticles_Implementation(AActor* Interactor) override;
	virtual void DeactivateDetectionParticles_Implementation(AActor* Interactor) override;

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	float RotationRate = 500.f;

	// Pitch clamp limits (degrees)
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinPitch = -30.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxPitch = 15.f;

	// Require left mouse button to be held for pitch input
	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bRequireLeftMouseForLook = true;

	// Runtime state for left mouse
	bool bIsLeftMouseDown = false;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere)
	float CurrentHealth;

	// Particle system shown when the player is detected (seen)
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* DetectionParticles;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	void OnLeftMouseDown();
	void OnLeftMouseUp();

};
