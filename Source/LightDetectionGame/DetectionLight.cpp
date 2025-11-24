// Fill out your copyright notice in the Description page of Project Settings.

#include "DetectionLight.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DirectionalLightComponent.h"
#include "LightInteractionInterface.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
ADetectionLight::ADetectionLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DirectionalLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Directional Light Component"));
}

// Called when the game starts or when spawned
void ADetectionLight::BeginPlay()
{
	Super::BeginPlay();

	CurrentDamagePerSecond = InitialDamagePerSecond;

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (BeamFX)
	{
    	BeamFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			BeamFX,
			FVector::ZeroVector,  // We'll set position dynamically
			FRotator::ZeroRotator,
			FVector(1.0f),
			false,
			false,
			ENCPoolMethod::None,
			true    // PreCullCheck
    	);

		// Ensure world-space control (important for beams)
		BeamFXComponent->SetUsingAbsoluteLocation(true);
	}
}	


// Called every frame
void ADetectionLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Determine seen state
	bool bPlayerInShadow = IsPlayerInShadow();
	bool bPlayerSeen = !bPlayerInShadow;

	// Toggle detection particles via interface only on change
	if(bPlayerSeen != bWasPlayerSeen)
	{
		if(PlayerPawn && PlayerPawn->GetClass()->ImplementsInterface(ULightInteractionInterface::StaticClass()))
		{
			if(bPlayerSeen)
			{
				ILightInteractionInterface::Execute_ActivateDetectionParticles(PlayerPawn, this);
				BeamFXComponent->Activate();
			}
			else
			{
				ILightInteractionInterface::Execute_DeactivateDetectionParticles(PlayerPawn, this);
				BeamFXComponent->Deactivate();
			}
		}

		bWasPlayerSeen = bPlayerSeen;
	}

	if(bPlayerInShadow)
	{
		// Recover light resistance over time
		UE_LOG(LogTemp, Display, TEXT("Not Seen"));
		CurrentDamagePerSecond = FMath::Max(InitialDamagePerSecond, CurrentDamagePerSecond - (DamageIncreasePerSecond * DeltaTime));
		CurrentDamagedTime = FMath::Max(0, CurrentDamagedTime - DeltaTime);
	}
	else
	{
		// Take damage and increase damage over time
		UE_LOG(LogTemp, Display, TEXT("Seen"));
		CurrentDamagedTime += DeltaTime;
		if(PlayerPawn && CurrentDamagedTime > DamageBeginingOffset)
		{
			CurrentDamagePerSecond += DamageIncreasePerSecond * DeltaTime;
			UGameplayStatics::ApplyDamage(PlayerPawn, CurrentDamagePerSecond * DeltaTime, nullptr, this, nullptr);
		}   
	}

}

bool ADetectionLight::IsPlayerInShadow() const
{
	FHitResult Hit;
	if(PlayerPawn)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(PlayerPawn);

		FVector Start = PlayerPawn -> GetActorLocation();
		Start.Z -= PlayerPawn -> GetDefaultHalfHeight();
		FVector End = Start - (DirectionalLightComponent -> GetForwardVector() * 10000);

		GetWorld() -> LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, Params);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

		if (BeamFXComponent)
        {
            BeamFXComponent->SetNiagaraVariableVec3(TEXT("User.BeamStart"), Start);
            BeamFXComponent->SetNiagaraVariableVec3(TEXT("User.BeamEnd"), End);
        }

		return Hit.bBlockingHit;
	}

	return false;
}

float ADetectionLight::GetCurrentOffsetTimePercentage() const
{
	return CurrentDamagedTime/DamageBeginingOffset;
}

