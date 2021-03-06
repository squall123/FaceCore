/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

/*######
## npc_agnetta_tyrsdottar
######*/

#define SAY_AGGRO                  -1571003
#define GOSSIP_AGNETTA             "Skip the warmup, sister... or are you too scared to face soemeone your own size?"

enum eAgnetta
{
    QUEST_ITS_THAT_YOUR_GOBLIN      = 12969,
    FACTION_HOSTILE_AT1             = 45
};

class npc_agnetta_tyrsdottar : public CreatureScript
{
public:
    npc_agnetta_tyrsdottar() : CreatureScript("npc_agnetta_tyrsdottar") { }

    struct npc_agnetta_tyrsdottarAI : public ScriptedAI
    {
        npc_agnetta_tyrsdottarAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void Reset()
        {
            me->RestoreFaction();
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_agnetta_tyrsdottarAI(creature);
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->GetQuestStatus(QUEST_ITS_THAT_YOUR_GOBLIN) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_AGNETTA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(13691, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            DoScriptText(SAY_AGGRO, pCreature);
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->setFaction(FACTION_HOSTILE_AT1);
            pCreature->AI()->AttackStart(pPlayer);
        }

        return true;
    }
};

/*######
## npc_frostborn_scout
######*/

#define GOSSIP_ITEM1    "Are you okay? I've come to take you back to Frosthold if you can stand."
#define GOSSIP_ITEM2    "I'm sorry that I didn't get here sooner. What happened?"
#define GOSSIP_ITEM3    "I'll go get some help. Hang in there."

enum eFrostbornScout
{
    QUEST_MISSING_SCOUTS  =  12864
};

class npc_frostborn_scout : public CreatureScript
{
public:
    npc_frostborn_scout() : CreatureScript("npc_frostborn_scout") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {

        if (pPlayer->GetQuestStatus(QUEST_MISSING_SCOUTS) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->PlayerTalkClass->SendGossipMenu(13611, pCreature->GetGUID());
        }

        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->PlayerTalkClass->SendGossipMenu(13612, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->PlayerTalkClass->SendGossipMenu(13613, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->PlayerTalkClass->SendGossipMenu(13614, pCreature->GetGUID());
            pPlayer->AreaExploredOrEventHappens(QUEST_MISSING_SCOUTS);
            break;
        }

        return true;
    }
};

/*######
## npc_thorim
######*/

#define GOSSIP_HN "Thorim?"
#define GOSSIP_SN1 "Can you tell me what became of Sif?"
#define GOSSIP_SN2 "He did more than that, Thorim. He controls Ulduar now."
#define GOSSIP_SN3 "It needn't end this way."

enum eThorim
{
    QUEST_SIBLING_RIVALRY = 13064,
    NPC_THORIM = 29445,
    GOSSIP_TEXTID_THORIM1 = 13799,
    GOSSIP_TEXTID_THORIM2 = 13801,
    GOSSIP_TEXTID_THORIM3 = 13802,
    GOSSIP_TEXTID_THORIM4 = 13803
};

class npc_thorim : public CreatureScript
{
public:
    npc_thorim() : CreatureScript("npc_thorim") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_SIBLING_RIVALRY) == QUEST_STATUS_INCOMPLETE) {
            pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_HN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_THORIM1, pCreature->GetGUID());
            return true;
        }
        return false;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_SN1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_THORIM2, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_SN2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_THORIM3, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_SN3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_THORIM4, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->CompleteQuest(QUEST_SIBLING_RIVALRY);
                break;
        }
        return true;
    }
};

/*######
## npc_goblin_prisoner
######*/

enum eGoblinPrisoner
{
    GO_RUSTY_CAGE = 191544
};

class npc_goblin_prisoner : public CreatureScript
{
public:
    npc_goblin_prisoner() : CreatureScript("npc_goblin_prisoner") { }

    struct npc_goblin_prisonerAI : public ScriptedAI
    {
        npc_goblin_prisonerAI(Creature* pCreature) : ScriptedAI (pCreature){}

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);

