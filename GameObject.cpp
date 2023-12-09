#include "GameObject.h"

// Constructor with parameters
GameObject::GameObject(std::string short_name, std::string long_description, std::string keyword)
        : short_name(std::move(short_name)), long_description(std::move(long_description)), keyword(std::move(keyword)) {}

// Default constructor
GameObject::GameObject() {
    this->short_name = "";
    this->long_description = "";
    this->keyword = "";
}

// Getter method to retrieve the short name of the game object
std::string GameObject::get_short_name() const {
    return short_name;
}

// Getter method to retrieve the long description of the game object
std::string GameObject::get_long_description() const {
    return long_description;
}

// Getter method to retrieve the keyword of the game object
std::string GameObject::get_keyword() const {
    return keyword;
}

// Overloaded equality operator for comparing game objects
bool GameObject::operator==(const GameObject& other) const {
    return (this->get_short_name() == other.get_short_name()) &&
           (this->get_long_description() == other.get_long_description()) &&
           (this->get_keyword() == other.get_keyword());
}

// Friend function to overload the output stream operator for printing game objects
std::ostream& operator<<(std::ostream& out, const GameObject& object) {
    out << object.get_short_name() << "\n";
    out << object.get_long_description() << "\n";
    out << object.get_keyword() << "\n";
    return out;
}

// Friend function to overload the input stream operator for reading game objects
std::istream& operator>>(std::istream& in, GameObject& object) {
    std::string short_name, long_description, keyword;
    in >> short_name;
    in >> long_description;
    in >> keyword;

    object.short_name = short_name;
    object.long_description = long_description;
    object.keyword = keyword;

    return in;
}

// Method to save the game object to an output stream
void GameObject::save(std::ostream& outFile) const {
    int shortNameSize = short_name.size();
    int longDescriptionSize = long_description.size();
    int keywordSize = keyword.size();

    // Write the size of the short name and the short name itself to the output stream
    outFile.write(reinterpret_cast<const char*>(&shortNameSize), sizeof(shortNameSize));
    outFile.write(short_name.c_str(), shortNameSize);

    // Write the size of the long description and the long description itself to the output stream
    outFile.write(reinterpret_cast<const char*>(&longDescriptionSize), sizeof(longDescriptionSize));
    outFile.write(long_description.c_str(), longDescriptionSize);

    // Write the size of the keyword and the keyword itself to the output stream
    outFile.write(reinterpret_cast<const char*>(&keywordSize), sizeof(keywordSize));
    outFile.write(keyword.c_str(), keywordSize);
}

// Method to load a game object from an input stream
void GameObject::load(std::istream& inFile) {
    int shortNameSize, longDescriptionSize, keywordSize;

    // Read the size of the short name from the input stream
    inFile.read(reinterpret_cast<char*>(&shortNameSize), sizeof(shortNameSize));

    // Resize the short name string and read the short name from the input stream
    short_name.resize(shortNameSize);
    inFile.read(&short_name[0], shortNameSize);

    // Read the size of the long description from the input stream
    inFile.read(reinterpret_cast<char*>(&longDescriptionSize), sizeof(longDescriptionSize));

    // Resize the long description string and read the long description from the input stream
    long_description.resize(longDescriptionSize);
    inFile.read(&long_description[0], longDescriptionSize);

    // Read the size of the keyword from the input stream
    inFile.read(reinterpret_cast<char*>(&keywordSize), sizeof(keywordSize));

    // Resize the keyword string and read the keyword from the input stream
    keyword.resize(keywordSize);
    inFile.read(&keyword[0], keywordSize);
}
