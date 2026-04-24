#ifndef __CPPGEN_GNET_SKILL3064
#define __CPPGEN_GNET_SKILL3064
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3064:public Skill
    {
      public:
        enum
        { SKILL_ID = 3064 };
          Skill3064 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3064Stub:public SkillStub
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
      Skill3064Stub ():SkillStub (3064)
        {
            occupation = 168;
            name = L"½µÁÙ";
            nativename = "½µÁÙ";
            icon = "Ð°Éñ½µÁÙ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
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
            action[0] = "Ä§_Ä§×ð½µÁÙ";
            action[1] = "Ä§_Ä§×ð½µÁÙ";
            action[2] = "Ä§_Ä§×ð½µÁÙ";
            action[3] = "Ä§_Ä§×ð½µÁÙ";
            action[4] = "Ä§_Ä§×ð½µÁÙ";
            action[5] = "Ä§_Ä§×ð½µÁÙ";
            action[6] = "Ä§_Ä§×ð½µÁÙ";
            action[7] = "Ä§_Ä§×ð½µÁÙ";
            action[8] = "Ä§_Ä§×ð½µÁÙ";
            action[9] = "Ä§_Ä§×ð½µÁÙ";
            action[10] = "Ä§_Ä§×ð½µÁÙ";
            action[11] = "Ä§_Ä§×ð½µÁÙ";
            action[12] = "Ä§_Ä§×ð½µÁÙ";
            action[13] = "Ä§_Ä§×ð½µÁÙ";
            action[14] = "Ä§_Ä§×ð½µÁÙ";
            action[15] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[16] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[17] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[18] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[19] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[20] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[21] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[22] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[23] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[24] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[25] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[26] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[27] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[28] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[29] = "Æï³Ë_Ä§_Ä§×ð½µÁÙ";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill3064Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 10300, 16900, 21800, 43300, 49900, 81300, 84500, 197600, 340200, 435500, 998600,
 1897100 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 23, 27, 31, 36, 41, 46, 51, 57, 63, 69, 75, 81 };
            return array[level - 1];
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
            return 720000 - 20000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 150 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 1);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               10 + skill->GetLevel (),
                               720 - 20 * skill->GetLevel (),
                               120 + 20 * skill->GetLevel (),
                               100 + 4 * skill->GetLevel (),
                               384 + 248 * skill->GetLevel (),
                               skill->GetLevel (),
                               5 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               30, 0.5 * skill->GetLevel (), 2 * skill->GetLevel (), 10 + 2 * skill->GetLevel (), 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (10000 + 1000 * skill->GetLevel () + 10000 * norm (skill->GetLevel () / 19.9));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (41930);
            skill->GetVictim ()->SetSummon (1);
            return true;
        }
#endif
    };
}
#endif
