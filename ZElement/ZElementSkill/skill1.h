#ifndef __CPPGEN_GNET_SKILL1
#define __CPPGEN_GNET_SKILL1
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1:public Skill
    {
      public:
        enum
        { SKILL_ID = 1 };
          Skill1 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1Stub ():SkillStub (1)
        {
            occupation = 0;
            name = L"Ä¬ÈÏ¼¼ÄÜ";
            nativename = "Ä¬ÈÏ¼¼ÄÜ";
            icon = "ÖØ»÷.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 11;
            eventflag = 1;
            spcost = 1;
            timetype = 1;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 3;
            clearmask = 2;
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
            preskillnum = 1;
            talent[0] = 1;
            talent[1] = 3;
            talent[2] = 5;
            talent[3] = 7;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[1] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[2] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[3] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[4] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[5] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
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
            rangetype = 7;
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
#endif
        }
        virtual ~ Skill1Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (5);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (10);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 500;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[1] = { 12 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5);
        }
        int GetCreditCost (int level) const
        {
            static int array[1] = { 1 };
            return array[level - 1];
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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetTime (5000);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetGloss (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (120000);
            skill->GetVictim ()->SetRatio (1.0);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (500);
            skill->GetVictim ()->SetEvilaura (1);
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
