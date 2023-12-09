#include "Room.h"

#include <iostream>
#include "wordwrap.h"

/**
 * Stores a static list of all rooms.
 */
std::list<Room*> Room::rooms;

/**
 * Room default constructor.
 * @param _name Room's name.
 * @param _desc Room's description.
 */
Room::Room(const string *_name, const string *_desc) :
        name(_name), description(_desc), north(nullptr), south(nullptr), east(nullptr), west(nullptr) {};

/**
 * Remove destroyed rooms from the static list.
 */
Room::~Room() {
    Room::rooms.remove(this);
}

/**
 * Prints the description of a room (the name and long description)
 */
void Room::describe() const {
    std::cout << *name << "\n";
    std::cout << *description << "\n";

    // Print the short names of objects in the room
    if (!gameObjects.empty()) {
        std::cout << "Objects in the room: ";
        for (const GameObject& object : gameObjects) {
            std::cout << object.get_short_name() << ", ";
        }
        std::cout << "\n";
    }
}

/**
 * Statically creates a room and then adds it to the global list.
 * @param _name Name for the new room.
 * @param _desc Description for the new room.
 * @param _object List of GameObjects in the room
 * @return A pointer to the newly created room.
 */
Room* Room::addRoom(const string *_name, string *_desc) {
    auto *newRoom = new Room(_name, _desc);
    newRoom->room_index = rooms.size();
    Room::rooms.push_back(newRoom);
    return newRoom;
}

/**
 * Adds an existing room to the static list.
 * @param room Pointer to the room to add.
 */
void Room::addRoom(Room* room) {
    Room::rooms.push_back(room);
}

/**
 * Gets the north exit of a room.
 * @return The room that is north of this one, or NULL if there is no exit.
 */
Room* Room::getNorth() const {
    return this->north;
}

Room* Room::getEast() const {
    return this->east;
}

Room* Room::getSouth() const {
    return this->south;
}

Room* Room::getWest() const {
    return this->west;
}

/**
 * Sets the north exit of a room.
 * @param _north Pointer to the room to be north of this one.
 */
void Room::setNorth(Room* _north) {
    this->north = _north;
}

void Room::setEast(Room* _east) {
    this->east = _east;
}

void Room::setSouth(Room* _south) {
    this->south = _south;
}

void Room::setWest(Room* _west) {
    this->west = _west;
}

/**
 * Adds a game object to the room.
 * @param object The GameObject to add.
 */
void Room::add_game_object(const GameObject& object) {
    gameObjects.push_back(object);
}

/**
 * Removes a game object from the room.
 * @param object The GameObject to remove.
 */
void Room::remove_game_object(const GameObject& object) {
    gameObjects.remove(object);
}

/**
 * Gets the list of game objects in the room.
 * @return A reference to the list of game objects.
 */
const std::list<GameObject>& Room::getGameObjects() const {
    return gameObjects;
}

/**
 * Clears all game objects from the room.
 */
void Room::clear_game_objects() {
    this->gameObjects.clear();
}

/**
 * Sets the game objects in the room.
 * @param object The list of game objects to set.
 */
void Room::setGameObjects(std::list<GameObject> &object) {
    this->gameObjects = object;
}

/**
 * Sets the index of the room.
 * @param newIndex The new index for the room.
 * @return The new room index.
 */
int Room::setRoomIndex(int newIndex) {
    room_index = newIndex;
    return room_index;
}

/**
 * Gets the list of dropped objects in the room.
 * @return A reference to the list of dropped objects.
 */
const std::list<GameObject> &Room::getDroppedObjects() const {
    return droppedObjects;
}

/**
 * Adds a dropped object to the room.
 * @param object The GameObject to add to the list of dropped objects.
 */
void Room::addDroppedObject(const GameObject &object) {
    droppedObjects.push_back(object);
}
