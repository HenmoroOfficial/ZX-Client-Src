#ifndef __CPPGEN_GNET_SKILL1587
#define __CPPGEN_GNET_SKILL1587
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1587:public Skill
    {
      public:
        enum
        { SKILL_ID = 1587 };
          Skill1587 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1587Stub:public SkillStub
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
      Skill1587Stub ():SkillStub (1587)
        {
            occupation = 150;
            name = L"Ö¹Ë®II";
            nativename = "Ö¹Ë®II";
            icon = "Ö¹Ë®II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 132;
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
            talent[0] = 2067;
            talent_size = 1;
            action[0] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[1] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[2] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[3] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[4] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[5] = "ºÏ»¶ÅÉ_Ö¹Ë®";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Ö¹Ë®";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
        virtual ~ Skill1587Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 293 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 8 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (550 + 50 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 370000 - 10000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
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
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               550 + 50 * skill->GetLevel (), 20 * skill->GetLevel (), 13 + 2 * skill->GetLevel (), 370 - 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (13000 + 2000 * skill->GetLevel ());
            skill->GetVictim ()->SetImmunesilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (13000 + 2000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (0.20 * skill->GetLevel () * (skill->GetPlayer ()->GetAtkrate () + skill->GetPlayer ()->GetDodge ()) *
                                           (1 + 0.2 * skill->GetT0 ()));
            skill->GetVictim ()->SetAdddefence (1);
            return true;
        }
#endif
    };
}
#endif
