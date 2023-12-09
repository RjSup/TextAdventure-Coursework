#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"

using std::string;
using std::unique_ptr;
using std::vector;

// Pointer to the current game state
State* currentState;

/**
 * Print out the command prompt then read a command into the provided string buffer.
 * @param buffer Pointer to the string buffer to use.
 */
void inputCommand(string* buffer) {
    // Clear the buffer
    buffer->clear();
    // Print the command prompt
    std::cout << "> ";
    // Read a line from standard input into the buffer
    std::getline(std::cin, *buffer);
}

// Function to save the game state to a binary file
void save_game(const State& gameState, const std::string& filename) {
    // Open the file for writing in binary mode
    std::ofstream outFile(filename, std::ios::binary);

    if (outFile.is_open()) {
        // Save the current room
        Room* currentRoom = gameState.getCurrentRoom();
        outFile.write(reinterpret_cast<char*>(&currentRoom), sizeof(currentRoom));

        // Save the inventory size
        int inventorySize = gameState.get_inventory().size();
        outFile.write(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));

        // Save each object in the inventory
        for (const GameObject& object : gameState.get_inventory()) {
            object.save(outFile);
        }

        // Save the dropped objects in each room
        for (const auto& room : Room::rooms) {
            // Save the room pointer
            outFile.write(reinterpret_cast<char*>(room), sizeof(room));

            int droppedObjectsSize = room->getDroppedObjects().size();
            outFile.write(reinterpret_cast<char*>(&droppedObjectsSize), sizeof(droppedObjectsSize));

            for (const GameObject& droppedObject : room->getDroppedObjects()) {
                droppedObject.save(outFile);
            }
        }

        std::cout << "Game saved successfully to " << filename << ".\n";
    } else {
        std::cout << "Unable to open file for saving.\n";
    }
}

