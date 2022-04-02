#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Patapolis.h"
#include "../V4Core.h"
#include "Altar.h"
#include "ButtonList.h"
#include "iostream"
#include <sstream>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"
#include "../StateManager.h"

PatapolisMenu::PatapolisMenu()
{
    //ctor

    //f_font.loadFromFile("resources/fonts/arial.ttf");

    //TO-DO: is this still needed?
    is_active = false;

    SPDLOG_INFO("Initializing Patapolis");

    Config* config = CoreManager::getInstance().getConfig();

    //sf::Context context;
    //TO-DO: is Scene still needed?
    //Scene::Initialise(_thisConfig, parent);
    updateStoryPoint(); // Update story_point before anything else
    
    //TO-DO: Patapolis menus under new system
    //altar_menu->initialise(_thisConfig, parent, this);
    //mater_menu.initialise(_thisConfig, parent, this);
    //SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       initialise(_thisConfig, parent, this);
    //obelisk_menu.Initialise(_thisConfig, parent, this);
    //parentMenu = curParentMenu;
    quality = config->GetInt("textureQuality");

    float resRatioX = config->GetInt("resX") / float(1280);
    float resRatioY = config->GetInt("resY") / float(720);

    f_font.loadFromFile(config->fontPath);

    t_title.createText(f_font, 38, sf::Color::White, "", quality, 1);
    t_title.t.setOutlineThickness(2);
    t_title.t.setOutlineColor(sf::Color::Black);

    string vx_params = "0,24,128,238;66,24,128,238;444,184,243,202;591,184,243,202;592,255,255,255;710,171,243,214;720,171,243,214";

    vector<string> v_vxparams = Func::Split(vx_params, ';');
    std::vector<sf::Vector2f> vx_pos;
    std::vector<sf::Color> vx_color;

    for (int i = 0; i < v_vxparams.size(); i++)
    {
        vector<string> tmp = Func::Split(v_vxparams[i], ',');

        sf::Vector2f tmp_vector;
        sf::Color tmp_color;

        tmp_vector.x = 0;
        tmp_vector.y = atof(tmp[0].c_str()) * resRatioY;

        if (tmp[0] == "-1")
        {
            tmp_vector.y = 720 * resRatioY;
        }

        tmp_color.r = atoi(tmp[1].c_str());
        tmp_color.g = atoi(tmp[2].c_str());
        tmp_color.b = atoi(tmp[3].c_str());

        sf::Vector2f tmp_vector2;

        tmp_vector2.x = 1280 * resRatioX;
        tmp_vector2.y = atof(tmp[0].c_str()) * resRatioY;

        if (tmp[0] == "-1")
        {
            tmp_vector2.y = 720 * resRatioY;
        }

        vx_pos.push_back(tmp_vector);
        vx_color.push_back(tmp_color);

        vx_pos.push_back(tmp_vector2);
        vx_color.push_back(tmp_color);
    }

    sf::VertexArray tmp(sf::TrianglesStrip, vx_pos.size());
    v_background = tmp;

    for (int i = 0; i < vx_pos.size(); i++)
    {
        v_background[i].position = vx_pos[i];
        v_background[i].color = vx_color[i];

        SPDLOG_TRACE("patapolis background vx_pos: {} {}", vx_pos[i].x, vx_pos[i].y);
    }

    floor_height = 54;
    r_ground.setSize(sf::Vector2f(11500 * resRatioX, floor_height * resRatioY));
    r_ground.setFillColor(sf::Color::Black);

    std::vector<string> l2_str = {"a", "b", "c", "c_winter1", "c_winter2", "d", "e", "f"};
    std::vector<string> l6_str = {"a", "b", "c", "d", "e"};

    for (int i = 0; i < l2_str.size(); i++)
    {
        ps_layer_2[l2_str[i]].loadFromFile("resources/graphics/bg/patapolis/2" + l2_str[i] + ".png", quality, 1);
    }

    for (int i = 0; i < l6_str.size(); i++)
    {
        ps_layer_6[l6_str[i]].loadFromFile("resources/graphics/bg/patapolis/6" + l6_str[i] + ".png", quality, 1);
    }


    addL6("a", 0, 722 - floor_height);
    addL6("a", 2700, 722 - floor_height);

    addL6("d", 3840, 726 - floor_height);
    addL6("c", 3800, 722 - floor_height);

    addL6("d", 4400, 740 - floor_height);
    addL6("c", 4650, 740 - floor_height);
    addL6("b", 4900, 726 - floor_height);
    addL6("d", 5100, 737 - floor_height);
    addL6("c", 5050, 725 - floor_height);
    addL6("d", 5500, 745 - floor_height);
    addL6("c", 5700, 723 - floor_height);
    addL6("d", 5760, 724 - floor_height);
    addL6("a", 5850, 722 - floor_height);
    addL6("c", 6100, 722 - floor_height);
    addL6("d", 6150, 733 - floor_height);
    addL6("c", 6400, 755 - floor_height);
    addL6("b", 6650, 722 - floor_height);

    addL6("e", 7640, 722 - floor_height);
    addL6("d", 7700, 731 - floor_height);
    addL6("c", 7670, 724 - floor_height);
    addL6("c", 7900, 750 - floor_height);

    addL6("d", 8250, 727 - floor_height);
    addL6("c", 8480, 742 - floor_height);
    addL6("d", 8710, 734 - floor_height);
    addL6("c", 8880, 723 - floor_height);
    addL6("d", 8930, 733 - floor_height);
    addL6("c", 9040, 757 - floor_height);
    addL6("d", 9180, 736 - floor_height);
    addL6("a", 9120, 722 - floor_height);
    addL6("c", 9260, 743 - floor_height);

    addL6("c", 9700, 728 - floor_height);
    addL6("d", 9740, 735 - floor_height);
    addL6("c", 9880, 760 - floor_height);
    addL6("d", 9930, 750 - floor_height);
    addL6("d", 10040, 737 - floor_height);

    L5 = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/5.png");
    L4 = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/4.png");

    L5.setPosition(3060, 740 - floor_height);
    L4.setPosition(2530, 750 - floor_height);

    addL2("a", 800, 766 - floor_height);
    addL2("b", 2900, 740 - floor_height);
    addL2("c", 3900, 720 - floor_height);
    addL2("c", 4300, 770 - floor_height);

    addL2("c", 5080, 750 - floor_height);
    addL2("c", 5220, 720 - floor_height);

    addL2("c", 5600, 770 - floor_height);
    addL2("c", 5740, 720 - floor_height);

    addL2("c", 6150, 760 - floor_height);
    addL2("c", 6320, 720 - floor_height);

    addL2("c", 6540, 760 - floor_height);

    addL2("c", 7540, 760 - floor_height);

    addL2("c", 7800, 720 - floor_height);
    addL2("c", 8000, 750 - floor_height);

    addL2("c", 8900, 770 - floor_height);
    addL2("c", 9060, 740 - floor_height);

    addL2("c", 9760, 740 - floor_height);

    addL2("c", 10300, 770 - floor_height);
    addL2("c", 10470, 740 - floor_height);

    addL2("c", 11000, 750 - floor_height);

    addL2("d", 300, 728 - floor_height);
    addL2("d", 940, 728 - floor_height);
    addL2("e", 1400, 728 - floor_height);
    addL2("f", 1800, 728 - floor_height);
    addL2("d", 4200, 728 - floor_height);
    addL2("d", 5000, 728 - floor_height);
    addL2("d", 5580, 728 - floor_height);
    addL2("d", 6400, 728 - floor_height);
    addL2("d", 7700, 728 - floor_height);
    addL2("d", 9000, 728 - floor_height);
    addL2("d", 10500, 728 - floor_height);

    edge = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/edge.png");
    bridge = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/bridge.png");
    rainbow = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/rainbow.png");

    for (int i = 0; i < 4; i++)
    {
        back_layer[i] = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/back_" + to_string(i + 1) + ".png");
        back_layer[i].setOrigin(0, back_layer[i].getLocalBounds().height);
        back_layer[i].setPosition(back_pos[i], 715);
    }

    // to be replaced to store only data in vectors and draw the same sprite in draw call
    addSparkle(11620 + 25, 400);
    addSparkle(11620 + 170, 380);
    addSparkle(11620 + 83, 270);
    addSparkle(11620 + 180, 265);
    addSparkle(11620 + 283, 295);
    addSparkle(11620 + 242, 159);
    addSparkle(11620 + 333, 235);
    addSparkle(11620 + 455, 261);
    addSparkle(11620 + 416, 100);
    addSparkle(11620 + 598, 127);
    addSparkle(11620 + 506, 186);
    addSparkle(11620 + 605, 257);
    addSparkle(11620 + 722, 203);
    addSparkle(11620 + 711, 332);
    addSparkle(11620 + 831, 341);
    addSparkle(11620 + 937, 401);
    addSparkle(11620 + 860, 244);

    wakapon.loadFromFile("resources/graphics/bg/patapolis/wakapon.png", quality, 1);
    wakapon.setPosition(11920, 462);
    wakapon.setOrigin(wakapon.getLocalBounds().width / 2, wakapon.getLocalBounds().height);

    //TO-DO: something doesn't work here. get Patapolis to load up first, then figure out what's wrong
    /* a_wakapon.LoadConfig(config, "resources/units/unit/wakapon.p4a");
    a_wakapon.setAnimationSegment("idle");
    a_wakapon.global_y = 395;

    a_sen.LoadConfig(config, "resources/units/unit/ranpurupon.p4a");
    a_sen.setAnimationSegment("idle");
    a_sen.global_y = 629; */

    world_egg.loadFromFile("resources/graphics/bg/patapolis/egg.png", quality, 1);
    world_egg.setPosition(12215, 462);
    world_egg.setOrigin(world_egg.getLocalBounds().width / 2, world_egg.getLocalBounds().height);

    light_1.loadFromFile("resources/graphics/bg/patapolis/light.png", quality, 1);
    light_1.setPosition(11715, 161);
    light_1.setOrigin(light_1.getLocalBounds().width / 2, light_1.getLocalBounds().height / 2);

    light_2.loadFromFile("resources/graphics/bg/patapolis/light.png", quality, 1);
    light_2.setPosition(12720, 161);
    light_2.setOrigin(light_2.getLocalBounds().width / 2, light_2.getLocalBounds().height / 2);

    egg_light.loadFromFile("resources/graphics/bg/patapolis/egg_light.png", quality, 1);
    egg_light.setPosition(12217, 288);
    egg_light.setOrigin(egg_light.getLocalBounds().width / 2, egg_light.getLocalBounds().height / 2);

    addRay(10, 331, 18, 321);
    addRay(21, 321, 25, 316);
    addRay(28, 313, 32, 308);
    addRay(34, 306, 37, 300);
    addRay(39, 298, 41, 291);
    addRay(41, 278, 39, 271);
    addRay(36, 254, 33, 246);
    addRay(31, 242, 27, 235);
    addRay(24, 232, 19, 226);
    addRay(16, 223, 9, 217);
    addRay(7, 214, 0, 211);
    addRay(2, 210, -2, 210);

    for (int i = 11; i >= 0; i--)
    {
        float x1 = coords[i].ox2 * (-1);
        float x2 = coords[i].ox1 * (-1);

        addRay(x1, coords[i].y2, x2, coords[i].y1);
    }

    for (int i = 0; i < coords.size(); i++)
    {
        LightRay tmp;
        tmp.angle = 30 + (i * (float(300) / float(coords.size())));
        tmp.max_distance = 110 + rand() % 20;
        tmp.min_distance = 10 + rand() % 20;
        tmp.cur_distance = rand() % 110 + 20;


        sf::VertexArray vtmp(sf::TrianglesStrip, 3);
        vtmp[0].position = sf::Vector2f(coords[i].x1, coords[i].y1);
        vtmp[0].color = sf::Color(215, 246, 255, 255);
        vtmp[1].position = sf::Vector2f(coords[i].x2, coords[i].y2);
        vtmp[1].color = sf::Color(215, 246, 255, 255);
        vtmp[2].position = sf::Vector2f(((coords[i].x1 + coords[i].x2) / 2) + (tmp.cur_distance * sin(tmp.angle * 3.14159265 / 180)), ((coords[i].y1 + coords[i].y2) / 2) + (tmp.cur_distance * cos(tmp.angle * 3.14159265 / 180)));
        vtmp[2].color = sf::Color(215, 246, 255, 255);
        tmp.triangle = vtmp;

        SPDLOG_TRACE("[coord #{}] {} {} {} {} {} {} {} {} {} {}", i, vtmp[0].position.x, vtmp[0].position.y, vtmp[1].position.x, vtmp[1].position.y, vtmp[2].position.x, vtmp[2].position.y, tmp.angle, tmp.cur_distance, tmp.min_distance, tmp.max_distance);

        lightrays.push_back(tmp);
    }

    barracks.loadFromFile("resources/graphics/bg/patapolis/barracks.png", quality, 1);
    barracks.setPosition(640, 765 - floor_height);
    barracks.setOrigin(barracks.getLocalBounds().width / 2, barracks.getLocalBounds().height);

    forge_main.loadFromFile("resources/graphics/bg/patapolis/forge_main.png", quality, 1);
    forge_main.setPosition(2300, 721 - floor_height);
    forge_main.setOrigin(forge_main.getLocalBounds().width / 2, forge_main.getLocalBounds().height);

    forge_back.loadFromFile("resources/graphics/bg/patapolis/blacksmith_forge.png", quality, 1);
    forge_back.setPosition(2300, 721 - floor_height - 29);
    forge_back.setOrigin(forge_back.getLocalBounds().width / 2, forge_back.getLocalBounds().height);

    forge_glow.loadFromFile("resources/graphics/bg/patapolis/blacksmith_forge_glow.png", quality, 1);
    forge_glow.setPosition(2300, 720 - floor_height - 178);
    forge_glow.setOrigin(forge_glow.getLocalBounds().width / 2, forge_glow.getLocalBounds().height);

    forge_fence.loadFromFile("resources/graphics/bg/patapolis/blacksmith_fence.png", quality, 1);
    forge_fence.setPosition(2043, 720 - floor_height - 306);
    forge_fence.setOrigin(forge_fence.getLocalBounds().width / 2, 0);

    forge_slab.loadFromFile("resources/graphics/bg/patapolis/slab.png", quality, 1);
    forge_slab.setPosition(2300 - 65, 720 - floor_height - 84);
    forge_slab.setOrigin(forge_slab.getLocalBounds().width / 2, forge_slab.getLocalBounds().height);

    forge_slab_glow.loadFromFile("resources/graphics/bg/patapolis/slab_glow.png", quality, 1);
    forge_slab_glow.setPosition(2300 - 72, 720 - floor_height - 92);
    forge_slab_glow.setOrigin(forge_slab_glow.getLocalBounds().width / 2, forge_slab_glow.getLocalBounds().height);

    market.loadFromFile("resources/graphics/bg/patapolis/market.png", quality, 1);
    market.setPosition(3960, 728 - floor_height);
    market.setOrigin(market.getLocalBounds().width / 2, market.getLocalBounds().height);

    mater.loadFromFile("resources/graphics/bg/patapolis/mater.png", quality, 1);
    mater.setPosition(9340, 728 - floor_height);
    mater.setOrigin(mater.getLocalBounds().width / 2, mater.getLocalBounds().height);

    festival_main.loadFromFile("resources/graphics/bg/patapolis/festival_main.png", quality, 1);
    festival_main.setPosition(5620, 721 - floor_height);
    festival_main.setOrigin(festival_main.getLocalBounds().width / 2, festival_main.getLocalBounds().height);

    altar.loadFromFile("resources/graphics/bg/patapolis/altar.png", quality, 1);
    altar.setPosition(7280, 720 - floor_height);
    altar.setOrigin(altar.getLocalBounds().width / 2, altar.getLocalBounds().height);

    obelisk.loadFromFile("resources/graphics/bg/patapolis/obelisk.png", quality, 1);
    obelisk.setPosition(8300, 722 - floor_height);
    obelisk.setOrigin(obelisk.getLocalBounds().width / 2, obelisk.getLocalBounds().height);

    paraget_main.loadFromFile("resources/graphics/bg/patapolis/paraget_main.png", quality, 1);
    paraget_main.setPosition(10600, 722 - floor_height);
    paraget_main.setOrigin(paraget_main.getLocalBounds().width / 2, paraget_main.getLocalBounds().height);

    paraget_crystal_glow.loadFromFile("resources/graphics/bg/patapolis/paraget_crystal_glow.png", quality, 1);
    paraget_crystal_glow.setPosition(10600, 430);
    paraget_crystal_glow.setOrigin(paraget_crystal_glow.getLocalBounds().width / 2, paraget_crystal_glow.getLocalBounds().height / 2);

    paraget_crystal.loadFromFile("resources/graphics/bg/patapolis/paraget_crystal.png", quality, 1);
    paraget_crystal.setPosition(10600, 430);
    paraget_crystal.setOrigin(paraget_crystal.getLocalBounds().width / 2, paraget_crystal.getLocalBounds().height / 2);

    locations.push_back(barracks.getPosition().x - 640);
    locations.push_back(forge_main.getPosition().x - 640);
    locations.push_back(market.getPosition().x - 640);
    locations.push_back(festival_main.getPosition().x - 640);
    locations.push_back(6450 - 640); ///Sen
    locations.push_back(altar.getPosition().x - 640);
    locations.push_back(obelisk.getPosition().x - 640);
    locations.push_back(mater.getPosition().x - 530); // mater needs to be offset differently because sprite is not centered
    locations.push_back(paraget_main.getPosition().x - 640);
    locations.push_back(11290);
    locations.push_back(11569);

    int range = rand() % 30 + 50;
    for (int i = 0; i < range; i++)
    {
        /// to be replaced so it uses a single texture drawn many times only
        addParagetSparkle(10549 + (rand() % 100), 330 + (rand() % 200));
    }

    /// same as above
    addFire(4, 3903, 583, true); // market
    addFire(4, 4017, 583, true);

    addFire(2, 5816, 574, true); // festival square
    addFire(2, 5431, 574, true);
    addFire(1, 5207, 368, true);
    addFire(1, 6039, 368, true);

    addFire(0, 8135, 479, true); // we should probably switch these to use something like obelisk.position.x so it's easier to move things
    addFire(0, 8465, 479, true);

    forge_big = addFire(2, 2050, 542, false); // blacksmith
    forge_purple = addFire(3, 2370, 404, false);

    p_smoke = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/smoke.png");
    p_smoke.setOrigin(p_smoke.getLocalBounds().width / 2, p_smoke.getLocalBounds().height / 2);

    addCloud("A", 9500, 140, 0, 0);
    addCloud("A", 8800, 240, 0, 0);
    addCloud("A", 8000, 170, 0, 0);

    ctrlTips.create(54, f_font, 20, sf::String("L/R: Move      X: Interact      Select: Save      Start: Title screen"), quality);

    //TO-DO: these things are not yet supported by new system. add compatibility
    /*
    mater_menu.save_loaded = save_loaded; // TEMPORARY REMOVE DIS
    
    credits.Initialise(config, v4Core);
    */
    screenFade.Create(config, 0, 512);

    float volume = (float(config->GetInt("masterVolume")) * (float(config->GetInt("bgmVolume")) / 100.f));

    sb_city_loop.loadFromFile("resources/sfx/bgm/patapolis.ogg");
    city_loop.setBuffer(sb_city_loop);
    city_loop.setLoop(true);
    city_loop.setVolume(volume);

    initialised = true;

    SetTitle(location);
    camPos = locations[location];

    initialized = true;
}

