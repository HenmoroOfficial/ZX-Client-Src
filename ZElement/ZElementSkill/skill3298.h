#ifndef __CPPGEN_GNET_SKILL3298
#define __CPPGEN_GNET_SKILL3298
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3298:public Skill
    {
      public:
        enum
        { SKILL_ID = 3298 };
          Skill3298 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3298Stub:public SkillStub
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
                return 1200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.02 * 3);
                skill->SetRatio (1 + 0.07 * 3);
                skill->SetCrithurt (0.5 * 3);
                skill->SetPlus (100 * skill->GetLevel () * (1 + 0.3 * 3));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3298Stub ():SkillStub (3298)
        {
            occupation = 128;
            name = L"»ÙÌìÃğµØ";
            nativename = "»ÙÌìÃğµØ";
            icon = "»ÙÌìÃğµØ.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
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
            action[0] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[1] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[2] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[3] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[4] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[5] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
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
        virtual ~ Skill3298Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000 - 2000 * 3;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (11);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (11);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 500, 25, 10, 100 * 4, 33, 120 * 4);

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
            skill->GetVictim ()->SetProbability (1.0 * (33 + 15 * 3));
            skill->GetVictim ()->SetTime (20000 + 100);
            skill->GetVictim ()->SetAssault (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (120 * 4 * (1 + 0.8 * 3) * (1 + INT (zrand (100) / (100 - 6 * 3))));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak5 (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
