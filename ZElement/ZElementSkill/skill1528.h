#ifndef __CPPGEN_GNET_SKILL1528
#define __CPPGEN_GNET_SKILL1528
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1528:public Skill
    {
      public:
        enum
        { SKILL_ID = 1528 };
          Skill1528 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1528Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1528Stub ():SkillStub (1528)
        {
            occupation = 148;
            name = L"¼ÅÃðÌì";
            nativename = "¼ÅÃðÌì";
            icon = "¼ÅÃðÌì.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            action[0] = "ÇàÔÆÃÅ_¼ÅÃðÌì";
            action[1] = "ÇàÔÆÃÅ_¼ÅÃðÌì";
            action[2] = "¹íÍõ×Ú_¼ÅÃðÌì";
            action[3] = "ºÏ»¶ÅÉ_¼ÅÃðÌì";
            action[4] = "ÌìÒôËÂ_¼ÅÃðÌì";
            action[5] = "¹íµÀ_¼ÅÃðÌì";
            action[6] = "·ÙÏã¹È_¼ÅÃðÌì";
            action[7] = "ÊÞÉñ_¼ÅÃðÌì";
            action[8] = "ºüÑý_¼ÅÃðÌì";
            action[9] = "ÐùÔ¯_¼ÅÃðÌì";
            action[10] = "»³¹â_¼ÅÃðÌì";
            action[11] = "Ì«ê»_¼ÅÃðÌì";
            action[12] = "Ìì»ª_¼ÅÃðÌì";
            action[13] = "ÁéÙí_¼ÅÃðÌì";
            action[14] = "Ó¢ÕÐ_¼ÅÃðÌì";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_¼ÅÃðÌì";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¼ÅÃðÌì";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¼ÅÃðÌì";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼ÅÃðÌì";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¼ÅÃðÌì";
            action[20] = "¹íµÀ_Æï³Ë_¼ÅÃðÌì";
            action[21] = "·ÙÏã¹È_Æï³Ë_¼ÅÃðÌì";
            action[22] = "ÊÞÉñ_Æï³Ë_¼ÅÃðÌì";
            action[23] = "ºüÑý_Æï³Ë_¼ÅÃðÌì";
            action[24] = "ÐùÔ¯_Æï³Ë_¼ÅÃðÌì";
            action[25] = "»³¹â_Æï³Ë_¼ÅÃðÌì";
            action[26] = "Ì«ê»_Æï³Ë_¼ÅÃðÌì";
            action[27] = "Ìì»ª_Æï³Ë_¼ÅÃðÌì";
            action[28] = "ÁéÙí_Æï³Ë_¼ÅÃðÌì";
            action[29] = "Ó¢ÕÐ_Æï³Ë_¼ÅÃðÌì";
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
        virtual ~ Skill1528Stub ()
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
            return 600000 - 18000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
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
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1875 + 125 * skill->GetLevel (),
                               18 + 2 * skill->GetLevel (),
                               18, 5 * skill->GetLevel (), 80, 1.9 + 0.1 * skill->GetLevel (), 14 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (14000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.8);
            skill->GetVictim ()->SetBuffid (8);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * ((1.9 + 0.1 * skill->GetLevel ()) * skill->GetPlayer ()->GetLevel ()));
            skill->GetVictim ()->SetTime (14000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetSilent (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (18 + 2 * skill->GetLevel ());
        }
#endif
    };
}
#endif