void PatapolisMenu::updateStoryPoint()
{
    SPDLOG_TRACE("Updating story point");
    SaveReader* saveReader = CoreManager::getInstance().getSaveReader();

    // While currently just doing
    //v4Core->saveReader.story_point = *std::max_element(v4Core->saveReader.missions_unlocked.begin(), v4Core->saveReader.missions_unlocked.end());
    // would suffice, we do this in case at some point we want a story point to change without unlocking a mission
    // for example, after a mission you're supposed to fail
    int last_mission = *std::max_element(saveReader->missions_unlocked.begin(), saveReader->missions_unlocked.end());
    if (last_mission < 6) // story_point 0-5 can just do that
    {
        saveReader->story_point = last_mission;
    }
}

void PatapolisMenu::addL6(std::string variant, float x, float y)
{
    SPDLOG_TRACE("Adding L6 layer: variant {} x {} y {}", variant, x, y);
    //PSprite tmp;
    //tmp.loadFromFile("resources/graphics/bg/patapolis/6" + variant + ".png", q, r);
    //tmp.setPosition(x, y);

    L tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.type = variant;
    layer_6.push_back(tmp);
}

void PatapolisMenu::addL2(std::string variant, float x, float y)
{
    SPDLOG_TRACE("Adding L2 layer: variant {} x {} y {}", variant, x, y);
    //PSprite tmp;
    //tmp.loadFromFile("resources/graphics/bg/patapolis/2" + variant + ".png", q, r);
    //tmp.setPosition(x, y);

    L tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.type = variant;
    layer_2.push_back(tmp);
}

