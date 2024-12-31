// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoulslikeGameMode.h"
#include "SoulslikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoulslikeGameMode::ASoulslikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
