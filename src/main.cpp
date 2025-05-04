#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "user.hpp"
#include "errors.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

void ClearInput() {
    std::cin.clear();
    #ifdef _WIN32
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    #else
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    #endif
}

int GetIntInput(const std::string& prompt = "") {
    int value;
    while (true) {
        if (!prompt.empty()) std::cout << prompt;
        if (std::cin >> value) break;
        ClearInput();
        std::cout << "Invalid input. Please enter an integer.\n";
    }
    return value;
}

template<typename T>
T GetTypedInput(const std::string& prompt = "Enter value: ") {
    T value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) break;
        ClearInput();
        std::cout << "Invalid input. Please try again.\n";
    }
    return value;
}

template<>
User GetTypedInput<User>(const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    
    User user;
    while (true) {
        try {
            std::cin >> user;
            break;
        } catch (const std::exception& e) {
            std::cout << "Invalid user input: " << e.what() << "\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!prompt.empty()) {
                std::cout << prompt;
            }
        }
    }
    return user;
}

class ISequenceWrapper {
public:
    virtual ~ISequenceWrapper() = default;
    virtual void Display() const = 0;
    virtual void AddToEnd() = 0;
    virtual void AddToFront() = 0;
    virtual void InsertElement() = 0;
    virtual void DeleteElement() = 0;
    virtual void AccessElement() const = 0;
    virtual ISequenceWrapper* CreateSubsequence() const = 0;
    virtual ISequenceWrapper* Combine(const ISequenceWrapper* other) const = 0;
    virtual const std::string& GetTypeName() const = 0;
    virtual const std::string& GetStructureName() const = 0;
};

template<typename T>
class SequenceWrapper : public ISequenceWrapper {
    ISequence<T>* sequence;
    std::string structure_type;
    std::string data_type;

public:
    SequenceWrapper(const std::string& struct_type, const std::string& type_name) 
        : structure_type(struct_type), data_type(type_name) {
        if (structure_type == "array") {
            sequence = new ArraySequence<T>();
        } else {
            sequence = new ListSequence<T>();
        }
    }

    ~SequenceWrapper() override {
        delete sequence;
    }

    void Display() const override {
        std::cout << "[ ";
        for (int i = 0; i < sequence->Size(); ++i) {
            std::cout << sequence->At(i) << " ";
        }
        std::cout << "] (Type: " << data_type << ", Structure: " << structure_type << ")\n";
    }

    void AddToEnd() override {
        T value = GetTypedInput<T>("Enter value to add at end: ");
        sequence->AddToEnd(value);
    }

    void AddToFront() override {
        T value = GetTypedInput<T>("Enter value to add at front: ");
        sequence->AddToFront(value);
    }

    void InsertElement() override {
        int position = GetIntInput("Enter position (0-" + std::to_string(sequence->Size()) + "): ");
        if (position < 0 || position > sequence->Size()) {
            throw Errors::InvalidPosition();
        }
        T value = GetTypedInput<T>("Enter value to insert: ");
        sequence->Insert(value, position);
    }

    void DeleteElement() override {
        int index = GetIntInput("Enter index to remove (0-" + std::to_string(sequence->Size() - 1) + "): ");
        if (index < 0 || index >= sequence->Size()) {
            throw Errors::InvalidPosition();
        }
        sequence->Delete(index);
    }

    void AccessElement() const override {
        int index = GetIntInput("Enter index to access (0-" + std::to_string(sequence->Size() - 1) + "): ");
        if (index < 0 || index >= sequence->Size()) {
            throw Errors::InvalidPosition();
        }
        std::cout << "Element at index " << index << ": " << sequence->At(index) << "\n";
    }

    ISequenceWrapper* CreateSubsequence() const override {
        int start = GetIntInput("Enter start index (0-" + std::to_string(sequence->Size() - 1) + "): ");
        int end = GetIntInput("Enter end index (" + std::to_string(start) + "-" + std::to_string(sequence->Size() - 1) + "): ");
        
        if (start < 0 || end >= sequence->Size() || start > end) {
            throw Errors::InvalidRange();
        }

        ISequence<T>* subsequence = sequence->Slice(start, end);
        auto* result = new SequenceWrapper<T>(structure_type, data_type);
        delete result->sequence;
        result->sequence = subsequence;
        return result;
    }

    ISequenceWrapper* Combine(const ISequenceWrapper* other) const override {
        const auto* other_sequence = dynamic_cast<const SequenceWrapper<T>*>(other);
        if (!other_sequence) {
            throw Errors::TypeMismatch();
        }

        ISequence<T>* combined = sequence->Combine(other_sequence->sequence);
        auto* result = new SequenceWrapper<T>(structure_type, data_type);
        delete result->sequence;
        result->sequence = combined;
        return result;
    }

    const std::string& GetTypeName() const override {
        return data_type;
    }

    const std::string& GetStructureName() const override {
        return structure_type;
    }
};

