// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "RenderingViewModel.h"
#include "RenderingViewModel.g.cpp"

using namespace WF;
using namespace MTSM;

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    RenderingViewModel::RenderingViewModel(MTSM::CascadiaSettings settings) noexcept :
        _settings{ std::move(settings) }
    {
    }
}
