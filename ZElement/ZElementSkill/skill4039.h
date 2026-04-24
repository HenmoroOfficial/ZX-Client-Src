#ifndef __CPPGEN_GNET_SKILL4039
#define __CPPGEN_GNET_SKILL4039
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4039:public Skill
    {
      public:
        enum
        { SKILL_ID = 4039 };
          Skill4039 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4039Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
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
                skill->SetPlus (2000);
                skill->SetPlus2 (skill->GetPlayer ()->GetMaxmp () * 0.02 * skill->GetLevel ());
                skill->SetCrithurt (0.05 * skill->GetT1 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4039Stub ():SkillStub (4039)
        {
            occupation = 60;
            name = L"界·天演式·六道";
            nativename = "界·天演式·六道";
            icon = "界·天演式·六道.dds";
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
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 4352;
            targetcnt = 6;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 4142;
            talent[1] = 4148;
            talent[2] = 4004;
            talent[3] = 4015;
            talent[4] = 4024;
            talent[5] = 4033;
            talent_size = 6;
            action[0] = "轩辕_日炎术_02";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "轩辕_日炎术_02";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "轩辕_骑乘_日炎术_02";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "轩辕_骑乘_日炎术_02";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 11;
            doenchant = true;
            dobless = true;
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
        virtual ~ Skill4039Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4036 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 400;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 1000 * (skill->GetT2 () + skill->GetT3 () + skill->GetT4 () + skill->GetT5 ());
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 136, 137, 138, 139, 140 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (20);
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
                               2 * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               4 * skill->GetLevel (), 5 * skill->GetLevel (), 6 * skill->GetLevel (), 7 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncfatalratio (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetAmount (skill->GetLevel ());
            skill->GetVictim ()->SetValue (4352);
            skill->GetVictim ()->SetTriggerskill (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1000);
            skill->GetVictim ()->SetPowerless (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