void DisplayTypeMenu() {
    std::cout << "Select data type:\n"
              << "1. Integer\n"
              << "2. Double\n"
              << "3. String\n"
              << "4. User\n";
}

void DisplayStructureMenu() {
    std::cout << "Select structure type:\n"
              << "1. Array\n"
              << "2. List\n";
}

void DisplayMainMenu() {
    std::cout << "\nMain Menu:\n"
              << "1. Display sequences\n"
              << "2. Add element to end\n"
              << "3. Add element to front\n"
              << "4. Remove element\n"
              << "5. Insert element at position\n"
              << "6. Access element by index\n"
              << "7. Create subsequence\n"
              << "8. Combine sequences\n"
              << "9. Add new sequence\n"
              << "10. Remove sequence\n"
              << "11. Exit\n"
              << "Enter your choice: ";
}

void RunInterface(std::vector<ISequenceWrapper*>& sequences) {
    while (true) {
        try {
            DisplayMainMenu();
            int choice = GetIntInput();

            if (choice != 9 && choice != 11 && sequences.empty()) {
                std::cout << "No sequences available. Please add a sequence first.\n";
                continue;
            }

            switch (choice) {
                case 1: // Display sequences
                    for (size_t i = 0; i < sequences.size(); ++i) {
                        std::cout << i << ": ";
                        sequences[i]->Display();
                    }
                    break;

                case 2: case 3: case 4: case 5: case 6: case 7: { // Sequence operations
                    int idx = GetIntInput("Select sequence index (0-" + std::to_string(sequences.size() - 1) + "): ");
                    if (idx < 0 || static_cast<size_t>(idx) >= sequences.size()) {
                        throw Errors::InvalidPosition();
                    }

                    switch (choice) {
                        case 2: sequences[idx]->AddToEnd(); break;
                        case 3: sequences[idx]->AddToFront(); break;
                        case 4: sequences[idx]->DeleteElement(); break;
                        case 5: sequences[idx]->InsertElement(); break;
                        case 6: sequences[idx]->AccessElement(); break;
                        case 7: {
                            ISequenceWrapper* subseq = sequences[idx]->CreateSubsequence();
                            sequences.push_back(subseq);
                            std::cout << "Subsequence added at index " << sequences.size() - 1 << "\n";
                            break;
                        }
                    }
                    break;
                }

                case 8: { // Combine sequences
                    int idx1 = GetIntInput("Select first sequence (0-" + std::to_string(sequences.size() - 1) + "): ");
                    int idx2 = GetIntInput("Select second sequence (0-" + std::to_string(sequences.size() - 1) + "): ");
                    
                    if (idx1 < 0 || idx2 < 0 || 
                        static_cast<size_t>(idx1) >= sequences.size() || 
                        static_cast<size_t>(idx2) >= sequences.size()) {
                        throw Errors::InvalidPosition();
                    }

                    if (sequences[idx1]->GetTypeName() != sequences[idx2]->GetTypeName()) {
                        throw Errors::TypeMismatch();
                    }

                    ISequenceWrapper* combined = sequences[idx1]->Combine(sequences[idx2]);
                    sequences.push_back(combined);
                    std::cout << "Combined sequence added at index " << sequences.size() - 1 << "\n";
                    break;
                }

                case 9: { // Add new sequence
                    DisplayTypeMenu();
                    int type_choice = GetIntInput("Select data type: ");
                    std::string type_name;
                    
                    switch (type_choice) {
                        case 1: type_name = "int"; break;
                        case 2: type_name = "double"; break;
                        case 3: type_name = "string"; break;
                        case 4: type_name = "user"; break;
                        default: throw Errors::InvalidArgument();
                    }

                    DisplayStructureMenu();
                    int struct_choice = GetIntInput("Select structure: ");
                    std::string struct_name = (struct_choice == 1) ? "array" : "list";

                    if (type_name == "int") {
                        sequences.push_back(new SequenceWrapper<int>(struct_name, type_name));
                    } 
                    else if (type_name == "double") {
                        sequences.push_back(new SequenceWrapper<double>(struct_name, type_name));
                    }
                    else if (type_name == "string") {
                        sequences.push_back(new SequenceWrapper<std::string>(struct_name, type_name));
                    }
                    else if (type_name == "user") {
                        sequences.push_back(new SequenceWrapper<User>(struct_name, type_name));
                    }
                    break;
                }

                case 10: { // Remove sequence
                    int idx = GetIntInput("Enter sequence index to remove (0-" + std::to_string(sequences.size() - 1) + "): ");
                    if (idx < 0 || static_cast<size_t>(idx) >= sequences.size()) {
                        throw Errors::InvalidPosition();
                    }
                    delete sequences[idx];
                    sequences.erase(sequences.begin() + idx);
                    break;
                }

                case 11: // Exit
                    for (auto* seq : sequences) {
                        delete seq;
                    }
                    return;

                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } 
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            ClearInput();
        }
    }
}

int main() {
    std::vector<ISequenceWrapper*> sequences;
    RunInterface(sequences);
    return 0;
}