#ifndef __CPPGEN_GNET_SKILL2055
#define __CPPGEN_GNET_SKILL2055
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2055:public Skill
    {
      public:
        enum
        { SKILL_ID = 2055 };
          Skill2055 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2055Stub:public SkillStub
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetLevel () + 0.04 * (skill->GetT0 () + skill->GetT1 () + skill->GetT2 ()));
                skill->SetPlus (0.02 * skill->GetLevel () * skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.04 * skill->GetLevel () + 0.04 * (skill->GetT0 () + skill->GetT1 () + skill->GetT2 ()));
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetPlus (0.02 * skill->GetLevel () * skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2055Stub ():SkillStub (2055)
        {
            occupation = 149;
            name = L"Æß½ÙÕ¶Áú¾÷II";
            nativename = "Æß½ÙÕ¶Áú¾÷II";
            icon = "Æß½ÙÕ¶Áú¾÷II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 2;
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
            talent[0] = 1563;
            talent[1] = 1564;
            talent[2] = 2054;
            talent_size = 3;
            action[0] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[1] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[2] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[3] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[4] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[5] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[6] = "ÇàÔÆÃÅ_Æß½ÙÕ¶Áú¾÷";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[21] = "ÇàÔÆÃÅ_Æï³Ë_Æß½ÙÕ¶Áú¾÷";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill2055Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 545 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19 + 0.6 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (99 * skill->GetLevel () + 333);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 * 2;
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
            return (float) (16 + 0.6 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7 + 0.6 * skill->GetLevel ());
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
                               99 * skill->GetLevel () + 333,
                               4 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               2 * skill->GetLevel (), 10 * skill->GetLevel (), 6, skill->GetLevel (), 0.6 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecskilldodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0.1 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncfatalhurt (1);
            return true;
        }
#endif
    };
}
#endif
