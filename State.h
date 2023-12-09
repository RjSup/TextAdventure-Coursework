#ifndef TEXTADV_STATE_H
#define TEXTADV_STATE_H

#include "Room.h"
#include <list>

// Declaration of the State class
class State {
    Room *currentRoom;       // Pointer to the current Room object the player is in
    std::list<GameObject> inventory;  // List of GameObjects the player is carrying

public:
    // Constructor takes a starting room and a list of GameObjects for the initial inventory
    explicit State(Room *startRoom, std::list<GameObject> list);

    // Method to move the player to a new room
    void goTo(Room *target);

    // Method to print the current location to the console
    void announceLoc() const;

    // Getter method to retrieve the current room
    Room* getCurrentRoom() const;

    // Methods to manipulate the player's inventory
    void add_to_inventory(const GameObject& object);      // Add an object to the inventory
    void remove_from_inventory(const GameObject& object); // Remove an object from the inventory
    const std::list<GameObject> get_inventory() const;     // Get the current inventory
    void clear_inventory();                                // Clear the entire inventory

    // Setter method to update the current room
    void setCurrentRoom(Room *newCurrentRoom);
};

// End of the include guard
#endif // TEXTADV_STATE_H