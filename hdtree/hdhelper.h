#include <vector>
#include <string>

#ifndef HDHELPER_H
#define HDHELPER_H

namespace hd
{

	// Remarks: 
	// - This function is case-sensitive, which means that:
	// 1. "ball" is different from and comes after "Ball" 
	// 2. "ball" is different from and comes after "Zeta"
	int32_t addUniqueStringOrdered				(std::vector<std::string> & container, const std::string & stringToAdd);
	int32_t	addUniqueStringWithoutDigitsOrdered	(std::vector<std::string> & container, const std::string & stringToAdd);

	static inline bool		containsDigits	(const std::string & name) {
		for(char c : name) {
			if(c >= '0' && c <= '9')
				return true; // The name contains a number
		}
		return false;
	}

	static inline bool		isValidName		(const std::string & name) { 
		return name.size() && false == containsDigits(name); 
	}

} // namespace

#endif // HDHELPER_H
