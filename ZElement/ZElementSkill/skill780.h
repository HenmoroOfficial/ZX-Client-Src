#ifndef __CPPGEN_GNET_SKILL780
#define __CPPGEN_GNET_SKILL780
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill780:public Skill
    {
      public:
        enum
        { SKILL_ID = 780 };
          Skill780 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill780Stub:public SkillStub
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
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill780Stub ():SkillStub (780)
        {
            occupation = 17;
            name = L"Ï§´º";
            nativename = "Ï§´º";
            icon = "Ï§´º.dds";
            maxlevel = 6;
            maxlearn = 5;
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
            skill_class = 1;
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
            talent[0] = 734;
            talent[1] = 763;
            talent[2] = 764;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_Ï§´º";
            action[1] = "ºÏ»¶ÅÉ_Ï§´º";
            action[2] = "ºÏ»¶ÅÉ_Ï§´º";
            action[3] = "ºÏ»¶ÅÉ_Ï§´º";
            action[4] = "ºÏ»¶ÅÉ_Ï§´º";
            action[5] = "ºÏ»¶ÅÉ_Ï§´º";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Ï§´º";
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
        virtual ~ Skill780Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 779 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (490 + 17 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 4800 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 135, 136, 136, 138, 139 };
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
            return _snwprintf (buffer, length, format, 490 + 17 * skill->GetLevel (), 12, 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (12000 * (1 + 0.1 * skill->GetT2 ()) + 100);
            skill->GetVictim ()->SetValue (2 * skill->GetLevel () + 2 * skill->GetT0 ());
            skill->GetVictim ()->SetBlessed (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (99);
            skill->GetVictim ()->SetValue (99);
            skill->GetVictim ()->SetExorcism (1);
            skill->GetVictim ()->SetProbability (1.0 * (30 * skill->GetT0 ()));
            skill->GetVictim ()->SetClearslow (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (3000 * norm (skill->GetT0 () / 1.5));
            skill->GetVictim ()->SetInvincible (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (293);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (2000);
            skill->GetVictim ()->SetReduceskillcd (1);
            return true;
        }
#endif
    };
}
#endif
