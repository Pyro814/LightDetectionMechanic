// Fill out your copyright notice in the Description page of Project Settings.

#include "Bob.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABob::ABob()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm -> bUsePawnControlRotation = true;
    SpringArm -> SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
    CameraComponent -> SetupAttachment(SpringArm);

	DetectionParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DetectionParticles"));
	DetectionParticles->SetupAttachment(RootComponent);
	DetectionParticles->bAutoActivate = false;
}

// Called to bind functionality to input
void ABob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent -> BindAxis(TEXT("MoveForward"), this, &ABob::MoveForward);
	PlayerInputComponent -> BindAxis(TEXT("MoveRight"), this, &ABob::MoveRight);
	PlayerInputComponent -> BindAxis(TEXT("LookUp"), this, &ABob::LookUp);
	PlayerInputComponent -> BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent -> BindAxis(TEXT("LookUpRate"), this, &ABob::LookUpRate);
	PlayerInputComponent -> BindAxis(TEXT("LookRightRate"), this, &ABob::LookRightRate);

	// Bind left mouse press/release so pitch only changes while held
	PlayerInputComponent->BindAction(TEXT("PressToLookUp"), IE_Pressed, this, &ABob::OnLeftMouseDown);
	PlayerInputComponent->BindAction(TEXT("PressToLookUp"), IE_Released, this, &ABob::OnLeftMouseUp);
}

void ABob::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ABob::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ABob::LookUpRate(float AxisValue)
{
	if (!Controller) return;

	float Delta = AxisValue * RotationRate * GetWorld() -> GetDeltaSeconds();
	FRotator NewRot = Controller->GetControlRotation();
	{
		float NewPitch = FRotator::NormalizeAxis(NewRot.Pitch + Delta);
		NewRot.Pitch = FMath::Clamp(NewPitch, MinPitch, MaxPitch);
	}
	Controller->SetControlRotation(NewRot);
}

void ABob::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld() -> GetDeltaSeconds());
}

void ABob::LookUp(float AxisValue)
{
	if (!Controller) return;

	// If requiring left mouse and it's not held, ignore pitch input
	if (bRequireLeftMouseForLook && !bIsLeftMouseDown) return;

	// AxisValue here is the raw input (e.g. mouse) so apply directly
	FRotator NewRot = Controller->GetControlRotation();
	{
		float NewPitch = FRotator::NormalizeAxis(NewRot.Pitch + AxisValue);
		NewRot.Pitch = FMath::Clamp(NewPitch, MinPitch, MaxPitch);
	}
	Controller->SetControlRotation(NewRot);
}

void ABob::OnLeftMouseDown()
{
	bIsLeftMouseDown = true;
}

void ABob::OnLeftMouseUp()
{
	bIsLeftMouseDown = false;
}

// Called when the game starts or when spawned
void ABob::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	DetectionParticles->DeactivateSystem();

}

// Called every frame
void ABob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABob::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = FMath::Min(CurrentHealth, Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));
	CurrentHealth -= DamageToApply;

	return DamageToApply;
}

float ABob::GetHealthPercentage_Implementation(AActor* Interactor)
{
	return CurrentHealth/MaxHealth;
}

void ABob::ActivateDetectionParticles_Implementation(AActor* Interactor)
{
	if(DetectionParticles)
	{
		DetectionParticles->ActivateSystem(true);
	}
}

void ABob::DeactivateDetectionParticles_Implementation(AActor* Interactor)
{
	if(DetectionParticles)
	{
		DetectionParticles->DeactivateSystem();
	}
}

