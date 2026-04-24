#ifndef __CPPGEN_GNET_SKILL1566
#define __CPPGEN_GNET_SKILL1566
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1566:public Skill
    {
      public:
        enum
        { SKILL_ID = 1566 };
          Skill1566 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1566Stub:public SkillStub
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
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1566Stub ():SkillStub (1566)
        {
            occupation = 149;
            name = L"´ó±¯ÖäII";
            nativename = "´ó±¯ÖäII";
            icon = "´ó±¯ÖäII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
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
            action[0] = "ÌìÒôËÂ_´ó±¯Öä";
            action[1] = "ÌìÒôËÂ_´ó±¯Öä";
            action[2] = "ÌìÒôËÂ_´ó±¯Öä";
            action[3] = "ÌìÒôËÂ_´ó±¯Öä";
            action[4] = "ÌìÒôËÂ_´ó±¯Öä";
            action[5] = "ÌìÒôËÂ_´ó±¯Öä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[16] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[17] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[18] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[19] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
            action[20] = "ÌìÒôËÂ_Æï³Ë_´ó±¯Öä";
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
        virtual ~ Skill1566Stub ()
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
            static int array[1] = { 12 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (350 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               350 * skill->GetLevel (),
                               2 * skill->GetLevel (), 2 * skill->GetLevel (), skill->GetLevel (), 45 + 5 * skill->GetLevel (), 5, 12);

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
            skill->GetVictim ()->SetTime (2000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue ((0.45 + 0.05 * skill->GetLevel ()) *
                                           (skill->GetPlayer ()->GetMaxatk () + skill->GetPlayer ()->GetMinatk ()) / 2);
            skill->GetVictim ()->SetBleeding (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * norm (skill->GetLevel () / 4.9)));
            skill->GetVictim ()->SetAmount (int (skill->GetLevel () / 4.9));
              skill->GetVictim ()->SetClearbuff (1);
              return true;
        }
#endif
    };
}
#endif
