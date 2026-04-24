#ifndef __CPPGEN_GNET_SKILL4017
#define __CPPGEN_GNET_SKILL4017
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4017:public Skill
    {
      public:
        enum
        { SKILL_ID = 4017 };
          Skill4017 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4017Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 875;
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
                return 125;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetPlus (skill->GetPlayer ()->GetLevel () * 3 + skill->GetPlayer ()->GetMaxmp () * skill->GetLevel () / 100);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4017Stub ():SkillStub (4017)
        {
            occupation = 57;
            name = L"碎涅·罡·连星";
            nativename = "碎涅·罡·连星";
            icon = "碎涅·罡·连星.dds";
            maxlevel = 6;
            maxlearn = 5;
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
            skill_class = 11;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = true;
            preCastSkillId = 4015;
            darkLightType = 2;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 4144;
            talent_size = 1;
            action[0] = "轩辕_线形光群";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "轩辕_线形光群";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "轩辕_骑乘_线形光群";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "轩辕_骑乘_线形光群";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 1;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 9;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill4017Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4015 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetLevel () * 3);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 65, 70, 75, 80, 85 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (2);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (17);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetPlayer ()->GetLevel () * 3, skill->GetLevel (), 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () * 3 + 10 * skill->GetLevel () + 10 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10);
        }
#endif
    };
}
#endif
