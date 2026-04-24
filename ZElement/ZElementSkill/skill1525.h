#ifndef __CPPGEN_GNET_SKILL1525
#define __CPPGEN_GNET_SKILL1525
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1525:public Skill
    {
      public:
        enum
        { SKILL_ID = 1525 };
          Skill1525 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1525Stub:public SkillStub
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
      Skill1525Stub ():SkillStub (1525)
        {
            occupation = 148;
            name = L"ÓñÕæ¾÷";
            nativename = "ÓñÕæ¾÷";
            icon = "ÓñÕæ¾÷.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 3;
            eventflag = 0;
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
            action[0] = "ÇàÔÆÃÅ_ÓñÕæ¾÷";
            action[1] = "ÇàÔÆÃÅ_ÓñÕæ¾÷";
            action[2] = "¹íÍõ×Ú_ÓñÕæ¾÷";
            action[3] = "ºÏ»¶ÅÉ_ÓñÕæ¾÷";
            action[4] = "ÌìÒôËÂ_ÓñÕæ¾÷";
            action[5] = "¹íµÀ_ÓñÕæ¾÷";
            action[6] = "·ÙÏã¹È_ÓñÕæ¾÷";
            action[7] = "ÊÞÉñ_ÓñÕæ¾÷";
            action[8] = "ºüÑý_ÓñÕæ¾÷";
            action[9] = "ÐùÔ¯_ÓñÕæ¾÷";
            action[10] = "»³¹â_ÓñÕæ¾÷";
            action[11] = "Ì«ê»_ÓñÕæ¾÷";
            action[12] = "Ìì»ª_ÓñÕæ¾÷";
            action[13] = "ÁéÙí_ÓñÕæ¾÷";
            action[14] = "Ó¢ÕÐ_ÓñÕæ¾÷";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ÓñÕæ¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÓñÕæ¾÷";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÓñÕæ¾÷";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÓñÕæ¾÷";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÓñÕæ¾÷";
            action[20] = "¹íµÀ_Æï³Ë_ÓñÕæ¾÷";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÓñÕæ¾÷";
            action[22] = "ÊÞÉñ_Æï³Ë_ÓñÕæ¾÷";
            action[23] = "ºüÑý_Æï³Ë_ÓñÕæ¾÷";
            action[24] = "ÐùÔ¯_Æï³Ë_ÓñÕæ¾÷";
            action[25] = "»³¹â_Æï³Ë_ÓñÕæ¾÷";
            action[26] = "Ì«ê»_Æï³Ë_ÓñÕæ¾÷";
            action[27] = "Ìì»ª_Æï³Ë_ÓñÕæ¾÷";
            action[28] = "ÁéÙí_Æï³Ë_ÓñÕæ¾÷";
            action[29] = "Ó¢ÕÐ_Æï³Ë_ÓñÕæ¾÷";
            action[30] = "0";
            rangetype = 0;
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
        virtual ~ Skill1525Stub ()
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
            return (float) (18);
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
            return 900000 - 27000 * skill->GetT0 ();
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
                               1250 + 50 * skill->GetLevel (), 12, 9 + skill->GetLevel (), 9 + skill->GetLevel (), 10 + 5 * skill->GetLevel (), 15);

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
            skill->GetVictim ()->SetAmount ((skill->GetPlayer ()->GetAtkrate () + skill->GetPlayer ()->GetLevel ()) * (9 + skill->GetLevel ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak2 (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (1520 + 8 * norm (zrand (100) / (100 - 10 - 5 * skill->GetLevel ())));
            skill->GetVictim ()->SetAmount (1520 + 9 * norm (zrand (100) / (100 - 10 - 5 * skill->GetLevel ())));
            skill->GetVictim ()->SetValue (1520 + 10 * norm (zrand (100) / (100 - 10 - 5 * skill->GetLevel ())));
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (900000 - 27000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (3014);
            skill->GetVictim ()->SetAmount (3014);
            skill->GetVictim ()->SetValue (3014);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
#endif
    };
}
#endif
