#ifndef __CPPGEN_GNET_SKILL3061
#define __CPPGEN_GNET_SKILL3061
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3061:public Skill
    {
      public:
        enum
        { SKILL_ID = 3061 };
          Skill3061 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3061Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetHp (skill->GetPlayer ()->GetHp () - 1050 - 100 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3061Stub ():SkillStub (3061)
        {
            occupation = 168;
            name = L"»ÃÃð<É·>";
            nativename = "»ÃÃð<É·>";
            icon = "Ä§¡¤»ÃÃð.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 2;
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
            action[0] = "ÇàÔÆÃÅ_»ÃÃð";
            action[1] = "ÇàÔÆÃÅ_»ÃÃð";
            action[2] = "¹íÍõ×Ú_»ÃÃð";
            action[3] = "ºÏ»¶ÅÉ_»ÃÃð";
            action[4] = "ÌìÒôËÂ_»ÃÃð";
            action[5] = "¹íµÀ_»ÃÃð";
            action[6] = "·ÙÏã¹È_»ÃÃð";
            action[7] = "ÊÞÉñ_»ÃÃð";
            action[8] = "ºüÑý_»ÃÃð";
            action[9] = "ÐùÔ¯_»ÃÃð";
            action[10] = "»³¹â_»ÃÃð";
            action[11] = "Ì«ê»_»ÃÃð";
            action[12] = "Ìì»ª_»ÃÃð";
            action[13] = "ÁéÙí_»ÃÃð";
            action[14] = "Ó¢ÕÐ_»ÃÃð";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_»ÃÃð";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_»ÃÃð";
            action[17] = "¹íÍõ×Ú_Æï³Ë_»ÃÃð";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_»ÃÃð";
            action[19] = "ÌìÒôËÂ_Æï³Ë_»ÃÃð";
            action[20] = "¹íµÀ_Æï³Ë_»ÃÃð";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ÃÃð";
            action[22] = "ÊÞÉñ_Æï³Ë_»ÃÃð";
            action[23] = "ºüÑý_Æï³Ë_»ÃÃð";
            action[24] = "ÐùÔ¯_Æï³Ë_»ÃÃð";
            action[25] = "»³¹â_Æï³Ë_»ÃÃð";
            action[26] = "Ì«ê»_Æï³Ë_»ÃÃð";
            action[27] = "Ìì»ª_Æï³Ë_»ÃÃð";
            action[28] = "ÁéÙí_Æï³Ë_»ÃÃð";
            action[29] = "Ó¢ÕÐ_Æï³Ë_»ÃÃð";
            action[30] = "0";
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
        virtual ~ Skill3061Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 9100, 10800, 20700, 26500, 39600, 52900, 84300, 119800, 229700, 389600, 525000,
 1237600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 42, 47, 53, 59, 65, 71, 77 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2100 + 400 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 720000 - 20000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (17);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               2100 + 400 * skill->GetLevel (),
                               1050 + 100 * skill->GetLevel (),
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               720 - 20 * skill->GetLevel (),
                               20 + 4 * skill->GetLevel (),
                               skill->GetLevel (), 5 + skill->GetLevel (), 2 + 0.5 * skill->GetLevel (), 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 + 4 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetLevel () * skill->GetPlayer ()->GetVar1 () +
                                           10 * norm (skill->GetLevel () / 19.9) * skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetBleeding (1);
            skill->GetVictim ()->SetProbability (1.0 * (zrand (100) < 15 + skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 5 ? 800 : 0));
            skill->GetVictim ()->SetTime (2000 + 500 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (200000);
            skill->GetVictim ()->SetRatio (3015);
            skill->GetVictim ()->SetAmount (3013);
            skill->GetVictim ()->SetValue (3013);
            skill->GetVictim ()->SetSetcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (200000);
            skill->GetVictim ()->SetRatio (3010);
            skill->GetVictim ()->SetAmount (3010);
            skill->GetVictim ()->SetValue (3010);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (30);
        }
#endif
    };
}
#endif
