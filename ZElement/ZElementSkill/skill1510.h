#ifndef __CPPGEN_GNET_SKILL1510
#define __CPPGEN_GNET_SKILL1510
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1510:public Skill
    {
      public:
        enum
        { SKILL_ID = 1510 };
          Skill1510 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1510Stub:public SkillStub
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
      Skill1510Stub ():SkillStub (1510)
        {
            occupation = 147;
            name = L"Ììî¸ÔÆÌå";
            nativename = "Ììî¸ÔÆÌå";
            icon = "Ììî¸ÔÆÌå.dds";
            maxlevel = 50;
            maxlearn = 12;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 8;
            clearmask = 64;
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
            talent[0] = 1508;
            talent[1] = 1505;
            talent_size = 2;
            action[0] = "ÇàÔÆÃÅ_Ììî¸ÔÆÌå";
            action[1] = "ÇàÔÆÃÅ_Ììî¸ÔÆÌå";
            action[2] = "¹íÍõ×Ú_Ììî¸ÔÆÌå";
            action[3] = "ºÏ»¶ÅÉ_Ììî¸ÔÆÌå";
            action[4] = "ÌìÒôËÂ_Ììî¸ÔÆÌå";
            action[5] = "¹íµÀ_Ììî¸ÔÆÌå";
            action[6] = "·ÙÏã¹È_Ììî¸ÔÆÌå";
            action[7] = "ÊŞÉñ_Ììî¸ÔÆÌå";
            action[8] = "ºüÑı_Ììî¸ÔÆÌå";
            action[9] = "Ì«ê»_Ììî¸ÔÆÌå";
            action[10] = "»³¹â_Ììî¸ÔÆÌå";
            action[11] = "Ì«ê»_Ììî¸ÔÆÌå";
            action[12] = "Ìì»ª_Ììî¸ÔÆÌå";
            action[13] = "ÁéÙí_Ììî¸ÔÆÌå";
            action[14] = "Ó¢ÕĞ_Ììî¸ÔÆÌå";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Ììî¸ÔÆÌå";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Ììî¸ÔÆÌå";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ììî¸ÔÆÌå";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ììî¸ÔÆÌå";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ììî¸ÔÆÌå";
            action[20] = "¹íµÀ_Æï³Ë_Ììî¸ÔÆÌå";
            action[21] = "·ÙÏã¹È_Æï³Ë_Ììî¸ÔÆÌå";
            action[22] = "ÊŞÉñ_Æï³Ë_Ììî¸ÔÆÌå";
            action[23] = "ºüÑı_Æï³Ë_Ììî¸ÔÆÌå";
            action[24] = "Ì«ê»_Æï³Ë_Ììî¸ÔÆÌå";
            action[25] = "»³¹â_Æï³Ë_Ììî¸ÔÆÌå";
            action[26] = "Ì«ê»_Æï³Ë_Ììî¸ÔÆÌå";
            action[27] = "Ìì»ª_Æï³Ë_Ììî¸ÔÆÌå";
            action[28] = "ÁéÙí_Æï³Ë_Ììî¸ÔÆÌå";
            action[29] = "Ó¢ÕĞ_Æï³Ë_Ììî¸ÔÆÌå";
            action[30] = "0";
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
        virtual ~ Skill1510Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1509 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 30000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[12] = { 105, 105, 105, 110, 115, 120, 125, 125, 125, 125, 125, 125 };
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
        int GetCreditCost (int level) const
        {
            static int array[12] = { 25, 50, 125, 250, 500, 1000, 1800, 3600, 7200, 14400, 14400, 14400 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 0, INT (3 * skill->GetLevel () + 0.2 * skill->GetLevel () * skill->GetLevel ()), 15, 5, 10);

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
            skill->GetVictim ()->SetTime (180000 * (1 + 0.05 * skill->GetT1 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((3 * skill->GetLevel () + 0.2 * skill->GetLevel () * skill->GetLevel ()) *
                                           (1 + 0.05 * skill->GetPlayer ()->GetLevel () / 15) * (1 + 0.05 * skill->GetT1 ()));
            skill->GetVictim ()->SetAddanti2 (1);
            return true;
        }
#endif
    };
}
#endif
