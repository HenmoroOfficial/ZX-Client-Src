#ifndef __CPPGEN_GNET_SKILL5105
#define __CPPGEN_GNET_SKILL5105
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5105:public Skill
    {
      public:
        enum
        { SKILL_ID = 5105 };
          Skill5105 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5105Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
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
                skill->SetCrit (0.05 * 2);
                skill->SetPlus (1280 + 160 * 5 + (skill->GetPlayer ()->GetHasbuff (4384) ? (0.5 + 0.1 * 5) * skill->GetPlayer ()->GetRage () : 0));
                skill->SetPlus2 (skill->GetPlayer ()->GetHasbuff (4384) ? 0 : (0.5 + 0.1 * 5) * skill->GetPlayer ()->GetRage ());
                skill->GetPlayer ()->SetRage (0);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5105Stub ():SkillStub (5105)
        {
            occupation = 128;
            name = L"ÄæÁÛ¶Ï";
            nativename = "ÄæÁÛ¶Ï";
            icon = "Å­Õ¶.dds";
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
            action[0] = "Ó¢ÕÐ_Å­Õ¶";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "Ó¢ÕÐ_Å­Õ¶";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "Ó¢ÕÐ_Å­Õ¶";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
        virtual ~ Skill5105Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (500);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1400;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1280 + 160 * 5, 0.5 + 0.1 * 5);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
