// Fill out your copyright notice in the Description page of Project Settings.


#include "DSUserWidget.h"
#include "DSPlayerController.h"
#include "DSMainWidget.h"

ADSPlayerController* UDSUserWidget::GetDsController()
{
	if (dsController == nullptr)
	{
		APlayerController* controller = GetOwningPlayer();
		dsController = Cast<ADSPlayerController>(controller);
	}

	return dsController;
}

UDSMainWidget* UDSUserWidget::GetMainWidget()
{
	if (dsMainWidget == nullptr)
	{
		UDSMainWidget* mode = GetDsController()->GetMainWidget();
		dsMainWidget = Cast<UDSMainWidget>(mode);
	}

	return dsMainWidget;
}
