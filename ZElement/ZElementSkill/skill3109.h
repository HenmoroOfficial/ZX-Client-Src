#ifndef __CPPGEN_GNET_SKILL3109
#define __CPPGEN_GNET_SKILL3109
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3109:public Skill
    {
      public:
        enum
        { SKILL_ID = 3109 };
          Skill3109 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3109Stub:public SkillStub
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
      Skill3109Stub ():SkillStub (3109)
        {
            occupation = 169;
            name = L"·ğ×ğ±ÓÓÓ<ìø>";
            nativename = "·ğ×ğ±ÓÓÓ<ìø>";
            icon = "·ğ¡¤·ğ×ğ±ÓÓÓ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 4;
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
            action[0] = "ÇàÔÆÃÅ_·ğ×ğÓÓ»¤";
            action[1] = "ÇàÔÆÃÅ_·ğ×ğÓÓ»¤";
            action[2] = "¹íÍõ×Ú_·ğ×ğÓÓ»¤";
            action[3] = "ºÏ»¶ÅÉ_·ğ×ğÓÓ»¤";
            action[4] = "ÌìÒôËÂ_·ğ×ğÓÓ»¤";
            action[5] = "¹íµÀ_·ğ×ğÓÓ»¤";
            action[6] = "·ÙÏã¹È_·ğ×ğÓÓ»¤";
            action[7] = "ÊŞÉñ_·ğ×ğÓÓ»¤";
            action[8] = "ºüÑı_·ğ×ğÓÓ»¤";
            action[9] = "ĞùÔ¯_·ğ×ğÓÓ»¤";
            action[10] = "»³¹â_·ğ×ğÓÓ»¤";
            action[11] = "Ì«ê»_·ğ×ğÓÓ»¤";
            action[12] = "Ìì»ª_·ğ×ğÓÓ»¤";
            action[13] = "ÁéÙí_·ğ×ğÓÓ»¤";
            action[14] = "Ó¢ÕĞ_·ğ×ğÓÓ»¤";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_·ğ×ğÓÓ»¤";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·ğ×ğÓÓ»¤";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·ğ×ğÓÓ»¤";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·ğ×ğÓÓ»¤";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·ğ×ğÓÓ»¤";
            action[20] = "¹íµÀ_Æï³Ë_·ğ×ğÓÓ»¤";
            action[21] = "·ÙÏã¹È_Æï³Ë_·ğ×ğÓÓ»¤";
            action[22] = "ÊŞÉñ_Æï³Ë_·ğ×ğÓÓ»¤";
            action[23] = "ºüÑı_Æï³Ë_·ğ×ğÓÓ»¤";
            action[24] = "ĞùÔ¯_Æï³Ë_·ğ×ğÓÓ»¤";
            action[25] = "»³¹â_Æï³Ë_·ğ×ğÓÓ»¤";
            action[26] = "Ì«ê»_Æï³Ë_·ğ×ğÓÓ»¤";
            action[27] = "Ìì»ª_Æï³Ë_·ğ×ğÓÓ»¤";
            action[28] = "ÁéÙí_Æï³Ë_·ğ×ğÓÓ»¤";
            action[29] = "Ó¢ÕĞ_Æï³Ë_·ğ×ğÓÓ»¤";
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
        virtual ~ Skill3109Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1572 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 9100, 10800, 20700, 26500, 39600, 52900, 84300, 119800, 229700, 389600, 525000,
 1237600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 42, 47, 53, 59, 65, 71, 77 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1335 + 35 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 20000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (12);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1335 + 35 * skill->GetLevel (),
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               15 + 2 * skill->GetLevel (), 600 - 20 * skill->GetLevel (), 5 * skill->GetLevel (), 126 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (15000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetValue (126 * skill->GetLevel ());
            skill->GetVictim ()->SetAdddefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetAmount (1 + INT (skill->GetLevel () / 19.9));
            skill->GetVictim ()->SetCleardebuff (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
