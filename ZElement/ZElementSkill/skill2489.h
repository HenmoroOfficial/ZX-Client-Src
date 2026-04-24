#ifndef __CPPGEN_GNET_SKILL2489
#define __CPPGEN_GNET_SKILL2489
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2489:public Skill
    {
      public:
        enum
        { SKILL_ID = 2489 };
          Skill2489 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2489Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1200;
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.01 * skill->GetLevel ());
                skill->SetPlus (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 623);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2489Stub ():SkillStub (2489)
        {
            occupation = 54;
            name = L"ÇìÁúÒôÈĞ";
            nativename = "ÇìÁúÒôÈĞ";
            icon = "ÇìÁúÒôÈĞ.dds";
            maxlevel = 15;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 8;
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
            talent[0] = 2618;
            talent_size = 1;
            action[0] = "Ìì»ª_Ì«¹ÅÒÅÒô";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "Ìì»ª_Ì«¹ÅÒÅÒô";
            action[13] = "";
            action[14] = "";
            action[15] = "Ìì»ª_Æï³Ë_Ì«¹ÅÒÅÒô";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "Ìì»ª_Æï³Ë_Ì«¹ÅÒÅÒô";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 12;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2489Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.3 * skill->GetLevel () * skill->GetLevel () - 3.5 * skill->GetLevel () + 401);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 105, 107, 109, 111, 113, 115, 117, 119, 121 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.3 * skill->GetLevel () * skill->GetLevel () - 3.5 * skill->GetLevel () + 401),
                               skill->GetLevel (),
                               INT (0.071 * skill->GetLevel () * skill->GetLevel () + 13 * skill->GetLevel () + 623), 10 + 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (12000 + 1000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * (0.1 + 0.05 * skill->GetLevel ()));
            skill->GetVictim ()->SetSubantisleep (1);
            return true;
        }
#endif
    };
}
#endif
