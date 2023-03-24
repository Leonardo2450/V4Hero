#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "MessageCloud.h"
#include <iostream>
#include <math.h>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"

using namespace std;

MessageCloud::MessageCloud()
{
}

void MessageCloud::setSize(float new_x, float new_y)
{
    xsize = new_x;
    ysize = new_y;
}

void MessageCloud::Create(int speed, sf::Vector2f start_pos, sf::Color color, bool can_speedup, int q, std::string font_path)
{
    SPDLOG_DEBUG("MessageCloud::Create()");

    timeout = speed;
    regular_timeout = timeout;
    quality = q;
    
    //speedable = can_speedup;
    speedable = true; //unskippable dialogues are dumb, let the player speed up anything

    font.loadFromFile(font_path);

    cloud.loadFromFile("resources/graphics/ui/dialog/message.png", q);
    cloud.setOrigin(cloud.getLocalBounds().width, cloud.getLocalBounds().height);
    cloud.setColor(color);

    cur_color = color;

    cross.loadFromFile("resources/graphics/ui/dialog/cross.png", q, 1);
    cross_highlight.loadFromFile("resources/graphics/ui/dialog/crosshighlight.png", q, 1);
    cross_arrow.loadFromFile("resources/graphics/ui/dialog/crossarrow.png", q, 1);

    triangle.setPointCount(3);
    triangle.setFillColor(cur_color);

    startpos = start_pos;

    dialogue_ptext.createText(font, 28, sf::Color(32, 32, 32, 255), "", quality, 1);

    visual_ptext.createText(font, 28, sf::Color(32, 32, 32, 255), "", quality, 1);
    visual_ptext.force_nonspeech = true;

    SPDLOG_DEBUG("MessageCloud::Create(): finished");
}

void MessageCloud::AddDialog(sf::String text, bool nextdialog)
{
    // here i dont think we need to use more than one ptext anymore
    // just push the new dialogues into a vector of sf::Strings and let PText parse it :)

    SPDLOG_DEBUG("MessageCloud::AddDialog()");

    dialogue_strings.push_back(text);
    next_dialog.push_back(nextdialog);

    SPDLOG_DEBUG("MessageCloud::AddDialog(): finished");
}

void MessageCloud::Show()
{
    if (!active)
    {
        SPDLOG_DEBUG("MessageCloud::Show()");

        active = true;
        done = false;

        if (dialogue_strings.size() <= 0)
        {
            SPDLOG_ERROR("No messages found in message cloud.");
            AddDialog(sf::String("no message"), true);
        }

        dialogue_ptext.setString(dialogue_strings[cur_dialog]);
        visual_ptext.setString(dialogue_strings[cur_dialog]);

        dest_xsize = visual_ptext.getLocalBounds().width + 64 + (visual_ptext.getLocalBounds().width / 10);
        dest_ysize = visual_ptext.getLocalBounds().height + 64 + (visual_ptext.getLocalBounds().height / 1.5);

        text_timeout.restart();

        SPDLOG_DEBUG("MessageCloud::Show(): finished");
    }
}

void MessageCloud::Hide()
{
    active = false;
}

void MessageCloud::End()
{
    dest_xsize = 0;
    dest_ysize = 0;

    done = true;
    interrupt = true;
}

void MessageCloud::NextDialog()
{
    if (cur_dialog < dialogue_strings.size() - 1)
    {
        ready = false;

        cur_dialog++;

        cur_char = 0;

        old_xsize = dest_xsize;
        old_ysize = dest_ysize;

        dialogue_ptext.setString(dialogue_strings[cur_dialog]);
        visual_ptext.setString(dialogue_strings[cur_dialog]);

        dest_xsize = visual_ptext.getLocalBounds().width + 64 + (visual_ptext.getLocalBounds().width / 10);
        dest_ysize = visual_ptext.getLocalBounds().height + 64 + (visual_ptext.getLocalBounds().height / 1.5);

        text_timeout.restart();
    } else
    {
        if(dialogue_ptext.goback)
        {
            goback = true;
        }
        
        dest_xsize = 0;
        dest_ysize = 0;

        done = true;
    }
}

void MessageCloud::SpeedUp()
{
    speedup = true;
}

