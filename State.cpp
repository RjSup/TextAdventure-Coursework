//
// Created by DrMark on 10/2/2017.
//

#include <fstream>
#include <iostream>
#include "State.h"

/**
 * Current state of the game.
 */

/**
 * Display the description of the room the player is in. */

void State::announceLoc() const {
    this->currentRoom->describe();
}

/**
 * Constructor.
 * @param startRoom Pointer to the room to start in.
 */
State::State(Room *startRoom, std::list<GameObject> list) : currentRoom(startRoom) {};

/**
 * Move to a specified room and print its description.
 * @param target Pointer to the room to move to.
 */
void State::goTo(Room *target) {
    this->currentRoom = target;
    this->announceLoc();
}

/**
 * Return a pointer to the current room.
 * @return Pointer to the current room.
 */
Room* State::getCurrentRoom() const {
    return this->currentRoom;
}

void State::setCurrentRoom(Room* newCurrentRoom) {
    this->currentRoom = newCurrentRoom;
}
// Methods to manipulate the player's inventory

void State::add_to_inventory(const GameObject& object) {
    inventory.push_back(object);
}

void State::remove_from_inventory(const GameObject& object) {
    inventory.remove(object);
}

const std::list<GameObject> State::get_inventory() const {
    return inventory;
}

void State::clear_inventory() {
    inventory.clear();
}




