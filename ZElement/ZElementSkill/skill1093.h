#ifndef __CPPGEN_GNET_SKILL1093
#define __CPPGEN_GNET_SKILL1093
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1093:public Skill
    {
      public:
        enum
        { SKILL_ID = 1093 };
          Skill1093 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1093Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetSkilldodge ());
                skill->SetRatio (1);
                skill->SetPlus (8 * skill->GetLevel () * skill->GetT0 () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (0.014 * skill->GetLevel () * skill->GetLevel () + 5.3 * skill->GetLevel () +
                                                                          58) + INT (0.05 * skill->GetLevel () +
                                                                                     0.55) * (0.6 * skill->GetLevel () * skill->GetLevel () +
                                                                                              9.4 * skill->GetLevel () - 12));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1093Stub ():SkillStub (1093)
        {
            occupation = 25;
            name = L"ÌŞ¹ÇĞ×ÑÀ";
            nativename = "ÌŞ¹ÇĞ×ÑÀ";
            icon = "ÌŞ¹ÇĞ×ÑÀ.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 1;
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
            talent[0] = 1139;
            talent_size = 1;
            action[0] = "¹íµÀ_ÌŞ¹ÇĞ×ÑÀ";
            action[1] = "¹íµÀ_ÌŞ¹ÇĞ×ÑÀ";
            action[2] = "¹íµÀ_ÌŞ¹ÇĞ×ÑÀ";
            action[3] = "¹íµÀ_ÌŞ¹ÇĞ×ÑÀ";
            action[4] = "¹íµÀ_ÌŞ¹ÇĞ×ÑÀ";
            action[5] = "¹íµÀ_ÌŞ¹ÇĞ×ÑÀ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_ÌŞ¹ÇĞ×ÑÀ";
            action[16] = "¹íµÀ_Æï³Ë_ÌŞ¹ÇĞ×ÑÀ";
            action[17] = "¹íµÀ_Æï³Ë_ÌŞ¹ÇĞ×ÑÀ";
            action[18] = "¹íµÀ_Æï³Ë_ÌŞ¹ÇĞ×ÑÀ";
            action[19] = "¹íµÀ_Æï³Ë_ÌŞ¹ÇĞ×ÑÀ";
            action[20] = "¹íµÀ_Æï³Ë_ÌŞ¹ÇĞ×ÑÀ";
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
        virtual ~ Skill1093Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 8 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (10);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 15, 17, 19, 21, 23, 25, 27, 29, 32 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               10,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (0.014 * skill->GetLevel () * skill->GetLevel () + 5.3 * skill->GetLevel () + 58) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.6 * skill->GetLevel () * skill->GetLevel () +
                                                                              9.4 * skill->GetLevel () - 12)));

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
            skill->GetVictim ()->SetProbability (1.0 * (33 * norm (skill->GetT0 ())));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetBuffid (6);
            skill->GetVictim ()->SetValue (5 * skill->GetT0 ());
            skill->GetVictim ()->SetSubattack (1);
            return true;
        }
#endif
    };
}
#endif
