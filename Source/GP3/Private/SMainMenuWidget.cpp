#include "SMainMenuWidget.h"

#define LOCTEXT_NAMESPACE "MainMenu" // Localization 

void SMainMenuWidget::Construct(const FArguments& InArgs)
{

	const FMargin ContentPadding = FMargin(500.f, 300.f); // Button padding

	const FText TitleText = LOCTEXT("GameTitle", "GP3 Game");
	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage) // Background
			.ColorAndOpacity(FColor::Black)
			
		]
		+ SOverlay::Slot() // Button
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(ContentPadding) 
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(TitleText)
			]
		]
		
	];
}

#undef LOCTEXT_NAMESPACE
