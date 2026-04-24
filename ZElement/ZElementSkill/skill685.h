#ifndef __CPPGEN_GNET_SKILL685
#define __CPPGEN_GNET_SKILL685
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill685:public Skill
    {
      public:
        enum
        { SKILL_ID = 685 };
          Skill685 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill685Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetLevel ());
                skill->SetCrithurt (0.15 * skill->GetLevel ());
                skill->SetPlus (4.2 * skill->GetLevel () * skill->GetLevel () + 86 * skill->GetLevel () + 785);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill685Stub ():SkillStub (685)
        {
            occupation = 10;
            name = L"ÌìÎè±¦ÂÖ";
            nativename = "ÌìÎè±¦ÂÖ";
            icon = "ÌìÎè±¦ÂÖ.dds";
            maxlevel = 5;
            maxlearn = 4;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            talent_type = 1;
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
            action[0] = "ÌìÒôËÂ_ÌìÎè±¦ÂÖ";
            action[1] = "ÌìÒôËÂ_ÌìÎè±¦ÂÖ";
            action[2] = "ÌìÒôËÂ_ÌìÎè±¦ÂÖ";
            action[3] = "ÌìÒôËÂ_ÌìÎè±¦ÂÖ";
            action[4] = "ÌìÒôËÂ_ÌìÎè±¦ÂÖ";
            action[5] = "ÌìÒôËÂ_ÌìÎè±¦ÂÖ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_ÌìÎè±¦ÂÖ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_ÌìÎè±¦ÂÖ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_ÌìÎè±¦ÂÖ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_ÌìÎè±¦ÂÖ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÌìÎè±¦ÂÖ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_ÌìÎè±¦ÂÖ";
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
        virtual ~ Skill685Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 684 };
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
            return (float) (812 + 105 * skill->GetLevel () - 0.078 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000 - 1000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[4] = { 120, 125, 130, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (13);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12.7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (9.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (812 + 105 * skill->GetLevel () - 0.078 * skill->GetLevel () * skill->GetLevel ()),
                               40,
                               12,
                               5 * skill->GetLevel (),
                               15 * skill->GetLevel (),
                               INT (4.2 * skill->GetLevel () * skill->GetLevel () + 86 * skill->GetLevel () + 785), 12 - skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetPlayer ()->GetLevel () + 50));
            skill->GetVictim ()->SetTime (2000 + INT (zrand (100 + 3 * skill->GetLevel ()) / 100) * 88000);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (3));
            skill->GetVictim ()->SetTime (90000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (90000 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetMpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (3));
            skill->GetVictim ()->SetTime (90000);
            skill->GetVictim ()->SetRatio (0.3 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (3));
            skill->GetVictim ()->SetTime (90000);
            skill->GetVictim ()->SetRatio (0.3 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecaccuracy (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
