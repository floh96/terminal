// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "ColorSchemesPageViewModel.g.h"
#include "ColorSchemeViewModel.h"
#include "Utils.h"
#include "ViewModelHelpers.h"
#include "ColorSchemes.h"

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    struct ColorSchemesPageViewModel : ColorSchemesPageViewModelT<ColorSchemesPageViewModel>, ViewModelHelper<ColorSchemesPageViewModel>
    {
    public:
        ColorSchemesPageViewModel(const MTSM::CascadiaSettings& settings);
        void UpdateSettings(const MTSM::CascadiaSettings& settings);

        void CurrentScheme(const Editor::ColorSchemeViewModel& newSelectedScheme);
        Editor::ColorSchemeViewModel CurrentScheme();
        bool HasCurrentScheme() const noexcept;

        Editor::ColorSchemeViewModel RequestAddNew();
        bool RequestRenameCurrentScheme(winrt::hstring newName);
        void RequestDeleteCurrentScheme();
        void RequestEditSelectedScheme();
        void RequestSetSelectedSchemeAsDefault();

        bool CanDeleteCurrentScheme() const;

        void SchemeListItemClicked(const WF::IInspectable& sender, const WUXC::ItemClickEventArgs& e);

        // DON'T YOU DARE ADD A `WINRT_CALLBACK(PropertyChanged` TO A CLASS DERIVED FROM ViewModelHelper. Do this instead:
        using ViewModelHelper<ColorSchemesPageViewModel>::PropertyChanged;

        WINRT_OBSERVABLE_PROPERTY(ColorSchemesSubPage, CurrentPage, _propertyChangedHandlers, ColorSchemesSubPage::Base);
        WINRT_OBSERVABLE_PROPERTY(WFC::IObservableVector<Editor::ColorSchemeViewModel>, AllColorSchemes, _propertyChangedHandlers, nullptr);

    private:
        Editor::ColorSchemeViewModel _CurrentScheme{ nullptr };
        MTSM::CascadiaSettings _settings;
        WFC::IMap<Editor::ColorSchemeViewModel, MTSM::ColorScheme> _viewModelToSchemeMap;

        void _MakeColorSchemeVMsHelper();
    };
};

namespace winrt::Microsoft::Terminal::Settings::Editor::factory_implementation
{
    BASIC_FACTORY(ColorSchemesPageViewModel);
}
