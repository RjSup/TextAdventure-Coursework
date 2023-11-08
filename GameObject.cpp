// GameObject.cpp
#include "GameObject.h"

#include <utility>

// Constructor to initialize the object with a short name, long description, and keyword
GameObject::GameObject(std::string  shortName, std::string  longDescription, std::string  keyword)
        : shortName(std::move(shortName)), longDescription(std::move(longDescription)), keyword(std::move(keyword)) {}

// Getter methods to access the object's properties
std::string GameObject::getShortName() const {
    return shortName;
}

std::string GameObject::getLongDescription() const {
    return longDescription;
}

std::string GameObject::getKeyword() const {
    return keyword;
}