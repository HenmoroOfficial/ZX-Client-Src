#ifndef __CPPGEN_GNET_SKILL301
#define __CPPGEN_GNET_SKILL301
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill301:public Skill
    {
      public:
        enum
        { SKILL_ID = 301 };
          Skill301 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill301Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill301Stub ():SkillStub (301)
        {
            occupation = 6;
            name = L"ÎÂÈá";
            nativename = "ÎÂÈá";
            icon = "ÎÂÈá.dds";
            maxlevel = 20;
            maxlearn = 8;
            type = 3;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 1;
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
            talent[0] = 727;
            talent[1] = 765;
            talent[2] = 768;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_ÎÂÈá";
            action[1] = "ºÏ»¶ÅÉ_ÎÂÈá";
            action[2] = "ºÏ»¶ÅÉ_ÎÂÈá";
            action[3] = "ºÏ»¶ÅÉ_ÎÂÈá";
            action[4] = "ºÏ»¶ÅÉ_ÎÂÈá";
            action[5] = "ºÏ»¶ÅÉ_ÎÂÈá";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_ÎÂÈá";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_ÎÂÈá";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_ÎÂÈá";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÎÂÈá";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_ÎÂÈá";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_ÎÂÈá";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill301Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 306 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.9 * skill->GetLevel () * skill->GetLevel () + 3.8 * skill->GetLevel () + 502);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000 - 1000 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 80, 82, 84, 86, 88, 90, 92, 94 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (6.5 + 0.5 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (6.5 + 0.5 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.9 * skill->GetLevel () * skill->GetLevel () + 3.8 * skill->GetLevel () + 502),
                               5.5 + 0.5 * skill->GetLevel (), INT (82 + 7.9 * skill->GetLevel ()), 1.8 + 1.2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (82 + 7.9 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetT0 ()));
            skill->GetVictim ()->SetTime ((1800 + 1200 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT1 ()) + 100);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (3050 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (35 * skill->GetT1 ());
            skill->GetVictim ()->SetSubdefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
#endif
    };
}
#endif
