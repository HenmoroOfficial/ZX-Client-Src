#ifndef __CPPGEN_GNET_SKILL551
#define __CPPGEN_GNET_SKILL551
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill551:public Skill
    {
      public:
        enum
        { SKILL_ID = 551 };
          Skill551 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill551Stub:public SkillStub
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
      Skill551Stub ():SkillStub (551)
        {
            occupation = 22;
            name = L"Òò¹ûÑ­»·";
            nativename = "Òò¹ûÑ­»·";
            icon = "Òò¹ûÑ­»·.dds";
            maxlevel = 20;
            maxlearn = 8;
            type = 2;
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
            talent[0] = 683;
            talent[1] = 701;
            talent_size = 2;
            action[0] = "ÌìÒôËÂ_Ò»ºÈ";
            action[1] = "ÌìÒôËÂ_Ò»ºÈ";
            action[2] = "ÌìÒôËÂ_Ò»ºÈ";
            action[3] = "ÌìÒôËÂ_Ò»ºÈ";
            action[4] = "ÌìÒôËÂ_Ò»ºÈ";
            action[5] = "ÌìÒôËÂ_Ò»ºÈ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Ò»ºÈ";
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
            dobless = true;
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
        virtual ~ Skill551Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 548 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (600 + 15 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 115, 118, 121, 124, 125, 125, 125, 125 };
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
                               600 + 15 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               2 * skill->GetLevel (), 50, 10, 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (600000 + 600000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio ((0.02 + 0.01 * skill->GetT1 ()) * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (50 + 10 * skill->GetT0 ());
            skill->GetVictim ()->SetRetort (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (5000 * skill->GetLevel () + 5000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.02);
            skill->GetVictim ()->SetValue (1);
            skill->GetVictim ()->SetAutorescurrect (1);
            return true;
        }
#endif
    };
}
#endif
