// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DS_API UDSUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class ADSPlayerController* GetDsController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UDSMainWidget* GetMainWidget();

private:
	TObjectPtr<class ADSPlayerController> dsController = nullptr;
	TObjectPtr<class UDSMainWidget> dsMainWidget = nullptr;
};
