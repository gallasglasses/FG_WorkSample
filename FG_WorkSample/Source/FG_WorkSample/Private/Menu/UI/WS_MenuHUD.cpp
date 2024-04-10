// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/WS_MenuHUD.h"
#include "Menu/UI/WS_MenuWidget.h"

void AWS_MenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<UWS_MenuWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport(); 
			MenuWidget->ShowOpening();
		}
	}
}
