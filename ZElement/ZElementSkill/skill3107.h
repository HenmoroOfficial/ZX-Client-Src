#ifndef __CPPGEN_GNET_SKILL3107
#define __CPPGEN_GNET_SKILL3107
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3107:public Skill
    {
      public:
        enum
        { SKILL_ID = 3107 };
          Skill3107 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3107Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3107Stub ():SkillStub (3107)
        {
            occupation = 169;
            name = L"µ¶É½µØÓü";
            nativename = "µ¶É½µØÓü";
            icon = "Î»ÒÆ¸îÁÑ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
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
            action[0] = "·ð_ÉñÊ¥·´À¡";
            action[1] = "·ð_ÉñÊ¥·´À¡";
            action[2] = "·ð_ÉñÊ¥·´À¡";
            action[3] = "·ð_ÉñÊ¥·´À¡";
            action[4] = "·ð_ÉñÊ¥·´À¡";
            action[5] = "·ð_ÉñÊ¥·´À¡";
            action[6] = "·ð_ÉñÊ¥·´À¡";
            action[7] = "·ð_ÉñÊ¥·´À¡";
            action[8] = "·ð_ÉñÊ¥·´À¡";
            action[9] = "·ð_ÉñÊ¥·´À¡";
            action[10] = "·ð_ÉñÊ¥·´À¡";
            action[11] = "·ð_ÉñÊ¥·´À¡";
            action[12] = "·ð_ÉñÊ¥·´À¡";
            action[13] = "·ð_ÉñÊ¥·´À¡";
            action[14] = "·ð_ÉñÊ¥·´À¡";
            action[15] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[16] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[17] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[18] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[19] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[20] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[21] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[22] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[23] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[24] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[25] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[26] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[27] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[28] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
            action[29] = "Æï³Ë_·ð_ÉñÊ¥·´À¡";
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
            skill_limit = 1;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3107Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
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
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 3000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (12);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               4 + 0.5 * skill->GetLevel (),
                               120 - 3 * skill->GetLevel (), 12, INT (1 + INT (skill->GetLevel () / 4.9)), 20 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4050 + 500 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 4000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * (0.2 + 0.02 * skill->GetLevel ()) *
                                           (1 + norm (skill->GetLevel () / 19.9) * 0.2));
            skill->GetVictim ()->SetDamagemove (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (1 + INT (skill->GetLevel () / 4.9));
        }
#endif
    };
}
#endif
