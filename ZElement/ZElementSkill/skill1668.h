#ifndef __CPPGEN_GNET_SKILL1668
#define __CPPGEN_GNET_SKILL1668
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1668:public Skill
    {
      public:
        enum
        { SKILL_ID = 1668 };
          Skill1668 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1668Stub:public SkillStub
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
                skill->SetRatio (1.1);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1668Stub ():SkillStub (1668)
        {
            occupation = 146;
            name = L"ËéÈÐ£ºÃî";
            nativename = "ËéÈÐ£ºÃî";
            icon = "ËéÈÐ.dds";
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
            preskillnum = 0;
            action[0] = "¹íµÀ_ËéÈÐ";
            action[1] = "¹íµÀ_ËéÈÐ";
            action[2] = "¹íµÀ_ËéÈÐ";
            action[3] = "¹íµÀ_ËéÈÐ";
            action[4] = "¹íµÀ_ËéÈÐ";
            action[5] = "¹íµÀ_ËéÈÐ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_ËéÈÐ";
            action[16] = "¹íµÀ_Æï³Ë_ËéÈÐ";
            action[17] = "¹íµÀ_Æï³Ë_ËéÈÐ";
            action[18] = "¹íµÀ_Æï³Ë_ËéÈÐ";
            action[19] = "¹íµÀ_Æï³Ë_ËéÈÐ";
            action[20] = "¹íµÀ_Æï³Ë_ËéÈÐ";
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
        virtual ~ Skill1668Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 6);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (850);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 252000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 4);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 2);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 850);

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
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetBarehanded (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetType () == 0 ? 15 : 0));
            skill->GetVictim ()->SetTime (12000 + 100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetSubmp (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetDrainmagic (1);
            return true;
        }
#endif
    };
}
#endif
