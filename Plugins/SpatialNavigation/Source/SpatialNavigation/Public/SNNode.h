// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class SNNode
{
public:
	FIntVector Coordinates;

	TArray<SNNode*> Neighbors;

	float FScore = FLT_MAX;
};
