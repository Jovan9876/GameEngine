# 2D ECS Game Engine

A custom-built 2D game engine written in C++ utilizing the **SDL3** library. 
This engine implements a robust **Entity-Component-System (ECS)** architecture
that efficiently handles game objects, rendering, physics, and state management.

## Key Features

### Core Architecture
* **Entity-Component-System (ECS):** Decouples data (Components) from logic (Systems).
    * **Entities:** Distinct IDs representing game objects.
    * **Components:** Data containers for attributes like `Transform`, `Sprite`, `Velocity`, `Collider`, and `Gravity`.
    * **Systems:** Logic processors including `RenderSystem`, `MovementSystem`, `CollisionSystem`, `GravitySystem`, and `AnimationSystem`.
 
### Rendering & Graphics
* **SDL3 Integration:** Uses SDL3 for window creation, rendering, and input handling.
* **Sprite Management:** Supports static sprites and animated spritesheets.
* **XML Animations:** Loads animation frames and clips directly from XML files using `TinyXML2`.
* **Camera System:** Dynamic camera that follows the player, with clamping and viewport management.

### Physics & World
* **Procedural Generation:** `MapGeneration` class capable of generating large vertical levels with randomized platform placement.
* **Collision Detection:** AABB (Axis-Aligned Bounding Box) collision detection for entities and tiles.
* **Platform Mechanics:** Support for various platform types:
    * **Static:** Standard solid platforms.
    * **Moving:** Platforms that move between two points.
    * **Breakable:** Platforms that disappear upon contact.

### Managers
* **Asset Manager:** Centralized loading and retrieval of textures, fonts (TTF), and XML animations.
* **Scene Manager:** Handles scene transitions (e.g., Main Menu to Gameplay) and manages the active `Scene` object.
* **Audio Manager:** Wrapper for `SDL_mixer` to handle background music and sound effects.

## Project Structure

```text
GameEngine/
├── asset/                 # Game resources (images, audio, fonts, XML)
├── src/
│   ├── ecs/               # Core ECS logic (Entity, Component, World)
│   │   └── system/        # Logic systems (Render, Physics, Animation)
│   ├── manager/           # Resource and State managers (Asset, Scene, Audio)
│   ├── utils/             # Utilities (Vector2D, Collision, TinyXML2)
│   ├── Game.cpp           # Main game loop and initialization
│   ├── main.cpp           # Entry point
│   ├── MapGeneration.cpp  # Procedural level generation algorithms
│   └── ...
├── CMakeLists.txt         # Build configuration
└── README.md
