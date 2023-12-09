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
    explicit State(Room *startRoom, std::list<GameObject> list);
    void goTo(Room *target);
    void announceLoc() const;
    Room* getCurrentRoom() const;

    // Methods to manipulate the player's inventory
    void add_to_inventory(const GameObject& object);
    void remove_from_inventory(const GameObject& object);
    const std::list<GameObject> get_inventory() const;
    void clear_inventory();

    void setCurrentRoom(Room *newCurrentRoom);
};

#endif // TEXTADV_STATE_H