#include "PlatformInput.h"
#include <map>


unsigned int FGenericPlatformInput::GetStandardPrintableKeyMap(unsigned int* KeyCodes, std::string* KeyNames, unsigned int MaxMappings, bool bMapUppercaseKeys, bool bMapLowercaseKeys)
{
	unsigned int NumMappings = 0;

#define ADDKEYMAP(KeyCode, KeyName)		if (NumMappings<MaxMappings) { KeyCodes[NumMappings]=KeyCode; KeyNames[NumMappings]=KeyName; ++NumMappings; };

	ADDKEYMAP('0', TEXT("Zero"));
	ADDKEYMAP('1', TEXT("One"));
	ADDKEYMAP('2', TEXT("Two"));
	ADDKEYMAP('3', TEXT("Three"));
	ADDKEYMAP('4', TEXT("Four"));
	ADDKEYMAP('5', TEXT("Five"));
	ADDKEYMAP('6', TEXT("Six"));
	ADDKEYMAP('7', TEXT("Seven"));
	ADDKEYMAP('8', TEXT("Eight"));
	ADDKEYMAP('9', TEXT("Nine"));

	// we map both upper and lower
	if (bMapUppercaseKeys)
	{
		ADDKEYMAP('A', TEXT("A"));
		ADDKEYMAP('B', TEXT("B"));
		ADDKEYMAP('C', TEXT("C"));
		ADDKEYMAP('D', TEXT("D"));
		ADDKEYMAP('E', TEXT("E"));
		ADDKEYMAP('F', TEXT("F"));
		ADDKEYMAP('G', TEXT("G"));
		ADDKEYMAP('H', TEXT("H"));
		ADDKEYMAP('I', TEXT("I"));
		ADDKEYMAP('J', TEXT("J"));
		ADDKEYMAP('K', TEXT("K"));
		ADDKEYMAP('L', TEXT("L"));
		ADDKEYMAP('M', TEXT("M"));
		ADDKEYMAP('N', TEXT("N"));
		ADDKEYMAP('O', TEXT("O"));
		ADDKEYMAP('P', TEXT("P"));
		ADDKEYMAP('Q', TEXT("Q"));
		ADDKEYMAP('R', TEXT("R"));
		ADDKEYMAP('S', TEXT("S"));
		ADDKEYMAP('T', TEXT("T"));
		ADDKEYMAP('U', TEXT("U"));
		ADDKEYMAP('V', TEXT("V"));
		ADDKEYMAP('W', TEXT("W"));
		ADDKEYMAP('X', TEXT("X"));
		ADDKEYMAP('Y', TEXT("Y"));
		ADDKEYMAP('Z', TEXT("Z"));
	}

	if (bMapLowercaseKeys)
	{
		ADDKEYMAP('a', TEXT("A"));
		ADDKEYMAP('b', TEXT("B"));
		ADDKEYMAP('c', TEXT("C"));
		ADDKEYMAP('d', TEXT("D"));
		ADDKEYMAP('e', TEXT("E"));
		ADDKEYMAP('f', TEXT("F"));
		ADDKEYMAP('g', TEXT("G"));
		ADDKEYMAP('h', TEXT("H"));
		ADDKEYMAP('i', TEXT("I"));
		ADDKEYMAP('j', TEXT("J"));
		ADDKEYMAP('k', TEXT("K"));
		ADDKEYMAP('l', TEXT("L"));
		ADDKEYMAP('m', TEXT("M"));
		ADDKEYMAP('n', TEXT("N"));
		ADDKEYMAP('o', TEXT("O"));
		ADDKEYMAP('p', TEXT("P"));
		ADDKEYMAP('q', TEXT("Q"));
		ADDKEYMAP('r', TEXT("R"));
		ADDKEYMAP('s', TEXT("S"));
		ADDKEYMAP('t', TEXT("T"));
		ADDKEYMAP('u', TEXT("U"));
		ADDKEYMAP('v', TEXT("V"));
		ADDKEYMAP('w', TEXT("W"));
		ADDKEYMAP('x', TEXT("X"));
		ADDKEYMAP('y', TEXT("Y"));
		ADDKEYMAP('z', TEXT("Z"));
	}

	ADDKEYMAP(';', TEXT("Semicolon"));
	ADDKEYMAP('=', TEXT("Equals"));
	ADDKEYMAP(',', TEXT("Comma"));
	ADDKEYMAP('-', TEXT("Hyphen"));
	ADDKEYMAP('.', TEXT("Period"));
	ADDKEYMAP('/', TEXT("Slash"));
	ADDKEYMAP('`', TEXT("Tilde"));
	ADDKEYMAP('[', TEXT("LeftBracket"));
	ADDKEYMAP('\\', TEXT("Backslash"));
	ADDKEYMAP(']', TEXT("RightBracket"));
	ADDKEYMAP('\'', TEXT("Apostrophe"));
	ADDKEYMAP(' ', TEXT("SpaceBar"));

	// AZERTY Keys
	ADDKEYMAP('&', TEXT("Ampersand"));
	ADDKEYMAP('*', TEXT("Asterix"));
	ADDKEYMAP('^', TEXT("Caret"));
	ADDKEYMAP(':', TEXT("Colon"));
	ADDKEYMAP('$', TEXT("Dollar"));
	ADDKEYMAP('!', TEXT("Exclamation"));
	ADDKEYMAP('(', TEXT("LeftParantheses"));
	ADDKEYMAP(')', TEXT("RightParantheses"));
	ADDKEYMAP('"', TEXT("Quote"));
	ADDKEYMAP('_', TEXT("Underscore"));
	ADDKEYMAP(224, TEXT("A_AccentGrave"));
	ADDKEYMAP(231, TEXT("C_Cedille"));
	ADDKEYMAP(233, TEXT("E_AccentAigu"));
	ADDKEYMAP(232, TEXT("E_AccentGrave"));
	ADDKEYMAP(167, TEXT("Section"));

#undef ADDKEYMAP

	return NumMappings;
}

