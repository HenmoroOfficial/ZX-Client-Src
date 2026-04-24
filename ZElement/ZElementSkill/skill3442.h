#ifndef __CPPGEN_GNET_SKILL3442
#define __CPPGEN_GNET_SKILL3442
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3442:public Skill
    {
      public:
        enum
        { SKILL_ID = 3442 };
          Skill3442 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3442Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetDodge ());
                skill->SetRatio (1 + 0.10 * skill->GetT0 ());
                skill->SetPlus (1432 + 127 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3442Stub ():SkillStub (3442)
        {
            occupation = 67;
            name = L"¾ø³¾ÁùºÏ";
            nativename = "¾ø³¾ÁùºÏ";
            icon = "¾ø³¾ÁùºÏ.dds";
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
            talent[0] = 3386;
            talent[1] = 3751;
            talent[2] = 3752;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_×ÆÉÕ³Í½ä";
            action[1] = "·ÙÏã¹È_×ÆÉÕ³Í½ä";
            action[2] = "·ÙÏã¹È_×ÆÉÕ³Í½ä";
            action[3] = "·ÙÏã¹È_×ÆÉÕ³Í½ä";
            action[4] = "·ÙÏã¹È_×ÆÉÕ³Í½ä";
            action[5] = "·ÙÏã¹È_×ÆÉÕ³Í½ä";
            action[6] = "·ÙÏã¹È_×ÆÉÕ³Í½ä";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_×ÆÉÕ³Í½ä";
            action[16] = "·ÙÏã¹È_Æï³Ë_×ÆÉÕ³Í½ä";
            action[17] = "·ÙÏã¹È_Æï³Ë_×ÆÉÕ³Í½ä";
            action[18] = "·ÙÏã¹È_Æï³Ë_×ÆÉÕ³Í½ä";
            action[19] = "·ÙÏã¹È_Æï³Ë_×ÆÉÕ³Í½ä";
            action[20] = "·ÙÏã¹È_Æï³Ë_×ÆÉÕ³Í½ä";
            action[21] = "·ÙÏã¹È_Æï³Ë_×ÆÉÕ³Í½ä";
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
        virtual ~ Skill3442Stub ()
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
            static int array[1] = { 24 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (11 * skill->GetLevel () + 624);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 * (1 - 0.03 * skill->GetT2 ());
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 105, 107, 109, 111, 113, 115, 117, 119, 122 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (14 + 0.5 * skill->GetLevel ());
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
                               11 * skill->GetLevel () + 624,
                               14 + 0.5 * skill->GetLevel (),
                               1432 + 127 * skill->GetLevel (),
                               8 + 2 * skill->GetLevel (),
                               6, 10 + 2 * skill->GetLevel (), 8 + 2 * skill->GetLevel (), 6, 10 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (8 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio ((10 + 2 * skill->GetLevel ()) * 0.01);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecaccuracy (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () > 0 ? (-1) : 0));
            skill->GetVictim ()->SetValue (3 * skill->GetT0 () * skill->GetPlayer ()->GetVar2 ());
            skill->GetVictim ()->SetDirecthurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 + skill->GetT1 ()));
            skill->GetVictim ()->SetTime (14100);
            skill->GetVictim ()->SetRatio (6);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (6);
            skill->GetVictim ()->SetValue (0.25 * skill->GetPlayer ()->GetVar1 () * (1 + 0.1 * skill->GetT1 ()));
            skill->GetVictim ()->SetFiring (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (8 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetDodge () * (10 + 2 * skill->GetLevel ()) * 0.01);
            skill->GetVictim ()->SetAdddodge (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
