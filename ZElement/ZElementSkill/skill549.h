#ifndef __CPPGEN_GNET_SKILL549
#define __CPPGEN_GNET_SKILL549
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill549:public Skill
    {
      public:
        enum
        { SKILL_ID = 549 };
          Skill549 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill549Stub:public SkillStub
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
      Skill549Stub ():SkillStub (549)
        {
            occupation = 22;
            name = L"Ä¦Ú­ĞÄ¾­";
            nativename = "Ä¦Ú­ĞÄ¾­";
            icon = "Ä¦Ú­ĞÄ¾­.dds";
            maxlevel = 20;
            maxlearn = 8;
            type = 2;
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
            talent[0] = 700;
            talent[1] = 717;
            talent_size = 2;
            action[0] = "ÌìÒôËÂ_Ä¦Ú­ĞÄ¾­";
            action[1] = "ÌìÒôËÂ_Ä¦Ú­ĞÄ¾­";
            action[2] = "ÌìÒôËÂ_Ä¦Ú­ĞÄ¾­";
            action[3] = "ÌìÒôËÂ_Ä¦Ú­ĞÄ¾­";
            action[4] = "ÌìÒôËÂ_Ä¦Ú­ĞÄ¾­";
            action[5] = "ÌìÒôËÂ_Ä¦Ú­ĞÄ¾­";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ĞÄ¾­";
            action[16] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ĞÄ¾­";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ĞÄ¾­";
            action[18] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ĞÄ¾­";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ĞÄ¾­";
            action[20] = "ÌìÒôËÂ_Æï³Ë_Ä¦Ú­ĞÄ¾­";
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
        virtual ~ Skill549Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 546 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (750 + 35 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 120000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 110, 112, 114, 116, 118, 120, 122, 124 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (26);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (24);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 750 + 35 * skill->GetLevel (), 2, 2 * skill->GetLevel (), 120, 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (15000 + 4000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.01 * (skill->GetLevel () + skill->GetT1 ()) * skill->GetPlayer ()->GetMaxhp () *
                                           (4 * skill->GetT1 () + 15));
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetRatio (220);
            skill->GetVictim ()->SetAmount (220 + 569 * norm (zrand (100) / (100 - 30 * skill->GetT0 ())));
            skill->GetVictim ()->SetValue (218);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (120000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (60 * 4 * skill->GetLevel () * (1 + 0.2 * skill->GetT0 ()));
            skill->GetVictim ()->SetMpgen (1);
            return true;
        }
#endif
    };
}
#endif