void PatapolisMenu::addSparkle(float x, float y)
{
    SPDLOG_TRACE("Adding Sparkle: x {} y {}", x, y);

    float scale = ((rand() % 400) + 750) / float(1000);

    Sparkle tmp;

    tmp.initScale = scale;
    tmp.curScale = scale;
    tmp.x = x;
    tmp.y = y;
    tmp.baseX = x;

    sparkles.push_back(tmp);
}

void PatapolisMenu::addParagetSparkle(float x, float y)
{
    SPDLOG_TRACE("Adding Paraget Sparkle: x {} y {}", x, y);

    float scale = ((rand() % 400) + 750) / float(1000);
    float angle = (rand() % 36000) / float(100);

    ParagetSparkle tmp;

    int choice = rand() % 3 + 1;
    string nm = "paraget_sparkle_" + to_string(choice) + ".png";

    tmp.tx_name = nm;
    tmp.initScale = scale;
    tmp.curScale = scale;
    tmp.x = x;
    tmp.y = y;
    tmp.angle = angle;
    tmp.baseX = x;
    tmp.baseY = y;

    paraget_sparkles.push_back(tmp);
}

void PatapolisMenu::addRay(float x1, float y1, float x2, float y2)
{
    SPDLOG_TRACE("Adding Light Ray: x1 {} y1 {} x2 {} y2 {}", x1, y1, x2, y2);

    RayStart rs;
    rs.x1 = x1;
    rs.y1 = 5 + y1;
    rs.x2 = x2;
    rs.y2 = 5 + y2;

    rs.ox1 = x1;
    rs.ox2 = x2;

    coords.push_back(rs);
}

PatapolisMenu::Fire PatapolisMenu::addFire(int type, float x, float y, bool add)
{
    SPDLOG_TRACE("Adding Fire: type {} x {} y {} add {}", type, x, y, add);
    Fire tmp;

    string str_type = "";

    switch (type)
    {
        case 0: {
            str_type = "small";
            break;
        }

        case 1: {
            str_type = "med";
            break;
        }

        case 2: {
            str_type = "large";
            break;
        }

        case 3: {
            str_type = "purple";
            break;
        }

        case 4: {
            str_type = "market";
            break;
        }
    }

    tmp.fire[0] = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/" + str_type + "_fire1.png");
    tmp.fire[1] = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/" + str_type + "_fire2.png");
    tmp.fire[2] = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/" + str_type + "_fire3.png");

    tmp.glow = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/" + str_type + "_glow.png");

    tmp.baseX = x;
    tmp.baseY = y;
    tmp.x = x;
    tmp.y = y;

    if (add)
        fires.push_back(tmp);

    return tmp;
}

void PatapolisMenu::addSmokeParticle(float x, float y)
{
    SPDLOG_TRACE("Adding Smoke Particle: x {} y {}", x, y);

    SmokeParticle tmp;
    tmp.smk = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/smoke.png");
    tmp.x = x;
    tmp.y = y;
    tmp.baseX = x;
    tmp.baseY = y;
    tmp.curScale = 0.1;
    smoke.push_back(tmp);
}

