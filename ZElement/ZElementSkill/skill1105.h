#ifndef __CPPGEN_GNET_SKILL1105
#define __CPPGEN_GNET_SKILL1105
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1105:public Skill
    {
      public:
        enum
        { SKILL_ID = 1105 };
          Skill1105 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1105Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1105Stub ():SkillStub (1105)
        {
            occupation = 26;
            name = L"ÓÄÈª¹í²½";
            nativename = "ÓÄÈª¹í²½";
            icon = "ÓÄÈª¹í²½.dds";
            maxlevel = 20;
            maxlearn = 8;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 2;
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
            skill_class = 4;
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
            talent[0] = 1162;
            talent[1] = 1168;
            talent[2] = 1170;
            talent_size = 3;
            action[0] = "ÓÄÈª¹í²½";
            action[1] = "ÓÄÈª¹í²½";
            action[2] = "ÓÄÈª¹í²½";
            action[3] = "ÓÄÈª¹í²½";
            action[4] = "ÓÄÈª¹í²½";
            action[5] = "ÓÄÈª¹í²½";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÓÄÈª¹í²½";
            action[16] = "ÓÄÈª¹í²½";
            action[17] = "ÓÄÈª¹í²½";
            action[18] = "ÓÄÈª¹í²½";
            action[19] = "ÓÄÈª¹í²½";
            action[20] = "ÓÄÈª¹í²½";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
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
        virtual ~ Skill1105Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1103 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
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
            return 180000 - 18000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 46, 49, 52, 55, 58, 61, 64, 68 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format, 0, 12 + 3 * skill->GetLevel (), 25 + 15 * skill->GetLevel (), 10 + 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime ((12000 + 3000 * skill->GetLevel () + 2000 * skill->GetT2 () + 100) *
                                          (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))));
            skill->GetVictim ()->SetRatio (0.25 + 0.15 * skill->GetLevel () + 0.15 * skill->GetT0 ());
            skill->GetVictim ()->SetAmount (2);
            skill->GetVictim ()->SetValue (10 + 5 * skill->GetLevel ());
            skill->GetVictim ()->SetFocusanti (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * skill->GetT0 ()));
            skill->GetVictim ()->SetClearwrap (1);
            return true;
        }
#endif
    };
}
#endif
