#ifndef __CPPGEN_GNET_SKILL3357
#define __CPPGEN_GNET_SKILL3357
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3357:public Skill
    {
      public:
        enum
        { SKILL_ID = 3357 };
          Skill3357 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3357Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetDodge ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3357Stub ():SkillStub (3357)
        {
            occupation = 128;
            name = L"·ÙÏã·âÉñÁ§Áú¼¼ÄÜ";
            nativename = "·ÙÏã·âÉñÁ§Áú¼¼ÄÜ";
            icon = "";
            maxlevel = 999;
            maxlearn = 999;
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
            action[0] = "";
            action[1] = "";
            action[2] = "";
            action[3] = "";
            action[4] = "";
            action[5] = "";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "";
            action[16] = "";
            action[17] = "";
            action[18] = "";
            action[19] = "";
            action[20] = "";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "";
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
        virtual ~ Skill3357Stub ()
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
            return 0;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (10);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 2);
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
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (int (skill->GetPlayer ()->GetVar1 () / 100) == 1 ? 100 : 0));
            skill->GetVictim ()->SetRatio (0.01 * (5 + 2 * (skill->GetPlayer ()->GetVar1 () - 100 * int (skill->GetPlayer ()->GetVar1 () / 100))));
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (int (skill->GetPlayer ()->GetVar1 () / 100) == 2 ? 100 : 0));
            skill->GetVictim ()->SetTime (6300);
            skill->GetVictim ()->SetRatio (0.01 * (10 + 3 * (skill->GetPlayer ()->GetVar1 () - 100 * int (skill->GetPlayer ()->GetVar1 () / 100))));
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetDeccrithurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (int (skill->GetPlayer ()->GetVar1 () / 100) == 3 ? 100 : 0));
            skill->GetVictim ()->SetTime (6300);
            skill->GetVictim ()->SetRatio (0.05 +
                                           int (skill->GetLevel () / 4) *
                                           ((skill->GetPlayer ()->GetVar1 () - 100 * int (skill->GetPlayer ()->GetVar1 () / 100))));
              skill->GetVictim ()->SetBuffid (2);
              skill->GetVictim ()->SetDeccritrate (1);
              return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (zrand (5) + 1);
        }
#endif
    };
}
#endif
