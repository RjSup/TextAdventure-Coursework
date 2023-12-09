#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <string>

/**
 * Represents a game object in the text-based adventure game.
 */
class GameObject {
public:
    // Constructor with parameters
    GameObject(std::string short_name, std::string long_description, std::string keyword);

    // Default constructor
    GameObject();

    // Getter methods to retrieve information about the game object
    std::string get_short_name() const;
    std::string get_long_description() const;
    std::string get_keyword() const;

    // Overloaded equality operator for comparing game objects
    bool operator==(const GameObject& other) const;

    // Friend function to overload the output stream operator for printing game objects
    friend std::ostream& operator<<(std::ostream& out, const GameObject& object);

    // Friend function to overload the input stream operator for reading game objects
    friend std::istream& operator>>(std::istream& in, GameObject& object);

    // Method to save the game object to an output stream
    void save(std::ostream& outFile) const;

    // Method to load a game object from an input stream
    void load(std::istream& inFile);

private:
    // Member variables representing properties of the game object
    std::string short_name;
    std::string long_description;
    std::string keyword;
};

// End of the include guard
#endif // GAMEOBJECT_H
