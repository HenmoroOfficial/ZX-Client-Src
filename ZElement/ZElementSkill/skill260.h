#ifndef __CPPGEN_GNET_SKILL260
#define __CPPGEN_GNET_SKILL260
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill260:public Skill
    {
      public:
        enum
        { SKILL_ID = 260 };
          Skill260 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill260Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill260Stub ():SkillStub (260)
        {
            occupation = 1;
            name = L"Èñ½ğÖä";
            nativename = "Èñ½ğÖä";
            icon = "Èñ½ğÖä.dds";
            maxlevel = 12;
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
            skill_class = 0;
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
            talent[0] = 637;
            talent_size = 1;
            action[0] = "¹íÍõ×Ú_Èñ½ğÖä";
            action[1] = "¹íÍõ×Ú_Èñ½ğÖä";
            action[2] = "¹íÍõ×Ú_Èñ½ğÖä";
            action[3] = "¹íÍõ×Ú_Èñ½ğÖä";
            action[4] = "¹íÍõ×Ú_Èñ½ğÖä";
            action[5] = "¹íÍõ×Ú_Èñ½ğÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Èñ½ğÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Èñ½ğÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Èñ½ğÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Èñ½ğÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Èñ½ğÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Èñ½ğÖä";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 2;
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
        virtual ~ Skill260Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 245 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.24 * skill->GetLevel () * skill->GetLevel () + 52 * skill->GetLevel () + 124);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 35, 40, 45, 49, 51 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (16);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.24 * skill->GetLevel () * skill->GetLevel () + 52 * skill->GetLevel () + 124), 16, skill->GetLevel (), 30);

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
            skill->GetVictim ()->SetTime (1800000 + 900000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () + 0.01 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncattack (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (30);
        }
#endif
    };
}
#endif
