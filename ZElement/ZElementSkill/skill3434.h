#ifndef __CPPGEN_GNET_SKILL3434
#define __CPPGEN_GNET_SKILL3434
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3434:public Skill
    {
      public:
        enum
        { SKILL_ID = 3434 };
          Skill3434 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3434Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMp ());
                skill->SetRatio (1.0 + 0.15 * skill->GetLevel () + 0.15 * skill->GetT0 () + 0.1 * skill->GetT1 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3434Stub ():SkillStub (3434)
        {
            occupation = 66;
            name = L"Ñ×±øÖË»ê";
            nativename = "Ñ×±øÖË»ê";
            icon = "Ñ×±øÖË»ê.dds";
            maxlevel = 20;
            maxlearn = 6;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 3385;
            talent[1] = 3754;
            talent_size = 2;
            action[0] = "·ÙÏã¹È_»ğÑæ³Í½ä";
            action[1] = "·ÙÏã¹È_»ğÑæ³Í½ä";
            action[2] = "·ÙÏã¹È_»ğÑæ³Í½ä";
            action[3] = "·ÙÏã¹È_»ğÑæ³Í½ä";
            action[4] = "·ÙÏã¹È_»ğÑæ³Í½ä";
            action[5] = "·ÙÏã¹È_»ğÑæ³Í½ä";
            action[6] = "·ÙÏã¹È_»ğÑæ³Í½ä";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_»ğÑæ³Í½ä";
            action[16] = "·ÙÏã¹È_Æï³Ë_»ğÑæ³Í½ä";
            action[17] = "·ÙÏã¹È_Æï³Ë_»ğÑæ³Í½ä";
            action[18] = "·ÙÏã¹È_Æï³Ë_»ğÑæ³Í½ä";
            action[19] = "·ÙÏã¹È_Æï³Ë_»ğÑæ³Í½ä";
            action[20] = "·ÙÏã¹È_Æï³Ë_»ğÑæ³Í½ä";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ğÑæ³Í½ä";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3434Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3433 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1375);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 26000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 80, 84, 88, 90, 96, 100 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (11);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (9);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, (100 + 15 * skill->GetLevel ()), 3 + 2 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHasmultbuff (113) == 1 ? 120 : 0));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar2 () * (0.03 + 0.02 * skill->GetLevel ()) *
                                           (int (skill->GetPlayer ()->GetHasbuff (5430) == 1 ? 1 : 0) +
                                            int (skill->GetPlayer ()->GetHasbuff (5431) == 1 ? 1 : 0) + int (skill->GetPlayer ()->GetHasbuff (5432) ==
                                                                                                             1 ? 1 : 0) +
                                            int (skill->GetPlayer ()->GetHasbuff (5433) == 1 ? 1 : 0) + int (skill->GetPlayer ()->GetHasbuff (5434) ==
                                                                                                             1 ? 1 : 0) +
                                            int (skill->GetPlayer ()->GetHasbuff (5435) == 1 ? 1 : 0) + int (skill->GetPlayer ()->GetHasbuff (5436) ==
                                                                                                             1 ? 1 : 0) +
                                            int (skill->GetPlayer ()->GetHasbuff (5437) == 1 ? 1 : 0) + int (skill->GetPlayer ()->GetHasbuff (5438) ==
                                                                                                             1 ? 1 : 0) +
                                            int (skill->GetPlayer ()->GetHasbuff (5439) == 1 ? 1 : 0)));
            skill->GetVictim ()->SetDirecthurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (12100);
            skill->GetVictim ()->SetRatio (6);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (6);
            skill->GetVictim ()->SetValue (0.15 * skill->GetT0 () * skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
#endif
    };
}
#endif
