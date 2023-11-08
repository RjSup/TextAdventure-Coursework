#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>

class GameObject {
public:
    // Constructor to initialize the object with a short name, long description, and keyword
    GameObject(std::string shortName, std::string longDescription, std::string keyword);

    // Getter methods to access the object's properties
    std::string getShortName() const;
    std::string getLongDescription() const;
    std::string getKeyword() const;

    // Overload the equality operator for GameObject
    bool operator==(const GameObject& other) const {
        return (this->getShortName() == other.getShortName()) &&
               (this->getLongDescription() == other.getLongDescription()) &&
               (this->getKeyword() == other.getKeyword());
    }

private:
    std::string shortName;
    std::string longDescription;
    std::string keyword;
};

#endif // GAMEOBJECT_H