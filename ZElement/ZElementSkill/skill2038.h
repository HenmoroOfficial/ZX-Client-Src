#ifndef __CPPGEN_GNET_SKILL2038
#define __CPPGEN_GNET_SKILL2038
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2038:public Skill
    {
      public:
        enum
        { SKILL_ID = 2038 };
          Skill2038 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2038Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 ((skill->GetPlayer ()->GetOccupation () == 10 ? 1 : 0));
                skill->GetPlayer ()->SetVar2 ((skill->GetPlayer ()->GetOccupation () == 11 ? 1 : 0));
                skill->GetPlayer ()->SetVar3 ((skill->GetPlayer ()->GetOccupation () == 12 ? 1 : 0));
                skill->GetPlayer ()->SetVar4 ((skill->GetPlayer ()->GetOccupation () == 22 ? 1 : 0));
                skill->GetPlayer ()->SetVar5 ((skill->GetPlayer ()->GetOccupation () == 23 ? 1 : 0));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2038Stub ():SkillStub (2038)
        {
            occupation = 149;
            name = L"Ä§Ö®ÊØ»¤";
            nativename = "Ä§Ö®ÊØ»¤";
            icon = "Ä§Ö®ÊØ»¤.dds";
            maxlevel = 30;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
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
            action[0] = "¹íµÀ_ÊØ»¤";
            action[1] = "ÇàÔÆÃÅ_ÊØ»¤";
            action[2] = "¹íÍõ×Ú_ÊØ»¤";
            action[3] = "ºÏ»¶ÅÉ_ÊØ»¤";
            action[4] = "ÌìÒôËÂ_ÊØ»¤";
            action[5] = "¹íµÀ_ÊØ»¤";
            action[6] = "·ÙÏã_ÊØ»¤";
            action[7] = "ÊŞÉñ_ÊØ»¤";
            action[8] = "ºüÑı_ÊØ»¤";
            action[9] = "ĞùÔ¯_ÊØ»¤";
            action[10] = "»³¹â_ÊØ»¤";
            action[11] = "Ì«ê»_ÊØ»¤";
            action[12] = "Ìì»ª_ÊØ»¤";
            action[13] = "ÁéÙí_ÊØ»¤";
            action[14] = "Ó¢ÕĞ_ÊØ»¤";
            action[15] = "¹íµÀ_Æï³Ë_ÊØ»¤";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÊØ»¤";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÊØ»¤";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÊØ»¤";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÊØ»¤";
            action[20] = "¹íµÀ_Æï³Ë_ÊØ»¤";
            action[21] = "·ÙÏã_Æï³Ë_ÊØ»¤";
            action[22] = "ÊŞÉñ_Æï³Ë_ÊØ»¤";
            action[23] = "ºüÑı_Æï³Ë_ÊØ»¤";
            action[24] = "ĞùÔ¯_Æï³Ë_ÊØ»¤";
            action[25] = "»³¹â_Æï³Ë_ÊØ»¤";
            action[26] = "Ì«ê»_Æï³Ë_ÊØ»¤";
            action[27] = "Ìì»ª_Æï³Ë_ÊØ»¤";
            action[28] = "ÁéÙí_Æï³Ë_ÊØ»¤";
            action[29] = "Ó¢ÕĞ_Æï³Ë_ÊØ»¤";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 1;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2038Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1546 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 900 * 1000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 130, 135, 140 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
        int GetCreditCost (int level) const
        {
            static int array[20] =
                { 12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000, 21000, 22000, 24000, 25500, 27500, 28500, 30000, 32000, 34000, 36000,
 40000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1000, 5 + 0.5 * skill->GetLevel (), 10 + 2 * skill->GetLevel (), 100 * skill->GetLevel (), 10, 900);

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime ((10 + 2 * skill->GetLevel ()) * 1000);
            skill->GetVictim ()->
                SetRatio ((skill->GetPlayer ()->GetVar1 () == 1 || skill->GetPlayer ()->GetVar2 () == 1 || skill->GetPlayer ()->GetVar3 () == 1
                           || skill->GetPlayer ()->GetVar4 () == 1 || skill->GetPlayer ()->GetVar5 () == 1) ? 1 : 0);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((skill->GetPlayer ()->GetCultivation () == 2 ? 2 : 1) * (0.05 + 0.005 * skill->GetLevel ()) *
                                           skill->GetPlayer ()->GetMaxhp () * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime ((10 + 2 * skill->GetLevel ()) * 1000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((skill->GetPlayer ()->GetCultivation () == 2 ? 2 : 1) * (0.05 + 0.005 * skill->GetLevel ()) *
                                           skill->GetPlayer ()->GetMaxmp () * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetMpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetCultivation () == 2 ? 120 : 0));
            skill->GetVictim ()->SetTime (10 * 1000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue (100 * skill->GetLevel ());
            skill->GetVictim ()->SetAdddefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
