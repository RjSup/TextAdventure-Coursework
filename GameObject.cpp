#include "GameObject.h"

GameObject::GameObject(std::string short_name, std::string long_description, std::string keyword)
        : short_name(std::move(short_name)), long_description(std::move(long_description)), keyword(std::move(keyword)) {}

GameObject::GameObject() {
    this->short_name = "";
    this->long_description = "";
    this->keyword = "";
}

std::string GameObject::get_short_name() const {
    return short_name;
}

std::string GameObject::get_long_description() const {
    return long_description;
}

std::string GameObject::get_keyword() const {
    return keyword;
}

bool GameObject::operator==(const GameObject& other) const {
    return (this->get_short_name() == other.get_short_name()) &&
           (this->get_long_description() == other.get_long_description()) &&
           (this->get_keyword() == other.get_keyword());
}

std::ostream& operator<<(std::ostream& out, const GameObject& object) {
    out << object.get_short_name();
    out << object.get_long_description();
    out << object.get_keyword();
    return out;
}

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

void GameObject::save(std::ostream& outFile) const {
    int shortNameSize = short_name.size();
    int longDescriptionSize = long_description.size();
    int keywordSize = keyword.size();

    outFile.write(reinterpret_cast<const char*>(&shortNameSize), sizeof(shortNameSize));
    outFile.write(short_name.c_str(), shortNameSize);

    outFile.write(reinterpret_cast<const char*>(&longDescriptionSize), sizeof(longDescriptionSize));
    outFile.write(long_description.c_str(), longDescriptionSize);

    outFile.write(reinterpret_cast<const char*>(&keywordSize), sizeof(keywordSize));
    outFile.write(keyword.c_str(), keywordSize);
}

void GameObject::load(std::istream& inFile) {
    int shortNameSize, longDescriptionSize, keywordSize;
    inFile.read(reinterpret_cast<char*>(&shortNameSize), sizeof(shortNameSize));

    short_name.resize(shortNameSize);
    inFile.read(&short_name[0], shortNameSize);

    inFile.read(reinterpret_cast<char*>(&longDescriptionSize), sizeof(longDescriptionSize));
    long_description.resize(longDescriptionSize);
    inFile.read(&long_description[0], longDescriptionSize);

    inFile.read(reinterpret_cast<char*>(&keywordSize), sizeof(keywordSize));
    keyword.resize(keywordSize);
    inFile.read(&keyword[0], keywordSize);
}