#ifndef __CPPGEN_GNET_SKILL770
#define __CPPGEN_GNET_SKILL770
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill770:public Skill
    {
      public:
        enum
        { SKILL_ID = 770 };
          Skill770 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill770Stub:public SkillStub
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
      Skill770Stub ():SkillStub (770)
        {
            occupation = 4;
            name = L"²»»Ú";
            nativename = "²»»Ú";
            icon = "²»»Ú.dds";
            maxlevel = 6;
            maxlearn = 3;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            talent_type = 1;
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
            action[0] = "ºÏ»¶ÅÉ_²»»Ú";
            action[1] = "ºÏ»¶ÅÉ_²»»Ú";
            action[2] = "ºÏ»¶ÅÉ_²»»Ú";
            action[3] = "ºÏ»¶ÅÉ_²»»Ú";
            action[4] = "ºÏ»¶ÅÉ_²»»Ú";
            action[5] = "ºÏ»¶ÅÉ_²»»Ú";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_²»»Ú";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_²»»Ú";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_²»»Ú";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_²»»Ú";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_²»»Ú";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_²»»Ú";
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
        virtual ~ Skill770Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 767 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (220 + 20 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[3] = { 120, 125, 130 };
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
            return _snwprintf (buffer, length, format,
                               220 + 20 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               14 + skill->GetLevel (), 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (14000 + 1000 * skill->GetLevel () + 1000);
            skill->GetVictim ()->SetRatio (0.0401 * skill->GetLevel ());
            skill->GetVictim ()->SetInccritrate (1);
            return true;
        }
#endif
    };
}
#endif