            if (GameObject* pGO = me->FindNearestGameObject(GO_RUSTY_CAGE, 5.0f))
            {
                if (pGO->GetGoState() == GO_STATE_ACTIVE)
                    pGO->SetGoState(GO_STATE_READY);
            }
        }

    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_goblin_prisonerAI(creature);
    }
};

/*######
## npc_victorious_challenger
######*/

#define GOSSIP_CHALLENGER            "Let's do this, sister."

enum eVictoriousChallenger
{
    QUEST_TAKING_ALL_CHALLENGERS    = 12971,
    QUEST_DEFENDING_YOUR_TITLE      = 13423,

    SPELL_SUNDER_ARMOR              = 11971,
    SPELL_REND_VC                   = 11977
};

class npc_victorious_challenger : public CreatureScript
{
public:
    npc_victorious_challenger() : CreatureScript("npc_victorious_challenger") { }

    struct npc_victorious_challengerAI : public ScriptedAI
    {
        npc_victorious_challengerAI(Creature* pCreature) : ScriptedAI(pCreature) {}

        uint32 SunderArmorTimer;
        uint32 RendTimer;

        void Reset()
        {
            me->RestoreFaction();

            SunderArmorTimer = 10000;
            RendTimer        = 15000;
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (RendTimer < diff)
            {
                DoCast(me->getVictim(), SPELL_REND_VC, true);
                RendTimer = 15000;
            }else RendTimer -= diff;

            if (SunderArmorTimer < diff)
            {
                DoCast(me->getVictim(), SPELL_SUNDER_ARMOR, true);
                SunderArmorTimer = 10000;
            }else SunderArmorTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void KilledUnit(Unit* /*victim*/)
        {
            me->RestoreFaction();
        }

    };

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_TAKING_ALL_CHALLENGERS) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(QUEST_DEFENDING_YOUR_TITLE) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHALLENGER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            return true;
        }

        return false;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->setFaction(14);
            pCreature->AI()->AttackStart(pPlayer);
        }

        return true;
    }

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_victorious_challengerAI(creature);
    }
};

/*######
## npc_loklira_crone
######*/

#define GOSSIP_LOKLIRACRONE     "Tell me about this proposal"
#define GOSSIP_LOKLIRACRONE1    "What happened then?"
#define GOSSIP_LOKLIRACRONE2    "You want me to take part in the Hyldsmeet to end the war?"
#define GOSSIP_LOKLIRACRONE3    "Very well. I'll take part in this competition."

enum eLokliraCrone
{
    QUEST_HYLDSMEET     = 12970,

    GOSSIP_TEXTID_LOK1  = 13778,
    GOSSIP_TEXTID_LOK2  = 13779,
    GOSSIP_TEXTID_LOK3  = 13780
};

class npc_loklira_crone : public CreatureScript
{
public:
    npc_loklira_crone() : CreatureScript("npc_loklira_crone") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_HYLDSMEET) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOKLIRACRONE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            return true;
        }
        return false;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOKLIRACRONE1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_LOK1, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOKLIRACRONE2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_LOK2, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOKLIRACRONE3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_LOK3, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->CompleteQuest(QUEST_HYLDSMEET);
                break;
        }
        return true;
    }
};

/////////////////////
///npc_injured_goblin
/////////////////////

enum eInjuredGoblin
{
    QUEST_BITTER_DEPARTURE     = 12832,
    SAY_QUEST_ACCEPT           =  -1800042,
    SAY_END_WP_REACHED         =  -1800043
};

#define GOSSIP_ITEM_1       "I am ready, lets get you out of here"

class npc_injured_goblin : public CreatureScript
{
public:
    npc_injured_goblin() : CreatureScript("npc_injured_goblin") { }

    struct npc_injured_goblinAI : public npc_escortAI
    {
        npc_injured_goblinAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        void WaypointReached(uint32 i)
        {
            Player* pPlayer = GetPlayerForEscort();
            switch (i)
            {
            case 26:
                DoScriptText(SAY_END_WP_REACHED, me, pPlayer);
                break;
            case 27:
                if (pPlayer)
                    pPlayer->GroupEventHappens(QUEST_BITTER_DEPARTURE, me);
                break;
            }
        }

        void EnterCombat(Unit* /*pWho*/) {}

