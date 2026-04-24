#ifndef __CPPGEN_GNET_SKILL258
#define __CPPGEN_GNET_SKILL258
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill258:public Skill
    {
      public:
        enum
        { SKILL_ID = 258 };
          Skill258 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill258Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHp () * 1000 / skill->GetPlayer ()->GetMaxhp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill258Stub ():SkillStub (258)
        {
            occupation = 2;
            name = L"ÃÍ»ğÖä";
            nativename = "ÃÍ»ğÖä";
            icon = "ÃÍ»ğÖä.dds";
            maxlevel = 20;
            maxlearn = 7;
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
            talent[0] = 623;
            talent[1] = 647;
            talent[2] = 662;
            talent_size = 3;
            action[0] = "¹íÍõ×Ú_ÃÍ»ğÖä";
            action[1] = "¹íÍõ×Ú_ÃÍ»ğÖä";
            action[2] = "¹íÍõ×Ú_ÃÍ»ğÖä";
            action[3] = "¹íÍõ×Ú_ÃÍ»ğÖä";
            action[4] = "¹íÍõ×Ú_ÃÍ»ğÖä";
            action[5] = "¹íÍõ×Ú_ÃÍ»ğÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»ğÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»ğÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»ğÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»ğÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»ğÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»ğÖä";
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
        virtual ~ Skill258Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 247 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.32 * skill->GetLevel () * skill->GetLevel () + 0.52 * skill->GetLevel () + 120);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 9000 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[7] = { 50, 52, 54, 56, 58, 60, 64 };
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
                               INT (0.32 * skill->GetLevel () * skill->GetLevel () + 0.52 * skill->GetLevel () + 120),
                               5 + skill->GetLevel (), 31 - skill->GetLevel (), 20);

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (20000 + 100 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.05 + 0.01 * skill->GetLevel () + 0.02 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetIncattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 - 7 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (20000 + 100 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.31 - 0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT1 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (60000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.00001 + 0.1 * skill->GetT1 ());
            skill->GetVictim ()->SetValue (0.06 * skill->GetT1 () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetAddhp2 (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 950 ? 120 * norm (skill->GetT1 () / 3.9) : 0));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
