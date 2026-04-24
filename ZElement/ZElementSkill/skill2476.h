#ifndef __CPPGEN_GNET_SKILL2476
#define __CPPGEN_GNET_SKILL2476
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2476:public Skill
    {
      public:
        enum
        { SKILL_ID = 2476 };
          Skill2476 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2476Stub:public SkillStub
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
                return 1400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.07 + 0.02 * skill->GetLevel ());
                skill->SetPlus (0.8 * (8.6 * skill->GetLevel () + 324));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2476Stub ():SkillStub (2476)
        {
            occupation = 52;
            name = L"¾ªÏÒ»ªÕÂ";
            nativename = "¾ªÏÒ»ªÕÂ";
            icon = "¾ªÏÒ»ªÕÂ.dds";
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
            talent[0] = 2614;
            talent[1] = 2619;
            talent[2] = 2620;
            talent_size = 3;
            action[0] = "Ìì»ª_¾ªÌÎ";
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
            action[12] = "Ìì»ª_¾ªÌÎ";
            action[13] = "";
            action[14] = "";
            action[15] = "Ìì»ª_Æï³Ë_¾ªÌÎ";
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
            action[27] = "Ìì»ª_Æï³Ë_¾ªÌÎ";
            action[28] = "";
            action[29] = "";
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
            weapon_limit = 12;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2476Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2475 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.4 * skill->GetLevel () * skill->GetLevel () + 2.7 * skill->GetLevel () + 265);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 60, 62, 64, 66, 68, 70, 72, 74, 76 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (18 + 2 * skill->GetT0 ());
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (120));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15 + 2 * skill->GetT0 ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8 + 2 * skill->GetT0 ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.4 * skill->GetLevel () * skill->GetLevel () + 2.7 * skill->GetLevel () + 265),
                               INT (0.8 * (8.6 * skill->GetLevel () + 324)), 7 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetRes6 () + 15 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (2000 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.2 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6 + 2 * skill->GetT1 ());
        }
#endif
    };
}
#endif
