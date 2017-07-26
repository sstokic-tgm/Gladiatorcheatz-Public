#pragma once

class GlowObjectManager
{
public:

	class GlowObjectDefinition_t
	{
	public:

		class Entity *pEntity;
		Vector3 m_vGlowColor;
		float m_flGlowAlpha;
		uint8_t pad_0014[4];
		float m_flSomeFloatThingy;
		uint8_t pad_001C[4];
		float m_flAnotherFloat;
		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;
		uint8_t pad_0027[5];
		int32_t m_bPulsatingChams;
		int32_t m_nSplitScreenSlot;
		int32_t m_nNextFreeSlot;

		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	GlowObjectDefinition_t *m_GlowObjectDefinitions;
	int max_size;
	int pad;
	int size;
	GlowObjectDefinition_t *m_Unknown;
	int	currentObjects;
};