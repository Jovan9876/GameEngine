//
// Created by jovan on 2025-10-21.
//

#include "Scene.h"
#include "AssetManager.h"
#include "MapGeneration.h"

Scene::Scene(const char *sceneName, const char *mapPath, const int windowWidth, const int windowHeight) : name(sceneName) {
    world.setWindowSize(windowWidth, windowHeight);

    MapGeneration generator;
    generator.minGapY = 60.0f;
    generator.maxGapY = 120.0f;
    generator.minWidth = 2;
    generator.maxWidth = 5;
    world.getMap().tileset = TextureManager::load("../asset/mario.png");
    generator.generatePlatforms(world.getMap(), 25, 1000, 200);

    // Create collider entities from map colliders
    for (auto &collider: world.getMap().colliders) {
    auto &e = world.createEntity();

    auto &t = e.addComponent<Transform>(
        Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f
    );

    auto &c = e.addComponent<Collider>("platform");
    c.rect.x = collider.rect.x;
    c.rect.y = collider.rect.y;
    c.rect.w = collider.rect.w;
    c.rect.h = collider.rect.h;

    SDL_Texture *tex = TextureManager::load("../asset/gametiles.png");
    SDL_FRect colSrc{0, 0, 120, 35};
    SDL_FRect colDst{c.rect.x, c.rect.y, c.rect.w, c.rect.h};
    e.addComponent<Sprite>(tex, colSrc, colDst);

    auto &plat = e.addComponent<Platform>();

        // Generate random number of platform types
    float random = static_cast<float>(rand()) / RAND_MAX;

    if (random < 0.2f) {
        // 20% Breakable
        plat.type = Platform::Type::Breakable;
        e.addComponent<BreakablePlatform>();


        SDL_Texture *tex = TextureManager::load("../asset/gametiles.png");
        SDL_FRect colSrc{0, 140, 120, 35};
        SDL_FRect colDst{c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        e.addComponent<Sprite>(tex, colSrc, colDst);

    }
    else if (random > 0.8f) {
        // 20% Moving
        plat.type = Platform::Type::Moving;

        SDL_Texture *tex = TextureManager::load("../asset/gametiles.png");
        SDL_FRect colSrc{0, 105, 120, 35};
        SDL_FRect colDst{c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        e.addComponent<Sprite>(tex, colSrc, colDst);

        auto &mp = e.addComponent<MovingPlatform>();

        // Use current tile center as middle point
        Vector2D center = t.position;

        // move tile left/right
        mp.startPoint = center + Vector2D(-32.0f, 0.0f);
        mp.endPoint   = center + Vector2D( 32.0f, 0.0f);

        mp.speed   = 100.0f;
        mp.moveToB = true;
    }
    else {
        // Everything else is static/normal type
        plat.type = Platform::Type::Static;
    }
}

    // Add coin items from itemSpawns points
    SDL_Texture *coinTex = TextureManager::load("../asset/coin.png");
    SDL_FRect coinSrc{0, 0, 32, 32};

    // for (const auto &spawn: world.getMap().itemSpawns) {
    //     auto &coin = world.createEntity();
    //
    //     auto &transform = coin.addComponent<Transform>(Vector2D(spawn.x, spawn.y), 0.0f, 1.0f);
    //     SDL_FRect coinDest{spawn.x, spawn.y, 32, 32};
    //     coin.addComponent<Sprite>(coinTex, coinSrc, coinDest);
    //
    //     auto &c = coin.addComponent<Collider>("item");
    //     c.rect.x = spawn.x;
    //     c.rect.y = spawn.y;
    //     c.rect.w = coinDest.w;
    //     c.rect.h = coinDest.h;
    // }

    auto &cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 32, world.getMap().height * 32);

    // Makes it so the player spawns on top of a platform
    float spawnY = world.getMap().height * 32 - 100;
    Collider* spawnPlatform = nullptr;

    for (auto& collider : world.getMap().colliders) {
        if (collider.rect.y >= spawnY - 200 && collider.rect.y <= spawnY) {
            spawnPlatform = &collider;
            break;
        }
    }

    auto &player(world.createEntity());
    float playerStartX, playerStartY;

    if (spawnPlatform) {
        playerStartX = spawnPlatform->rect.x + (spawnPlatform->rect.w / 2) - 32;
        playerStartY = spawnPlatform->rect.y - 64;
    } else {
        playerStartX = (world.getMap().width * 32) / 2;
        playerStartY = 100;
    }

    auto &playerTransform = player.addComponent<Transform>(Vector2D(playerStartX, playerStartY), 0.0f, 1.0f);

    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 240.0f);
    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture *tex = TextureManager::load("../asset/animations/CoatDoodle.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst{playerTransform.position.x, playerTransform.position.y, 64, 64};
    player.addComponent<Sprite>(tex, playerSrc, playerDst);

    auto &playerCollider = player.addComponent<Collider>("player");
    playerCollider.rect.w = playerDst.w;
    playerCollider.rect.h = playerDst.h;

    player.addComponent<PlayerTag>();
    player.addComponent<Gravity>(750.0f,750.0f);

    player.addComponent<ScreenWrap>(true, 0.0f);

    auto &spawner(world.createEntity());
    Transform t = spawner.addComponent<Transform>(Vector2D(windowWidth / 2, windowHeight - 5), 0.0f, 1.0f);
    spawner.addComponent<TimedSpawner>(2.0f, [this, t] {
        // Create projectile
        auto &e(world.createDeferredEntity());
        e.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
        e.addComponent<Velocity>(Vector2D(0, -1), 100.0f);

        Animation anim = AssetManager::getAnimation("enemy");
        e.addComponent<Animation>(anim);

        SDL_Texture *tex = TextureManager::load("../asset/animations/bird_anim.png");
        SDL_FRect enemySrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect dest{t.position.x, t.position.y, 32, 32};
        e.addComponent<Sprite>(tex, enemySrc, dest);

        Collider c = e.addComponent<Collider>("projectile");
        c.rect.w = dest.w;
        c.rect.h = dest.h;

        e.addComponent<ProjectileTag>();
    });

    // Add scene state
    auto &state(world.createEntity());
    state.addComponent<SceneState>();

    createScoreLabel();

    //Testing pause
    // pauseOverlay = &createSettingsOverlay(windowWidth, windowHeight);

}

Entity &Scene::createScoreLabel() {
    auto& playerScoreLabel(world.createEntity());
    Label label = {
        "Test String",
        AssetManager::getFont("arial"),
        {5, 45, 10, 255},
        LabelType::Score,
        "playerScore"
    };

    ScoreTracker scoreTracker = {};
    playerScoreLabel.addComponent<ScoreTracker>(scoreTracker);

    TextureManager::loadLabel(label);
    playerScoreLabel.addComponent<Label>(label);
    playerScoreLabel.addComponent<Transform>(Vector2D(10, 10), 0.0f, 1.0f);
    return playerScoreLabel;
}
    //TODO for settings / pause menu
// These two are for refactoring
// void initMainMenu(int windowWidth, int windowHeight);
// int initGameplay(const char* mappath, int windowWidth, int windowHeight);

void Scene::togglePause() {
    // isPaused = !isPaused;

    std::cout << "Pause toggled" << std::endl;
    if (pauseOverlay) {
        toggleSettingsOverlayVisibility(*pauseOverlay);
    }
}

Entity& Scene::createSettingsOverlay(int windowWidth, int windowHeight) {
    auto &overlay (world.createEntity());
    SDL_Texture *overlayTex = TextureManager::load("../asset/ui/settings.jpg");
    SDL_FRect overlaySrc {0, 0, windowWidth * 0.85f, windowHeight * 0.85f};
    SDL_FRect overlayDest {(float)windowWidth / 2 - overlaySrc.w / 2, (float)windowHeight / 2 - overlaySrc.h / 2, overlaySrc.w, overlaySrc.h};
    overlay.addComponent<Transform>(Vector2D(overlayDest.x, overlayDest.y), 0.0f, 1.0f);
    overlay.addComponent<Sprite>(overlayTex, overlaySrc, overlayDest, RenderLayer::UI, false);
    // createSettingsUIComponents(overlay);
    return overlay;
}
// // Entity& Scene::createCogButton(int windowWidth, int windowHeight, Entity& overlay);
// Potentially not needed
void Scene::createSettingsUIComponents(Entity& overlay) {
    if (!overlay.hasComponent<Children>()) {
        overlay.addComponent<Children>();
    }

    auto& overlayTransform = overlay.getComponent<Transform>();
    auto& overlaySprite = overlay.getComponent<Sprite>();

    float baseX = overlayTransform.position.x;
    float baseY = overlayTransform.position.y;

    // Don't need since we don't have mouse click
    // auto& closeButton = world.createEntity();
    // auto closeTransform = closeButton.addComponent<Transform>(Vector2D(baseX + overlaySprite.dst.w - 40, baseY + 10), 0.0f, 1.0f);
    //
    // SDL_Texture *text = TextureManager::load("../asset/ui/close.png");
    // SDL_FRect closeSrc{0, 0, 32, 32};
    // SDL_FRect closeDest{closeTransform.position.x, closeTransform.position.y, closeSrc.w, closeSrc.h};
    // closeButton.addComponent<Sprite>(text, closeSrc, closeDest, RenderLayer::UI, false);
    // closeButton.addComponent<Collider>("ui", closeDest);

    //important to use if i add text
    // closeButton.addComponent<Parent>(&overlay);
    // auto& parentChildren = overlay.getComponent<Children>();
    // parentChildren.children.push_back(&closeButton);

}
void Scene::toggleSettingsOverlayVisibility(Entity& overlay) {
    auto& sprite = overlay.getComponent<Sprite>();
    bool newVisibility = !sprite.visible;
    sprite.visible = newVisibility;

    if (overlay.hasComponent<Children>()) {
        auto& c = overlay.getComponent<Children>();

        for (auto& child : c.children) {
            if (child && child->hasComponent<Sprite>()) {
                child->getComponent<Sprite>().visible = newVisibility;
            }
        }
    }
}