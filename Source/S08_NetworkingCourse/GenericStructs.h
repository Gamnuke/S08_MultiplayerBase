// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericStructs.generated.h"

USTRUCT()
struct FPlayerData {
	GENERATED_BODY()

	UPROPERTY()
	int PlayerID;

	UPROPERTY()
	FString PlayerName;

	APlayerState* PlayerState;
};

/**
 * 
 */
class S08_NETWORKINGCOURSE_API GenericStructs
{
public:
	GenericStructs();
	~GenericStructs();
};
