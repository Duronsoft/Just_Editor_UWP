﻿//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Just_Editor_UWP;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	if (Windows::Foundation::Metadata::ApiInformation::IsTypePresent("Windows.ApplicationModel.Core.CoreApplicationViewTitleBar"))
	{
		Windows::ApplicationModel::Core::CoreApplication::GetCurrentView()->TitleBar->ExtendViewIntoTitleBar = true;
		Windows::UI::Xaml::Window::Current->SetTitleBar((UIElement^)titleRect);
	}
	auto titleBar = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->TitleBar;
	Windows::UI::Color btnColor = { 0, 0, 0, 0 };
	titleBar->ButtonBackgroundColor = btnColor;
	titleBar->ButtonInactiveBackgroundColor = btnColor;

/*	tabPanel = ref new DrnTabPanel(this);
	tabPanel->HoverBrush = ref new SolidColorBrush(Windows::UI::Color{ 0x90, 0x43, 0x43, 0x43 });
	tabPanel->SelectedColor = ref new SolidColorBrush(Windows::UI::Color{ 0xCC, 0x34, 0x34, 0x34 });
	mainGrid->Children->Append(tabPanel);
	mainGrid->SetRowSpan(tabPanel, 2);*/
}

