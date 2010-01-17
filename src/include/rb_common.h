#ifndef __RB_COMMON_H__
#define __RB_COMMON_H__

#ifdef __GNUC__
#include <stdint.h>
#include <ext/hash_map>
namespace std
{
 using namespace __gnu_cxx;
}

#define hash_map_t std::hash_map
#define hash_multimap_t std::hash_multimap

#define _MAX_PATH       256

#else
#include <hash_map>

#define hash_map_t stdext::hash_map
#define hash_multimap_t stdext::hash_multimap

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long	uint32_t;
typedef unsigned int	int_t;
typedef long int		int32_t;

#define NULL            0
#endif


#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4530 )
#pragma warning ( disable : 4312 )
#pragma warning ( disable : 4996 )
#pragma warning ( disable : 4005 )
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4005 )
#pragma warning ( disable : 4267 )
#pragma warning ( disable : 4183 )
#pragma warning ( disable : 4100 )
#pragma warning ( disable : 4127 )
#pragma warning ( disable : 4311 )
#pragma warning ( disable : 4099 )
#pragma warning ( disable : 4530 )
#pragma warning ( disable : 4244 )

#include <vector>
#include <string>
#include <algorithm>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#endif // __RB_COMMON_H__

