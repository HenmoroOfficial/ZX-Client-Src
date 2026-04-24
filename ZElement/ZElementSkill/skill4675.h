#ifndef __CPPGEN_GNET_SKILL4675
#define __CPPGEN_GNET_SKILL4675
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4675:public Skill
    {
      public:
        enum
        { SKILL_ID = 4675 };
          Skill4675 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4675Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 0;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetLevel ());
                skill->SetPlus (skill->GetPlayer ()->GetLevel () * 8 *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 3 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4675Stub ():SkillStub (4675)
        {
            occupation = 58;
            name = L"定风波被动";
            nativename = "定风波被动";
            icon = "";
            maxlevel = 10;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 1;
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
            talent[0] = 4115;
            talent[1] = 4116;
            talent[2] = 4117;
            talent[3] = 4123;
            talent[4] = 4151;
            talent_size = 5;
            action[0] = "0";
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
            action[13] = "";
            action[14] = "";
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
#endif
        }
        virtual ~ Skill4675Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () * 3 + 10 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (zrand (100) <
                                                  10 * skill->GetT3 ()? (skill->GetPlayer ()->GetVar1 () * 3 + 10 * skill->GetLevel ()) : 0));
            skill->GetVictim ()->SetTime (3000);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
#endif
    };
}
#endif
