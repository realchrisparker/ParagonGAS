# Paragon GAS
Paragon GAS (P-GAS) is a gameplay ability system sample that teaches developers how to use several of Unreal Engine features together to build a GAS-powered combat system using free FAB store assets.

Unreal Engine Version: 5.6.1

***
> NOTE: This is a sample project and does not represent a completed game.
***

## Epic Unreal Features
- Gameplay Ability System and Gameplay Tags
- Gameplay Ability System Combat System
- StateTree (with EQS)
- Custom Tools (New Item Menu Category, New Items, and Editor)
- Custom Subsystems
- Thread Safe Animation
- Custom Anim Notifiers (Windowing, etc.)
- Motion Matching

## Setup
The project uses the free Unreal Paragon characters. Currently, the characters needed to compile/run the sample are;
- Paragon: Wukong (Two-handed Melee Weapon)
- Paragon: Countess (Two Melee Weapons)
- Paragon: Sparrow (Two-handed Range Weapon)
- Paragon: Twinblast (Motion Matching)

You will need to add these to the project in their default directories from the Epic Games Launcher.

> NOTE: Animations used in the YouTube Videos (Solodev) are not included in this sample. These are paid for assets created by 9CG. Please check out their work and support this excellent creator.
- https://www.fab.com/listings/02b4b496-4591-4cee-8970-e28c08026eae
- https://www.fab.com/listings/cce2c969-b84c-4fb8-9fbc-b373d469cfec

This means there are Montages and Animation Sequences that are referenced but not included. You will need to create your own Montages from animation sequences you have, either free or licensed.

Each player character has two Animation Blueprints. The first uses the Paragon animations and blendspaces. The second uses the ones I created for the YouTube video series. Both utilize the base class PGAS_PlayerAnimInstanceBase.h, which implements thread-safe updates of variables.

To clarify, I chose to do it this way because the provided Paragon fighting animations from Epic did not fully showcase the combat system. This led me to decide to use licensed animations for the YouTube series, particularly for the concept of chaining attacks to make combos.
***

# FAB Content Used
- Paragon: Wukong (https://www.fab.com/listings/27054d0c-c26e-4fe3-b6f9-fa778dfcb8b6)
- Paragon: Countess (https://www.fab.com/listings/0bf014eb-f2ed-4029-adda-81a855eb5220)
- Paragon: Sparrow (https://www.fab.com/listings/7d76ddf0-d9ce-4d00-939e-d72793534d01)
- Paragon: Twinblast (https://www.fab.com/listings/9fa88852-5711-42e1-94fa-2491498a64da)
- Seven Swords (https://www.fab.com/listings/9184a25a-4124-4b3f-8909-f7d2c6104c8e)
- Kunai (https://www.fab.com/listings/bd0e0b2a-46c8-4f98-8df9-c8131a16a433)
- Free Realistic Sword Sound Effects Pack (https://www.fab.com/listings/041c5773-f40e-4ae6-bb8b-8a3f36b20c27)
- Mixamo Animations

# Engine Commands Used
In version 5.6 used the following:
- p.Chaos.EnableAsyncInitBody = true
- LevelStreaming.AllowIncrementalPreRegisterComponents = true
- LevelStreaming.AllowIncrementalPreUnRegisterComponents = true
