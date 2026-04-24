#ifndef __CPPGEN_GNET_SKILL1128
#define __CPPGEN_GNET_SKILL1128
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1128:public Skill
    {
      public:
        enum
        { SKILL_ID = 1128 };
          Skill1128 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1128Stub:public SkillStub
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
                skill->SetRatio (1 + 0.06 * skill->GetT1 () + 0.02 * skill->GetT4 () + 2 * norm (zrand (116) / 100.0));
                skill->SetCrit (0.01 * skill->GetT3 () + 0.03 * norm (skill->GetT4 () / 9.9));
                skill->SetCrithurt (0.10 * skill->GetT3 ());
                skill->SetPlus (0.091 * skill->GetLevel () * skill->GetLevel () + 24 * skill->GetLevel () + 1670);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1128Stub ():SkillStub (1128)
        {
            occupation = 28;
            name = L"¶¾ÁúÊ´Ìì";
            nativename = "¶¾ÁúÊ´Ìì";
            icon = "¶¾ÁúÊ´Ìì.dds";
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
            talent[0] = 1151;
            talent[1] = 1152;
            talent[2] = 1154;
            talent[3] = 1155;
            talent[4] = 1543;
            talent_size = 5;
            action[0] = "¹íµÀ_¶¾ÁúÊ´Ìì";
            action[1] = "¹íµÀ_¶¾ÁúÊ´Ìì";
            action[2] = "¹íµÀ_¶¾ÁúÊ´Ìì";
            action[3] = "¹íµÀ_¶¾ÁúÊ´Ìì";
            action[4] = "¹íµÀ_¶¾ÁúÊ´Ìì";
            action[5] = "¹íµÀ_¶¾ÁúÊ´Ìì";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_¶¾ÁúÊ´Ìì";
            action[16] = "¹íµÀ_Æï³Ë_¶¾ÁúÊ´Ìì";
            action[17] = "¹íµÀ_Æï³Ë_¶¾ÁúÊ´Ìì";
            action[18] = "¹íµÀ_Æï³Ë_¶¾ÁúÊ´Ìì";
            action[19] = "¹íµÀ_Æï³Ë_¶¾ÁúÊ´Ìì";
            action[20] = "¹íµÀ_Æï³Ë_¶¾ÁúÊ´Ìì";
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
        virtual ~ Skill1128Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1125 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (79 * skill->GetLevel () - 1.6 * skill->GetLevel () * skill->GetLevel () + 746);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000 - 900 * skill->GetT0 () - 300 * skill->GetT4 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 120, 122, 124, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (10);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (6);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (79 * skill->GetLevel () - 1.6 * skill->GetLevel () * skill->GetLevel () + 746),
                               30,
                               INT (0.091 * skill->GetLevel () * skill->GetLevel () + 24 * skill->GetLevel () + 1670),
                               93 - 3 * skill->GetLevel (),
                               INT (0.05 * skill->GetLevel () * (0.091 * skill->GetLevel () * skill->GetLevel () + 24 * skill->GetLevel () + 1670)));

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.05 * skill->GetLevel () *
                                            (0.091 * skill->GetLevel () * skill->GetLevel () + 24 * skill->GetLevel () + 1670));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 3 * skill->GetT2 ())) * 600));
            skill->GetVictim ()->SetTime (2050 * skill->GetT2 ());
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (30);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
