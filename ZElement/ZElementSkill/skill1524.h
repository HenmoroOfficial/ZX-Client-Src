#ifndef __CPPGEN_GNET_SKILL1524
#define __CPPGEN_GNET_SKILL1524
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1524:public Skill
    {
      public:
        enum
        { SKILL_ID = 1524 };
          Skill1524 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1524Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1524Stub ():SkillStub (1524)
        {
            occupation = 148;
            name = L"Ì«Ðé¾÷";
            nativename = "Ì«Ðé¾÷";
            icon = "Ì«Ðé¾÷.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 3;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 9;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            talent[0] = 1545;
            talent_size = 1;
            action[0] = "ÇàÔÆÃÅ_Ì«Ðé¾÷";
            action[1] = "ÇàÔÆÃÅ_Ì«Ðé¾÷";
            action[2] = "¹íÍõ×Ú_Ì«Ðé¾÷";
            action[3] = "ºÏ»¶ÅÉ_Ì«Ðé¾÷";
            action[4] = "ÌìÒôËÂ_Ì«Ðé¾÷";
            action[5] = "¹íµÀ_Ì«Ðé¾÷";
            action[6] = "·ÙÏã¹È_Ì«Ðé¾÷";
            action[7] = "ÊÞÉñ_Ì«Ðé¾÷";
            action[8] = "ºüÑý_Ì«Ðé¾÷";
            action[9] = "0";
            action[10] = "»³¹â_Ì«Ðé¾÷";
            action[11] = "Ì«ê»_Ì«Ðé¾÷";
            action[12] = "Ìì»ª_Ì«Ðé¾÷";
            action[13] = "ÁéÙí_Ì«Ðé¾÷";
            action[14] = "Ó¢ÕÐ_Ì«Ðé¾÷";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Ì«Ðé¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Ì«Ðé¾÷";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ì«Ðé¾÷";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ì«Ðé¾÷";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ì«Ðé¾÷";
            action[20] = "¹íµÀ_Æï³Ë_Ì«Ðé¾÷";
            action[21] = "·ÙÏã¹È_Æï³Ë_Ì«Ðé¾÷";
            action[22] = "ÊÞÉñ_Æï³Ë_Ì«Ðé¾÷";
            action[23] = "ºüÑý_Æï³Ë_Ì«Ðé¾÷";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_Ì«Ðé¾÷";
            action[26] = "Ì«ê»_Æï³Ë_Ì«Ðé¾÷";
            action[27] = "Ìì»ª_Æï³Ë_Ì«Ðé¾÷";
            action[28] = "ÁéÙí_Æï³Ë_Ì«Ðé¾÷";
            action[29] = "Ó¢ÕÐ_Æï³Ë_Ì«Ðé¾÷";
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
#endif
        }
        virtual ~ Skill1524Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1520 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1250 + 50 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 18000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12);
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
                               1250 + 50 * skill->GetLevel (), 12, 12 + skill->GetLevel (), 12 + skill->GetLevel (), 15 + 5 * skill->GetLevel (), 10);

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
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((skill->GetPlayer ()->GetDodge () + skill->GetPlayer ()->GetLevel ()) * (12 + skill->GetLevel ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak3 (1);
            skill->GetVictim ()->SetProbability (1.0 * (15 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (300000);
            skill->GetVictim ()->SetRatio (1573);
            skill->GetVictim ()->SetAmount (1574);
            skill->GetVictim ()->SetValue (1575);
            skill->GetVictim ()->SetSetcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (15 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (300000);
            skill->GetVictim ()->SetRatio (1576);
            skill->GetVictim ()->SetAmount (1577);
            skill->GetVictim ()->SetValue (1578);
            skill->GetVictim ()->SetSetcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (15 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (300000);
            skill->GetVictim ()->SetRatio (1579);
            skill->GetVictim ()->SetAmount (1580);
            skill->GetVictim ()->SetValue (1581);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
#endif
    };
}
#endif
