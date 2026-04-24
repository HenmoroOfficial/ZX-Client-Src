#ifndef __CPPGEN_GNET_SKILL3804
#define __CPPGEN_GNET_SKILL3804
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3804:public Skill
    {
      public:
        enum
        { SKILL_ID = 3804 };
          Skill3804 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3804Stub:public SkillStub
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
                skill->SetPlus (1 + zrand (INT (0.25 * skill->GetPlayer ()->GetHp () + 0.25 * skill->GetPlayer ()->GetMp ())));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3804Stub ():SkillStub (3804)
        {
            occupation = 146;
            name = L"ÓÄ»÷£º·Ù";
            nativename = "ÓÄ»÷£º·Ù";
            icon = "ÓÄ»÷.dds";
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
            action[0] = "¹íµÀ_ÓÄ»÷";
            action[1] = "¹íµÀ_ÓÄ»÷";
            action[2] = "¹íµÀ_ÓÄ»÷";
            action[3] = "¹íµÀ_ÓÄ»÷";
            action[4] = "¹íµÀ_ÓÄ»÷";
            action[5] = "¹íµÀ_ÓÄ»÷";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_ÓÄ»÷";
            action[16] = "¹íµÀ_Æï³Ë_ÓÄ»÷";
            action[17] = "¹íµÀ_Æï³Ë_ÓÄ»÷";
            action[18] = "¹íµÀ_Æï³Ë_ÓÄ»÷";
            action[19] = "¹íµÀ_Æï³Ë_ÓÄ»÷";
            action[20] = "¹íµÀ_Æï³Ë_ÓÄ»÷";
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
#endif
        }
        virtual ~ Skill3804Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 7);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
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
            return (float) (skill->GetPlayer ()->GetRange () + 4);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 2);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 0);

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
            skill->GetVictim ()->SetTime (30000 + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetValue (3854);
            skill->GetVictim ()->SetDelaycast (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetPowerless (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (15000 + 100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue (10);
            skill->GetVictim ()->SetAddspeed (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0.15 * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpgen (1);
            return true;
        }
#endif
    };
}
#endif
