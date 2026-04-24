#ifndef __CPPGEN_GNET_SKILL2035
#define __CPPGEN_GNET_SKILL2035
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2035:public Skill
    {
      public:
        enum
        { SKILL_ID = 2035 };
          Skill2035 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2035Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMinatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2035Stub ():SkillStub (2035)
        {
            occupation = 148;
            name = L"µÀÖ®Ô¹Å­";
            nativename = "µÀÖ®Ô¹Å­";
            icon = "ÏÉÖ®Ô¹Å­.dds";
            maxlevel = 30;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 9;
            clearmask = 129;
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
            action[0] = "¹íµÀ_Ô¹Å­";
            action[1] = "ÇàÔÆÃÅ_Ô¹Å­";
            action[2] = "¹íÍõ×Ú_Ô¹Å­";
            action[3] = "ºÏ»¶ÅÉ_Ô¹Å­";
            action[4] = "ÌìÒôËÂ_Ô¹Å­";
            action[5] = "¹íµÀ_Ô¹Å­";
            action[6] = "·ÙÏã_Ô¹Å­";
            action[7] = "ÊÞÉñ_Ô¹Å­";
            action[8] = "ºüÑý_Ô¹Å­";
            action[9] = "ÐùÔ¯_Ô¹Å­";
            action[10] = "»³¹â_Ô¹Å­";
            action[11] = "Ì«ê»_Ô¹Å­";
            action[12] = "Ìì»ª_Ô¹Å­";
            action[13] = "ÁéÙí_Ô¹Å­";
            action[14] = "Ó¢ÕÐ_Ô¹Å­";
            action[15] = "¹íµÀ_Æï³Ë_Ô¹Å­";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Ô¹Å­";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ô¹Å­";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ô¹Å­";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ô¹Å­";
            action[20] = "¹íµÀ_Æï³Ë_Ô¹Å­";
            action[21] = "·ÙÏã_Æï³Ë_Ô¹Å­";
            action[22] = "ÊÞÉñ_Æï³Ë_Ô¹Å­";
            action[23] = "ºüÑý_Æï³Ë_Ô¹Å­";
            action[24] = "ÐùÔ¯_Æï³Ë_Ô¹Å­";
            action[25] = "»³¹â_Æï³Ë_Ô¹Å­";
            action[26] = "Ì«ê»_Æï³Ë_Ô¹Å­";
            action[27] = "Ìì»ª_Æï³Ë_Ô¹Å­";
            action[28] = "ÁéÙí_Æï³Ë_Ô¹Å­";
            action[29] = "Ó¢ÕÐ_Æï³Ë_Ô¹Å­";
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
            skill_limit = 1;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2035Stub ()
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
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return (600 - 15 * skill->GetLevel ()) * 1000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 100, 105, 110, 115, 120, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 130, 135, 140 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
        int GetCreditCost (int level) const
        {
            static int array[20] =
                { 24000, 26000, 28000, 30000, 32000, 34000, 36000, 38000, 40000, 42000, 45000, 48000, 51000, 54000, 57000, 60000, 64000, 68000, 72000,
 80000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               100 * skill->GetLevel (),
                               20 + 3 * skill->GetLevel (),
                               20 + 3 * skill->GetLevel (),
                               2 * skill->GetLevel (), 10 + 2 * skill->GetLevel (), 4 * skill->GetLevel (), 600 - 15 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime ((10 + 2 * skill->GetLevel ()) * 1000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount ((skill->GetPlayer ()->GetCultivation () == 4 ? 2 : 1) * (0.2 + 0.03 * skill->GetLevel ()) *
                                            skill->GetPlayer ()->GetVar1 () * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime ((10 + 2 * skill->GetLevel ()) * 1000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount ((skill->GetPlayer ()->GetCultivation () == 4 ? 2 : 1) * (0.2 + 0.03 * skill->GetLevel ()) *
                                            skill->GetPlayer ()->GetVar2 () * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (600));
            skill->GetVictim ()->SetTime ((10 + 2 * skill->GetLevel ()) * 1000);
            skill->GetVictim ()->SetRatio ((skill->GetPlayer ()->GetCultivation () == 4 ? 0.04 : 0.02) * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (900 * 1000);
            skill->GetVictim ()->SetRatio (2040);
            skill->GetVictim ()->SetAmount (219);
            skill->GetVictim ()->SetValue (219);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
#endif
    };
}
#endif
