#ifndef __CPPGEN_GNET_SKILL3012
#define __CPPGEN_GNET_SKILL3012
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3012:public Skill
    {
      public:
        enum
        { SKILL_ID = 3012 };
          Skill3012 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3012Stub:public SkillStub
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
                skill->SetRatio (0);
                skill->SetSaint (skill->GetPlayer ()->GetSaint () * (1 + 0.3 * skill->GetLevel ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3012Stub ():SkillStub (3012)
        {
            occupation = 167;
            name = L"´ºÀ×Õð";
            nativename = "´ºÀ×Õð";
            icon = "±ù±©.dds";
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
            clearmask = 1;
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
            action[0] = "ÏÉ_±ù·âÊõ";
            action[1] = "ÏÉ_±ù·âÊõ";
            action[2] = "ÏÉ_±ù·âÊõ";
            action[3] = "ÏÉ_±ù·âÊõ";
            action[4] = "ÏÉ_±ù·âÊõ";
            action[5] = "ÏÉ_±ù·âÊõ";
            action[6] = "ÏÉ_±ù·âÊõ";
            action[7] = "ÏÉ_±ù·âÊõ";
            action[8] = "ÏÉ_±ù·âÊõ";
            action[9] = "ÏÉ_±ù·âÊõ";
            action[10] = "ÏÉ_±ù·âÊõ";
            action[11] = "ÏÉ_±ù·âÊõ";
            action[12] = "ÏÉ_±ù·âÊõ";
            action[13] = "ÏÉ_±ù·âÊõ";
            action[14] = "ÏÉ_±ù·âÊõ";
            action[15] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[16] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[17] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[18] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[19] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[20] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[21] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[22] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[23] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[24] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[25] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[26] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[27] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[28] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[29] = "Æï³Ë_ÏÉ_±ù·âÊõ";
            action[30] = "0";
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
        virtual ~ Skill3012Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 2400, 3500, 5600, 8000, 9100, 21700, 36400, 51500, 77500, 79800, 179300, 290600, 437200, 872700, 1620700 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 12, 15, 18, 22, 26, 30, 35, 40, 45, 50, 56, 62, 68, 74, 80 };
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
            return 180000 - 4000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               180 - 4 * skill->GetLevel (), 1 + 0.3 * skill->GetLevel (), 10 + 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetIsfrozen ()? 120 : 0));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMp () * 0.2);
            skill->GetVictim ()->SetDirecthurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (10 + 4 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetIsfrozen ()? 4000 : 0);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetFrozen (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (5 + skill->GetLevel ());
        }
#endif
    };
}
#endif
