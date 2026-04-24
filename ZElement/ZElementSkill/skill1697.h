#ifndef __CPPGEN_GNET_SKILL1697
#define __CPPGEN_GNET_SKILL1697
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1697:public Skill
    {
      public:
        enum
        { SKILL_ID = 1697 };
          Skill1697 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1697Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetHp (0.75 * skill->GetPlayer ()->GetHp ());
                skill->SetRatio (2);
                skill->SetCrit (0.5);
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetHp (0.75 * skill->GetPlayer ()->GetHp ());
                skill->SetCrit (0.5);
                skill->SetRatio (1);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill1697Stub ():SkillStub (1697)
        {
            occupation = 146;
            name = L"¿Õ·µ£º°µ";
            nativename = "¿Õ·µ£º°µ";
            icon = "¿Õ·µ.dds";
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
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
            action[0] = "¹íÍõ×Ú_¿Õ·µ";
            action[1] = "¹íÍõ×Ú_¿Õ·µ";
            action[2] = "¹íÍõ×Ú_¿Õ·µ";
            action[3] = "¹íÍõ×Ú_¿Õ·µ";
            action[4] = "¹íÍõ×Ú_¿Õ·µ";
            action[5] = "¹íÍõ×Ú_¿Õ·µ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¿Õ·µ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_¿Õ·µ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¿Õ·µ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_¿Õ·µ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_¿Õ·µ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_¿Õ·µ";
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill1697Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 8);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1100);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 360000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 6);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 2);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1100);

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
            skill->GetVictim ()->SetProbability (1.0 * (25));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetDiet (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetRatio (0.5 + 0.002 * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetAmount (1600 * skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetDechurt (1);
            return true;
        }
#endif
    };
}
#endif
