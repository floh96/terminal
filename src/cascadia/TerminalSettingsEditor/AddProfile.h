/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- AddProfile.h

Abstract:
- This creates the 'add new profile' page in the settings UI and handles
  user interaction with it, raising events to the main page as necessary


Author(s):
- Pankaj Bhojwani - March 2021

--*/

#pragma once

#include "AddProfile.g.h"
#include "AddProfilePageNavigationState.g.h"
#include "Utils.h"

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    struct AddProfilePageNavigationState : AddProfilePageNavigationStateT<AddProfilePageNavigationState>
    {
    public:
        AddProfilePageNavigationState(const MTSM::CascadiaSettings& settings) :
            _Settings{ settings } {}

        void RequestAddNew()
        {
            _AddNewHandlers(winrt::guid{});
        }

        void RequestDuplicate(GUID profile)
        {
            _AddNewHandlers(profile);
        }

        WINRT_PROPERTY(MTSM::CascadiaSettings, Settings, nullptr)
        WINRT_CALLBACK(AddNew, AddNewArgs);
    };

    struct AddProfile : public HasScrollViewer<AddProfile>, AddProfileT<AddProfile>
    {
    public:
        AddProfile();

        void OnNavigatedTo(const WUX::Navigation::NavigationEventArgs& e);

        void AddNewClick(const IInspectable& sender, const WUX::RoutedEventArgs& eventArgs);
        void DuplicateClick(const IInspectable& sender, const WUX::RoutedEventArgs& eventArgs);
        void ProfilesSelectionChanged(const IInspectable& sender, const WUX::RoutedEventArgs& eventArgs);

        WINRT_PROPERTY(Editor::AddProfilePageNavigationState, State, nullptr);
        WINRT_CALLBACK(PropertyChanged, WUX::Data::PropertyChangedEventHandler);
        WINRT_OBSERVABLE_PROPERTY(bool, IsProfileSelected, _PropertyChangedHandlers, nullptr);
    };
}

namespace winrt::Microsoft::Terminal::Settings::Editor::factory_implementation
{
    BASIC_FACTORY(AddProfile);
}
