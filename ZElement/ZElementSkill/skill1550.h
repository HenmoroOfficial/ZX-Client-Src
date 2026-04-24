#ifndef __CPPGEN_GNET_SKILL1550
#define __CPPGEN_GNET_SKILL1550
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1550:public Skill
    {
      public:
        enum
        { SKILL_ID = 1550 };
          Skill1550 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1550Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxhp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1550Stub ():SkillStub (1550)
        {
            occupation = 149;
            name = L"¿ñÉñ";
            nativename = "¿ñÉñ";
            icon = "¿ñÉñ.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 2;
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
            talent[0] = 1549;
            talent[1] = 1552;
            talent_size = 2;
            action[0] = "ÇàÔÆÃÅ_¿ñÉñ";
            action[1] = "ÇàÔÆÃÅ_¿ñÉñ";
            action[2] = "¹íÍõ×Ú_¿ñÉñ";
            action[3] = "ºÏ»¶ÅÉ_¿ñÉñ";
            action[4] = "ÌìÒôËÂ_¿ñÉñ";
            action[5] = "¹íµÀ_¿ñÉñ";
            action[6] = "·ÙÏã¹È_¿ñÉñ";
            action[7] = "ÊŞÉñ_¿ñÉñ";
            action[8] = "ºüÑı_¿ñÉñ";
            action[9] = "ĞùÔ¯_¿ñÉñ";
            action[10] = "»³¹â_¿ñÉñ";
            action[11] = "Ì«ê»_¿ñÉñ";
            action[12] = "Ìì»ª_¿ñÉñ";
            action[13] = "ÁéÙí_¿ñÉñ";
            action[14] = "Ó¢ÕĞ_¿ñÉñ";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_¿ñÉñ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¿ñÉñ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¿ñÉñ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¿ñÉñ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¿ñÉñ";
            action[20] = "¹íµÀ_Æï³Ë_¿ñÉñ";
            action[21] = "·ÙÏã¹È_Æï³Ë_¿ñÉñ";
            action[22] = "ÊŞÉñ_Æï³Ë_¿ñÉñ";
            action[23] = "ºüÑı_Æï³Ë_¿ñÉñ";
            action[24] = "ĞùÔ¯_Æï³Ë_¿ñÉñ";
            action[25] = "»³¹â_Æï³Ë_¿ñÉñ";
            action[26] = "Ì«ê»_Æï³Ë_¿ñÉñ";
            action[27] = "Ìì»ª_Æï³Ë_¿ñÉñ";
            action[28] = "ÁéÙí_Æï³Ë_¿ñÉñ";
            action[29] = "Ó¢ÕĞ_Æï³Ë_¿ñÉñ";
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
        virtual ~ Skill1550Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1552 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (325 + 25 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 720000 - 36000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
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
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 325 + 25 * skill->GetLevel (), 30, 15 * skill->GetLevel (), 30, skill->GetLevel (), 12);

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
            skill->GetVictim ()->SetTime (30000 + 3000 * skill->GetT0 () + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (30 + 3 * skill->GetT0 ());
            skill->GetVictim ()->SetValue (15 * skill->GetLevel ());
            skill->GetVictim ()->SetFrenzied (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000 + 100);
            skill->GetVictim ()->SetRatio (0.0101 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncskillaccu (1);
            skill->GetVictim ()->SetTime ((30000 + 100) * int (skill->GetLevel () / 10));
              skill->GetVictim ()->SetBuffid (2);
              skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * 0.1 * int (skill->GetLevel () / 10));
              skill->GetVictim ()->SetAddhp (1);
              return true;
        }
#endif
    };
}
#endif
