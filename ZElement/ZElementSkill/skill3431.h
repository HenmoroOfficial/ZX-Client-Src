#ifndef __CPPGEN_GNET_SKILL3431
#define __CPPGEN_GNET_SKILL3431
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3431:public Skill
    {
      public:
        enum
        { SKILL_ID = 3431 };
          Skill3431 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3431Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3431Stub ():SkillStub (3431)
        {
            occupation = 65;
            name = L"Òµ»ğÉ±";
            nativename = "Òµ»ğÉ±";
            icon = "Òµ»ğÉ±.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 3;
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
            talent[0] = 3412;
            talent[1] = 3413;
            talent[2] = 3396;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_»ğÑæ½á½ç";
            action[1] = "·ÙÏã¹È_»ğÑæ½á½ç";
            action[2] = "·ÙÏã¹È_»ğÑæ½á½ç";
            action[3] = "·ÙÏã¹È_»ğÑæ½á½ç";
            action[4] = "·ÙÏã¹È_»ğÑæ½á½ç";
            action[5] = "·ÙÏã¹È_»ğÑæ½á½ç";
            action[6] = "·ÙÏã¹È_»ğÑæ½á½ç";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_»ğÑæ½á½ç";
            action[16] = "·ÙÏã¹È_Æï³Ë_»ğÑæ½á½ç";
            action[17] = "·ÙÏã¹È_Æï³Ë_»ğÑæ½á½ç";
            action[18] = "·ÙÏã¹È_Æï³Ë_»ğÑæ½á½ç";
            action[19] = "·ÙÏã¹È_Æï³Ë_»ğÑæ½á½ç";
            action[20] = "·ÙÏã¹È_Æï³Ë_»ğÑæ½á½ç";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ğÑæ½á½ç";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 5;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 1024;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill3431Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3430 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (342 + 6 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 * (1 - 0.1 * skill->GetT2 ());
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 55, 57, 59, 61, 63, 65, 67, 70, 73 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (30);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 342 + 6 * skill->GetLevel (), (8 + 3 * skill->GetLevel ()));

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetCharging () > 1950 && skill->GetLevel () >= 9 ? 120 : 0));
            skill->GetVictim ()->SetTime ((8 + 3 * skill->GetLevel () + 3 * skill->GetT1 ()) * 1000);
            skill->GetVictim ()->SetRatio (20);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (45507);
            skill->GetVictim ()->SetValue (354);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetCharging () > 1950 && skill->GetLevel () >= 9 ? 120 : 0));
            skill->GetVictim ()->SetTime ((8 + 3 * skill->GetLevel () + 3 * skill->GetT1 ()) * 1000);
            skill->GetVictim ()->SetRatio (20);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (45507);
            skill->GetVictim ()->SetValue (6);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetCharging () > 1950 && skill->GetLevel () >= 9 ? 120 : 0));
            skill->GetVictim ()->SetTime ((8 + 3 * skill->GetLevel () + 3 * skill->GetT1 ()) * 1000);
            skill->GetVictim ()->SetRatio (24);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (45507);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetCharging () > 1950 && skill->GetLevel () >= 9 ? 120 : 0));
            skill->GetVictim ()->SetTime ((8 + 3 * skill->GetLevel () + 3 * skill->GetT1 ()) * 1000);
            skill->GetVictim ()->SetRatio (21.5);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50297);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime ((8 + 3 * skill->GetLevel () + 3 * skill->GetT1 ()) * 1000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (45507);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetCharging () > 500 && skill->GetLevel () >= 3 ? 120 : 0));
            skill->GetVictim ()->SetTime ((8 + 3 * skill->GetLevel () + 3 * skill->GetT1 ()) * 1000);
            skill->GetVictim ()->SetRatio (5);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (45507);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetCharging () > 1000 && skill->GetLevel () >= 5 ? 120 : 0));
            skill->GetVictim ()->SetTime ((8 + 3 * skill->GetLevel () + 3 * skill->GetT1 ()) * 1000);
            skill->GetVictim ()->SetRatio (10);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50295);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetCharging () > 1500 && skill->GetLevel () >= 7 ? 120 : 0));
            skill->GetVictim ()->SetTime ((8 + 3 * skill->GetLevel () + 3 * skill->GetT1 ()) * 1000);
            skill->GetVictim ()->SetRatio (15);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (50296);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6);
        }
#endif
    };
}
#endif
