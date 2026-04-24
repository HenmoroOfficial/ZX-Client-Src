#ifndef __CPPGEN_GNET_SKILL1517
#define __CPPGEN_GNET_SKILL1517
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1517:public Skill
    {
      public:
        enum
        { SKILL_ID = 1517 };
          Skill1517 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1517Stub:public SkillStub
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
                skill->SetRatio (1);
                skill->SetPlus (0.02 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1517Stub ():SkillStub (1517)
        {
            occupation = 129;
            name = L"ÀÉÇéæªÒâ½£";
            nativename = "ÀÉÇéæªÒâ½£";
            icon = "Ğ°½£ĞÇÓğ.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            preskillnum = 0;
            action[0] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[1] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[2] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[3] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[4] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[5] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[6] = "·ÙÏã¹È_³õ¼¶»ğ¹¥";
            action[7] = "ÊŞÉñ_ÃÍ»÷";
            action[8] = "ºüÑı_ÃÍ»÷";
            action[9] = "ĞùÔ¯_ÃÍ»÷";
            action[10] = "»³¹â_ÃÍ»÷";
            action[11] = "Ì«ê»_´¸»÷";
            action[12] = "Ìì»ª_ÃÍ»÷";
            action[13] = "ÁéÙí_ÃÍ»÷";
            action[14] = "Ó¢ÕĞ_ÃÍ»÷";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[21] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ğ¹¥";
            action[22] = "ÊŞÉñ_Æï³Ë_ÃÍ»÷";
            action[23] = "ºüÑı_Æï³Ë_ÃÍ»÷";
            action[24] = "ĞùÔ¯_Æï³Ë_ÃÍ»÷";
            action[25] = "»³¹â_Æï³Ë_ÃÍ»÷";
            action[26] = "Ì«ê»_Æï³Ë_´¸»÷";
            action[27] = "Ìì»ª_Æï³Ë_ÃÍ»÷";
            action[28] = "ÁéÙí_Æï³Ë_ÃÍ»÷";
            action[29] = "Ó¢ÕĞ_Æï³Ë_ÃÍ»÷";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
        virtual ~ Skill1517Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (600);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 900000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (600), 2, 60, 3, 33);

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
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetAmount (3 * (skill->GetPlayer ()->GetMaxmp () + skill->GetPlayer ()->GetMaxhp ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (33));
            skill->GetVictim ()->SetTime (60000);
            skill->GetVictim ()->SetPowerless (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (12);
        }
#endif
    };
}
#endif
