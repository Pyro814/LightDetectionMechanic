// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DetectionLight.generated.h"

UCLASS()
class LIGHTDETECTIONGAME_API ADetectionLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADetectionLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	float GetCurrentOffsetTimePercentage() const;

private:

	UPROPERTY(EditAnywhere)
	float InitialDamagePerSecond = 10.f;

	UPROPERTY(EditAnywhere)
	float DamageIncreasePerSecond = 10.f;

	UPROPERTY(EditAnywhere)
	float DamageBeginingOffset = 3.f;

	UPROPERTY(VisibleAnywhere)
	float CurrentDamagedTime;

	UPROPERTY(VisibleAnywhere)
	float CurrentDamagePerSecond;

	UPROPERTY(VisibleAnywhere)
	class UDirectionalLightComponent* DirectionalLightComponent;

	UPROPERTY(EditAnywhere, Category="FX")
	class UNiagaraSystem* BeamFX;

	UPROPERTY()
	class UNiagaraComponent* BeamFXComponent;

	APawn* PlayerPawn;	

	bool IsPlayerInShadow() const;

	bool PlayerInShadow;

	// Track previous seen state so we only toggle once on change
	bool bWasPlayerSeen = false;
};
