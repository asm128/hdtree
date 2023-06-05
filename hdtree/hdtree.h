#include "hdhelper.h"
#include "hdlog.h"

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
            fail_if(itCat == Categories.end());
            fail_if_failed(itCat->AddMaterial(materialName));
            return 0;
        }

        // This is just a more compact version of addUniqueStringWithoutDigitsOrdered(). 
        int32_t                 AddCategory (const std::string & categoryName) { 
            fail_if(0 == categoryName.size() || containsDigits(categoryName));

            for(std::vector<Category>::iterator itCat = Categories.begin(); itCat != Categories.end(); ++itCat) {
                const std::string   & name          = itCat->Name;
                const bool          equalSize       = categoryName.size() == name.size();
                const int           signOrEqual     = memcmp(name.data(), categoryName.data(), std::min(name.size(), categoryName.size()));
                fail_if(equalSize && signOrEqual == 0); // same size and contents, failure

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