#pragma once

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

struct Error {
    int code;
    std::string message;
};

enum class ErrorCode {
    OK = 0,
    IMMUTABLE,
    INDEX_OUT_OF_RANGE,
    INVALID_ARGUMENT,
    EMPTY_CONTAINER,
    EMPTY_LIST,
    INCOMPATIBLE_TYPES,
    EMPTY_VALUE,
    INVALID_SIZE,
    INVALID_RANGE,
    NEGATIVE_COUNT,
    NULL_LIST,
    CONCAT_TYPE_MISMATCH,
    INVALID_POSITION,
    TYPE_MISMATCH
};

inline std::vector<Error> ErrorsList = {
    {0, "Success"},
    {1, "Immutable object"},
    {2, "Index out of range"},
    {3, "Invalid argument"},
    {4, "Container is empty"},
    {5, "List is empty"},
    {6, "Incompatible types"},
    {7, "Empty value"},
    {8, "Invalid size specified"},
    {9, "Invalid range specified"},
    {10, "Negative count"},
    {11, "Null list"},
    {12, "Cannot concat sequences of different types"},
    {13, "Invalid position"},
    {14, "Type mismatch"}
};

namespace Errors {

    class BaseError : public std::exception {
    protected:
        ErrorCode code;
        std::string msg;
    public:
        BaseError(ErrorCode code_, const std::string& custom_message = "") 
            : code(code_) 
        {
            if (custom_message.empty()) {
                msg = ErrorsList[static_cast<int>(code_)].message;
            } else {
                msg = ErrorsList[static_cast<int>(code_)].message + ": " + custom_message;
            }
        }

        const char* what() const noexcept override {
            return msg.c_str();
        }

        ErrorCode Code() const noexcept {
            return code;
        }
    };

    inline std::logic_error Immutable() {
        return std::logic_error(ErrorsList[static_cast<int>(ErrorCode::IMMUTABLE)].message);
    }

    inline std::out_of_range IndexOutOfRange() {
        return std::out_of_range(ErrorsList[static_cast<int>(ErrorCode::INDEX_OUT_OF_RANGE)].message);
    }

    inline std::invalid_argument InvalidArgument(const std::string& message = "") {
        if (message.empty())
            return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::INVALID_ARGUMENT)].message);
        else
            return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::INVALID_ARGUMENT)].message + ": " + message);
    }

    inline std::out_of_range EmptyContainer() {
        return std::out_of_range(ErrorsList[static_cast<int>(ErrorCode::EMPTY_CONTAINER)].message);
    }

    inline std::out_of_range EmptyList() {
        return std::out_of_range(ErrorsList[static_cast<int>(ErrorCode::EMPTY_LIST)].message);
    }

    inline std::invalid_argument IncompatibleTypes() {
        return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::INCOMPATIBLE_TYPES)].message);
    }

    inline std::runtime_error EmptyValue() {
        return std::runtime_error(ErrorsList[static_cast<int>(ErrorCode::EMPTY_VALUE)].message);
    }

    inline std::invalid_argument InvalidSize() {
        return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::INVALID_SIZE)].message);
    }

    inline std::out_of_range InvalidRange() {
        return std::out_of_range(ErrorsList[static_cast<int>(ErrorCode::INVALID_RANGE)].message);
    }

    inline std::invalid_argument NegativeCount() {
        return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::NEGATIVE_COUNT)].message);
    }

    inline std::invalid_argument NullList() {
        return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::NULL_LIST)].message);
    }

    inline std::logic_error ConcatTypeMismatchError() {
        return std::logic_error(ErrorsList[static_cast<int>(ErrorCode::CONCAT_TYPE_MISMATCH)].message);
    }

    inline std::out_of_range InvalidPosition() {
        return std::out_of_range(ErrorsList[static_cast<int>(ErrorCode::INVALID_POSITION)].message);
    }

    inline std::logic_error TypeMismatch(const std::string& message = "") {
        if (message.empty())
            return std::logic_error(ErrorsList[static_cast<int>(ErrorCode::TYPE_MISMATCH)].message);
        else
            return std::logic_error(ErrorsList[static_cast<int>(ErrorCode::TYPE_MISMATCH)].message + ": " + message);
    }
}