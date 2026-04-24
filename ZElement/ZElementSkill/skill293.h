#ifndef __CPPGEN_GNET_SKILL293
#define __CPPGEN_GNET_SKILL293
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill293:public Skill
    {
      public:
        enum
        { SKILL_ID = 293 };
          Skill293 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill293Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill293Stub ():SkillStub (293)
        {
            occupation = 6;
            name = L"Ö¹Ë®";
            nativename = "Ö¹Ë®";
            icon = "Ö¹Ë®.dds";
            maxlevel = 14;
            maxlearn = 8;
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
            talent[0] = 751;
            talent[1] = 763;
            talent[2] = 764;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[1] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[2] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[3] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[4] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[5] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
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
        virtual ~ Skill293Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 303 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (117 + 47 * skill->GetLevel () - 1.3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return (140000 - 10000 * skill->GetLevel ()) * (100 - 8 * skill->GetT1 ()) / 100;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 100, 103, 106, 108, 109, 110, 112, 114 };
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
                               INT (117 + 47 * skill->GetLevel () - 1.3 * skill->GetLevel () * skill->GetLevel ()), 140 - 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetRatio (289);
            skill->GetVictim ()->SetAmount (291);
            skill->GetVictim ()->SetValue (302);
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetRatio (220 + 79 * INT (skill->GetLevel () / 5.9));
            skill->GetVictim ()->SetAmount (287 + 20 * INT (skill->GetLevel () / 6.9));
            skill->GetVictim ()->SetValue (218 + 74 * INT (skill->GetLevel () / 7.9));
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetRatio (306 + 224 * norm (skill->GetT0 () / 0.8));
            skill->GetVictim ()->SetAmount (306 + 225 * norm (skill->GetT0 () / 1.5));
            skill->GetVictim ()->SetValue (218);
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetTime (4500 * skill->GetT2 ());
            skill->GetVictim ()->SetImmunesleep (1);
            return true;
        }
#endif
    };
}
#endif
