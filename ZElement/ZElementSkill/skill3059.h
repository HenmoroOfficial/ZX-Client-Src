#ifndef __CPPGEN_GNET_SKILL3059
#define __CPPGEN_GNET_SKILL3059
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3059:public Skill
    {
      public:
        enum
        { SKILL_ID = 3059 };
          Skill3059 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3059Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1400;
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3059Stub ():SkillStub (3059)
        {
            occupation = 168;
            name = L"ÒµÕÏ";
            nativename = "ÒµÕÏ";
            icon = "ºÚ°µÊõ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 1;
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
            preskillnum = 0;
            action[0] = "Ä§_ºÚ°µÊõ";
            action[1] = "Ä§_ºÚ°µÊõ";
            action[2] = "Ä§_ºÚ°µÊõ";
            action[3] = "Ä§_ºÚ°µÊõ";
            action[4] = "Ä§_ºÚ°µÊõ";
            action[5] = "Ä§_ºÚ°µÊõ";
            action[6] = "Ä§_ºÚ°µÊõ";
            action[7] = "Ä§_ºÚ°µÊõ";
            action[8] = "Ä§_ºÚ°µÊõ";
            action[9] = "Ä§_ºÚ°µÊõ";
            action[10] = "Ä§_ºÚ°µÊõ";
            action[11] = "Ä§_ºÚ°µÊõ";
            action[12] = "Ä§_ºÚ°µÊõ";
            action[13] = "Ä§_ºÚ°µÊõ";
            action[14] = "Ä§_ºÚ°µÊõ";
            action[15] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[16] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[17] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[18] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[19] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[20] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[21] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[22] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[23] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[24] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[25] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[26] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[27] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[28] = "Æï³Ë_Ä§_ºÚ°µÊõ";
            action[29] = "Æï³Ë_Ä§_ºÚ°µÊõ";
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
        virtual ~ Skill3059Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 360100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 4000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               2 + 0.3 * skill->GetLevel (), 120 - 4 * skill->GetLevel (), 12 + skill->GetLevel (), 20 + 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 + 3 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 30));
            skill->GetVictim ()->SetTime (2000 + 300 * skill->GetLevel () + 4000 * norm (skill->GetLevel () / 19.9));
            skill->GetVictim ()->SetDarkness (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetIsignite ()? 120 : 0));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (1000);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetHp () * 0.2);
            skill->GetVictim ()->SetSubhp (1);
            return true;
        }
#endif
    };
}
#endif