void MessageCloud::Draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if (!firstrender)
        firstrender = true;

    if (speedable)
    {
        // here we should do something like
        // ptext timeout = 20ms=50letters/s temporarily
        // then when key is not held, return to normal speed

        if (inputCtrl->isKeyHeld(InputController::Keys::CIRCLE))
            SpeedUp();
    }

    if (ready)
    {
        // advance current text id

        if (inputCtrl->isKeyPressed(InputController::Keys::CROSS))
        {
            NextDialog();
        }
    }

    //for (unsigned int i = 0; i < dialogue_strings.size(); i++)
    //    ptext[i].update(window);

    if (active)
    {
        x = startpos.x - xsize / 40;
        y = startpos.y - ysize;

        float xsize_diff = (dest_xsize - xsize) * 10 / fps;
        float ysize_diff = (dest_ysize - ysize) * 10 / fps;

        //cout << "diff: " << xsize << " " << ysize << " " << dest_xsize << " " << dest_ysize << " " << xsize_diff << " " << ysize_diff << endl;

        setSize(xsize + xsize_diff, ysize + ysize_diff);

        if (((floor(xsize) <= 1) || (floor(ysize) <= 1)) && (done))
        {
            if(goback && !interrupt)
            {
                SPDLOG_DEBUG("Go back to first message");
                cur_dialog = 0;
                done = false;
                active = false;
                ready = false;
                Show();
            }
            else
            {
                active = false;
            }
        }

        /// adjusting the size of clouds and drawing them
        /// they are no longer an array, instead we just transform the single texture 

        float scale_x = 1080.f / xsize;
        float scale_y = 250.f / ysize;

        cloud.setColor(cur_color);
        cloud.setPosition(x, y);

        cloud.setScale(1.f / scale_x, 1.f / scale_y);
        cloud.draw(window);

        cloud.setScale(-1.f / scale_x, 1.f / scale_y);
        cloud.draw(window);

        cloud.setScale(1.f / scale_x, -1.f / scale_y);
        cloud.draw(window);

        cloud.setScale(-1.f / scale_x, -1.f / scale_y);
        cloud.draw(window);

        float rX = window->getSize().x / float(1280);
        float rY = window->getSize().y / float(720);

        triangle.setPoint(0, sf::Vector2f(startpos.x * rX, startpos.y * rY));
        triangle.setPoint(1, sf::Vector2f((x - (xsize / 25)) * rX, y * rY));
        triangle.setPoint(2, sf::Vector2f((x + (xsize / 25)) * rX, y * rY));

        triangle.setFillColor(cur_color);
        window->draw(triangle);

        if (!done)
        {
            dialogue_ptext.speedup = speedup;
            dialogue_ptext.setPosition(x - visual_ptext.getLocalBounds().width / 2, y - 4 - visual_ptext.getLocalBounds().height / 2);
            dialogue_ptext.draw(window);

            //loaded_text[cur_dialog].setPosition(x - ptext[cur_dialog].getLocalBounds().width / 2, y - 4 - ptext[cur_dialog].getLocalBounds().height / 2);
            //showtext[cur_dialog].setString(viewed_text[cur_dialog]);
            //loaded_text[cur_dialog].draw(window);

            if (dialogue_ptext.speech_done)
            {
                ready = true;

                arrow_y -= 36 / fps;

                if (arrow_y <= -20)
                    arrow_y = -20;

                if (arrow_timeout.getElapsedTime().asSeconds() >= 0.75)
                {
                    arrow_y = 0;
                    arrow_timeout.restart();
                }

                cross.setOrigin(cross.getLocalBounds().width / 2, cross.getLocalBounds().height / 2);
                cross_highlight.setOrigin(cross_highlight.getLocalBounds().width / 2, cross_highlight.getLocalBounds().height / 2);
                cross_arrow.setOrigin(cross_highlight.getLocalBounds().width / 2, cross_highlight.getLocalBounds().height);

                float cross_x = x + xsize / 2 - 8;
                float cross_y = y + ysize / 2 - 20;

                cross.setPosition(cross_x, cross_y);
                cross_highlight.setPosition(cross_x, cross_y);
                cross_arrow.setPosition(cross_x - 2, cross_y + 20 + arrow_y);

                cross_highlight.setColor(sf::Color(255, 255, 255, 128 + (arrow_y * 6.4)));

                cross.draw(window);
                cross_highlight.draw(window);
                cross_arrow.draw(window);
            }
        }

        speedup = false;
    }
}
