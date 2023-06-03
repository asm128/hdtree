#include "hdhelper.h"

#ifndef HDTREE_H
#define HDTREE_H

namespace hd
{
    struct Category {
        std::string                 Name;
        std::vector<std::string>    Materials;

        inline int32_t              AddMaterial (const std::string & materialName) { 
            return addUniqueStringWithoutDigitsOrdered(Materials, materialName);
        }
    };

    struct MaterialTree {
        std::vector<Category>   Categories;

        inline int32_t          AddMaterial (int32_t categoryIndex, const std::string & materialName) { 
            return Categories[categoryIndex].AddMaterial(materialName); 
        }

        int32_t                 AddMaterial (const std::string & categoryName, const std::string & materialName) { 
            std::vector<Category>::iterator itCat = std::find_if(Categories.begin(), Categories.end(), [categoryName](const Category & category) { return category.Name == categoryName; });
            return (itCat == Categories.end()) ? -1 : itCat->AddMaterial(materialName);
        }

        // This is just a more compact version of addUniqueStringWithoutDigitsOrdered(). 
        int32_t                 AddCategory (const std::string & categoryName) { 
            if(0 == categoryName.size() || containsDigits(categoryName))
                return -1;

            for(std::vector<Category>::iterator itCat = Categories.begin(); itCat != Categories.end(); ++itCat) {
                const std::string   & name          = itCat->Name;
                const bool          equalSize       = categoryName.size() == name.size();
                const int           signOrEqual     = memcmp(name.data(), categoryName.data(), std::min(name.size(), categoryName.size()));
                if(equalSize && signOrEqual == 0) // same size and contents, failure
                    return -1;

                if(signOrEqual > 0 || (signOrEqual == 0 && name.size() > categoryName.size())) {
                    int index = int(itCat - Categories.begin());
                    Categories.insert(itCat, {categoryName});
                    return index;
                }
            }
            Categories.push_back({categoryName});
            return int(Categories.size() - 1);
        }
    };
} // namespace 

#endif // HDTREE_H