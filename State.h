//
// Created by DrMark on 10/2/2017.
//

#ifndef TEXTADV_STATE_H
#define TEXTADV_STATE_H

#include "Room.h"
#include <list> // Include for managing the inventory of GameObjects

class State {
    Room *currentRoom;
    std::list<GameObject> inventory; // List of GameObjects the player is carrying

public:
    explicit State(Room *startRoom);
    void goTo(Room *target);
    void announceLoc() const;
    Room* getCurrentRoom() const;

    // Methods to manipulate the player's inventory
    void addToInventory(const GameObject& object);
    void removeFromInventory(const GameObject& object);
    const std::list<GameObject>& getInventory() const;
};

#endif // TEXTADV_STATE_H