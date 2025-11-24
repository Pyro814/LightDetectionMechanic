// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LightInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULightInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIGHTDETECTIONGAME_API ILightInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface")
    void ActivateDetectionParticles(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface")
    void DeactivateDetectionParticles(AActor* Interactor);
};
