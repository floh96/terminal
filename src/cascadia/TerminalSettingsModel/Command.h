/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- Command.h

Abstract:
- A command represents a single entry in the Command Palette. This is an object
  that has a user facing "name" to display to the user, and an associated action
  which can be dispatched.

- For more information, see GH#2046, #5400, #5674, and #6635

Author(s):
- Mike Griese - June 2020

--*/
#pragma once

#include "Command.g.h"
#include "TerminalWarnings.h"
#include "Profile.h"
#include "ActionAndArgs.h"
#include "SettingsTypes.h"

// fwdecl unittest classes
namespace SettingsModelLocalTests
{
    class DeserializationTests;
    class CommandTests;
};

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    struct Command : CommandT<Command>
    {
        Command();
        com_ptr<Command> Copy() const;

        static winrt::com_ptr<Command> FromJson(const Json::Value& json,
                                                std::vector<SettingsLoadWarnings>& warnings);

        static void ExpandCommands(WFC::IMap<winrt::hstring, MTSM::Command> commands,
                                   WFC::IVectorView<MTSM::Profile> profiles,
                                   WFC::IVectorView<MTSM::ColorScheme> schemes,
                                   WFC::IVector<SettingsLoadWarnings> warnings);

        static std::vector<SettingsLoadWarnings> LayerJson(WFC::IMap<winrt::hstring, MTSM::Command>& commands,
                                                           const Json::Value& json);
        Json::Value ToJson() const;

        bool HasNestedCommands() const;
        bool IsNestedCommand() const noexcept;
        WFC::IMapView<winrt::hstring, MTSM::Command> NestedCommands() const;

        bool HasName() const noexcept;
        hstring Name() const noexcept;
        void Name(const hstring& name);

        MTControl::KeyChord Keys() const noexcept;
        hstring KeyChordText() const noexcept;
        std::vector<MTControl::KeyChord> KeyMappings() const noexcept;
        void RegisterKey(const MTControl::KeyChord& keys);
        void EraseKey(const MTControl::KeyChord& keys);

        hstring IconPath() const noexcept;
        void IconPath(const hstring& val);

        WUX::Data::INotifyPropertyChanged::PropertyChanged_revoker propertyChangedRevoker;

        WINRT_CALLBACK(PropertyChanged, WUX::Data::PropertyChangedEventHandler);
        WINRT_PROPERTY(ExpandCommandType, IterateOn, ExpandCommandType::None);
        WINRT_PROPERTY(MTSM::ActionAndArgs, ActionAndArgs);

    private:
        Json::Value _originalJson;
        WFC::IMap<winrt::hstring, MTSM::Command> _subcommands{ nullptr };
        std::vector<MTControl::KeyChord> _keyMappings;
        std::optional<std::wstring> _name;
        std::optional<std::wstring> _iconPath;
        bool _nestedCommand{ false };

        static std::vector<MTSM::Command> _expandCommand(Command* const expandable,
                                                         WFC::IVectorView<MTSM::Profile> profiles,
                                                         WFC::IVectorView<MTSM::ColorScheme> schemes,
                                                         WFC::IVector<SettingsLoadWarnings>& warnings);
        friend class SettingsModelLocalTests::DeserializationTests;
        friend class SettingsModelLocalTests::CommandTests;
    };
}

namespace winrt::Microsoft::Terminal::Settings::Model::factory_implementation
{
    BASIC_FACTORY(Command);
}
