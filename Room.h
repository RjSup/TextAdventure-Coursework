#ifndef TEXTADV_ROOM_H

#define TEXTADV_ROOM_H


#include <string>

#include <forward_list>

#include <list>
#include <iostream>

#include "GameObject.h"


using std::string;

/**
 * Represents a room (accessible location in the game).
 */

class Room {


    /**
     * Short name used as a header.
     */


    const string* name;
    /**
     * Full description of the room.
     */


    const string* description;
    /**
     * Pointer to room that is north of this one.
     */
    Room* north;
    Room* south;
    Room* east;
    Room* west;
    std::list<GameObject> gameObjects;  // List of GameObjects in the room
    std::list<GameObject> droppedObjects; // List of dropped GameObjects
    int room_index;
public:
    Room(const string *_name, const string *_desc);

    /**
     * Removes a destroyed room from the global list if it's there.
     */
    ~Room();

    /**
     * Outputs the name and description of the room
     * in standard format.
     */


    void describe() const;

    /**
     * List storing all rooms that have been registered via addRoom().
     */


    static std::list<Room*> rooms;


    /**
     * Creates a new Room with the given parameters and register it with the static list.
     * @param _name Name of the room.
     * @param _desc Description of the room.
     */
    static Room* addRoom(const string *_name, string* _desc);
    static void addRoom(Room* room);

    // getter and setters for room exits
    Room* getNorth() const;
    void setNorth(Room* _north);

    Room* getEast() const;
    void setEast(Room* _east);

    Room* getSouth() const;
    void setSouth(Room* _south);

    Room* getWest() const;
    void setWest(Room* _west);


    // Methods to manipulate the list of GameObjects
    void add_game_object(const GameObject& object);
    void remove_game_object(const GameObject& object);
    const std::list<GameObject>& getGameObjects() const;

    void clear_game_objects();

    void setGameObjects(std::list<GameObject> &object);

    // Methods for managing dropped objects
    void addDroppedObject(const GameObject& object);
    const std::list<GameObject>& getDroppedObjects() const;

    // Method to set the index of the room
    int setRoomIndex(int newIndex);

};

#endif //TEXTADV_ROOM_H