#include <cstdint>

#ifndef HDN2_H_2365
#define HDN2_H_2365
namespace hd
{
	template<typename T>
	struct n2 { T x, y; };

	typedef n2<uint32_t> n2u32;
	typedef n2<long>	 n2lng;
} // hd

#endif // HDN2_H_2365