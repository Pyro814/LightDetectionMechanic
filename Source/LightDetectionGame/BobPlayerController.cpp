// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/UserWidget.h"
#include "BobPlayerController.h"

void ABobPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if(HUD)
    {
        HUD -> AddToViewport();
    }
}