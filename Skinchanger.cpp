#include "Gamehooking.hpp"
#include "Skinchanger.hpp"

std::unordered_map<int, Skin> skins =
{
	{ weapon_awp, Skin(344, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_ak47, Skin(600, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_m4a1, Skin(512, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_m4a1_silencer, Skin(548, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_deagle, Skin(277, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_glock, Skin(38, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_usp_silencer, Skin(332, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_fiveseven, Skin(252, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_scar20, Skin(391, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_elite, Skin(249, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_ssg08, Skin(319, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_tec9, Skin(179, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_g3sg1, Skin(493, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_famas, Skin(604, -1, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_knife, Skin(417, weapon_knife_m9_bayonet, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_knife_t, Skin(417, weapon_knife_karambit, 0, 0.0005f, 1337, "Gladiatorcheatz", "") },
	{ weapon_knife_gut, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_gut.mdl") },
	{ weapon_knife_flip, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_flip.mdl") },
	{ weapon_knife_bayonet, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_bayonet.mdl") },
	{ weapon_knife_m9_bayonet, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_m9_bay.mdl") },
	{ weapon_knife_karambit, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_karam.mdl") },
	{ weapon_knife_tactical, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_tactical.mdl") },
	{ weapon_knife_butterfly, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_butterfly.mdl") },
	{ weapon_knife_survival_bowie, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_survival_bowie.mdl") },
	{ weapon_knife_falchion, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_falchion_advanced.mdl") },
	{ weapon_knife_push, Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_push.mdl") }
};

bool bForceFullUpdate = true;

const char *Skinchanger::getModelByItemDI(int idx)
{
	switch (idx)
	{
	case weapon_knife_gut:

		return "models/weapons/v_knife_gut.mdl";

	case weapon_knife_flip:

		return "models/weapons/v_knife_flip.mdl";

	case weapon_knife_bayonet:

		return "models/weapons/v_knife_bayonet.mdl";

	case weapon_knife_m9_bayonet:

		return "models/weapons/v_knife_m9_bay.mdl";

	case weapon_knife_karambit:

		return "models/weapons/v_knife_karam.mdl";

	case weapon_knife_tactical:

		return "models/weapons/v_knife_tactical.mdl";

	case weapon_knife_butterfly:

		return "models/weapons/v_knife_butterfly.mdl";

	case weapon_knife_survival_bowie:

		return "models/weapons/v_knife_survival_bowie.mdl";

	case weapon_knife_falchion:

		return "models/weapons/v_knife_falchion_advanced.mdl";

	case weapon_knife_push:

		return "models/weapons/v_knife_push.mdl";

	default:

		return NULL;
	}
}

const char *Skinchanger::knifeToString(int idx)
{
	Skin skin = skins[idx];

	switch (skin.itemDI)
	{
	case weapon_knife_bayonet:

		return "bayonet";

	case weapon_knife_flip:

		return "knife_flip";

	case weapon_knife_gut:

		return "knife_gut";

	case weapon_knife_karambit:

		return "knife_karambit";

	case weapon_knife_m9_bayonet:

		return "knife_m9_bayonet";

	case weapon_knife_tactical:

		return "knife_tactical";

	case weapon_knife_falchion:

		return "knife_falchion";

	case weapon_knife_survival_bowie:

		return "knife_survival_bowie";

	case weapon_knife_butterfly:

		return "knife_butterfly";

	case weapon_knife_push:

		return "knife_push";

	default:

		return NULL;
	}
}

void Skinchanger::change(ClientFrameStage_en stage)
{
	if (!p_Engine->isIG() || !Global::locPlayer || Global::locPlayer == NULL)
		return;

	if (mVars.bEnableSkinChange)
	{
		if (stage != ClientFrameStage_en::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			return;

		UINT *weapons = Global::locPlayer->getWeapons();

		if (!weapons)
			return;

		for (int i = 0; i < 64; i++)
		{
			if (weapons[i] == -1)
				continue;

			EntityWeapon *weap = reinterpret_cast<EntityWeapon*>(p_ClientEntList->GetClientEntityFromHandle(weapons[i]));

			if (!weap)
				continue;

			auto kExist = skins.find(*weap->getItemDefIdx());
			if (kExist == skins.end())
				continue;

			Skin curSkin = skins[*weap->getItemDefIdx()];

			if (curSkin.pk != -1)
				*weap->getFallbackPK() = curSkin.pk;

			if (curSkin.itemDI != -1)
				*weap->getItemDefIdx() = curSkin.itemDI;

			if (const char* modelFilename = getModelByItemDI(*weap->getItemDefIdx()))
				*(int*)((DWORD)weap + offs.modIdx) = p_ModelInfo->GetModelIndex(modelFilename);

			if (curSkin.s != -1)
				*weap->getFallbackS() = curSkin.s;

			if (curSkin.w != -1)
				*weap->getFallbackW() = curSkin.w;

			if (curSkin.st != -1)
			{
				*weap->getFallbackST() = curSkin.st;

				playerinfo_t playerInfo;
				if (p_Engine->getPlyrInfo(p_Engine->getLocPlyr(), &playerInfo))
					*weap->getAccId() = playerInfo.xuidlow;
			}

			if (curSkin.custName != "")
				snprintf(weap->getCustName(), 32, "%s", curSkin.custName.c_str());

			*weap->getItemIdHigh() = -1;
		}

		Entity *viewModel = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntityFromHandle(Global::locPlayer->getViewModel()));
		if (!viewModel)
			return;

		EntityWeapon *activWeap = Global::locPlayer->getWeapon();
		if (!activWeap)
			return;

		auto kExist = skins.find(*activWeap->getItemDefIdx());
		if (kExist == skins.end())
			return;

		Skin currSkin = skins[*activWeap->getItemDefIdx()];

		if (currSkin.model != "")
			*viewModel->getModelIdx() = p_ModelInfo->GetModelIndex(currSkin.model.c_str());
	}

	if (bForceFullUpdate)
	{
		p_Engine->execClientCMD_Unrestricted("record x;stop", 0);
		bForceFullUpdate = false;
	}
}

void Skinchanger::fireEventOnClientSide(GameEvent *gEvent)
{
	if (!mVars.bEnableSkinChange)
		return;

	if (!p_Engine->isIG())
		return;

	if (!gEvent || strcmp(gEvent->getName(), "player_death") != 0)
		return;

	if (!gEvent->getInt("attacker") || p_Engine->getPlyrForUserId(gEvent->getInt("attacker")) != p_Engine->getLocPlyr())
		return;

	const char *weapon = gEvent->getString("weapon");
	if (strcmp(weapon, "knife_default_ct") == 0)
	{
		const char* name = knifeToString(weapon_knife);
		gEvent->setString("weapon", name ? "" : weapon);
	}
	else if (strcmp(weapon, "knife_t") == 0)
	{
		const char* name = knifeToString(weapon_knife_t);
		gEvent->setString("weapon", name ? "" : weapon);
	}
}

void Proxies::viewModelSequence(const PrimitiProxyData *pData, void *pStruct, void *pOut)
{
	PrimitiProxyData *pPData = const_cast<PrimitiProxyData*>(pData);

	Entity *viewModel = reinterpret_cast<Entity*>(pStruct);

	if (viewModel)
	{
		Entity *entOwn = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(viewModel->getOwner() & 0xFFF));

		if (entOwn && entOwn == Global::locPlayer)
		{
			model_t *model = const_cast<model_t*>(p_ModelInfo->GetModel(*viewModel->getModelIdx()));
			std::string modelName = p_ModelInfo->GetModelName(model);

			int nSequence = pPData->m_Value.m_Int;
			if (modelName == "models/weapons/v_knife_butterfly.mdl")
			{
				switch (nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:

					nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;

				case SEQUENCE_DEFAULT_LOOKAT01:

					nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;

				default:

					nSequence++;
				}
			}
			else if (modelName == "models/weapons/v_knife_falchion_advanced.mdl")
			{
				switch (nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:

					nSequence = SEQUENCE_FALCHION_IDLE1;
					break;

				case SEQUENCE_DEFAULT_HEAVY_MISS1:

					nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;

				case SEQUENCE_DEFAULT_LOOKAT01:

					nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;

				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:

					break;

				default:

					nSequence--;
				}
			}
			else if (modelName == "models/weapons/v_knife_push.mdl")
			{
				switch (nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:

					nSequence = SEQUENCE_DAGGERS_IDLE1;
					break;

				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:

					nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;

				case SEQUENCE_DEFAULT_HEAVY_MISS1:

					nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;

				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:

					nSequence += 3;
					break;

				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:

					break;

				default:

					nSequence += 2;
				}
			}
			else if (modelName == "models/weapons/v_knife_survival_bowie.mdl")
			{
				switch (nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:

					break;

				case SEQUENCE_DEFAULT_IDLE2:

					nSequence = SEQUENCE_BOWIE_IDLE1;
					break;

				default:

					nSequence--;
				}
			}

			pPData->m_Value.m_Int = nSequence;
		}
	}

	o_viewModelSequence(pPData, pStruct, pOut);
}