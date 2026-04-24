#ifndef __CPPGEN_GNET_SKILL1119
#define __CPPGEN_GNET_SKILL1119
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1119:public Skill
    {
      public:
        enum
        { SKILL_ID = 1119 };
          Skill1119 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1119Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT0 () + 0.02 * skill->GetT4 ());
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.10 * skill->GetT3 ());
                skill->SetPlus (736 + 20 * skill->GetLevel () + 0.1 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1119Stub ():SkillStub (1119)
        {
            occupation = 27;
            name = L"Á¶¹ÇµØ´Ì";
            nativename = "Á¶¹ÇµØ´Ì";
            icon = "Á¶¹ÇµØ´Ì.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 1;
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
            skill_class = 4;
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
            talent[0] = 1148;
            talent[1] = 1149;
            talent[2] = 1151;
            talent[3] = 1155;
            talent[4] = 1543;
            talent_size = 5;
            action[0] = "¹íµÀ_Á¶¹ÇµØ´Ì";
            action[1] = "¹íµÀ_Á¶¹ÇµØ´Ì";
            action[2] = "¹íµÀ_Á¶¹ÇµØ´Ì";
            action[3] = "¹íµÀ_Á¶¹ÇµØ´Ì";
            action[4] = "¹íµÀ_Á¶¹ÇµØ´Ì";
            action[5] = "¹íµÀ_Á¶¹ÇµØ´Ì";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_Á¶¹ÇµØ´Ì";
            action[16] = "¹íµÀ_Æï³Ë_Á¶¹ÇµØ´Ì";
            action[17] = "¹íµÀ_Æï³Ë_Á¶¹ÇµØ´Ì";
            action[18] = "¹íµÀ_Æï³Ë_Á¶¹ÇµØ´Ì";
            action[19] = "¹íµÀ_Æï³Ë_Á¶¹ÇµØ´Ì";
            action[20] = "¹íµÀ_Æï³Ë_Á¶¹ÇµØ´Ì";
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
        virtual ~ Skill1119Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1116 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (9);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (450 + 15 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 900 * skill->GetT2 () - 300 * skill->GetT4 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 90, 92, 94, 96, 98, 100, 102, 104, 107 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (9);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (9);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (450 + 15 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()),
                               24,
                               INT (736 + 20 * skill->GetLevel () + 0.1 * skill->GetLevel () * skill->GetLevel ()),
                               16, INT (368 + 10 * skill->GetLevel () + 0.05 * skill->GetLevel () * skill->GetLevel ()), 33);

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((736 + 20 * skill->GetLevel () + 0.1 * skill->GetLevel () * skill->GetLevel ()) *
                                            (1 + 0.2 * skill->GetT1 ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (60 + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (16000 + 100);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (24);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.48);
        }
#endif
    };
}
#endif
