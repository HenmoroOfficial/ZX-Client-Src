#ifndef __CPPGEN_GNET_SKILL5064
#define __CPPGEN_GNET_SKILL5064
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5064:public Skill
    {
      public:
        enum
        { SKILL_ID = 5064 };
          Skill5064 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5064Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * 6 + 0.07 * 3);
                skill->SetPlus ((835 + 22 * 6 + 6 * 6) * (1 + INT (zrand (99 + 7 * 3) / 100)));
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * 6 + 0.07 * 3);
                skill->SetPlus ((835 + 22 * 6 + 6 * 6) * (1 + INT (zrand (99 + 7 * 3) / 100)));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * 6 + 0.07 * 3);
                skill->SetPlus ((835 + 22 * 6 + 6 * 6) * (1 + INT (zrand (99 + 7 * 3) / 100)));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * 6 + 0.07 * 3);
                skill->SetPlus ((835 + 22 * 6 + 6 * 6) * (1 + INT (zrand (99 + 7 * 3) / 100)));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill5064Stub ():SkillStub (5064)
        {
            occupation = 128;
            name = L"À×Òô·¨ÚÍ";
            nativename = "À×Òô·¨ÚÍ";
            icon = "À×Òô·¨ÚÍ.dds";
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
            action[0] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[1] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[2] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[3] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[4] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[5] = "ÌìÒôËÂ_À×Òô·¨ÚÍ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_À×Òô·¨ÚÍ";
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill5064Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19 + 1.6 * 6);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000 - 3200 * 3;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16 + 1.5 * 6);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7 + 1.5 * 6);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1000,
                               2 * 6, INT (835 + 22 * 6 + 6 * 6), 4 * 6, INT ((500 + 11 * 6 + 1.5 * 6 * 6) * 6), 32 * 6, 1.5 * 6, 1.5 * 10, 1.5 * 10);

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (4000 * 6);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((500 + 11 * 6 + 1.5 * 6 * 6) * 6);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (6 * 3));
            skill->GetVictim ()->SetTime (4050 * 3);
            skill->GetVictim ()->SetRatio (0.03 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000 * 6);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (15);
            skill->GetVictim ()->SetSubanti (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (32 * 6 * (1 + 3));
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
#endif
    };
}
#endif
