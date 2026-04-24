#ifndef __CPPGEN_GNET_SKILL3444
#define __CPPGEN_GNET_SKILL3444
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3444:public Skill
    {
      public:
        enum
        { SKILL_ID = 3444 };
          Skill3444 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3444Stub:public SkillStub
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
                skill->SetCrit ((-0.02) * (skill->GetPlayer ()->GetReborncount () == 0 ? 1 : 0));
                skill->GetPlayer ()->SetVar2 (int (skill->GetPlayer ()->GetHasbuff (4141) == 1));
                  skill->SetPlus ((1913 + 135 * skill->GetLevel ()) * (1 + skill->GetPlayer ()->GetVar2 () * (0.1 + skill->GetLevel () * 0.1)) +
                                  int (zrand (100) <
                                       30 * skill->GetT1 ()? 1 : 0) * skill->GetPlayer ()->GetVar2 () * skill->GetPlayer ()->GetMaxhp () * 0.01 * 3 *
                                  skill->GetT1 () + 330 * skill->GetT1 ());
                  skill->SetRatio (1 + skill->GetPlayer ()->GetVar2 () * (0.1 + 0.1 * skill->GetLevel ()));
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3444Stub ():SkillStub (3444)
        {
            occupation = 67;
            name = L"³ãÑ×Ç§Àï";
            nativename = "³ãÑ×Ç§Àï";
            icon = "³ãÑ×Ç§Àï.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            talent[0] = 3387;
            talent[1] = 3389;
            talent[2] = 3752;
            talent_size = 3;
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
            action[30] = "";
            rangetype = 2;
            doenchant = false;
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
        virtual ~ Skill3444Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3443 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (21 * skill->GetLevel () + 660);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 10000 * skill->GetT0 () - 120000 * 0.03 * skill->GetT2 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 120, 122, 124, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15 + skill->GetLevel () + 2 * skill->GetT0 ());
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
                               21 * skill->GetLevel () + 660,
                               (15 + skill->GetLevel ()), 25, 1913 + 135 * skill->GetLevel (), (10 + 10 * skill->GetLevel ()));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHasbuff (4141) == 1 ? 120 : 0));
            skill->GetVictim ()->SetRatio (4141);
            skill->GetVictim ()->SetAmount (4141);
            skill->GetVictim ()->SetValue (4141);
            skill->GetVictim ()->SetDispel (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
