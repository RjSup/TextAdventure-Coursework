#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"

using std::string;
using std::unique_ptr;
using std::vector;

// Forward declarations
void inputCommand(string* buffer);
void save_game(const State& gameState, const std::string& filename);
void load_game(State& gameState, const std::string& filename);
void initRooms();
void initState();
void gameLoop();

State* currentState;

/**
 * Print out the command prompt then read a command into the provided string buffer.
 * @param buffer Pointer to the string buffer to use.
 */
void inputCommand(string* buffer) {
    buffer->clear();
    std::cout << "> ";
    std::getline(std::cin, *buffer);
}

/**
 * Sets up the map.
 */
void initRooms() {
    // Create instances of Room
    auto* r5 = new Room(&r5_name, &r5_desc);
    auto* r4 = new Room(&r4_name, &r4_desc);
    auto* r3 = new Room(&r3_name, &r3_desc);
    auto* r2 = new Room(&r2_name, &r2_desc);
    auto* r1 = new Room(&r1_name, &r1_desc);

    // Add rooms to the list of rooms
    Room::addRoom(r1);
    Room::addRoom(r2);
    Room::addRoom(r3);
    Room::addRoom(r4);
    Room::addRoom(r5);

    // Set up room connections
    r1->setNorth(r2);
    r2->setSouth(r1);
    r2->setEast(r3);
    r3->setWest(r2);
    r3->setSouth(r4);
    r4->setNorth(r3);
    r4->setWest(r5);
    r5->setEast(r4);

    // Create game objects
    GameObject object1("laptop", "A smart looking laptop", "laptop");
    GameObject object2("snus", "SNUS, try one!", "snus");
    GameObject object3("shoe", "some smart shoes", "shoe");
    GameObject object4("game", "A fps game on CD", "game");
    GameObject object5("drink", "A tasty carbonated drink", "drink");

    // Add game objects to rooms
    r1->add_game_object(object1);
    r1->add_game_object(object2);
    r2->add_game_object(object3);
    r3->add_game_object(object5);
    r4->add_game_object(object4);
}

/**
 * Sets up the game state.
 */
void initState() {
    // Create a new game state with the starting room and an empty inventory
    currentState = new State(Room::rooms.front(), std::list<GameObject>());
}

/**
 * The main game loop.
 */
