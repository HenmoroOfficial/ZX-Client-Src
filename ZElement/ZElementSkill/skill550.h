#ifndef __CPPGEN_GNET_SKILL550
#define __CPPGEN_GNET_SKILL550
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill550:public Skill
    {
      public:
        enum
        { SKILL_ID = 550 };
          Skill550 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill550Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill550Stub ():SkillStub (550)
        {
            occupation = 22;
            name = L"¼ÅÃð±ÎÈÕ";
            nativename = "¼ÅÃð±ÎÈÕ";
            icon = "¼ÅÃð±ÎÈÕ.dds";
            maxlevel = 20;
            maxlearn = 8;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 3;
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
            talent[0] = 682;
            talent[1] = 694;
            talent[2] = 713;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_¼ÅÃðÖä";
            action[1] = "ÌìÒôËÂ_¼ÅÃðÖä";
            action[2] = "ÌìÒôËÂ_¼ÅÃðÖä";
            action[3] = "ÌìÒôËÂ_¼ÅÃðÖä";
            action[4] = "ÌìÒôËÂ_¼ÅÃðÖä";
            action[5] = "ÌìÒôËÂ_¼ÅÃðÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_¼ÅÃðÖä";
            action[16] = "ÌìÒôËÂ_Æï³Ë_¼ÅÃðÖä";
            action[17] = "ÌìÒôËÂ_Æï³Ë_¼ÅÃðÖä";
            action[18] = "ÌìÒôËÂ_Æï³Ë_¼ÅÃðÖä";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¼ÅÃðÖä";
            action[20] = "ÌìÒôËÂ_Æï³Ë_¼ÅÃðÖä";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 3;
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
        virtual ~ Skill550Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 547 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (38 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (550 + 10 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 15000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 115, 117, 119, 121, 123, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (18);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (17 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               550 + 10 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               48 + 2 * skill->GetLevel (),
                               skill->GetLevel (),
                               10 * skill->GetLevel (), 17 + skill->GetLevel (), 18, 3 * skill->GetLevel (), 11 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (48 + 2 * skill->GetLevel () + 13 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (11000 + 1000 * skill->GetLevel () + 100 + 8000 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.01001 * skill->GetLevel () + 0.02001 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (11000 + 1000 * skill->GetLevel () + 100 + 8000 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.10 * skill->GetLevel () + 0.2 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccrithurt (1);
            skill->GetVictim ()->SetTime (1000 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (0.1 * skill->GetT0 () * skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetSubantisleep (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (3 * skill->GetLevel ());
        }
#endif
    };
}
#endif
