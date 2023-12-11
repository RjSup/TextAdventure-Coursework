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
    size_t shortNameSize = short_name.size();
    size_t longDescriptionSize = long_description.size();
    size_t keywordSize = keyword.size();

    // Check for potential overflow before conversion
    if (shortNameSize > static_cast<size_t>(std::numeric_limits<int>::max()) ||
        longDescriptionSize > static_cast<size_t>(std::numeric_limits<int>::max()) ||
        keywordSize > static_cast<size_t>(std::numeric_limits<int>::max())) {
        // Handle error: String size too large to fit into an int
        return;
    }

    // Convert to int after ensuring it won't overflow
    int intShortNameSize = static_cast<int>(shortNameSize);
    int intLongDescriptionSize = static_cast<int>(longDescriptionSize);
    int intKeywordSize = static_cast<int>(keywordSize);

    // Write the size of the short name and the short name itself to the output stream
    outFile.write(reinterpret_cast<const char*>(&intShortNameSize), sizeof(intShortNameSize));
    outFile.write(short_name.c_str(), intShortNameSize);

    // Write the size of the long description and the long description itself to the output stream
    outFile.write(reinterpret_cast<const char*>(&intLongDescriptionSize), sizeof(intLongDescriptionSize));
    outFile.write(long_description.c_str(), intLongDescriptionSize);

    // Write the size of the keyword and the keyword itself to the output stream
    outFile.write(reinterpret_cast<const char*>(&intKeywordSize), sizeof(intKeywordSize));
    outFile.write(keyword.c_str(), intKeywordSize);
}

// Method to load a game object from an input stream
void GameObject::load(std::istream& inFile) {
    size_t shortNameSize, longDescriptionSize, keywordSize;

    // Read the size of the short name from the input stream
    inFile.read(reinterpret_cast<char*>(&shortNameSize), sizeof(shortNameSize));

    // Check for potential overflow before conversion
    if (shortNameSize > static_cast<size_t>(std::numeric_limits<int>::max())) {
        // Handle error: String size too large to fit into an int
        return;
    }

    // Convert to int after ensuring it won't overflow
    int intShortNameSize = static_cast<int>(shortNameSize);

    // Resize the short name string and read the short name from the input stream
    short_name.resize(static_cast<std::string::size_type>(intShortNameSize));
    inFile.read(&short_name[0], intShortNameSize);

    // Read the size of the long description from the input stream
    inFile.read(reinterpret_cast<char*>(&longDescriptionSize), sizeof(longDescriptionSize));

    // Check for potential overflow before conversion
    if (longDescriptionSize > static_cast<size_t>(std::numeric_limits<int>::max())) {
        // Handle error: String size too large to fit into an int
        return;
    }

    // Convert to int after ensuring it won't overflow
    int intLongDescriptionSize = static_cast<int>(longDescriptionSize);

    // Resize the long description string and read the long description from the input stream
    long_description.resize(static_cast<std::string::size_type>(intLongDescriptionSize));
    inFile.read(&long_description[0], intLongDescriptionSize);

    // Read the size of the keyword from the input stream
    inFile.read(reinterpret_cast<char*>(&keywordSize), sizeof(keywordSize));

    // Check for potential overflow before conversion
    if (keywordSize > static_cast<size_t>(std::numeric_limits<int>::max())) {
        // Handle error: String size too large to fit into an int
        return;
    }

    // Convert to int after ensuring it won't overflow
    int intKeywordSize = static_cast<int>(keywordSize);

    // Resize the keyword string and read the keyword from the input stream
    keyword.resize(static_cast<std::string::size_type>(intKeywordSize));
    inFile.read(&keyword[0], intKeywordSize);
}
