﻿//
// DrnFileDialog.xaml.cpp
// Implementation of the DrnFileDialog class
//

#include "pch.h"
#include "DrnFileDialog.xaml.h"

using namespace Just_Editor_UWP;

using namespace Platform;
using namespace concurrency;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

Just_Editor_UWP::DrnFileDialog::DrnFileDialog(Platform::String^ dialogTitle, Platform::String^ fileName, Windows::Storage::StorageFile^ dFile)
{
	InitializeComponent();
	IsClosed = true;
	this->RequestedTheme = AppConfig->IsDark ? Windows::UI::Xaml::ElementTheme::Dark : Windows::UI::Xaml::ElementTheme::Light;
	TitleBlock->Text = dialogTitle;
	FileNameBox->Text = fileName;
	DialogFile = dFile;
	FilePathBlock->Text = dFile == nullptr ? L"(None)" : dFile->Path;
}



void Just_Editor_UWP::DrnFileDialog::DialogClsBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Hide();
}


void Just_Editor_UWP::DrnFileDialog::ChangePathBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	create_task(Drn_UWP::PickAFolder()).then([this](Windows::Storage::StorageFolder^ nFolder) 
		{
			if (nFolder == nullptr)
				return;
			newFolder = nFolder;
			FilePathBlock->Text = newFolder->Path;
		});
}


void Just_Editor_UWP::DrnFileDialog::SaveBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (newFolder != nullptr)
	{
		create_task(newFolder->CreateFileAsync(FileNameBox->Text, Windows::Storage::CreationCollisionOption::ReplaceExisting)).then([this](Windows::Storage::StorageFile^ newFile)
			{
				if (newFile == nullptr)
					return;
				if (DialogFile != nullptr)
					DialogFile->CopyAndReplaceAsync(newFile);

				DialogFile = newFile;
				InfoChanged(this);
			});
	}
	else if (dialogFile != nullptr && dialogFile->Name != FileNameBox->Text)
		create_task(dialogFile->RenameAsync(FileNameBox->Text)).then([this](task<void> changeNameTask) 
			{
				try
				{
					changeNameTask.get();
					InfoChanged(this);
				}
				catch (COMException^ ex)
				{
					Drn_UWP::ShowMessageBox(L"Rename Failed", L"Error Code:" + ex->HResult.ToString() + L"\n" + ex->Message);
				}
			}, task_continuation_context::use_current());
	else
		InfoChanged(this);

	this->Hide();
}


void Just_Editor_UWP::DrnFileDialog::ContentDialog_Closed(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogClosedEventArgs^ args)
{
	IsClosed = true;
}


void Just_Editor_UWP::DrnFileDialog::ContentDialog_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IsClosed = false;
}


void Just_Editor_UWP::DrnFileDialog::FileNameBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	std::wstring wStr = FileNameBox->Text->Data();
	for (int i = (int)wStr.length(), j; --i >= 0;)
	{
		for (j = 0; j < 9; j++)
		{
			if (wStr[i] == UnabledWords[j])
			{
				ChangePathBtn->IsEnabled = false;
				SaveBtn->IsEnabled = false;
				return;
			}
		}
	}
	ChangePathBtn->IsEnabled = true;
	SaveBtn->IsEnabled = true;
}
