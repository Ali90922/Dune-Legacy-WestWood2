/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <units/Saboteur.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>
#include <House.h>
#include <Map.h>
#include <Game.h>
#include <ScreenBorder.h>
#include <Explosion.h>
#include <SoundPlayer.h>

/*
 *  Saboteur class: Specialized unit derived from InfantryBase.
 *  Represents a stealthy unit capable of destroying enemy structures
 *  upon reaching close proximity.
 */

Saboteur::Saboteur(House *newOwner) : InfantryBase(newOwner)
{
    Saboteur::init();

    setHealth(getMaxHealth()); // Set initial health to max

    // Set visibility: not visible to all, but visible to the owner team
    setVisible(VIS_ALL, false);
    setVisible(getOwner()->getTeamID(), true);
    attackMode = GUARD; // Default attack mode is guard
}

Saboteur::Saboteur(InputStream &stream) : InfantryBase(stream)
{
    Saboteur::init();
}

/*
 *  Initializes the Saboteur specific properties like ID and graphics.
 */
void Saboteur::init()
{
    itemID = Unit_Saboteur;        // Set unit type to Saboteur
    owner->incrementUnits(itemID); // Register unit creation with owner

    graphicID = ObjPic_Saboteur;                                           // Set the graphic ID
    graphic = pGFXManager->getObjPic(graphicID, getOwner()->getHouseID()); // Load graphic

    numImagesX = 4; // Number of horizontal images in the sprite sheet
    numImagesY = 3; // Number of vertical images in the sprite sheet

    numWeapons = 0; // Saboteur doesn't use standard weapons
}

Saboteur::~Saboteur() = default;

/*
 *  Checks the position of the Saboteur and updates visibility based on proximity
 *  to other units or structures. Stealth mechanics are handled here.
 */
void Saboteur::checkPos()
{
    InfantryBase::checkPos();

    if (active)
    {
        bool canBeSeen[NUM_TEAMS]; // Array to store visibility status for each team
        for (int i = 0; i < NUM_TEAMS; i++)
        {
            canBeSeen[i] = false; // Initialize visibility to false for all teams
        }

        // Check surrounding tiles for visibility
        for (int x = location.x - 2; (x <= location.x + 2); x++)
        {
            for (int y = location.y - 2; (y <= location.y + 2); y++)
            {
                if (currentGameMap->tileExists(x, y) && currentGameMap->getTile(x, y)->hasAnObject())
                {
                    canBeSeen[currentGameMap->getTile(x, y)->getObject()->getOwner()->getTeamID()] = true;
                }
            }
        }

        // Update visibility for each team
        for (int i = 0; i < NUM_TEAMS; i++)
        {
            setVisible(i, canBeSeen[i]);
        }

        setVisible(getOwner()->getTeamID(), true); // Owner team can always see it
    }
}

/*
 *  Updates the Saboteur's state each game tick.
 *  Handles movement, checks proximity to targets, and triggers explosions.
 */
bool Saboteur::update()
{
    if (active)
    {
        if (!moving)
        {
            // Check if close enough to blow up the target
            if (target.getObjPointer() != nullptr)
            { // Ensure target is valid
                if (getOwner()->getTeamID() != target.getObjPointer()->getOwner()->getTeamID())
                {
                    Coord closestPoint = target.getObjPointer()->getClosestPoint(location);

                    // If close enough, destroy the target and the Saboteur itself
                    if (blockDistance(location, closestPoint) <= 1.5_fix)
                    {
                        if (isVisible(getOwner()->getTeamID()))
                        {
                            screenborder->shakeScreen(18); // Shake the screen on explosion
                        }

                        ObjectBase *pObject = target.getObjPointer();
                        destroy();             // Destroy the Saboteur
                        pObject->setHealth(0); // Set target's health to 0
                        pObject->destroy();    // Destroy the target
                        return false;          // Return false as the Saboteur is no longer active
                    }
                }
            }
        }
    }

    return InfantryBase::update(); // Call base class update method
}

/*
 *  Deploys the Saboteur to a new location on the map.
 *  Sets visibility rules accordingly.
 */
void Saboteur::deploy(const Coord &newLocation)
{
    UnitBase::deploy(newLocation);

    setVisible(VIS_ALL, false);                // Initially invisible to all
    setVisible(getOwner()->getTeamID(), true); // Visible to owner team
}

/*
 *  Determines if the Saboteur can attack a given object.
 *  Saboteurs can attack structures and non-infantry ground units.
 */
bool Saboteur::canAttack(const ObjectBase *object) const
{
    if (object != nullptr)
    {
        // Allow attacks on structures and certain ground units but not infantry or sandworms
        if ((object->isAStructure() || (object->isAGroundUnit() && !object->isInfantry() && object->getItemID() != Unit_Sandworm)) && (object->getOwner()->getTeamID() != owner->getTeamID()) // Ensure target is an enemy
            && object->isVisible(getOwner()->getTeamID()))
        { // Target must be visible

            return true;
        }
    }

    return false; // Otherwise, cannot attack
}

/*
 *  Handles the destruction of the Saboteur, triggering an explosion effect
 *  and playing the appropriate sound effect.
 */
void Saboteur::destroy()
{
    Coord realPos(lround(realX), lround(realY));                                                         // Get real position of Saboteur
    Uint32 explosionID = currentGame->randomGen.getRandOf({Explosion_Medium1, Explosion_Medium2});       // Random explosion type
    currentGame->getExplosionList().push_back(new Explosion(explosionID, realPos, owner->getHouseID())); // Create explosion

    if (isVisible(getOwner()->getTeamID()))
    {
        soundPlayer->playSoundAt(Sound_ExplosionLarge, location); // Play explosion sound
    }

    InfantryBase::destroy(); // Call base class destroy method
}

/*
 * Activates a stealth mode for the Saboteur, reducing its detectability by enemy units.
 * When stealth mode is active, only specific detection units or structures can reveal it.
 */
void Saboteur::activateStealthMode()
{
    setVisible(VIS_ALL, false); // Make the Saboteur invisible to all teams
    stealthActive = true;       // Set stealth mode active
    stealthTimer = 300;         // Set a timer for how long the stealth mode will last
    // Additional logic for detection could be added here
}

/*
 * Manages the stealth timer and deactivates stealth mode if the timer runs out.
 * This function should be called during the update cycle.
 */
void Saboteur::manageStealthTimer()
{
    if (stealthActive && stealthTimer > 0)
    {
        stealthTimer--; // Decrement the timer
        if (stealthTimer == 0)
        {
            deactivateStealthMode(); // Deactivate stealth mode when timer runs out
        }
    }
}

/*
 * Deactivates stealth mode, making the Saboteur visible again to enemy units.
 * This function is called when the stealth timer runs out or the Saboteur takes an action.
 */
void Saboteur::deactivateStealthMode()
{
    stealthActive = false;     // Set stealth mode inactive
    setVisible(VIS_ALL, true); // Make the Saboteur visible to all teams
}

/*
 * Performs a self-detonation action when the Saboteur reaches its target.
 * This action is used as a last resort to cause maximum damage to enemy structures or units.
 */
void Saboteur::selfDetonate()
{
    if (active)
    {
        screenborder->shakeScreen(25); // Stronger screen shake to indicate a powerful explosion
        destroy();                     // Destroy the Saboteur
        // Additional damage or effects could be applied to nearby objects
    }
}
