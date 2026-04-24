#ifndef __CPPGEN_GNET_SKILL1098
#define __CPPGEN_GNET_SKILL1098
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1098:public Skill
    {
      public:
        enum
        { SKILL_ID = 1098 };
          Skill1098 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1098Stub:public SkillStub
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
      Skill1098Stub ():SkillStub (1098)
        {
            occupation = 25;
            name = L"Ä§ÔªĞ°Öä";
            nativename = "Ä§ÔªĞ°Öä";
            icon = "Ä§ÔªĞ°Öä.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 7;
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
            talent[0] = 1175;
            talent_size = 1;
            action[0] = "¹íµÀ_Ä§ÔªĞ°Öä";
            action[1] = "¹íµÀ_Ä§ÔªĞ°Öä";
            action[2] = "¹íµÀ_Ä§ÔªĞ°Öä";
            action[3] = "¹íµÀ_Ä§ÔªĞ°Öä";
            action[4] = "¹íµÀ_Ä§ÔªĞ°Öä";
            action[5] = "¹íµÀ_Ä§ÔªĞ°Öä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_Ä§ÔªĞ°Öä";
            action[16] = "¹íµÀ_Æï³Ë_Ä§ÔªĞ°Öä";
            action[17] = "¹íµÀ_Æï³Ë_Ä§ÔªĞ°Öä";
            action[18] = "¹íµÀ_Æï³Ë_Ä§ÔªĞ°Öä";
            action[19] = "¹íµÀ_Æï³Ë_Ä§ÔªĞ°Öä";
            action[20] = "¹íµÀ_Æï³Ë_Ä§ÔªĞ°Öä";
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
        virtual ~ Skill1098Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1094 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
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
            return 6000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 25, 30, 35, 40, 45, 50, 55, 60, 65 };
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
            return _snwprintf (buffer, length, format, 0, 10, 60 + 120 * skill->GetLevel () + 6 * skill->GetLevel () * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (600000 + 600000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue ((60 + 120 * skill->GetLevel () + 6 * skill->GetLevel () * skill->GetLevel ()) *
                                           (1 + 0.1 * skill->GetT0 ()));
            skill->GetVictim ()->SetAddmp (1);
            return true;
        }
#endif
    };
}
#endif