void PatapolisMenu::addCloud(std::string type, float x, float y, float xsize, float ysize)
{
    SPDLOG_TRACE("Adding cloud type {} x {} y {} xsize {} ysize {}", type, x, y, xsize, ysize);

    if (type == "A")
    {
        CloudA cloud;
        cloud.cloud = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/1a.png");
        cloud.cloud.baseX = x;
        cloud.cloud.baseY = y;
        cloud.x = x;
        cloud.y = y;

        clouds_A.push_back(cloud);
    } else if (type == "B")
    {
        float resRatio = float(thisConfig->GetInt("resX")) / float(1280);

        CloudB cloud;
        cloud.cloud.setFillColor(sf::Color::White);
        cloud.cloud.setRadius(640 * resRatio);

        float scale_x = xsize / 640.f;
        float scale_y = ysize / 640.f;

        cloud.cloud.setScale(scale_x * resRatio, scale_y * resRatio);

        cloud.base_x = x;
        cloud.base_y = y;

        clouds_B.push_back(cloud);
    }
}

void PatapolisMenu::Initialise(Config* _thisConfig, V4Core* parent, Menu* curParentMenu)
{
    

    if (doWaitKeyPress)
    {
        v4Core->loadingWaitForKeyPress();
        v4Core->changeRichPresence("In Patapolis", "logo", "");
    }

}

