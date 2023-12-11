#include <iostream>
#include <vector>

class GameObject {
private:
    std::string short_name;
    std::string long_description;
    std::string keyword;

public:
    // Constructors, getters, and operator overloads...

    // Method to save the game object to an output stream
    void save(std::ostream& outFile) const {
        // Use static_cast to address narrowing conversion warnings
        int shortNameSize = static_cast<int>(short_name.size());
        int longDescriptionSize = static_cast<int>(long_description.size());
        int keywordSize = static_cast<int>(keyword.size());

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
    void load(std::istream& inFile) {
        int shortNameSize, longDescriptionSize, keywordSize;

        // Read the size of the short name from the input stream
        inFile.read(reinterpret_cast<char*>(&shortNameSize), sizeof(shortNameSize));

        // Read the short name from the input stream
        std::vector<char> shortNameBuffer(shortNameSize);
        inFile.read(shortNameBuffer.data(), shortNameSize);
        short_name.assign(shortNameBuffer.data(), shortNameSize);

        // Read the size of the long description from the input stream
        inFile.read(reinterpret_cast<char*>(&longDescriptionSize), sizeof(longDescriptionSize));

        // Read the long description from the input stream
        std::vector<char> longDescriptionBuffer(longDescriptionSize);
        inFile.read(longDescriptionBuffer.data(), longDescriptionSize);
        long_description.assign(longDescriptionBuffer.data(), longDescriptionSize);

        // Read the size of the keyword from the input stream
        inFile.read(reinterpret_cast<char*>(&keywordSize), sizeof(keywordSize));

        // Read the keyword from the input stream
        std::vector<char> keywordBuffer(keywordSize);
        inFile.read(keywordBuffer.data(), keywordSize);
        keyword.assign(keywordBuffer.data(), keywordSize);
    }
};
