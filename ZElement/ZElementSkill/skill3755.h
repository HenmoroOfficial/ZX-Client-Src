#ifndef __CPPGEN_GNET_SKILL3755
#define __CPPGEN_GNET_SKILL3755
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3755:public Skill
    {
      public:
        enum
        { SKILL_ID = 3755 };
          Skill3755 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3755Stub:public SkillStub
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
                return 1200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHp ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetHp (skill->GetPlayer ()->GetHp () - 0.05 * skill->GetLevel () * skill->GetPlayer ()->GetHp ());
                skill->SetPlus (0.02 * skill->GetLevel () * (skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetHp ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3755Stub ():SkillStub (3755)
        {
            occupation = 149;
            name = L"³ãÑ×Ç§ÀïII";
            nativename = "³ãÑ×Ç§ÀïII";
            icon = "³ãÑ×Ç§ÀïII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
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
            action[0] = "·ÙÏã¹È_»ğÑæÂûÑÓ";
            action[1] = "·ÙÏã¹È_»ğÑæÂûÑÓ";
            action[2] = "·ÙÏã¹È_»ğÑæÂûÑÓ";
            action[3] = "·ÙÏã¹È_»ğÑæÂûÑÓ";
            action[4] = "·ÙÏã¹È_»ğÑæÂûÑÓ";
            action[5] = "·ÙÏã¹È_»ğÑæÂûÑÓ";
            action[6] = "·ÙÏã¹È_»ğÑæÂûÑÓ";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_»ğÑæÂûÑÓ";
            action[16] = "·ÙÏã¹È_Æï³Ë_»ğÑæÂûÑÓ";
            action[17] = "·ÙÏã¹È_Æï³Ë_»ğÑæÂûÑÓ";
            action[18] = "·ÙÏã¹È_Æï³Ë_»ğÑæÂûÑÓ";
            action[19] = "·ÙÏã¹È_Æï³Ë_»ğÑæÂûÑÓ";
            action[20] = "·ÙÏã¹È_Æï³Ë_»ğÑæÂûÑÓ";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ğÑæÂûÑÓ";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
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
        virtual ~ Skill3755Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3444 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (950);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 45000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
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
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               40,
                               5 * skill->GetLevel (),
                               2 * skill->GetLevel (), 10 * skill->GetLevel (), 5 + skill->GetLevel (), 90 - 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHasmultbuff (113) == 1 ? 10 * skill->GetLevel () : 0));
            skill->GetVictim ()->SetTime (5000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (6);
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetAmount (8);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar2 () * 0.3);
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
#endif
    };
}
#endif