        void Reset() {}

        void JustDied(Unit* /*pKiller*/)
        {
            Player* pPlayer = GetPlayerForEscort();
            if (HasEscortState(STATE_ESCORT_ESCORTING) && pPlayer)
                pPlayer->FailQuest(QUEST_BITTER_DEPARTURE);
        }

       void UpdateAI(const uint32 uiDiff)
        {
            npc_escortAI::UpdateAI(uiDiff);
            if (!UpdateVictim())
                return;
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_injured_goblinAI(creature);
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_BITTER_DEPARTURE) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->PlayerTalkClass->SendGossipMenu(9999999, pCreature->GetGUID());
        }
        else
            pPlayer->SEND_GOSSIP_MENU(999999, pCreature->GetGUID());
        return true;
    }

    bool OnQuestAccept(Player* /*pPlayer*/, Creature* pCreature, Quest const *quest)
    {
        if (quest->GetQuestId() == QUEST_BITTER_DEPARTURE)
            DoScriptText(SAY_QUEST_ACCEPT, pCreature);

        return false;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        npc_escortAI* pEscortAI = CAST_AI(npc_injured_goblin::npc_injured_goblinAI, pCreature->AI());

        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pEscortAI->Start(true, true, pPlayer->GetGUID());
            pCreature->setFaction(113);
        }
        return true;
    }
};

/*######
## npc_roxi_ramrocket
######*/

#define SPELL_MECHANO_HOG           60866
#define SPELL_MEKGINEERS_CHOPPER    60867

class npc_roxi_ramrocket : public CreatureScript
{
public:
    npc_roxi_ramrocket() : CreatureScript("npc_roxi_ramrocket") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        //Quest Menu
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        //Trainer Menu
        if( pCreature->isTrainer() )
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, GOSSIP_TEXT_TRAIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);

        //Vendor Menu
        if( pCreature->isVendor() )
            if(pPlayer->HasSpell(SPELL_MECHANO_HOG) || pPlayer->HasSpell(SPELL_MEKGINEERS_CHOPPER))
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*sender*/, uint32 action)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch(action)
        {
        case GOSSIP_ACTION_TRAIN:
            pPlayer->SEND_TRAINERLIST(pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_TRADE:
            pPlayer->SEND_VENDORLIST(pCreature->GetGUID());
            break;
        }
        return true;
    }
};

/*######
## Quest: Cold Hearted (12856)
######*/

enum eColdHearted
{
    SPELL_KILL_CREDIT_PRISONER          = 55144,
    SPELL_KILL_CREDIT_DRAKE             = 55143,
    SPELL_SUMMON_LIBERATED              = 55073
};

const Position FreedDrakeWaypoints[6] =
{
    {7250.15f, -2327.22f, 869.03f, 0.0f},
    {7118.79f, -2122.05f, 841.32f, 0.0f},
    {7052.86f, -1905.99f, 888.59f, 0.0f},
    {7038.24f, -1822.77f, 857.94f, 0.0f},
    {7044.09f, -1792.25f, 841.69f, 0.0f},
    {7071.20f, -1780.73f, 822.42f, 0.0f}
};

class npc_freed_protodrake : public CreatureScript
{
public:
    npc_freed_protodrake() : CreatureScript("npc_freed_protodrake") { }

    struct npc_freed_protodrakeAI : public ScriptedAI
    {
        npc_freed_protodrakeAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint8 count;
        bool wp_reached;
        bool movementStarted;

        void Reset()
        {
            count = 0;
            wp_reached = false;
            movementStarted = false;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE || id != count)
                return;

            if (id < 5)
            {
                ++count;
                wp_reached = true;
            }
            else // reached village, give credits
            {
                Unit* player = me->GetVehicleKit()->GetPassenger(0); // get player
                if (player && player->GetTypeId() == TYPEID_PLAYER)
                {
                    for (uint8 i = 1; i < 4; ++i) // try to get prisoners
                        if (Unit* prisoner = me->GetVehicleKit()->GetPassenger(i))
                        {
                            if (prisoner->GetTypeId() != TYPEID_UNIT)
                                return;

                            DoCast(player, SPELL_KILL_CREDIT_PRISONER, true);
                            //DoCast(player, SPELL_SUMMON_LIBERATED, true);
                            prisoner->ExitVehicle();
                            prisoner->ToCreature()->AI()->DoAction(0);
                        }

                    DoCast(player, SPELL_KILL_CREDIT_DRAKE, true);
                    player->ExitVehicle();
                }
            }
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (!me->isCharmed() && !movementStarted)
            {
                me->SetSpeed(MOVE_FLIGHT, 5.0f);
                movementStarted = true;
                wp_reached = true;
            }

