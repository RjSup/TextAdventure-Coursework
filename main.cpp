#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"

using std::string;
using std::unique_ptr;
using std::vector;

string commandBuffer;
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
    auto* r5 = new Room(&r5name, &r5desc);
    auto* r4 = new Room(&r4name, &r4desc);
    auto* r3 = new Room(&r3name, &r3desc);
    auto* r2 = new Room(&r2name, &r2desc);
    auto* r1 = new Room(&r1name, &r1desc);
    Room::addRoom(r1);
    Room::addRoom(r2);
    Room::addRoom(r3);
    Room::addRoom(r4);
    Room::addRoom(r5);
    r1->setNorth(r2);
    r2->setSouth(r1);
    r2->setEast(r3);
    r3->setWest(r2);
    r3->setSouth(r4);
    r4->setNorth(r3);
    r4->setWest(r5);
    r5->setEast(r4);

    // create game objects
    GameObject object1("Laptop", "An old dusty laptop", "laptop");
    GameObject object2("Drink", "It's Dr Pepper, your favourite", "drink");
    GameObject object3("Shoe", "Some cool shoes", "shoe");
    GameObject object4("snus", "An unopened pack of snus, try one!", "snus");

    // add game object to rooms
    r1->addGameObject(object1);
    r2->addGameObject(object2);
    r2->addGameObject(object3);
    r3->addGameObject(object4);
}

/**
 * Sets up the game state.
 */
void initState() {
    currentState = new State(Room::rooms.front());
}

/**
 * The main game loop.
 */
void gameLoop() {
    //an array of valid commands
    std::string commands[] = {
            "north", "east", "south", "west", "quit", "get", "drop", "inventory", "examine"
    };

    // array of functions corresponding to each command
    void (*actions[])() = {
            [] { currentState->goTo(currentState->getCurrentRoom()->getNorth()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getEast()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getSouth()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getWest()); },
    };

    bool gameOver = false;
    while (!gameOver) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words;
        std::istringstream iss(input);
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                  std::back_inserter(words));

        if (!words.empty()) {
            std::string firstWord = words[0];

            // Check the "get" command
            if (firstWord == "get") {
                if (words.size() > 1) {
                    std::string secondWord = words[1];

                    // Search for the object in the current room
                    const std::list<GameObject> &currentRoomObjects = currentState->getCurrentRoom()->getGameObjects();
                    auto objectInRoom = std::find_if(currentRoomObjects.begin(), currentRoomObjects.end(),
                                                     [secondWord](const GameObject &object) {
                                                         return object.getKeyword() == secondWord;
                                                     });

                    if (objectInRoom != currentRoomObjects.end()) {
                        // Object found in the room, move it to the inventory
                        currentState->addToInventory(*objectInRoom);
                        currentState->getCurrentRoom()->removeGameObject(*objectInRoom);

                        std::cout << "You picked up " << objectInRoom->getShortName() << ".\n";
                    } else {
                        std::cout << "The object does not exist in the room.\n";
                    }
                } else {
                    std::cout << "Please specify the object you want to get.\n";
                }
            } else if (firstWord == "drop") {
                if (words.size() > 1) {
                    std::string secondWord = words[1];

                    // Search for the object in the player's inventory
                    const std::list<GameObject> &inventory = currentState->getInventory();
                    auto objectInInventory = std::find_if(inventory.begin(), inventory.end(),
                                                          [secondWord](const GameObject &object) {
                                                              return object.getKeyword() == secondWord;
                                                          });

                    if (objectInInventory != inventory.end()) {
                        // Object found in the inventory, move it to the current room
                        currentState->getCurrentRoom()->addGameObject(*objectInInventory);
                        currentState->removeFromInventory(*objectInInventory);

                        std::cout << "You dropped " << objectInInventory->getShortName() << " in the room.\n";
                    } else {
                        std::cout << "The object does not exist in your inventory.\n";
                    }
                } else {
                    std::cout << "Please specify the object you want to drop.\n";
                }
            } else if (firstWord == "inventory") {
                // Print out the short names of all objects in the inventory
                const std::list<GameObject> &inventory = currentState->getInventory();
                if (!inventory.empty()) {
                    std::cout << "Inventory:\n";
                    for (const GameObject &object: inventory) {
                        std::cout << object.getShortName() << "\n";
                    }
                } else {
                    std::cout << "Your inventory is empty.\n";
                }
            } else if (firstWord == "examine") {
                if (words.size() > 1) {
                    std::string secondWord = words[1];

                    // Search for the object in the player's inventory
                    const std::list<GameObject> &inventory = currentState->getInventory();
                    auto objectInInventory = std::find_if(inventory.begin(), inventory.end(),
                                                          [secondWord](const GameObject &object) {
                                                              return object.getKeyword() == secondWord;
                                                          });

                    // Search for the object in the current room if not found in the inventory
                    if (objectInInventory == inventory.end()) {
                        const std::list<GameObject> &currentRoomObjects = currentState->getCurrentRoom()->getGameObjects();
                        objectInInventory = std::find_if(currentRoomObjects.begin(), currentRoomObjects.end(),
                                                         [secondWord](const GameObject &object) {
                                                             return object.getKeyword() == secondWord;
                                                         });
                    }

                    if (objectInInventory != inventory.end()) {
                        std::cout << objectInInventory->getLongDescription() << "\n";
                    } else {
                        std::cout << "The object does not exist in your inventory or the current room.\n";
                    }
                } else {
                    std::cout << "Please specify the object you want to examine.\n";
                }
            } else {
                bool commandFound = false;
                for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
                    if (commands[i] == firstWord) {
                        commandFound = true;
                        actions[i]();
                        break;
                    }
                }

                // If the command hasn't been set, the command wasn't understood
                if (!commandFound) {
                    wrapOut(&badCommand);
                }
            }
        }
    }
}

int main() {
    initWordWrap();
    initRooms();
    initState();
    currentState->announceLoc();
    gameLoop();
    return 0;
}
