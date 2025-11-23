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
    world.getMap().tileset = TextureManager::load("../asset/tileset.png");
    generator.generatePlatforms(world.getMap(), 20, 100, 50);

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

    SDL_Texture *tex = TextureManager::load("../asset/tileset.png");
    SDL_FRect colSrc{0, 32, 32, 32};
    SDL_FRect colDst{c.rect.x, c.rect.y, c.rect.w, c.rect.h};
    e.addComponent<Sprite>(tex, colSrc, colDst);

    auto &plat = e.addComponent<Platform>();   // only once

    float random = static_cast<float>(rand()) / RAND_MAX;

    if (random < 0.2f) {
        // 20% Breakable
        plat.type = Platform::Type::Breakable;
        e.addComponent<BreakablePlatform>();
    }
    else if (random > 0.8f) {
        // 20% Moving
        plat.type = Platform::Type::Moving;

        auto &mp = e.addComponent<MovingPlatform>();

        // Use current tile center as middle point
        Vector2D center = t.position;

        // Move one tile left/right; adjust 32.f if your tiles are different
        mp.startPoint = center + Vector2D(-32.0f, 0.0f);
        mp.endPoint   = center + Vector2D( 32.0f, 0.0f);

        mp.speed   = 80.0f;   // pixels per second – tweak this
        mp.moveToB = true;    // start going start -> end
    }
    else {
        // Everything else is static
        plat.type = Platform::Type::Static; // or Normal if that's your enum
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

    // Add entites
    auto &player(world.createEntity());
    auto &playerTransform = player.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);
    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 240.0f);

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture *tex = TextureManager::load("../asset/animations/vampire_anim.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst{playerTransform.position.x, playerTransform.position.y, 64, 64};
    player.addComponent<Sprite>(tex, playerSrc, playerDst);

    auto &playerCollider = player.addComponent<Collider>("player");
    playerCollider.rect.w = playerDst.w;
    playerCollider.rect.h = playerDst.h;

    player.addComponent<PlayerTag>();
    player.addComponent<Gravity>(200.0f,200.0f);

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
}