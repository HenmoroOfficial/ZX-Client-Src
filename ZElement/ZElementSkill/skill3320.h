#ifndef __CPPGEN_GNET_SKILL3320
#define __CPPGEN_GNET_SKILL3320
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3320:public Skill
    {
      public:
        enum
        { SKILL_ID = 3320 };
          Skill3320 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3320Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetHide ());
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetLevel ());
                skill->SetRatio (1.02 + 0.02 * 5 + (skill->GetPlayer ()->GetHide () > 0 ? 0.1 * 3 : 0));
                skill->SetPlus (277 + 10 * 5);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3320Stub ():SkillStub (3320)
        {
            occupation = 128;
            name = L"ÎÞÏë";
            nativename = "ÎÞÏë";
            icon = "ÎÞÏë.dds";
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
            action[0] = "»³¹â_¹í÷ÈÒ»»÷";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "»³¹â_¹í÷ÈÒ»»÷";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "»³¹â_Æï³Ë_¹í÷ÈÒ»»÷";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_¹í÷ÈÒ»»÷";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
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
        virtual ~ Skill3320Stub ()
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
            return (float) (300);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
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
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2 + 2 * 5, 277 + 10 * 5, 1 + 5 / 5.0, 3);

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar2 () >
                                                  0 ? 600 : (skill->GetPlayer ()->GetVar1 () + skill->GetPlayer ()->GetVar1 () * 5 / 5)));
            skill->GetVictim ()->SetTime (3200);
            skill->GetVictim ()->SetSilent (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (3 > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (2000 * 3);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (15 * 3);
            skill->GetVictim ()->SetValue (30 * 3);
            skill->GetVictim ()->SetDimming (1);
            return true;
        }
#endif
    };
}
#endif
