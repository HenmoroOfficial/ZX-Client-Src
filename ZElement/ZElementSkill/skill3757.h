#ifndef __CPPGEN_GNET_SKILL3757
#define __CPPGEN_GNET_SKILL3757
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3757:public Skill
    {
      public:
        enum
        { SKILL_ID = 3757 };
          Skill3757 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3757Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 3500;
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
                return 5000;
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
                return 1500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3757Stub ():SkillStub (3757)
        {
            occupation = 150;
            name = L"°ÔÐ°ÕóII";
            nativename = "°ÔÐ°ÕóII";
            icon = "°ÔÐ°ÕóII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 3;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 132;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 1;
            cyclegfx = "·¨Õó¹ÌÕó";
            cyclemode = 1;
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
            talent[0] = 3408;
            talent[1] = 3758;
            talent[2] = 3419;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[1] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[2] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[3] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[4] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[5] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[6] = "·ÙÏã¹È_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[16] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[17] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[18] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[19] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[20] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[21] = "·ÙÏã¹È_Æï³Ë_Õó·¨¾«ÑÐ¡¾¹ÌÕó¡¿";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = true;
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill3757Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3432 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1200);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 10000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 900000 - 900000 * 0.02 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (28);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 5 * skill->GetLevel (), 3 * skill->GetLevel (), 10, 10, 10);

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
            skill->GetVictim ()->SetProbability (1.0 * (20 * skill->GetT0 ()));
            skill->GetVictim ()->SetBreakcasting (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (101));
            skill->GetVictim ()->SetTime (1000 *
                                          (5 * skill->GetLevel () + 10 * skill->GetPlayer ()->GetCyclemembercnt () - 10 +
                                           10 * int (skill->GetCharging () / 4800)) +1000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (3 * skill->GetLevel () + 5 * skill->GetT2 () + 10 * skill->GetPlayer ()->GetCyclemembercnt () - 10);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (3604);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetCycle (1);
            skill->GetVictim ()->SetTime (500);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetEvilaura (1);
            skill->GetVictim ()->SetRatio (zrand (100) < skill->GetPlayer ()->GetSkilllevel (3759) * 2 ? 3449 : 218);
            skill->GetVictim ()->SetAmount (218);
            skill->GetVictim ()->SetValue (218);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
