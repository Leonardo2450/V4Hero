#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "StateManager.h"
#include "CoreManager.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
    for (auto& t : loadingThreads)
    {
        t.detach();
    }
}

StateManager& StateManager::getInstance()
{
    static StateManager instance;
    return instance;
}

void StateManager::updateCurrentState()
{
    switch (currentGameState)
    {
        case ENTRY: {
            if (mainMenuPtr != nullptr)
            {
                if (mainMenuPtr->initialized)
                {
                    setState(MAINMENU);
                }
            }
            break;
        }

        case NEWGAMEMENU: {
            //newGameMenuPtr->Update();
            break;
        }

        case MAINMENU: {
            if (mainMenuPtr == nullptr)
            {
                mainMenuPtr = new MainMenu;
            }

            mainMenuPtr->Update();
            break;
        }

        case OPTIONSMENU: {
            if (optionsMenuPtr == nullptr)
            {
                optionsMenuPtr = new OptionsMenu;
            }

            //optionsMenuPtr->Update();
            break;
        }

        case MISSIONCONTROLLER: {
            //missionControllerPtr->Update();
            break;
        }
    }
}

void StateManager::initState(int state)
{
    switch (state)
    {
        case NEWGAMEMENU: {

            break;
        }

        case MAINMENU: {

            // For initializing MainMenu, we want to initialize OptionsMenu as well, so the transition is seamless
            if (mainMenuPtr == nullptr)
            {
                mainMenuPtr = new MainMenu;
            }

            if (optionsMenuPtr == nullptr)
            {
                optionsMenuPtr = new OptionsMenu;
            }

            break;
        }

        case MISSIONCONTROLLER: {

            break;
        }
    }
}

void StateManager::initStateMT(int state)
{
    loadingThreads.push_back(std::thread(&StateManager::initState, this, state));
}

void StateManager::parseCurrentStateEvents(sf::Event& event)
{
    switch (currentGameState)
    {
        case NEWGAMEMENU: {
            break;
        }

        case MAINMENU: {
            if (mainMenuPtr == nullptr)
            {
                mainMenuPtr = new MainMenu;
                mainMenuPtr->Initialise(CoreManager::getInstance().getConfig(), CoreManager::getInstance().getCore());
            }

            mainMenuPtr->EventFired(event);
            break;
        }

        case MISSIONCONTROLLER: {
            break;
        }
    }
}

void StateManager::setState(int state)
{
    SPDLOG_DEBUG("Changing state to {}", state);
    currentGameState = state;
}