unsigned int FWindowsPlatformInput::GetKeyMap(unsigned int* KeyCodes, std::string* KeyNames, unsigned int MaxMappings)
{
#define ADDKEYMAP(KeyCode, KeyName)		if (NumMappings<MaxMappings) { KeyCodes[NumMappings]=KeyCode; KeyNames[NumMappings]=KeyName; ++NumMappings; };

	unsigned int NumMappings = 0;

	if (KeyCodes && KeyNames && (MaxMappings > 0))
	{
		ADDKEYMAP(VK_LBUTTON, "LeftMouseButton");
		ADDKEYMAP(VK_RBUTTON, "RightMouseButton");
		ADDKEYMAP(VK_MBUTTON, "MiddleMouseButton");

		ADDKEYMAP(VK_XBUTTON1, "ThumbMouseButton");
		ADDKEYMAP(VK_XBUTTON2, "ThumbMouseButton2");

		ADDKEYMAP(VK_BACK, "BackSpace");
		ADDKEYMAP(VK_TAB, "Tab");
		ADDKEYMAP(VK_RETURN, "Enter");
		ADDKEYMAP(VK_PAUSE, "Pause");

		ADDKEYMAP(VK_CAPITAL, "CapsLock");
		ADDKEYMAP(VK_ESCAPE, "Escape");
		ADDKEYMAP(VK_SPACE, "SpaceBar");
		ADDKEYMAP(VK_PRIOR, "PageUp");
		ADDKEYMAP(VK_NEXT, TEXT("PageDown"));
		ADDKEYMAP(VK_END, TEXT("End"));
		ADDKEYMAP(VK_HOME, TEXT("Home"));

		ADDKEYMAP(VK_LEFT, TEXT("Left"));
		ADDKEYMAP(VK_UP, TEXT("Up"));
		ADDKEYMAP(VK_RIGHT, TEXT("Right"));
		ADDKEYMAP(VK_DOWN, TEXT("Down"));

		ADDKEYMAP(VK_INSERT, TEXT("Insert"));
		ADDKEYMAP(VK_DELETE, TEXT("Delete"));

		ADDKEYMAP(VK_NUMPAD0, TEXT("NumPadZero"));
		ADDKEYMAP(VK_NUMPAD1, TEXT("NumPadOne"));
		ADDKEYMAP(VK_NUMPAD2, TEXT("NumPadTwo"));
		ADDKEYMAP(VK_NUMPAD3, TEXT("NumPadThree"));
		ADDKEYMAP(VK_NUMPAD4, TEXT("NumPadFour"));
		ADDKEYMAP(VK_NUMPAD5, TEXT("NumPadFive"));
		ADDKEYMAP(VK_NUMPAD6, TEXT("NumPadSix"));
		ADDKEYMAP(VK_NUMPAD7, TEXT("NumPadSeven"));
		ADDKEYMAP(VK_NUMPAD8, TEXT("NumPadEight"));
		ADDKEYMAP(VK_NUMPAD9, TEXT("NumPadNine"));

		ADDKEYMAP(VK_MULTIPLY, TEXT("Multiply"));
		ADDKEYMAP(VK_ADD, TEXT("Add"));
		ADDKEYMAP(VK_SUBTRACT, TEXT("Subtract"));
		ADDKEYMAP(VK_DECIMAL, TEXT("Decimal"));
		ADDKEYMAP(VK_DIVIDE, TEXT("Divide"));

		ADDKEYMAP(VK_F1, TEXT("F1"));
		ADDKEYMAP(VK_F2, TEXT("F2"));
		ADDKEYMAP(VK_F3, TEXT("F3"));
		ADDKEYMAP(VK_F4, TEXT("F4"));
		ADDKEYMAP(VK_F5, TEXT("F5"));
		ADDKEYMAP(VK_F6, TEXT("F6"));
		ADDKEYMAP(VK_F7, TEXT("F7"));
		ADDKEYMAP(VK_F8, TEXT("F8"));
		ADDKEYMAP(VK_F9, TEXT("F9"));
		ADDKEYMAP(VK_F10, TEXT("F10"));
		ADDKEYMAP(VK_F11, TEXT("F11"));
		ADDKEYMAP(VK_F12, TEXT("F12"));

		ADDKEYMAP(VK_NUMLOCK, TEXT("NumLock"));

		ADDKEYMAP(VK_SCROLL, TEXT("ScrollLock"));

		ADDKEYMAP(VK_LSHIFT, TEXT("LeftShift"));
		ADDKEYMAP(VK_RSHIFT, TEXT("RightShift"));
		ADDKEYMAP(VK_LCONTROL, TEXT("LeftControl"));
		ADDKEYMAP(VK_RCONTROL, TEXT("RightControl"));
		ADDKEYMAP(VK_LMENU, TEXT("LeftAlt"));
		ADDKEYMAP(VK_RMENU, TEXT("RightAlt"));
		ADDKEYMAP(VK_LWIN, TEXT("LeftCommand"));
		ADDKEYMAP(VK_RWIN, TEXT("RightCommand"));

		std::map<unsigned int, unsigned int> ScanToVKMap;
#define MAP_OEM_VK_TO_SCAN(KeyCode) { const unsigned int CharCode = MapVirtualKey(KeyCode,2); if (CharCode != 0) { ScanToVKMap.insert(std::make_pair(CharCode,KeyCode)); } }
		MAP_OEM_VK_TO_SCAN(VK_OEM_1);
		MAP_OEM_VK_TO_SCAN(VK_OEM_2);
		MAP_OEM_VK_TO_SCAN(VK_OEM_3);
		MAP_OEM_VK_TO_SCAN(VK_OEM_4);
		MAP_OEM_VK_TO_SCAN(VK_OEM_5);
		MAP_OEM_VK_TO_SCAN(VK_OEM_6);
		MAP_OEM_VK_TO_SCAN(VK_OEM_7);
		MAP_OEM_VK_TO_SCAN(VK_OEM_8);
		MAP_OEM_VK_TO_SCAN(VK_OEM_PLUS);
		MAP_OEM_VK_TO_SCAN(VK_OEM_COMMA);
		MAP_OEM_VK_TO_SCAN(VK_OEM_MINUS);
		MAP_OEM_VK_TO_SCAN(VK_OEM_PERIOD);
		MAP_OEM_VK_TO_SCAN(VK_OEM_102);
#undef  MAP_OEM_VK_TO_SCAN

		static const unsigned int MAX_KEY_MAPPINGS(256);
		unsigned int CharCodes[MAX_KEY_MAPPINGS];
		std::string CharKeyNames[MAX_KEY_MAPPINGS];
		const unsigned int CharMappings = GetCharKeyMap(CharCodes, CharKeyNames, MAX_KEY_MAPPINGS);

		for (unsigned int MappingIndex = 0; MappingIndex < CharMappings; ++MappingIndex)
		{
			ScanToVKMap.erase(CharCodes[MappingIndex]);
		}

		for (auto ItemScanToVKMap : ScanToVKMap)
		{
			ADDKEYMAP(ItemScanToVKMap.first, std::to_string(ItemScanToVKMap.second));
		}
	}

	assert(NumMappings < MaxMappings);
	return NumMappings;

#undef ADDKEYMAP
}