void gameLoop() {
    // An array of valid commands
    std::string commands[] = {
            "north", "east", "south", "west", "get", "drop", "inventory", "examine", "save", "load", "quit"
    };

    void (*actions[])() = {
            [] { currentState->goTo(currentState->getCurrentRoom()->getNorth()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getEast()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getSouth()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getWest()); },
            [] {
                string objectKeyword;
                std::cout << "Enter the object to pick up: ";
                inputCommand(&objectKeyword);

                auto& currentRoomObjects = currentState->getCurrentRoom()->getGameObjects();
                auto objectInRoom = std::find_if(currentRoomObjects.begin(), currentRoomObjects.end(),
                                                 [objectKeyword](const GameObject& object) {
                                                     return object.get_keyword() == objectKeyword;
                                                 });

                if (objectInRoom != currentRoomObjects.end()) {
                    currentState->add_to_inventory(*objectInRoom);
                    currentState->getCurrentRoom()->remove_game_object(*objectInRoom);
                    std::cout << "Picked up " << objectInRoom->get_short_name() << ".\n";
                } else {
                    std::cout << "The object does not exist in the room.\n";
                }
            },
            [] {
                string objectKeyword;
                std::cout << "Enter the object to drop: ";
                inputCommand(&objectKeyword);

                std::list<GameObject> inventory = currentState->get_inventory();
                auto objectInInventory = std::find_if(inventory.begin(), inventory.end(),
                                                      [objectKeyword](const GameObject& object) {
                                                          return object.get_keyword() == objectKeyword;
                                                      });

                if (objectInInventory != inventory.end()) {
                    currentState->getCurrentRoom()->add_game_object(*objectInInventory);
                    currentState->remove_from_inventory(*objectInInventory);
                    std::cout << "You dropped " << objectInInventory->get_short_name() << " in the room.\n";
                } else {
                    std::cout << "The object does not exist in your inventory.\n";
                }
            },
            [] {
                const auto& inventory = currentState->get_inventory();
                if (!inventory.empty()) {
                    std::cout << "Inventory:\n";
                    for (const GameObject& object : inventory) {
                        std::cout << object.get_short_name() << "\n";
                    }
                } else {
                    std::cout << "Your inventory is empty.\n";
                }
            },
            [] {
                string objectKeyword;
                std::cout << "Enter the object to examine: ";
                inputCommand(&objectKeyword);

                std::list<GameObject> inventory = currentState->get_inventory();
                auto objectInInventory = std::find_if(inventory.begin(), inventory.end(),
                                                      [objectKeyword](const GameObject& object) {
                                                          return object.get_keyword() == objectKeyword;
                                                      });

                if (objectInInventory != inventory.end()) {
                    std::cout << objectInInventory->get_long_description() << "\n";
                } else {
                    auto& currentRoomObjects = currentState->getCurrentRoom()->getGameObjects();
                    auto objectInInventory = std::find_if(
                            currentRoomObjects.begin(), currentRoomObjects.end(),
                            [objectKeyword](const GameObject &object) {
                                return object.get_keyword() == objectKeyword;
                            });
                    if (objectInInventory != currentRoomObjects.end()) {
                        std::cout << objectInInventory->get_long_description() << "\n";
                    } else {
                        std::cout << "The object is not in your inventory or the current room.\n";
                    }
                }
            },
            [] {
                string file_name;
                std::cout << "Enter File Name To Save: \n";
                inputCommand(&file_name);
                save_game(*currentState, file_name);
            },
            [] {
                string file_name;
                std::cout << "Enter File Name To Load: \n";
                inputCommand(&file_name);
                load_game(*currentState, file_name);
            },
            [] { exit(0); }
    };

    bool gameOver = false;
    while (!gameOver) {
        std::string input;
        inputCommand(&input);

        std::vector<std::string> words;
        std::istringstream iss(input);
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                  std::back_inserter(words));

        if (!words.empty()) {
            std::string firstWord = words[0];

            // Checking if the command is valid
            auto commandIterator = std::find(std::begin(commands), std::end(commands), firstWord);
            if (commandIterator != std::end(commands)) {
                actions[std::distance(std::begin(commands), commandIterator)]();
            } else {
                wrapOut(&badCommand);
            }
        }
    }
}

// Main function
int main() {
    // Initializing everything
    initWordWrap();
    initRooms();
    initState();
    currentState->announceLoc();
    gameLoop();
    return 0;
}

void save_game(const State& gameState, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile.is_open()) {
        Room* currentRoom = gameState.getCurrentRoom();
        outFile.write(reinterpret_cast<char*>(&currentRoom), sizeof(currentRoom));

        int inventorySize = gameState.get_inventory().size();
        outFile.write(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));

        for (const GameObject& object : gameState.get_inventory()) {
            object.save(outFile);
        }

        for (const auto& room : Room::rooms) {
            outFile.write(reinterpret_cast<char*>(room), sizeof(room));

            int droppedObjectsSize = room->getDroppedObjects().size();
            outFile.write(reinterpret_cast<char*>(&droppedObjectsSize), sizeof(droppedObjectsSize));

            for (const GameObject& droppedObject : room->getDroppedObjects()) {
                droppedObject.save(outFile);
            }
        }

        std::cout << "Game saved as " << filename << ".\n";
    } else {
        std::cout << "Unable to open the file for saving.\n";
    }
}

void load_game(State& gameState, const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (inFile.is_open()) {
        Room* currentRoom;
        inFile.read(reinterpret_cast<char*>(&currentRoom), sizeof(currentRoom));
        gameState.setCurrentRoom(currentRoom);

        int inventorySize;
        inFile.read(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));

        for (int i = 0; i < inventorySize; ++i) {
            GameObject object;
            object.load(inFile);
            gameState.add_to_inventory(object);
        }

        for (auto& room : Room::rooms) {
            int droppedObjectsSize;
            inFile.read(reinterpret_cast<char*>(&droppedObjectsSize), sizeof(droppedObjectsSize));

            room->clearDroppedObjects();  // Clear existing dropped objects

            for (int i = 0; i < droppedObjectsSize; ++i) {
                GameObject droppedObject;
                droppedObject.load(inFile);
                room->addDroppedObject(droppedObject);
            }
        }

        std::cout << "Game loaded successfully from " << filename << ".\n";
        gameState.announceLoc();
    } else {
        std::cout << "Unable to open file for loading.\n";
    }
}
