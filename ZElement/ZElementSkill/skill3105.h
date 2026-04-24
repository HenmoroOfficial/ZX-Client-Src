#ifndef __CPPGEN_GNET_SKILL3105
#define __CPPGEN_GNET_SKILL3105
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3105:public Skill
    {
      public:
        enum
        { SKILL_ID = 3105 };
          Skill3105 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3105Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1500;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3105Stub ():SkillStub (3105)
        {
            occupation = 169;
            name = L"ÆÐÌáÐÄ¾­";
            nativename = "ÆÐÌáÐÄ¾­";
            icon = "ÉñÊ¥Ö®Ô´.dds";
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
            clearmask = 4;
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
            action[0] = "·ð_ÉñÊ¥Ö®Å­";
            action[1] = "·ð_ÉñÊ¥Ö®Å­";
            action[2] = "·ð_ÉñÊ¥Ö®Å­";
            action[3] = "·ð_ÉñÊ¥Ö®Å­";
            action[4] = "·ð_ÉñÊ¥Ö®Å­";
            action[5] = "·ð_ÉñÊ¥Ö®Å­";
            action[6] = "·ð_ÉñÊ¥Ö®Å­";
            action[7] = "·ð_ÉñÊ¥Ö®Å­";
            action[8] = "·ð_ÉñÊ¥Ö®Å­";
            action[9] = "·ð_ÉñÊ¥Ö®Å­";
            action[10] = "·ð_ÉñÊ¥Ö®Å­";
            action[11] = "·ð_ÉñÊ¥Ö®Å­";
            action[12] = "·ð_ÉñÊ¥Ö®Å­";
            action[13] = "·ð_ÉñÊ¥Ö®Å­";
            action[14] = "·ð_ÉñÊ¥Ö®Å­";
            action[15] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[16] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[17] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[18] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[19] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[20] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[21] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[22] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[23] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[24] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[25] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[26] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[27] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[28] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
            action[29] = "Æï³Ë_·ð_ÉñÊ¥Ö®Å­";
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
        virtual ~ Skill3105Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
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
            return 200000 - 5000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
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
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               20 + skill->GetLevel (), 200 - 5 * skill->GetLevel (), 48 * skill->GetLevel (), 20 + 8 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (20000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (24 * skill->GetLevel () * (20 + skill->GetLevel ()) * (1 + norm (skill->GetLevel () / 19.9) * 0.2));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDivinityfury (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (20000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (20 + 8 * skill->GetLevel ());
            skill->GetVictim ()->SetDivinityburst (1);
            return true;
        }
#endif
    };
}
#endif