            if (wp_reached)
            {
                wp_reached = false;
                me->GetMotionMaster()->MovePoint(count, FreedDrakeWaypoints[count]);
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_freed_protodrakeAI(pCreature);
    }
};

/*######
## npc_brunnhildar_prisoner
######*/

enum eBrunnhildarPrisoner
{
    SPELL_ICE_BLOCK                   = 54894,
    SPELL_ICE_SHARD                   = 55046,
    SPELL_ICE_SHARD_IMPACT            = 55047
};

class npc_brunnhildar_prisoner : public CreatureScript
{
public:
    npc_brunnhildar_prisoner() : CreatureScript("npc_brunnhildar_prisoner") {}

    struct npc_brunnhildar_prisonerAI : public ScriptedAI
    {
        npc_brunnhildar_prisonerAI(Creature* pCreature) : ScriptedAI(pCreature) {}

        uint32 uiCheckTimer;

        void Reset()
        {
            uiCheckTimer = 10*IN_MILLISECONDS;
            DoCast(me, SPELL_ICE_BLOCK, true);
        }

        void DoAction(const int32 /*param*/)
        {
            me->Kill(me);
            me->Respawn();
        }

        void SpellHit(Unit* caster, const SpellEntry* spell) 
        {
            if (spell->Id == SPELL_ICE_SHARD)
            {
                DoCast(me, SPELL_ICE_SHARD_IMPACT, true);

                if (caster->IsVehicle())
                {
                    uint8 seat = caster->GetVehicleKit()->GetNextEmptySeat(0, true);
                    if (seat <= 0)
                        return;

                    me->EnterVehicle(caster);
                    me->RemoveAurasDueToSpell(SPELL_ICE_BLOCK);
                    caster->SetSpeed(MOVE_FLIGHT, 3.0f);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiCheckTimer < diff)
            {
                if (!me->HasUnitState(UNIT_STAT_ONVEHICLE))
                {
                    // return home
                    if (me->GetDistance(me->GetHomePosition()) > 30.0f)
                        DoAction(0);
                }
                else
                {
                    if (me->GetPositionY() > -2595.0f)
                    {
                        // remove player control
                        if (Unit* base = me->GetVehicleBase())
                            if (base->isCharmed())
                                base->RemoveCharmedBy(base->GetCharmer());
                    }
                }

                uiCheckTimer = 10*IN_MILLISECONDS;
            }else uiCheckTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_brunnhildar_prisonerAI(creature);
    }
};

class npc_icefang : public CreatureScript
{
public:
    npc_icefang() : CreatureScript("npc_icefang") { }

    struct npc_icefangAI : public npc_escortAI
    {
        npc_icefangAI(Creature* creature) : npc_escortAI(creature) {}

        void AttackStart(Unit* /*who*/) {}
        void EnterCombat(Unit* /*who*/) {}
        void EnterEvadeMode() {}

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (apply)
                    Start(false, true, who->GetGUID());
            }
        }

        void WaypointReached(uint32 /*wp*/)
        {
        }

        void JustDied(Unit* /*killer*/)
        {
        }

        void OnCharmed(bool /*apply*/)
        {
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_icefangAI (creature);
    }
};

void AddSC_storm_peaks()
{
    new npc_agnetta_tyrsdottar;
    new npc_frostborn_scout;
    new npc_thorim;
    new npc_goblin_prisoner;
    new npc_victorious_challenger;
    new npc_loklira_crone;
    new npc_injured_goblin;
    new npc_roxi_ramrocket;
    new npc_brunnhildar_prisoner;
    new npc_icefang;
}
