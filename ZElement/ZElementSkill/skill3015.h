#ifndef __CPPGEN_GNET_SKILL3015
#define __CPPGEN_GNET_SKILL3015
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3015:public Skill
    {
      public:
        enum
        { SKILL_ID = 3015 };
          Skill3015 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3015Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes4 ());
                skill->SetRatio (1 + 0.05 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3015Stub ():SkillStub (3015)
        {
            occupation = 167;
            name = L"¼ÅÃğÌì<Ğş>";
            nativename = "¼ÅÃğÌì<Ğş>";
            icon = "ÏÉ¡¤¼ÅÃğÌì.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
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
            action[0] = "ÇàÔÆÃÅ_¼ÅÃğÌì";
            action[1] = "ÇàÔÆÃÅ_¼ÅÃğÌì";
            action[2] = "¹íÍõ×Ú_¼ÅÃğÌì";
            action[3] = "ºÏ»¶ÅÉ_¼ÅÃğÌì";
            action[4] = "ÌìÒôËÂ_¼ÅÃğÌì";
            action[5] = "¹íµÀ_¼ÅÃğÌì";
            action[6] = "·ÙÏã¹È_¼ÅÃğÌì";
            action[7] = "ÊŞÉñ_¼ÅÃğÌì";
            action[8] = "ºüÑı_¼ÅÃğÌì";
            action[9] = "ĞùÔ¯_¼ÅÃğÌì";
            action[10] = "»³¹â_¼ÅÃğÌì";
            action[11] = "Ì«ê»_¼ÅÃğÌì";
            action[12] = "Ìì»ª_¼ÅÃğÌì";
            action[13] = "ÁéÙí_¼ÅÃğÌì";
            action[14] = "Ó¢ÕĞ_¼ÅÃğÌì";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_¼ÅÃğÌì";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¼ÅÃğÌì";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¼ÅÃğÌì";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼ÅÃğÌì";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¼ÅÃğÌì";
            action[20] = "¹íµÀ_Æï³Ë_¼ÅÃğÌì";
            action[21] = "·ÙÏã¹È_Æï³Ë_¼ÅÃğÌì";
            action[22] = "ÊŞÉñ_Æï³Ë_¼ÅÃğÌì";
            action[23] = "ºüÑı_Æï³Ë_¼ÅÃğÌì";
            action[24] = "ĞùÔ¯_Æï³Ë_¼ÅÃğÌì";
            action[25] = "»³¹â_Æï³Ë_¼ÅÃğÌì";
            action[26] = "Ì«ê»_Æï³Ë_¼ÅÃğÌì";
            action[27] = "Ìì»ª_Æï³Ë_¼ÅÃğÌì";
            action[28] = "ÁéÙí_Æï³Ë_¼ÅÃğÌì";
            action[29] = "Ó¢ÕĞ_Æï³Ë_¼ÅÃğÌì";
            action[30] = "0";
            rangetype = 4;
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
        virtual ~ Skill3015Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1528 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 10 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 260100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 6);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1875 + 125 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 18000 * skill->GetT0 () - 15000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (18);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (170));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1875 + 125 * skill->GetLevel (),
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               600 - 15 * skill->GetLevel (), 18, 5 * skill->GetLevel (), 80, 3 * skill->GetLevel (), 10 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (600));
            skill->GetVictim ()->SetTime (10000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.8);
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (10000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetSilent (1);
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
