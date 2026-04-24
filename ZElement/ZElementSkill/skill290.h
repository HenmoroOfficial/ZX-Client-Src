#ifndef __CPPGEN_GNET_SKILL290
#define __CPPGEN_GNET_SKILL290
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill290:public Skill
    {
      public:
        enum
        { SKILL_ID = 290 };
          Skill290 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill290Stub:public SkillStub
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
                skill->SetPlus (28 + 3 * skill->GetLevel () + 0.5 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill290Stub ():SkillStub (290)
        {
            occupation = 4;
            name = L"ÇéÍø";
            nativename = "ÇéÍø";
            icon = "ÇéÍø.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            talent[0] = 722;
            talent[1] = 736;
            talent[2] = 744;
            talent[3] = 748;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_ÇéÍø";
            action[1] = "ºÏ»¶ÅÉ_ÇéÍø";
            action[2] = "ºÏ»¶ÅÉ_ÇéÍø";
            action[3] = "ºÏ»¶ÅÉ_ÇéÍø";
            action[4] = "ºÏ»¶ÅÉ_ÇéÍø";
            action[5] = "ºÏ»¶ÅÉ_ÇéÍø";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_ÇéÍø";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_ÇéÍø";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_ÇéÍø";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÇéÍø";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_ÇéÍø";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_ÇéÍø";
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
        virtual ~ Skill290Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 289 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12 + 0.5 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (20);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 30, 32, 34, 36, 38, 40, 42, 44, 46 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7 + 0.5 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7 + 0.5 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               0.5 * skill->GetLevel (),
                               20,
                               INT (28 + 3 * skill->GetLevel () + 0.5 * skill->GetLevel () * skill->GetLevel ()),
                               INT (37 + 3.7 * skill->GetLevel ()), 1.3 + 0.7 * skill->GetLevel ());

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
                                                 (37 + 3.7 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetT0 () + 15 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime ((1300 + 700 * skill->GetLevel () + 2000 * skill->GetT0 ()) * (1 + 0.2 * skill->GetT1 ()) *
                                          (INT (zrand (100) / (100 - 8 * skill->GetT1 ())) + 1) + 100);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (6000 * (1 + 0.3 * skill->GetT2 ()) + 100);
            skill->GetVictim ()->SetRatio ((0.08 + 0.03 * skill->GetT3 ()) * (INT (zrand (100) / (100 - 4 * skill->GetT2 ())) + 1));
            skill->GetVictim ()->SetInchurt2 (1);
            return true;
        }
#endif
    };
}
#endif
