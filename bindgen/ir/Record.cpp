#include "Record.h"
#include "../Utils.h"

Field::Field(std::string name, std::shared_ptr<const Type> type)
    : TypeAndName(std::move(name), std::move(type)) {}

Field::Field(std::string name, std::shared_ptr<const Type> type,
             uint64_t offsetInBits)
    : TypeAndName(std::move(name), std::move(type)),
      offsetInBits(offsetInBits) {}

uint64_t Field::getOffsetInBits() const { return offsetInBits; }

Record::Record(std::string name, std::vector<std::shared_ptr<Field>> fields,
               std::shared_ptr<Location> location)
    : LocatableType(std::move(location)), name(std::move(name)),
      fields(std::move(fields)) {}

bool Record::usesType(
    const std::shared_ptr<const Type> &type, bool stopOnTypeDefs,
    std::vector<std::shared_ptr<const Type>> &visitedTypes) const {

    if (contains(this, visitedTypes)) {
        return false;
    }

    visitedTypes.push_back(shared_from_this());

    for (const auto &field : fields) {
        if (field->getType() && (*field->getType() == *type ||
            field->getType()->usesType(type, stopOnTypeDefs, visitedTypes))) {
            visitedTypes.pop_back();
            return true;
        }
    }
    visitedTypes.pop_back();
    return false;
}

std::string Record::getName() const { return name; }

bool Record::hasHelperMethods() const { return !fields.empty(); }
