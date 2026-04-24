#ifndef __CPPGEN_GNET_SKILL3570
#define __CPPGEN_GNET_SKILL3570
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3570:public Skill
    {
      public:
        enum
        { SKILL_ID = 3570 };
          Skill3570 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3570Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes4 ());
                skill->SetRatio (1.2 + 0.05 * skill->GetLevel ());
                skill->
                    SetPlus ((skill->GetPlayer ()->GetRes1 () + skill->GetPlayer ()->GetRes2 () + skill->GetPlayer ()->GetRes3 () +
                              skill->GetPlayer ()->GetRes4 () + skill->GetPlayer ()->GetRes5 ()) * 0.2 * skill->GetLevel () +
                             norm (skill->GetT0 ()) * skill->GetPlayer ()->GetMaxhp () * 0.02 * skill->GetT0 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3570Stub ():SkillStub (3570)
        {
            occupation = 167;
            name = L"Ì«Ô¨Òµº£<Ðþ>";
            nativename = "Ì«Ô¨Òµº£<Ðþ>";
            icon = "ÏÉ¡¤Ì«Ô¨Òµº£.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
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
            talent[0] = 3406;
            talent_size = 1;
            action[0] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[1] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[2] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[3] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[4] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[5] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[6] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[16] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[17] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[18] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[19] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[20] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 3;
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
        virtual ~ Skill3570Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3452 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (6 * skill->GetLevel () * skill->GetLevel () + 6 * skill->GetLevel () + 1660);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (14);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               6 * skill->GetLevel () * skill->GetLevel () + 6 * skill->GetLevel () + 1660,
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               30, 10, 15, (20 + 5 * skill->GetLevel ()), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetRes4 ()) >
                                                  0 ? (skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetRes4 ()) : 0));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (100 + skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetValue (3863);
            skill->GetVictim ()->SetDelaycast (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10);
        }
#endif
    };
}
#endif