// Function to load the game state from a binary file
void load_game(State& gameState, const std::string& filename) {
    // Open the file for reading in binary mode
    std::ifstream inFile(filename, std::ios::binary);

    if (inFile.is_open()) {
        // Load the current room pointer
        Room* currentRoom;
        inFile.read(reinterpret_cast<char*>(&currentRoom), sizeof(currentRoom));
        gameState.setCurrentRoom(currentRoom);

        // Load the inventory size
        int inventorySize;
        inFile.read(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));

        // Load each object in the inventory
        for (int i = 0; i < inventorySize; ++i) {
            GameObject object;
            object.load(inFile);
            gameState.add_to_inventory(object);
        }

        // Load dropped objects in each room
        while (inFile.peek() != EOF) {
            // Load the room pointer
            Room* room;
            inFile.read(reinterpret_cast<char*>(&room), sizeof(room));

            int droppedObjectsSize;
            inFile.read(reinterpret_cast<char*>(&droppedObjectsSize), sizeof(droppedObjectsSize));

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
    GameObject object1("Laptop", "An old dusty laptop", "laptop");
    GameObject object2("Drink", "It's Dr Pepper, your favorite", "drink");
    GameObject object3("Shoe", "Some cool shoes", "shoe");
    GameObject object4("snus", "An unopened pack of snus, try one!", "snus");

    // Add game objects to rooms
    r1->add_game_object(object1);
    r2->add_game_object(object2);
    r2->add_game_object(object3);
    r3->add_game_object(object4);
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
            "north", "east", "south", "west", "save", "load", "quit"
    };

    // Array of functions corresponding to each command
    void (*actions[])() = {
            [] { currentState->goTo(currentState->getCurrentRoom()->getNorth()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getEast()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getSouth()); },
            [] { currentState->goTo(currentState->getCurrentRoom()->getWest()); },
            [] {
                std::string file_name;
                std::cout << "Enter File Name To Save: \n";
                std::cin >> file_name;
                save_game(*currentState, file_name);
            },
            [] {
                std::string file_name;
                std::cout << "Enter File Name To Load: \n";
                std::cin >> file_name;
                load_game(*currentState, file_name);
            }
    };

    bool gameOver = false;
    while (!gameOver) {
        // Display command prompt
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        // Tokenize the input into words
        std::vector<std::string> words;
        std::istringstream iss(input);
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
        std::back_inserter(words));

        if (!words.empty()) {
            std::string firstWord = words[0];

            // Check for specific commands
            if (firstWord == "get") {
                if (words.size() > 1) {
                    std::string secondWord = words[1];

                    // Search for the object in the current room
                    const std::list<GameObject>& currentRoomObjects = currentState->getCurrentRoom()->getGameObjects();
                    auto objectInRoom = std::find_if(currentRoomObjects.begin(), currentRoomObjects.end(),
                                                     [secondWord](const GameObject& object) {
                                                         return object.get_keyword() == secondWord;
                                                     });

                    if (objectInRoom != currentRoomObjects.end()) {
                        // Object found in the room, move it to the inventory
                        currentState->add_to_inventory(*objectInRoom);
                        currentState->getCurrentRoom()->remove_game_object(*objectInRoom);

                        std::cout << "You picked up" << objectInRoom->get_short_name() << ".\n";
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
                    const std::list<GameObject>& inventory = currentState->get_inventory();
                    auto objectInInventory = std::find_if(inventory.begin(), inventory.end(),
                                                          [secondWord](const GameObject& object) {
                                                              return object.get_keyword() == secondWord;
                                                          });

                    if (objectInInventory != inventory.end()) {
                        // Object found in the inventory, move it to the current room
                        currentState->getCurrentRoom()->add_game_object(*objectInInventory);
                        currentState->remove_from_inventory(*objectInInventory);

                        std::cout << "You dropped " << objectInInventory->get_short_name() << " in the room.\n";
                    } else {
                        std::cout << "The object does not exist in your inventory.\n";
                    }
                } else {
                    std::cout << "Please specify the object you want to drop.\n";
                }
            } else if (firstWord == "inventory") {
                // Print out the short names of all objects in the inventory
                const std::list<GameObject>& inventory = currentState->get_inventory();
                if (!inventory.empty()) {
                    std::cout << "Inventory:\n";
                    for (const GameObject& object : inventory) {
                        std::cout << object.get_short_name() << "\n";
                    }
                } else {
                    std::cout << "Your inventory is empty.\n";
                }
            } else if (firstWord == "examine") {
                if (words.size() > 1) {
                    std::string secondWord = words[1];

                    // Search for the object in the player's inventory
                    const std::list<GameObject>& inventory = currentState->get_inventory();
                    auto objectInInventory = std::find_if(inventory.begin(), inventory.end(),
                                                          [secondWord](const GameObject& object) {
                                                              return object.get_keyword() == secondWord;
                                                          });

                    // Search for the object in the current room if not found in the inventory
                    if (objectInInventory == inventory.end()) {
                        const std::list<GameObject>& currentRoomObjects = currentState->getCurrentRoom()->getGameObjects();
                        objectInInventory = std::find_if(currentRoomObjects.begin(), currentRoomObjects.end(),
                                                         [secondWord](const GameObject& object) {
                                                             return object.get_keyword() == secondWord;
                                                         });
                    }

                    if (objectInInventory != inventory.end()) {
                        std::cout << objectInInventory->get_long_description() << "\n";
                    } else {
                        std::cout << "The object does not exist in your inventory or the current room.\n";
                    }
                } else {
                    std::cout << "Please specify the object you want to examine.\n";
                }
            } else {
                // Check if the command is a valid game command
                bool commandFound = false;
                for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
                    if (commands[i] == firstWord) {
                        // Execute the corresponding action function
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

// Main function
int main() {
    // Initialize word wrapping
    initWordWrap();
    // Set up rooms and objects
    initRooms();
    // Set up initial game state
    initState();
    // Announce the initial location
    currentState->announceLoc();
    // Enter the main game loop
    gameLoop();
    return 0;
}
