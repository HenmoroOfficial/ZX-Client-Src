#ifndef __CPPGEN_GNET_SKILL3300
#define __CPPGEN_GNET_SKILL3300
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3300:public Skill
    {
      public:
        enum
        { SKILL_ID = 3300 };
          Skill3300 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3300Stub:public SkillStub
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
                skill->SetCrit (0.05);
                skill->SetRatio (3 + 0.05 * 3);
                skill->SetPlus (35 * 3 * 6 + 980 + 50 * 6 + 2 * 6 * 6);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3300Stub ():SkillStub (3300)
        {
            occupation = 128;
            name = L"¾ÉÃÎ";
            nativename = "¾ÉÃÎ";
            icon = "¾ÉÃÎ.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[1] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[2] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[3] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[4] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[5] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
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
        virtual ~ Skill3300Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 500, 5, 200, INT (980 + 50 * 6 + 2 * 6 * 6), 10 + 3 * 6, 2);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * ((60 + 8 * 3) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.48 * (INT (zrand (100) / (100 - 4 * 3)) + 1));
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * ((10 + 3 * 6 + 5 * 3)));
            skill->GetVictim ()->SetAmount (2);
            skill->GetVictim ()->SetClearbuff (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (8 * 3));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.02 * 3 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
