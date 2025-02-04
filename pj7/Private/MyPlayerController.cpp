// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	: InputMappingContext{ nullptr },
	MoveAction{ nullptr },
	JumpAction{ nullptr },
	LookAction{ nullptr },
	SprintAction{ nullptr }
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ULocalPlayer* LocalPlayer = GetLocalPlayer();
	// UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	// Subsystem->AddMappingContext(InputMappingContext, 0);

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
