#ifndef __CPPGEN_GNET_SKILL4531
#define __CPPGEN_GNET_SKILL4531
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4531:public Skill
    {
      public:
        enum
        { SKILL_ID = 4531 };
          Skill4531 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4531Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2400;
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
      Skill4531Stub ():SkillStub (4531)
        {
            occupation = 10;
            name = L"ÍùÉúÖä";
            nativename = "ÍùÉúÖä";
            icon = "ÍùÉúÖä.dds";
            maxlevel = 7;
            maxlearn = 6;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 1;
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
            skill_class = 3;
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
            talent[0] = 688;
            talent_size = 1;
            action[0] = "ÌìÒôËÂ_ÍùÉúÖä";
            action[1] = "ÌìÒôËÂ_ÍùÉúÖä";
            action[2] = "ÌìÒôËÂ_ÍùÉúÖä";
            action[3] = "ÌìÒôËÂ_ÍùÉúÖä";
            action[4] = "ÌìÒôËÂ_ÍùÉúÖä";
            action[5] = "ÌìÒôËÂ_ÍùÉúÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_ÍùÉúÖä";
            action[16] = "ÌìÒôËÂ_Æï³Ë_ÍùÉúÖä";
            action[17] = "ÌìÒôËÂ_Æï³Ë_ÍùÉúÖä";
            action[18] = "ÌìÒôËÂ_Æï³Ë_ÍùÉúÖä";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÍùÉúÖä";
            action[20] = "ÌìÒôËÂ_Æï³Ë_ÍùÉúÖä";
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
        virtual ~ Skill4531Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 266 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (16 + 2 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.3 * skill->GetLevel () * skill->GetLevel () + 29 * skill->GetLevel () + 134);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 360000 - 60000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 25, 28, 31, 34, 37, 40 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14 + 2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12 + 2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.3 * skill->GetLevel () * skill->GetLevel () + 29 * skill->GetLevel () + 134),
                               5 + 10 * skill->GetLevel (), 360 - 60 * skill->GetLevel (), 12 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetRatio (0.02 - 0.01 * INT (skill->GetLevel () / 5.5));
            skill->GetVictim ()->SetValue (0.05 + 0.1 * skill->GetLevel () + 0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetSalvation (1);
            return true;
        }
#endif
    };
}
#endif
