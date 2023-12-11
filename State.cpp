#include "State.h"

/**
 * Display the description of the room the player is in.
 */
void State::announceLoc() const {
    this->currentRoom->describe();  // Call the describe() method of the current room
}

/**
 * Constructor.
 * @param startRoom Pointer to the room to start in.
 */
State::State(Room *startRoom) : currentRoom(startRoom) {}

/**
 * Move to a specified room and print its description.
 * @param target Pointer to the room to move to.
 */
void State::goTo(Room *target) {
    this->currentRoom = target;     // Update the current room to the target room
    this->announceLoc();            // Print the description of the new current room
}

/**
 * Return a pointer to the current room.
 * @return Pointer to the current room.
 */
Room* State::getCurrentRoom() const {
    return this->currentRoom;        // Return the pointer to the current room
}

/**
 * Setter method to update the current room.
 * @param newCurrentRoom Pointer to the new current room.
 */
void State::setCurrentRoom(Room* newCurrentRoom) {
    if (newCurrentRoom != nullptr) {
        this->currentRoom = newCurrentRoom; // Update the current room to the new room
    } else {
        // Handle the case when newCurrentRoom is nullptr
        std::cerr << "Error: Cannot set current room to nullptr." << std::endl;
        // set the current room to the starting room.
        if (!Room::rooms.empty()) {
            this->currentRoom = Room::rooms.front();
        } else {
            // If there are no rooms.
            std::cerr << "Error: No rooms available." << std::endl;
        }
    }
}

// Methods to manipulate the player's inventory
/**
 * Add an object to the player's inventory.
 * @param object The GameObject to add.
 */
void State::add_to_inventory(const GameObject& object) {
    inventory.push_back(object);   // Add the object to the end of the inventory list
}

/**
 * Remove an object from the player's inventory.
 * @param object The GameObject to remove.
 */
void State::remove_from_inventory(const GameObject& object) {
    inventory.remove(object);      // Remove the object from the inventory list
}

/**
 * Get a copy of the player's current inventory.
 * @return A copy of the inventory list.
 */
std::list<GameObject> State::get_inventory() const {
    return inventory;              // Return a copy of the inventory list
}

