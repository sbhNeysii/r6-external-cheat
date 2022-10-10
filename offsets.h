#pragma once
#include "memory.h"
#include "hexrays.h"

namespace offsets {
	std::uint64_t gamemanager() { // credits: jangut
		return (((memory.read<std::uint64_t>(memory.base + 0x84CB508) + 0x21AEA2FE5E55D415i64) ^ 0xD) - 0x2B48AC28BCCB7A90i64);
	}

	std::uint32_t roundstate() { // credits: spoon0x
		std::uint64_t roundmanager = __ROL8__(__ROL8__(memory.read<std::uintptr_t>(memory.base + 0x7973D98), 0x31) ^ 0xBEFF6ADEC20B78C1ui64, 0x36);
		return std::rotl<std::uint32_t>((memory.read<std::uint32_t>(roundmanager + 0xF8) + 0xD3A45CC) ^ 0xCFB1AB4B, 6);
	}

	static std::pair<std::uint64_t, std::uint32_t> entitymanager(uint64_t game_manager) { // credits: jangut
		const auto decryption = [&game_manager](const std::uint32_t offset = 0) -> std::uint64_t {
			return _rotl64(memory.read<std::uint64_t>(game_manager + 0x78 + offset) ^ 0x7482B76FBCEA7157i64, 0x1D) - 0x33DFAE10DB8C8A5Fi64;
		};
		return { decryption(), static_cast<std::uint32_t>(decryption(8) & 0x3FFFFFFF) };
	}

	namespace player {
		std::uint64_t pawn(std::uint64_t controller) {
			return ((memory.read<std::uint64_t>(controller + 0x38) ^ 0xA37CF47B905E722Cui64) - 0x7CC563BB768D928i64) ^ 0xE47A9645F0C461B4ui64; // credits: CedPlay
		}

		std::uint64_t actor(std::uint64_t pawn) {
			return __ROL8__(memory.read<std::uint64_t>(pawn + 0x18) + 0x3DB6458A1B99414Fi64, 59) - 0x4D9FA150B1036211i64; // credits: CedPlay
		}

		inline std::uint64_t outline_component(std::uint64_t actor) { // credits: SuperSlav231
			std::uintptr_t component = memory.read<std::uintptr_t>(actor + 0x1C8);
			component ^= 0xB0569C7F4A13FA4C;
			component += 0x8432F515C37D6822;
			return component;
		}

		inline void outlines(std::uint64_t actor) { 
			std::uintptr_t outlinestate = memory.read<std::uintptr_t>(outline_component(actor) + 0xB0);
			outlinestate = memory.read<std::uintptr_t>(outline_component(actor) + 0xB0);
			if (outlinestate != 0 && outlinestate != 0x20748000)
				memory.write<std::uintptr_t>(outline_component(actor) + 0xB0, 0x20748000);
		}
	}
}