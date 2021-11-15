#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "IntroductionMenu.h"
#include "../V4Core.h"

IntroductionMenu::IntroductionMenu()
{
    is_active = false;
}

void IntroductionMenu::Initialise(Config* thisConfig, V4Core* parent, Menu* curParentMenu)
{
    SPDLOG_DEBUG("Initialize Introduction menu");
    Scene::Initialise(thisConfig, parent);
    //buttonList.Initialise(&m_font,*thisConfig,keymap,&(v4core->currentController),this);
    parentMenu = curParentMenu;
    SPDLOG_DEBUG("Initial values loaded, loading assets");

    msgcloud.Create(45, sf::Vector2f(640, 480), sf::Color::White, true, thisConfig->GetInt("textureQuality"), thisConfig->fontPath);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_1")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_2")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_3")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_4")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_5")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_6")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_7")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_8")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_9")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_10")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_11")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_12")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_13")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetString("newgame_intro_14")), true);

    timeout.restart();

    SPDLOG_DEBUG("Introduction menu initialized.");
}

void IntroductionMenu::EventFired(sf::Event event)
{
}

void IntroductionMenu::Update(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{
    if (timeout.getElapsedTime().asSeconds() > 1)
    {
        if (!msgcloud.done)
            msgcloud.Show();
    }

    msgcloud.Draw(window, fps, inputCtrl);

    if (!msgcloud.done)
        timeout2.restart();

    if (timeout2.getElapsedTime().asSeconds() > 3)
    {
        sf::Thread loadingThreadInstance(&V4Core::loadingThread, parentMenu->v4Core);
        parentMenu->v4Core->continue_loading = true;
        parentMenu->v4Core->window.setActive(false);
        loadingThreadInstance.launch();

        parentMenu->Hide();
        parentMenu->is_active = false;
        Hide();
        is_active = false;

        v4Core->currentController.Initialise(*thisConfig, thisConfig->GetString("mission1Background"), *v4Core);
        v4Core->currentController.StartMission("mis1_0.p4m", false, 1);
    }
}

IntroductionMenu::~IntroductionMenu()
{
}
