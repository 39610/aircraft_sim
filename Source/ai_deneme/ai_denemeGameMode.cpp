// Copyright Epic Games, Inc. All Rights Reserved.

#include "ai_denemeGameMode.h"
#include "ai_denemeCharacter.h"
#include "UObject/ConstructorHelpers.h"

Aai_denemeGameMode::Aai_denemeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