void PatapolisMenu::EventFired(sf::Event event)
{
    //TO-DO: fire events for barracks menu? probably through CoreManager
    //if (barracks_menu.is_active)
    //{
    //    barracks_menu.EventFired(event);
    //} else 
    if (obelisk_menu.is_active)
    {
        obelisk_menu.EventFired(event);
    } else if (mater_menu.is_active)
    {
        mater_menu.EventFired(event);
    } else if (is_active)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            /*if (event.key.code == thisConfig->GetInt("keybindBack"))
            {
                thisConfig->thisCore->SaveToDebugLog("Left from Patapolis to Title screen.");
                this->Hide();
                this->isActive = false;
                parentMenu->Show();
                parentMenu->isActive=true;
            }*/

        } else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }
}
void PatapolisMenu::SetTitle(int menuPosition)
{
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    Config* config = CoreManager::getInstance().getConfig();
    SaveReader* saveReader = CoreManager::getInstance().getSaveReader();

    sf::String a = "L/R: Move      ";

    if (draw_ID.size() <= 1)
    {
        draw_ID.push_back(menuPosition);
    } else
    {
        draw_ID.erase(draw_ID.begin());
        draw_ID.push_back(menuPosition);
    }

    std::string draw_ID_log = "";

    for (auto i : draw_ID)
        draw_ID_log += to_string(i);

    SPDLOG_TRACE("draw_ID: {}", draw_ID_log);

    switch (menuPosition)
    {
        case Buildings::MARKET:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_trader")));
            break;
        case Buildings::FORGE:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_blacksmith")));
            break;
        case Buildings::BARRACKS:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_barracks")));
            a += "X: Interact      ";
            break;
        case Buildings::FESTIVAL:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_festival")));
            break;
        case Buildings::SEN: {
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_sen")));
            a += "X: Interact      ";

            ///activate her dialogue
            messageclouds.clear();

            MessageCloud tmp;
            tmp.Create(20, sf::Vector2f(a_sen.getGlobalPosition().x - 5, a_sen.getGlobalPosition().y - 25), sf::Color(170, 182, 250, 255), false, config->GetInt("textureQuality"), config->fontPath);
            tmp.msgcloud_ID = 0;

            vector<int> missions = saveReader->missions_unlocked;

            switch (saveReader->story_point)
            {
                case 0:
                default: {
                    // Start of game, shouldn't be accessible
                    break;
                }
                case 1: {
                    // Shida Valley
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_1")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_4")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_5")), true);
                    break;
                }
                case 2: {
                    // Patapine Grove Unlocked
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_3")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_6")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_7")), true);
                    break;
                }
                case 3: {
                    // Patapine Grove Beaten
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_2")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_8")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_9")), true);
                    break;
                }
                case 4: {
                    // Ejiji Cliffs Unlocked
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_3")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_10")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_11")), true);
                    break;
                }
                case 5: {
                    // Ejiji Cliffs Beaten
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_1")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_12")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_sen_13")), true);

                    tmp.msgcloud_ID = 2;
                    break;
                }
            }

            messageclouds.push_back(tmp);

            SPDLOG_DEBUG("Creating message cloud at {} {}", a_sen.getGlobalPosition().x - 5, a_sen.getGlobalPosition().y - 25);

            break;
        }
        case Buildings::ALTAR:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_altar")));
            a += "X: Interact      ";
            break;
        case Buildings::OBELISK:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_obelisk")));
            a += "X: Interact      ";
            break;
        case Buildings::MATER:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_mater")));
            break;
        case Buildings::PARAGET:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_paraget")));
            break;
        case Buildings::WAKAPON: {
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_wakapon")));

            a += "X: Interact      ";

            ///activate her dialogue
            messageclouds.clear();

            MessageCloud tmp;
            tmp.Create(20, sf::Vector2f(a_wakapon.getGlobalPosition().x - 5, a_wakapon.getGlobalPosition().y - 25), sf::Color(255, 255, 255, 255), false, config->GetInt("textureQuality"), config->fontPath);

            vector<int> missions = saveReader->missions_unlocked;

            if (std::find(missions.begin(), missions.end(), 1) != missions.end())
            {
                ///shida valley is unlocked

                if (missions.size() == 1) ///if it's the only mission
                {
                    ///shida valley dialogue
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_1")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_2")), true);
                    tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_3")), true);
                } else
                {
                    if (std::find(missions.begin(), missions.end(), 2) != missions.end()) ///patapine fortress STORY mission (non-repeatable)
                    {
                        ///patapine unlocked dialogue
                        tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_1")), true);
                        tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_4")), true);
                        tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_5")), true);
                    } else if (std::find(missions.begin(), missions.end(), 3) != missions.end()) ///patapine fortress REPEATABLE mission
                    {
                        if (missions.size() == 2) ///ejiji cliffs not unlocked yet (only shida and patapine)
                        {
                            ///ejiji locked dialogue
                            tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_1")), true);
                            tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_6")), true);
                            tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_7")), true);
                        } else
                        {
                            if (std::find(missions.begin(), missions.end(), 4) != missions.end()) ///ejiji cliff STORY mission
                            {
                                ///ejiji unlocked dialogue
                                tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_1")), true);
                                tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_8")), true);
                                tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_9")), true);
                            } else ///ejiji cliff REPEATABLE mission
                            {
                                tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_1")), true);
                                tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_10")), true);
                                tmp.AddDialog(Func::ConvertToUtf8String(strRepo->GetString("npc_wakapon_11")), true);
                            }
                        }
                    }
                }
            }

            tmp.msgcloud_ID = 1;
            messageclouds.push_back(tmp);

            SPDLOG_DEBUG("Creating message cloud at {} {}", a_wakapon.getGlobalPosition().x - 5, a_wakapon.getGlobalPosition().y - 25);

            break;
        }
        case Buildings::EGG:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis_egg")));
            break;
        default:
            t_title.setString(Func::ConvertToUtf8String(strRepo->GetString("patapolis")));
            break;
    }

    a += "Select: Save      Start: Title screen";

    ctrlTips.create(54, f_font, 20, a, quality);
}
float EaseIn(float time, float startValue, float change, float duration)
{
    time /= duration / 2;
    if (time < 1)
    {
        return change / 2 * time * time + startValue;
    }

    time--;
    return -change / 2 * (time * (time - 2) - 1) + startValue;
};
void PatapolisMenu::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    Config* config = CoreManager::getInstance().getConfig();
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    //if (is_active) TO-DO: is this still needed?
    if (true)
    {
        auto lastView = window->getView();
        window->setView(window->getDefaultView());

        window->draw(v_background);

        camDest = locations[location];
        if (mater_menu.is_active) {
            camDest += materoffset; // when mater menu is open, offset slightly so we still see the tree
        }

        float camDistance = abs(camDest - camPos); // why abs? we literally make it negative again a couple lines down
        float camSpeed = camDistance * 3;

        if (camPos != camDest)
        {
            if (left)
            {
                if (camPos < camDest)
                    camPos += camSpeed / fps;
                else
                    camPos -= camSpeed / fps;
            } else
            {
                if (camPos > camDest)
                    camPos -= camSpeed / fps;
                else
                    camPos += camSpeed / fps;
            }
        }

        for (int i = 0; i < clouds_A.size(); i++)
        {
            clouds_A[i].cloud.lx = clouds_A[i].cloud.baseX - (camPos / 1.3);
            clouds_A[i].cloud.ly = clouds_A[i].cloud.baseY;
            clouds_A[i].cloud.draw(window);
        }

        for (int i = 0; i < clouds_B.size(); i++)
        {
            float resRatio = window->getSize().x / float(1280);

            clouds_B[i].x = clouds_B[i].base_x - (camPos / 1.3);
            clouds_B[i].cloud.setPosition(clouds_B[i].x * resRatio, clouds_B[i].y * resRatio);
            window->draw(clouds_B[i].cloud);
        }

        rainbow.setOrigin(0, rainbow.getLocalBounds().height);
        rainbow.setPosition(9000 - (camPos / 1.3), 592);
        rainbow.draw(window);

        for (int i = 0; i < 4; i++)
        {
            back_layer[i].lx = back_layer[i].baseX - (camPos / 1.3);
            back_layer[i].draw(window);
        }

        for (int i = 0; i < layer_6.size(); i++)
        {
            //cout << "layer_6[" << i << "]: " << layer_6[i].getGlobalBounds().width/2 << " " << layer_6[i].getGlobalBounds().height << " " << layer_6[i].getPosition().x << " " << layer_6[i].getPosition().y << endl;
            //layer_6[i].lx = layer_6[i].baseX - (camPos / 1.1428571428571428571428571428571);

            //layer_6[i].setOrigin(layer_6[i].getLocalBounds().width / 2, layer_6[i].getLocalBounds().height);
            //layer_6[i].draw(window);

            ps_layer_6[layer_6[i].type].setPosition(layer_6[i].x, layer_6[i].y);
            ps_layer_6[layer_6[i].type].lx = layer_6[i].x - (camPos / 1.1428571428571428571428571428571);
            ps_layer_6[layer_6[i].type].setOrigin(ps_layer_6[layer_6[i].type].getLocalBounds().width / 2, ps_layer_6[layer_6[i].type].getLocalBounds().height);
            ps_layer_6[layer_6[i].type].draw(window);
        }

        L5.lx = L5.baseX - (camPos / 1.1111111111111111111);
        L4.lx = L4.baseX - (camPos / 1.0526315789473684210526315789474);
        wakapon.lx = wakapon.baseX - camPos;
        a_wakapon.global_x = wakapon.baseX - camPos;
        a_sen.global_x = 6450.0 - camPos;
        world_egg.lx = world_egg.baseX - camPos;
        light_1.lx = light_1.baseX - camPos;
        light_2.lx = light_2.baseX - camPos;
        egg_light.lx = egg_light.baseX - camPos;

        market.lx = market.baseX - camPos;
        mater.lx = mater.baseX - camPos;
        forge_main.lx = forge_main.baseX - camPos;
        forge_back.lx = forge_back.baseX - camPos;
        forge_glow.lx = forge_glow.baseX - camPos;
        forge_fence.lx = forge_fence.baseX - camPos;
        forge_slab.lx = forge_slab.baseX - camPos;
        forge_slab_glow.lx = forge_slab_glow.baseX - camPos;
        barracks.lx = barracks.baseX - camPos;
        festival_main.lx = festival_main.baseX - camPos;
        altar.lx = altar.baseX - camPos;
        obelisk.lx = obelisk.baseX - camPos;
        paraget_main.lx = paraget_main.baseX - camPos;
        paraget_crystal.lx = paraget_crystal.baseX - camPos;
        paraget_crystal_glow.lx = paraget_crystal_glow.baseX - camPos;

        rayX = rayXbase - camPos;

        floor_x = floor_x_base - camPos;

        L5.setOrigin(L5.getLocalBounds().width / 2, L5.getLocalBounds().height);
        L4.setOrigin(L4.getLocalBounds().width / 2, L4.getLocalBounds().height);

        L5.draw(window);
        L4.draw(window);


        for (int i = 0; i < layer_2.size(); i++)
        {
            //cout << "layer_6[" << i << "]: " << layer_6[i].getGlobalBounds().width/2 << " " << layer_6[i].getGlobalBounds().height << " " << layer_6[i].getPosition().x << " " << layer_6[i].getPosition().y << endl;
            
            //layer_2[i].lx = layer_2[i].baseX - camPos;

            //layer_2[i].setOrigin(layer_2[i].getLocalBounds().width / 2, layer_2[i].getLocalBounds().height);
            //layer_2[i].draw(window);

            ps_layer_2[layer_2[i].type].setPosition(layer_2[i].x, layer_2[i].y);
            ps_layer_2[layer_2[i].type].lx = layer_2[i].x - camPos;
            ps_layer_2[layer_2[i].type].setOrigin(ps_layer_2[layer_2[i].type].getLocalBounds().width / 2, ps_layer_2[layer_2[i].type].getLocalBounds().height);
            ps_layer_2[layer_2[i].type].draw(window);
        }

        float resRatioX = window->getSize().x / float(1280);
        float resRatioY = window->getSize().y / float(720);

        /// Fires
        for (int i = 0; i < fires.size(); i++)
        {
            fires[i].id += float(10) / fps;

            if (round(fires[i].id) >= 3)
                fires[i].id = 0;

            int id = round(fires[i].id);

            fires[i].glowsize += float(90) / fps;

            if (fires[i].glowsize >= 360)
                fires[i].glowsize -= 360;

            fires[i].glow.setOrigin(fires[i].glow.getLocalBounds().width / 2, fires[i].glow.getLocalBounds().height / 2);
            fires[i].glow.setScale(1.05 + (cos(fires[i].glowsize * 3.141592 / 180) / 10));
            fires[i].glow.setPosition(fires[i].baseX - camPos, fires[i].baseY);

            fires[i].glow.draw(window);

            fires[i].fire[id].setOrigin(fires[i].fire[id].getLocalBounds().width / 2, fires[i].fire[id].getLocalBounds().height / 2);
            fires[i].fire[id].setPosition(fires[i].baseX - camPos, fires[i].baseY);

            fires[i].fire[id].draw(window);
        }

        //cout << "for: " << endl;

        for (auto i : draw_ID)
        {
            //cout << i << endl;

            switch (i)
            {
                case Buildings::MARKET: ///Marketplace
                {
                    //for some reason, barracks are marketplace. what the fuck?
                    barracks.draw(window);
                    break;
                }

                case Buildings::FORGE: ///Blacksmith
                {
                    forge_big.id += float(12) / fps;

                    if (round(forge_big.id) >= 3)
                        forge_big.id = 0;

                    int id = round(forge_big.id);

                    forge_big.glowsize += float(90) / fps;

                    if (forge_big.glowsize >= 360)
                        forge_big.glowsize -= 360;

                    forge_big.glow.setOrigin(forge_big.glow.getLocalBounds().width / 2, forge_big.glow.getLocalBounds().height / 2);
                    forge_big.glow.setScale(1.05 + (cos(forge_big.glowsize * 3.141592 / 180) / 10));
                    forge_big.glow.setPosition(forge_big.baseX - camPos, forge_big.baseY);

                    forge_big.glow.draw(window);

                    forge_big.fire[id].setOrigin(forge_big.fire[id].getLocalBounds().width / 2, forge_big.fire[id].getLocalBounds().height / 2);
                    forge_big.fire[id].setPosition(forge_big.baseX - camPos, forge_big.baseY);

                    forge_big.fire[id].draw(window);

                    forge_back.draw(window);

                    forge_glowsize += float(90) / fps;

                    if (forge_glowsize >= 360)
                        forge_glowsize = 0;

                    forge_glow.setOrigin(forge_glow.getLocalBounds().width / 2, forge_glow.getLocalBounds().height / 2);
                    forge_glow.setScale(1.05 + (cos(forge_glowsize * 3.141592 / 180) / 10));
                    forge_glow.draw(window);

                    forge_purple.id += float(12) / fps;

                    if (round(forge_purple.id) >= 3)
                        forge_purple.id = 0;

                    id = round(forge_purple.id);

                    forge_purple.glowsize += float(90) / fps;

                    if (forge_purple.glowsize >= 360)
                        forge_purple.glowsize -= 360;

                    forge_purple.glow.setOrigin(forge_purple.glow.getLocalBounds().width / 2, forge_purple.glow.getLocalBounds().height / 2);
                    forge_purple.glow.setScale(1.05 + (cos(forge_purple.glowsize * 3.141592 / 180) / 10));
                    forge_purple.glow.setPosition(forge_purple.baseX - camPos, forge_purple.baseY);

                    forge_purple.glow.draw(window);

                    forge_purple.fire[id].setOrigin(forge_purple.fire[id].getLocalBounds().width / 2, forge_purple.fire[id].getLocalBounds().height / 2);
                    forge_purple.fire[id].setPosition(forge_purple.baseX - camPos, forge_purple.baseY);

                    forge_purple.fire[id].draw(window);

                    forge_slab_glow.setOrigin(forge_slab_glow.getLocalBounds().width / 2, forge_slab_glow.getLocalBounds().height / 2);
                    forge_slab_glow.setScale(1.05 + (cos(forge_glowsize * 3.141592 / 180) / 10));
                    forge_slab_glow.draw(window);

                    forge_main.draw(window);

                    forge_slab.draw(window);

                    forge_fence.draw(window);
                    break;
                }

                case Buildings::BARRACKS: ///Barracks
                {
                    //for some reason, marketplace is barracks. ??????
                    market.draw(window);
                    break;
                }

                case Buildings::MATER:
                {
                    mater.draw(window);
                    break;
                }

                case Buildings::PARAGET: ///Paraget
                {
                    crystal_y += float(90) / fps;

                    if (crystal_y >= 360)
                        crystal_y -= 360;

                    crystal_y2 += float(30) / fps;

                    if (crystal_y2 >= 360)
                        crystal_y2 -= 360;

                    paraget_crystal.ly = paraget_crystal.baseY + sin(crystal_y * 3.141592 / 180) * 10;
                    paraget_crystal_glow.ly = paraget_crystal.baseY + sin(crystal_y * 3.141592 / 180) * 10;

                    paraget_crystal_glow.setScale(1.05 + (cos(crystal_y2 * 3.141592 / 180) / 10));
                    paraget_crystal_glow.draw(window);
                    paraget_main.draw(window);
                    paraget_crystal.draw(window);

                    for (int i = 0; i < paraget_sparkles.size(); i++)
                    {
                        paraget_sparkles[i].x = paraget_sparkles[i].baseX - camPos;

                        if (paraget_sparkles[i].alpha > 0)
                        {
                            paraget_sparkles[i].alpha -= float(300) / fps;
                        } else
                        {
                            paraget_sparkles[i].alpha = 0;
                        }

                        if (paraget_sparkles[i].curScale > 0)
                        {
                            paraget_sparkles[i].curScale -= float(1.35) / fps;
                        } else
                        {
                            paraget_sparkles[i].curScale = 0;
                        }

                        if (paraget_sparkles[i].curScale <= 0)
                        {
                            paraget_sparkles[i].timer -= float(1000) / fps;
                        }

                        if (paraget_sparkles[i].timer <= 0)
                        {
                            paraget_sparkles[i].curScale = paraget_sparkles[i].initScale;
                            paraget_sparkles[i].alpha = 255;
                            paraget_sparkles[i].timer = 400 + (rand() % 200);
                            paraget_sparkles[i].y = paraget_sparkles[i].baseY + sin(crystal_y * 3.141592 / 180) * 10;
                        }

                        PSprite& sprk = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/" + paraget_sparkles[i].tx_name);

                        sprk.setColor(sf::Color(255, 255, 255, paraget_sparkles[i].alpha));
                        sprk.setScale(paraget_sparkles[i].curScale);
                        sprk.setOrigin(sprk.getLocalBounds().width / 2, sprk.getLocalBounds().height / 2);
                        sprk.setPosition(paraget_sparkles[i].x, paraget_sparkles[i].y);
                        sprk.draw(window);
                    }

                    break;
                }

                case Buildings::OBELISK: {
                    obelisk.draw(window);
                    break;
                }
            }
        }

        festival_main.draw(window);

        /// Smoke
        smokepath1 += float(40) / fps;

        if (smokepath1 >= 360)
            smokepath1 = 0;

        if (rand() % 100 == 1)
            addSmokeParticle(altar.baseX - 186 + (cos(smokepath1 * 3.141592 / 180) * 2), 410);
        if (rand() % 100 == 1)
            addSmokeParticle(altar.baseX + 186 + (sin(smokepath1 * 3.141592 / 180) * 2), 410);

        vector<int> e_s;

        for (int i = 0; i < smoke.size(); i++)
        {
            smoke[i].y -= float(20) / fps;
            smoke[i].x = smoke[i].baseX - camPos;

            smoke[i].alpha -= float(60) / fps;

            if (smoke[i].alpha <= 0)
                smoke[i].alpha = 0;

            smoke[i].curScale += float(0.14) / fps;

            smoke[i].smk.setPosition(smoke[i].x, smoke[i].y);
            smoke[i].smk.setColor(sf::Color(255, 255, 255, smoke[i].alpha));
            smoke[i].smk.setScale(smoke[i].curScale);

            smoke[i].smk.draw(window);

            if (smoke[i].alpha <= 0)
            {
                e_s.push_back(i);
            }
        }

        for (int i = 0; i < e_s.size(); i++)
        {
            smoke.erase(smoke.begin() + e_s[i] - i);
        }

        /// Altar
        altar.draw(window);

        r_ground.setPosition(floor_x * resRatioX, (float(720) - floor_height) * resRatioY);
        window->draw(r_ground);

        edge.setOrigin(0, edge.getLocalBounds().height);
        edge.setPosition(floor_x + 11500, 720);
        edge.draw(window);

        /// Remaining sparkles
        for (int i = 0; i < sparkles.size(); i++)
        {
            sparkles[i].x = sparkles[i].baseX - camPos;

            if (sparkles[i].alpha > 0)
            {
                sparkles[i].alpha -= float(300) / fps;
            } else
            {
                sparkles[i].alpha = 0;
            }

            if (sparkles[i].curScale > 0)
            {
                sparkles[i].curScale -= float(1.35) / fps;
            } else
            {
                sparkles[i].curScale = 0;
            }

            if (sparkles[i].curScale <= 0)
            {
                sparkles[i].timer -= float(1000) / fps;
            }

            if (sparkles[i].timer <= 0)
            {
                sparkles[i].curScale = sparkles[i].initScale;
                sparkles[i].alpha = 255;
                sparkles[i].timer = 900 + (rand() % 200);
            }

            PSprite& sprk = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/sparkle.png");

            sprk.setColor(sf::Color(255, 255, 255, sparkles[i].alpha));
            sprk.setScale(sparkles[i].curScale);
            sprk.setOrigin(sprk.getLocalBounds().width / 2, sprk.getLocalBounds().height / 2);
            sprk.setPosition(sparkles[i].x, sparkles[i].y);
            sprk.draw(window);
        }

        /// Egg light and light rays
        if (!light)
        {
            light_1.setScale(light_1.scaleX + (float(0.15) / fps));
            light_2.setScale(light_2.scaleX + (float(0.15) / fps));
            egg_light.setScale(egg_light.scaleX + (float(0.15) / fps));

            if (light_1.scaleX > 1.15)
            {
                light = true;
                light_1.setScale(1.15);
                light_2.setScale(1.15);
                egg_light.setScale(1.15);
            }
        } else
        {
            light_1.setScale(light_1.scaleX - (float(0.15) / fps));
            light_2.setScale(light_2.scaleX - (float(0.15) / fps));
            egg_light.setScale(egg_light.scaleX - (float(0.15) / fps));

            if (light_1.scaleX < 0.9)
            {
                light = false;
                light_1.setScale(0.9);
                light_2.setScale(0.9);
                egg_light.setScale(0.9);
            }
        }

        light_1.draw(window);
        light_2.draw(window);
        egg_light.draw(window);

        for (int i = 0; i < lightrays.size(); i++)
        {
            if (i != 11)
            {
                if (!lightrays[i].rise)
                {
                    lightrays[i].cur_distance -= float(80) / fps;

                    if (lightrays[i].cur_distance <= lightrays[i].min_distance)
                    {
                        lightrays[i].cur_distance = lightrays[i].min_distance;
                        lightrays[i].rise = true;
                    }
                } else
                {
                    lightrays[i].cur_distance += float(80) / fps;

                    if (lightrays[i].cur_distance >= lightrays[i].max_distance)
                    {
                        lightrays[i].cur_distance = lightrays[i].max_distance;
                        lightrays[i].rise = false;
                    }
                }

                float x1 = coords[i].x1 + rayX;
                float y1 = coords[i].y1;

                float x2 = coords[i].x2 + rayX;
                float y2 = coords[i].y2;

                lightrays[i].triangle[0].position = sf::Vector2f(x1 * resRatioX, y1 * resRatioY);
                lightrays[i].triangle[1].position = sf::Vector2f(x2 * resRatioX, y2 * resRatioY);
                lightrays[i].triangle[2].position = sf::Vector2f((((x1 + x2) / 2) + (lightrays[i].cur_distance * sin(lightrays[i].angle * 3.14159265 / 180))) * resRatioX, (((y1 + y2) / 2) + (lightrays[i].cur_distance * cos(lightrays[i].angle * 3.14159265 / 180))) * resRatioY);

                //cout << "[coord #" << i << "]" << lightrays[i].triangle[0].position.x << " " << lightrays[i].triangle[0].position.y << " " << lightrays[i].triangle[1].position.x << " " << lightrays[i].triangle[1].position.y << " " << lightrays[i].triangle[2].position.x << " " << lightrays[i].triangle[2].position.y << endl;

                window->draw(lightrays[i].triangle);
            }
        }

        bridge.setOrigin(0, bridge.getLocalBounds().height); // could consider not drawing this + wakapon unless player camera is close
        bridge.setPosition(floor_x + 11200, 720);
        bridge.draw(window);

        //TO-DO: AnimatedObject waits for a refactor, so im commenting it out for now
        /*
        /// Wakapon
        a_wakapon.fps = fps;
        a_wakapon.Draw(window);

        /// Sen
        a_sen.fps = fps;
        a_sen.Draw(window);
        */

        world_egg.draw(window);

        //TO-DO: replace with something else probably core manager blabla
        /* if (barracks_menu.mission_started)
        {
            obelisk_menu.displayMissions = false;
            //obelisk_menu.sel_location = 1;
            obelisk_menu.sel_mission = 0;
            obelisk_menu.Hide();
            SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_started = false;
        }*/

        /* TO-DO: whole ass section of not yet refactored classes.
        if(barracks_menu.is_active)
        {
            SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       update(window, fps, inputCtrl);
        } else if (altar_menu->is_active)
        {
            altar_menu->update(window, fps, inputCtrl);
        } else if (mater_menu.is_active)
        {
            mater_menu.update(window, fps, inputCtrl);
        } else if (obelisk_menu.is_active)
        {
            obelisk_menu.Update(window, fps, inputCtrl);
        } else if (credits.is_active)
        {
            credits.draw(window, fps, inputCtrl);
        } else*/
        {
            if (city_loop.getStatus() == sf::Sound::Status::Stopped)
            {
                SPDLOG_DEBUG("Play city_loop");
                city_loop.play();
            }

            t_title.setOrigin(t_title.getLocalBounds().width / 2, t_title.getLocalBounds().height / 2);
            t_title.setPosition(640, 80);
            t_title.draw(window);

            vector<int> m_rm;

            if (dialogboxes.size() <= 0)
            {
                for (int i = 0; i < messageclouds.size(); i++)
                {
                    if (messageclouds[i].firstrender)
                        messageclouds[i].Show();

                    if ((messageclouds[i].msgcloud_ID == 0) || (messageclouds[i].msgcloud_ID == 2))
                        messageclouds[i].startpos = sf::Vector2f(a_sen.getGlobalPosition().x - 5, a_sen.getGlobalPosition().y - 25);
                    else if (messageclouds[i].msgcloud_ID == 1)
                        messageclouds[i].startpos = sf::Vector2f(a_wakapon.getGlobalPosition().x - 5, a_wakapon.getGlobalPosition().y - 25);

                    if (messageclouds[i].done)
                    {
                        if (location == 4)
                        {
                            if (messageclouds[i].msgcloud_ID == 2)
                            {
                                ///Create ending dialogbox here
                                std::vector<sf::String> a = {Func::ConvertToUtf8String(strRepo->GetString("patapolis_demo_pick1")), Func::ConvertToUtf8String(strRepo->GetString("patapolis_demo_pick2"))};

                                PataDialogBox db;
                                db.Create(f_font, Func::ConvertToUtf8String(strRepo->GetString("patapolis_demofinish")), a, config->GetInt("textureQuality"));
                                db.id = 4;
                                dialogboxes.push_back(db);

                                messageclouds[i].Hide();
                            }
                        }
                    }

                    if ((messageclouds[i].done) && (floor(messageclouds[i].xsize) == 0) && (floor(messageclouds[i].ysize) == 0))
                    {
                        messageclouds[i].Hide();
                    }

                    messageclouds[i].Draw();

                    if ((!messageclouds[i].active) && (messageclouds[i].done))
                        m_rm.push_back(i);
                }
            }

            for (int i = 0; i < m_rm.size(); i++)
            {
                messageclouds.erase(messageclouds.begin() + m_rm[i] - i);
            }
        }

        vector<int> db_e; ///dialog box erase

        for (int i = 0; i < dialogboxes.size(); i++)
        {
            dialogboxes[i].x = 640;
            dialogboxes[i].y = 360;
            dialogboxes[i].Draw();

            if (dialogboxes[i].closed)
                db_e.push_back(i);
        }

        for (int i = 0; i < db_e.size(); i++)
        {
            dialogboxes.erase(dialogboxes.begin() + db_e[i] - i);
        }

        window->setView(lastView);

        lastView = window->getView();
        window->setView(window->getDefaultView());

        if ((!obelisk_menu.is_active) && (!mater_menu.is_active) && (!credits.is_active))
        {
            ctrlTips.x = 0;
            ctrlTips.y = (720 - ctrlTips.ySize);
            //TO-DO: control tips without window argument
            //ctrlTips.draw(window);
        }

        /*if(fade_alpha > 0)
        {
            fade_alpha -= float(500) / fps;
        }

        if(fade_alpha <= 0)
        {
            fade_alpha = 0;
        }

        fade_box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
        fade_box.setFillColor(sf::Color(0,0,0,fade_alpha));
        window->draw(fade_box);*/

        screenFade.draw();

        if (screenFade.checkFinished())
        {
            if (goto_id != -1)
            {
                switch (goto_id)
                {
                    case 0: ///Barracks
                    {
                        city_loop.stop();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       Show();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       is_active = true;
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       obelisk = false;
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       loadInventory();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       updateInputControls();

                        screenFade.Create(0, 1536);
                        StateManager::getInstance().setState(StateManager::BARRACKS);

                        break;
                    }

                    case 1: ///Obelisk
                    {
                        city_loop.stop();
                        obelisk_menu.Reload();
                        obelisk_menu.Show();
                        obelisk_menu.is_active = true;

                        screenFade.Create(thisConfig, 0, 1536);

                        break;
                    }

                    case 2: ///Exit barracks
                    {
                        city_loop.stop();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       Hide();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       is_active = false;
                        Show();
                        is_active = true;

                        screenFade.Create(thisConfig, 0, 1536);

                        break;
                    }

                    case 3: ///Exit obelisk
                    {
                        city_loop.stop();
                        obelisk_menu.Hide();
                        obelisk_menu.is_active = false;
                        Show();
                        is_active = true;

                        screenFade.Create(thisConfig, 0, 1536);

                        break;
                    }

                    case 4: ///Enter barracks (obelisk)
                    {
                        city_loop.stop();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       Show();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       is_active = true;
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       obelisk = true;
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_id = obelisk_menu.missions[obelisk_menu.sel_mission].mis_ID;
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_file = obelisk_menu.missions[obelisk_menu.sel_mission].mission_file;

                        if (config->thisCore->saveReader.mission_levels[obelisk_menu.missions[obelisk_menu.sel_mission].mis_ID] != 0)
                            SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_multiplier = 0.85 + config->thisCore->saveReader.mission_levels[obelisk_menu.missions[obelisk_menu.sel_mission].mis_ID] * 0.15;
                        else
                            SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_multiplier = 1;

                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       loadInventory();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       updateInputControls();

                        screenFade.Create(thisConfig, 0, 1536);

                        break;
                    }

                    case 5: ///Enter mission
                    {
                        city_loop.stop();
                        sf::Thread loadingThreadInstance(&V4Core::loadingThread, v4Core);
                        v4Core->continue_loading = true;
                        window->setActive(false);
                        loadingThreadInstance.launch();

                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       currentController->Initialise(*thisConfig, config->GetString("mission1Background"), *v4Core);
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       currentController->StartMission(SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_file, 1, SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_id, SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_multiplier);
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       Hide();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       is_active = false;

                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_started = true;

                        v4Core->continue_loading = false;

                        screenFade.Create(thisConfig, 0, 1536);

                        break;
                    }

                    case 6: ///credits
                    {
                        city_loop.stop();
                        credits.is_active = true;
                        credits.restart();

                        screenFade.Create(thisConfig, 0, 512);

                        break;
                    }

                    case 7: ///go to title screen
                    {
                        city_loop.stop();
                        this->Hide();
                        this->is_active = false;
                        parentMenu->Show();
                        parentMenu->is_active = true;

                        parentMenu->screenFade.Create(thisConfig, 0, 512);

                        break;
                    }
                }

                goto_id = -1;
            }
        }

        window->setView(lastView);

        if (dialogboxes.size() <= 0)
        {
            if ((!obelisk_menu.is_active) && (StateManager::getInstance().getState() != StateManager::PATAPOLIS_ALTAR) && (!mater_menu.is_active) && (!credits.is_active) && (screenFade.checkFinished()))
            {
                if ((inputCtrl->isKeyPressed(InputController::Keys::LEFT)) || (inputCtrl->isKeyPressed(InputController::Keys::LTRIGGER)))
                {
                    if (location > 0)
                    {
                        location--;
                        left = true;

                        for (int i = 0; i < messageclouds.size(); i++)
                            messageclouds[i].End();

                        SetTitle(location);
                    }
                } else if ((inputCtrl->isKeyPressed(InputController::Keys::RIGHT)) || (inputCtrl->isKeyPressed(InputController::Keys::RTRIGGER)))
                {
                    if (location < locations.size() - 1)
                    {
                        location++;
                        left = false;

                        for (int i = 0; i < messageclouds.size(); i++)
                            messageclouds[i].End();

                        SetTitle(location);
                    }
                } else if (inputCtrl->isKeyPressed(InputController::Keys::CROSS))
                {
                    // select the current menu item
                    switch (location)
                    {
                        case Buildings::MARKET:
                            /// trader/random
                            // open the world map
                            break;
                        case Buildings::BARRACKS:
                            /// armory/barracks

                            screenFade.Create(1, 1536);
                            goto_id = 0;

                            /*SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       Show();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       is_active = true;
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       obelisk = false;
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       ReloadInventory();
                        SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       UpdateInputControls();*/
                            break;
                        case Buildings::FESTIVAL:
                            /// festival
                            // open barracks screen
                            break;
                        case Buildings::ALTAR:
                            /// altar
                            // open mater menu
                            
                            
                            /* altar_menu->save_loaded = save_loaded;
                            altar_menu->reloadInventory();
                            altar_menu->showAltar();
                            altar_menu->is_active = true;
                            altar_menu->showAltar(); */

                            StateManager::getInstance().setState(StateManager::PATAPOLIS_ALTAR);

                            break;
                        case Buildings::OBELISK:
                            /// obelisk

                            screenFade.Create(thisConfig, 1, 1536);
                            goto_id = 1;

                            /*obelisk_menu.Reload();
                        obelisk_menu.Show();
                        obelisk_menu.isActive = true;*/
                            break;
                        case Buildings::MATER:
                            /// festival
                            // open barracks screen
                            mater_menu.save_loaded = save_loaded;
                            mater_menu.is_active = true;
                            mater_menu.showMater();
                            break;
                        default:
                            /// nothing
                            break;
                    }
                }

                if (inputCtrl->isKeyPressed(InputController::Keys::START))
                {
                    std::vector<sf::String> a = {Func::ConvertToUtf8String(strRepo->GetString("nav_yes")), Func::ConvertToUtf8String(strRepo->GetString("nav_no"))};

                    PataDialogBox db;
                    db.Create(f_font, Func::ConvertToUtf8String(strRepo->GetString("patapolis_returntomain")), a, config->GetInt("textureQuality"));
                    db.id = 0;
                    dialogboxes.push_back(db);
                } else if (inputCtrl->isKeyPressed(InputController::Keys::SELECT))
                {
                    std::vector<sf::String> a = {Func::ConvertToUtf8String(strRepo->GetString("nav_yes")), Func::ConvertToUtf8String(strRepo->GetString("nav_no"))};

                    PataDialogBox db;
                    db.Create(f_font, Func::ConvertToUtf8String(strRepo->GetString("patapolis_save")), a, config->GetInt("textureQuality"));
                    db.id = 2;
                    dialogboxes.push_back(db);
                }
            }
        } else
        {
            if (inputCtrl->isKeyPressed(InputController::Keys::CROSS))
            {
                switch (dialogboxes[dialogboxes.size() - 1].CheckSelectedOption())
                {
                    case 0: {
                        if (dialogboxes[dialogboxes.size() - 1].id == 0)
                        {
                            SPDLOG_DEBUG("Open second dialogbox");
                            dialogboxes[dialogboxes.size() - 1].Close();

                            std::vector<sf::String> a = {Func::ConvertToUtf8String(strRepo->GetString("nav_yes")), Func::ConvertToUtf8String(strRepo->GetString("nav_no"))};

                            PataDialogBox db;
                            db.Create(f_font, Func::ConvertToUtf8String(strRepo->GetString("patapolis_returntomainsave")), a, config->GetInt("textureQuality"));
                            db.id = 1;
                            dialogboxes.push_back(db);

                            break;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 1)
                        {
                            SPDLOG_INFO("Left from Patapolis to Title screen.");
                            screenFade.Create(thisConfig, 1, 512);
                            goto_id = 7;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 2)
                        {
                            SPDLOG_DEBUG("Saving game should happen here. Game not saving yet.");
                            v4Core->saveReader.Save();
                            SPDLOG_INFO("Saved the game.");

                            dialogboxes[dialogboxes.size() - 1].Close();

                            std::vector<sf::String> a = {Func::ConvertToUtf8String(strRepo->GetString("nav_understood"))};

                            PataDialogBox db;
                            db.Create(f_font, Func::ConvertToUtf8String(strRepo->GetString("patapolis_saved")), a, config->GetInt("textureQuality"));
                            db.id = 3;
                            dialogboxes.push_back(db);

                            break;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 3)
                        {
                            SPDLOG_DEBUG("Done.");

                            dialogboxes[dialogboxes.size() - 1].Close();
                            break;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 4)
                        {
                            SPDLOG_INFO("Going into credits!");
                            screenFade.Create(thisConfig, 1, 512);
                            goto_id = 6;

                            dialogboxes[dialogboxes.size() - 1].Close();
                            break;
                        }
                    }

                    case 1: {
                        SPDLOG_DEBUG("Back to Patapolis");
                        dialogboxes[dialogboxes.size() - 1].Close();

                        break;
                    }
                }
            }
        }
    } else
    {
        fade_alpha = 255;
    }
}

void PatapolisMenu::UpdateButtons()
{
    /// this should update the text on all the buttons
}
void PatapolisMenu::OnExit()
{
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
PatapolisMenu::~PatapolisMenu()
{
    //dtor
}
