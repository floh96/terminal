// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.
#pragma once

#include "Commandline.h"

#ifdef UNIT_TESTING
// fwdecl unittest classes
namespace TerminalAppLocalTests
{
    class CommandlineTest;
};
#endif

namespace TerminalApp
{
    class AppCommandlineArgs;
};

class MTApp::AppCommandlineArgs final
{
public:
    static constexpr std::string_view NixHelpFlag{ "-?" };
    static constexpr std::string_view WindowsHelpFlag{ "/?" };
    static constexpr std::wstring_view PlaceholderExeName{ L"wt.exe" };

    AppCommandlineArgs();
    ~AppCommandlineArgs() = default;

    int ParseCommand(const Commandline& command);
    int ParseArgs(winrt::array_view<const winrt::hstring>& args);

    static std::vector<Commandline> BuildCommands(const std::vector<const wchar_t*>& args);
    static std::vector<Commandline> BuildCommands(winrt::array_view<const winrt::hstring>& args);

    void ValidateStartupCommands();
    std::vector<MTSM::ActionAndArgs>& GetStartupActions();
    bool IsHandoffListener() const noexcept;
    const std::string& GetExitMessage();
    bool ShouldExitEarly() const noexcept;

    std::optional<uint32_t> GetPersistedLayoutIdx() const noexcept;
    std::optional<MTSM::LaunchMode> GetLaunchMode() const noexcept;
    std::optional<MTSM::LaunchPosition> GetPosition() const noexcept;
    std::optional<til::size> GetSize() const noexcept;

    int ParseArgs(const MTSM::ExecuteCommandlineArgs& args);
    void DisableHelpInExitMessage();
    void FullResetState();

    std::string_view GetTargetWindow() const noexcept;

private:
    static const std::wregex _commandDelimiterRegex;

    CLI::App _app{ "wt - the Windows Terminal" };

    // This is a helper struct to encapsulate all the options for a subcommand
    // that produces a NewTerminalArgs.
    struct NewTerminalSubcommand
    {
        CLI::App* subcommand;
        CLI::Option* commandlineOption;
        CLI::Option* profileNameOption;
        CLI::Option* startingDirectoryOption;
        CLI::Option* titleOption;
        CLI::Option* tabColorOption;
        CLI::Option* suppressApplicationTitleOption;
        CLI::Option* colorSchemeOption;
    };

    struct NewPaneSubcommand : public NewTerminalSubcommand
    {
        CLI::Option* _horizontalOption;
        CLI::Option* _verticalOption;
        CLI::Option* _duplicateOption;
    };

    // --- Subcommands ---
    NewTerminalSubcommand _newTabCommand;
    NewTerminalSubcommand _newTabShort;
    NewPaneSubcommand _newPaneCommand;
    NewPaneSubcommand _newPaneShort;
    CLI::App* _focusTabCommand;
    CLI::App* _focusTabShort;
    CLI::App* _moveFocusCommand;
    CLI::App* _moveFocusShort;
    CLI::App* _movePaneCommand;
    CLI::App* _movePaneShort;
    CLI::App* _swapPaneCommand;
    CLI::App* _focusPaneCommand;
    CLI::App* _focusPaneShort;

    // Are you adding a new sub-command? Make sure to update _noCommandsProvided!

    std::string _profileName;
    std::string _startingDirectory;
    std::string _startingTitle;
    std::string _startingTabColor;
    std::string _startingColorScheme;
    bool _suppressApplicationTitle{ false };

    MTSM::FocusDirection _moveFocusDirection{ MTSM::FocusDirection::None };
    MTSM::FocusDirection _swapPaneDirection{ MTSM::FocusDirection::None };

    // _commandline will contain the command line with which we'll be spawning a new terminal
    std::vector<std::string> _commandline;

    bool _splitVertical{ false };
    bool _splitHorizontal{ false };
    bool _splitDuplicate{ false };
    float _splitPaneSize{ 0.5f };

    int _movePaneTabIndex{ -1 };
    int _focusTabIndex{ -1 };
    bool _focusNextTab{ false };
    bool _focusPrevTab{ false };

    int _focusPaneTarget{ -1 };
    // Are you adding more args here? Make sure to reset them in _resetStateToDefault

    const Commandline* _currentCommandline{ nullptr };
    std::optional<MTSM::LaunchMode> _launchMode{ std::nullopt };
    std::optional<MTSM::LaunchPosition> _position{ std::nullopt };
    std::optional<til::size> _size{ std::nullopt };
    bool _isHandoffListener{ false };
    std::vector<MTSM::ActionAndArgs> _startupActions;
    std::string _exitMessage;
    bool _shouldExitEarly{ false };

    int _loadPersistedLayoutIdx{};
    std::string _windowTarget{};
    // Are you adding more args or attributes here? If they are not reset in _resetStateToDefault, make sure to reset them in FullResetState

    MTSM::NewTerminalArgs _getNewTerminalArgs(NewTerminalSubcommand& subcommand);
    void _addNewTerminalArgs(NewTerminalSubcommand& subcommand);
    void _buildParser();
    void _buildNewTabParser();
    void _buildSplitPaneParser();
    void _buildFocusTabParser();
    void _buildMoveFocusParser();
    void _buildMovePaneParser();
    void _buildSwapPaneParser();
    void _buildFocusPaneParser();
    bool _noCommandsProvided();
    void _resetStateToDefault();
    int _handleExit(const CLI::App& command, const CLI::Error& e);

    static void _addCommandsForArg(std::vector<Commandline>& commands, std::wstring_view arg);

#ifdef UNIT_TESTING
    friend class TerminalAppLocalTests::CommandlineTest;
#endif
};
