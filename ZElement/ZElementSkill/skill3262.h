#ifndef __CPPGEN_GNET_SKILL3262
#define __CPPGEN_GNET_SKILL3262
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3262:public Skill
    {
      public:
        enum
        { SKILL_ID = 3262 };
          Skill3262 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3262Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.4);
                skill->SetCrithurt (2);
                skill->SetRatio (4.37);
                skill->SetPlus (1200);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3262Stub ():SkillStub (3262)
        {
            occupation = 128;
            name = L"∫œª∂-≤¯√‡";
            nativename = "∫œª∂-≤¯√‡";
            icon = "≤¯√‡.dds";
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
            talent[0] = 742;
            talent[1] = 746;
            talent[2] = 749;
            talent[3] = 753;
            talent_size = 4;
            action[0] = "∫œª∂≈…_≤¯√‡";
            action[1] = "∫œª∂≈…_≤¯√‡";
            action[2] = "∫œª∂≈…_≤¯√‡";
            action[3] = "∫œª∂≈…_≤¯√‡";
            action[4] = "∫œª∂≈…_≤¯√‡";
            action[5] = "∫œª∂≈…_≤¯√‡";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "∫œª∂≈…_∆Ô≥À_≤¯√‡";
            action[16] = "∫œª∂≈…_∆Ô≥À_≤¯√‡";
            action[17] = "∫œª∂≈…_∆Ô≥À_≤¯√‡";
            action[18] = "∫œª∂≈…_∆Ô≥À_≤¯√‡";
            action[19] = "∫œª∂≈…_∆Ô≥À_≤¯√‡";
            action[20] = "∫œª∂≈…_∆Ô≥À_≤¯√‡";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "ººƒ‹π•ª˜1";
            rangetype = 0;
            doenchant = false;
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
        virtual ~ Skill3262Stub ()
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
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
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
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
