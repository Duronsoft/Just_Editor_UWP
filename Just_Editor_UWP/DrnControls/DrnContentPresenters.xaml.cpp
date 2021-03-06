﻿//
// DrnContentPresenters.xaml.cpp
// Implementation of the DrnContentPresenters class
//

#include "pch.h"
#include "DrnContentPresenters.xaml.h"

using namespace Just_Editor_UWP;

using namespace Microsoft::WRL;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;


// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DrnContentPresenters::DrnContentPresenters()
{
	tVisual = Windows::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(this);

	if (drnD2D == nullptr)
		drnD2D = new DrnD2D;

	this->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &Just_Editor_UWP::DrnContentPresenters::OnSizeChanged);
}

void DrnContentPresenters::Init()
{
	auto surfaceSource = ref new VirtualSurfaceImageSource((int)this->RenderSize.Width, (int)this->RenderSize.Height);
	IInspectable* sisInspectable = (IInspectable*) reinterpret_cast<IInspectable*>(surfaceSource);
	sisInspectable->QueryInterface(__uuidof(IVirtualSurfaceImageSourceNative), (void**)&m_sisNative);
	m_sisNative->SetDevice(drnD2D->dxgiDevice.Get());

	ImageBrush^ brush = ref new ImageBrush();
	brush->ImageSource = surfaceSource;
//	this->Background = brush;
}

void DrnContentPresenters::Draw()
{
	drnD2D->d2dContext->BeginDraw();

	ID2D1SolidColorBrush* colorBrush;
	drnD2D->d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &colorBrush);
	drnD2D->d2dContext->Clear();
	unsigned int sLen = textContent->Length();
	if (sLen)
		drnD2D->d2dContext->DrawText(textContent->Data(), sLen, drnD2D->txtFormat.Get(), D2D1::RectF(0.0f, tVisual->Offset.y, std::numeric_limits<float>::max(), 23.0f), colorBrush);

	drnD2D->d2dContext->EndDraw();
}


void Just_Editor_UWP::DrnContentPresenters::OnSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	if (m_sisNative != nullptr)
	{
		m_sisNative->Resize((INT)e->NewSize.Width, (INT)e->NewSize.Height);
	}
	else
	{
		Init();
		Draw();
	}
}
