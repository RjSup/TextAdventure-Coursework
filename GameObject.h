#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <string>

class GameObject {
public:
    GameObject(std::string short_name, std::string long_description, std::string keyword);
    GameObject(); // Default constructor

    std::string get_short_name() const;
    std::string get_long_description() const;
    std::string get_keyword() const;

    bool operator==(const GameObject& other) const;

    friend std::ostream& operator<<(std::ostream& out, const GameObject& object);
    friend std::istream& operator>>(std::istream& in, GameObject& object);

    void save(std::ostream& outFile) const;
    void load(std::istream& inFile);

private:
    std::string short_name;
    std::string long_description;
    std::string keyword;
};

#endif // GAMEOBJECT_H