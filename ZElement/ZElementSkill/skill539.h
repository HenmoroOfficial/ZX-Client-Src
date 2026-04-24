#ifndef __CPPGEN_GNET_SKILL539
#define __CPPGEN_GNET_SKILL539
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill539:public Skill
    {
      public:
        enum
        { SKILL_ID = 539 };
          Skill539 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill539Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill539Stub ():SkillStub (539)
        {
            occupation = 19;
            name = L"天玄冰";
            nativename = "天玄冰";
            icon = "天玄冰.dds";
            maxlevel = 18;
            maxlearn = 6;
            type = 2;
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
            skill_class = 2;
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
            preskillnum = 1;
            talent[0] = 571;
            talent[1] = 616;
            talent_size = 2;
            action[0] = "青云门_天玄冰";
            action[1] = "青云门_天玄冰";
            action[2] = "青云门_天玄冰";
            action[3] = "青云门_天玄冰";
            action[4] = "青云门_天玄冰";
            action[5] = "青云门_天玄冰";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_天玄冰";
            action[16] = "青云门_骑乘_天玄冰";
            action[17] = "青云门_骑乘_天玄冰";
            action[18] = "青云门_骑乘_天玄冰";
            action[19] = "青云门_骑乘_天玄冰";
            action[20] = "青云门_骑乘_天玄冰";
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
            rangetype = 5;
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
        virtual ~ Skill539Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 537 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (560 + 10 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 155000 - 5000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 106, 109, 113, 117, 121, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               560 + 10 * skill->GetLevel (),
                               12,
                               350 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel (),
                               9 * skill->GetLevel (), 2 * skill->GetLevel (), 155 - 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (12000 * (1 + 0.15 * skill->GetT0 ()) + 200);
            skill->GetVictim ()->SetAmount ((0.09 * skill->GetLevel () + 0.03 * skill->GetLevel () * skill->GetT1 ()) *
                                            skill->GetPlayer ()->GetMaxhp () * (1 + 0.15 * skill->GetT0 ()));
            skill->GetVictim ()->SetValue ((350 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel ()) *
                                           (1 + 0.35 * skill->GetT1 ()));
            skill->GetVictim ()->SetIceshield (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2000 * skill->GetLevel () * (1 + 0.15 * skill->GetT0 ()) + 200);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetChihun (1);
            return true;
        }
#endif
    };
}
#endif
