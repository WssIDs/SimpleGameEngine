#pragma once

using uint8 = unsigned char;		// 8-bit  unsigned.
using uint16 = unsigned short;		// 16-bit unsigned.
using uint32 = unsigned int;		// 32-bit unsigned.
using uint64 = unsigned long long;	// 64-bit unsigned.

using int8 = signed char;			// 8-bit  signed.
using int16 = signed short int;		// 16-bit signed.
using int32 = signed int;			// 32-bit signed.
using int64 = signed long long;		// 64-bit signed.


// Character types.
using ANSICHAR = char;		// An ANSI character       -                  8-bit fixed-width representation of 7-bit characters.
using WIDECHAR = wchar_t;	// A wide character        - In-memory only.  ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms.
using CHAR8 = uint8;		// An 8-bit character type - In-memory only.  8-bit representation.  Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
using CHAR16 = uint16;		// A 16-bit character type - In-memory only.  16-bit representation.  Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
using CHAR32 = uint32;		// A 32-bit character type - In-memory only.  32-bit representation.  Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
//using TCHAR = WIDECHAR;		// A switchable character  - In-memory only.  Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.