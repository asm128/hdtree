#include "hdhelper.h"

#include <algorithm>

// The current implementation does a linear search. 
// While it could be implemented for doing a binary search, it is possible that 
// the cache misses it would trigger end up degrading performance instead
int32_t hd::addUniqueStringOrdered (std::vector<std::string> & container, const std::string & stringToAdd) { 
    for(std::vector<std::string>::iterator itCat = container.begin(); itCat != container.end(); ++itCat) {
        const std::string   & name      = *itCat;
        const int           index       = int(itCat - container.begin());
        const bool          equalSize   = stringToAdd.size() == name.size();
        const int           signOrEqual = memcmp(name.data(), stringToAdd.data(), std::min(name.size(), stringToAdd.size()));
        if(signOrEqual > 0) { // whatever compared character in "name" is > the character in categoryName, means we need to insert, regardless of their sizes
            container.insert(itCat, {stringToAdd});
            return index;
        }

        if(equalSize) {
            fail_if(name.size() == 0 || signOrEqual == 0); // same size and contents, failure
            else // if not equal, categoryName is greater than name, so test next or add at the end.
                continue;
        }

        if(signOrEqual == 0) {
            if(name.size() < stringToAdd.size()) 
                continue; // categoryName is greater than name, so test next or add at the end.
            else { // if name is greater, insert category here
                container.insert(itCat, {stringToAdd});
                return index;
            }
        }
    }
    container.push_back({stringToAdd});
    return int(container.size() - 1);
}
